'''
canonicalization routines
'''

from _Setup import *

from PyUtil.debugManager import DebugManager
from PyUtil.symtab import Symtab,SymtabEntry,SymtabError,globalTypeTable
from PyUtil.typetab import TypetabEntry
from PyUtil.argreplacement import replaceArgs

from PyFort.intrinsic import is_intrinsic,getGenericName, isUsedNonStandard,getBuiltInTypes
from PyFort.inference import InferenceError,expressionType,appType,isArrayReference,canonicalTypeClass,expressionShape,isSpecExpression
import PyFort.flow as flow
import PyFort.fortExp as fe
import PyFort.fortStmts as fs
import PyFort.fortUnit as fortUnit
import Canon.function2subroutine as function2subroutine
import Canon.subroutinizedIntrinsics as subroutinizedIntrinsics

import copy
import itertools

_tmp_prefix   = 'oad_ctmp'

class CanonError(Exception):
    '''exception for errors that occur during canonicalization'''
    _keepGoing=False
    
    @staticmethod
    def keepGoing():
        CanonError._keepGoing=True

    def __init__(self,msg,lineNumber):
        self.msg  = msg
        self.lineNumber = lineNumber

    def __str__(self):
        errString='\nERROR: CanonError at line '+str(self.lineNumber)+': '+str(self.msg)
        return (errString)

class UnitCanonicalizer(object):
    'class to facilitate canonicalization on a per-unit basis'

    _hoistConstantsFlag = False
    _hoistStringsFlag = False
    _functionBlockFlag = False
    _createResultDeclFlag = False
    _keepFunctionDecl = True
    _subroutinizeIntegerFunctions = False

    _warnedTempVars4to8BytesForced = False
    
    _ourPassiveTypes=[fs.IntegerStmt,fs.CharacterStmt]
    _ourPassiveTypeIds=[getBuiltInTypes().index((fs.CharacterStmt,4)),
                        getBuiltInTypes().index((fs.IntegerStmt,4)),
                        getBuiltInTypes().index((fs.IntegerStmt,8))]

    @staticmethod
    def setHoistConstantsFlag(hoistConstantsFlag):
        UnitCanonicalizer._hoistConstantsFlag = hoistConstantsFlag

    @staticmethod
    def setHoistStringsFlag(hoistStringsFlag):
        UnitCanonicalizer._hoistStringsFlag = hoistStringsFlag

    @staticmethod
    def setFunctionBlockFlag(functionBlockFlag):
        UnitCanonicalizer._functionBlockFlag = functionBlockFlag

    @staticmethod
    def setCreateResultDeclFlag(createResultFlag):
        UnitCanonicalizer._createResultDeclFlag = createResultFlag

    @staticmethod
    def setKeepFunctionDef(keepFunctionDefFlag):
        UnitCanonicalizer._keepFunctionDecl = keepFunctionDefFlag

    @staticmethod
    def setSubroutinizeIntegerFunctions(flag):
        UnitCanonicalizer._subroutinizeIntegerFunctions = flag

    _overloadingMode = False

    @staticmethod
    def setOverloadingMode():
        UnitCanonicalizer._overloadingMode = True
        
    def __init__(self,aUnit,srModuleUsed=False):
        self.__myUnit = aUnit
        self.__myNewDecls = []
        self.__myNewExecs = []
        self.__tempCounter = 0
        self.__recursionDepth = 0
        self.__outParam = ''
        self.__resultDecl = None
        self.__processedFunctions = []
        self.__stmtFunctionStmts = []

    def shouldSubroutinizeFunction(self,theApp,parentStmt):
        '''
        A function should be subroutinized if it is an intrinsic and subroutinizedIntrinsics.shouldSubroutinize
        returns true or if is not an intrinsic
        '''
        DebugManager.debug('UnitCanonicalizer.shouldSubroutinizeFunction called on "'+str(theApp)+'"')
        if not isinstance(theApp,fe.App):
            raise CanonError('UnitCanonicalizer.shouldSubroutinizeFunction called on non-App object '+str(theApp),parentStmt.lineNumber)
        if isinstance(theApp.head,fe.Sel):
            return False
        theSymtabEntry = self.__myUnit.symtab.lookup_name(theApp.head)
        if theSymtabEntry: 
            if isinstance(theSymtabEntry.entryKind,SymtabEntry.StatementFunctionEntryKind):
                return False
            if isinstance(theSymtabEntry.entryKind,SymtabEntry.VariableEntryKind):
                raise CanonError('UnitCanonicalizer.shouldSubroutinizeFunction called on array reference '+str(theApp)+" with "+theSymtabEntry.debug(theApp.head),parentStmt.lineNumber)
        try:
            appTypeEntry = appType(theApp,self.__myUnit.symtab,parentStmt.lineNumber)
        except InferenceError,errorObj:
            DebugManager.warning("cannot determine return type and canonicalize function call; "+errorObj.msg,parentStmt.lineNumber)
            return False
        if is_intrinsic(theApp.head):
            if UnitCanonicalizer._overloadingMode:
                return False
            baseType=appTypeEntry.getBaseTypeEntry()
            DebugManager.debug('UnitCanonicalizer.shouldSubroutinizeFunction: It\'s an intrinsic of type '+appTypeEntry.debug())
            return subroutinizedIntrinsics.shouldSubroutinize(theApp) and (UnitCanonicalizer._subroutinizeIntegerFunctions or not (isinstance(baseType.entryKind,TypetabEntry.BuiltInEntryKind) and (baseType.entryKind.type_name=='integer_4')))
        elif isinstance(appTypeEntry.entryKind,TypetabEntry.NamedTypeEntryKind):
            # this is an implicit initialization; do not subroutinize
            return False
        else:
            # need to fix for case where app.head is a type (implicit initialization)
            return True

    def __fixSpecExpression(self,theExp,lineNumber):
        ''' we can safely drop some slice expressions if the inquiry fynction does not refer to it '''
        if (isinstance(theExp,fe.App) and theExp.head.lower() in ["size","lbound","ubound"]
            and 
            len(theExp.args)==2) :
            dim=None
            if (isinstance(theExp.args[1],fe.NamedParam) and theExp.args[1].myId.lower()=="dim" and fe._int_re.match(theExp.args[1].myRHS)):
                dim=int(theExp.args[1].myRHS)
            elif(fe._int_re.match(theExp.args[1])):
                dim=int(theExp.args[1])
            if dim:
                argDims=[]
                for pos,argDim in enumerate(theExp.args[0].args):
                    if pos+1!=dim:
                        argDims.append(fe.Zslice())
                    else:
                        argDims.append(argDim)
                # now replace it
                theExp.args[0].args=argDims

    def __newTemp(self,anExpression,parentStmt):
        '''The new temporary variable assumes the value of anExpression'''
        theNewTemp = _tmp_prefix + str(self.__tempCounter)
        self.__tempCounter += 1
        expTypeEntry = expressionType(anExpression,self.__myUnit.symtab,parentStmt.lineNumber)
        if isinstance(expTypeEntry.entryKind,TypetabEntry.CharacterEntryKind):
            charLenEntry=globalTypeTable.charLenTab.lookupCharLenId(expTypeEntry.entryKind.charLenId)
            varTypeClass=fs.CharacterStmt
            typeKind=charLenEntry.getCharLenExp()
            varModifierList=[typeKind]
        else:
            (varTypeClass,typeKind)=globalTypeTable.intrinsicIdToTypeMap[expTypeEntry.getBaseTypeId()]
            varModifierList=[]
        varShape=expressionShape(anExpression,self.__myUnit.symtab,parentStmt.lineNumber)
        if (not UnitCanonicalizer._warnedTempVars4to8BytesForced and isinstance(expTypeEntry.entryKind,TypetabEntry.BuiltInEntryKind) and expTypeEntry.entryKind.type_name=='real_4'):
            print >>sys.stderr,'WARNING: single precision temporary variable introduced; issued for first occurence only, here in unit '+str(self.__myUnit.uinfo)+' for expression >'+str(anExpression)+'< on line '+str(parentStmt.lineNumber)
            UnitCanonicalizer._warnedTempVars4to8BytesForced=True
        DebugManager.debug('replaced with '+str(theNewTemp)+' of type '+expTypeEntry.debug())
        typeAttrList=[]
        needsAlloc=False
        if varShape:
            declDimArgs=[]
            for dim in varShape:
                if (not isSpecExpression(dim,self.__myUnit.symtab,parentStmt.lineNumber)):
                    self.__fixSpecExpression(dim,parentStmt.lineNumber)
                if (not isSpecExpression(dim,self.__myUnit.symtab,parentStmt.lineNumber)):
                    needsAlloc=True
                    declDimArgs.append(fe.Zslice())
                else:
                    declDimArgs.append(dim)
            typeAttrList.append(fe.App('dimension',declDimArgs))
        if needsAlloc:
            typeAttrList.append("allocatable")
            DebugManager.warning("temporary  "+theNewTemp+" declared as allocatable but allocation logic is as of yet not implented",parentStmt.lineNumber)
        theNewDecl = varTypeClass(varModifierList,typeAttrList,[theNewTemp])
        typeId = globalTypeTable.getType(theNewDecl,self.__myUnit.symtab)
        self.__myNewDecls.append(theNewDecl)
        self.__myUnit.symtab.enter_name(theNewTemp,
                                        SymtabEntry(SymtabEntry.VariableEntryKind(),
                                                    type=(varTypeClass,varModifierList),
                                                    dimensions=varShape,
                                                    origin='temp',
                                                    typetab_id=typeId))
        return (theNewTemp,varTypeClass,varModifierList)

    def __canonicalizeIntrinsicEllipsisRec(self,head,args):
        if (len(args)==2):
            return fe.App(head,args)
        else:
            return fe.App(head,[args[0],self.__canonicalizeIntrinsicEllipsisRec(head,args[1:])])
        
    def __canonicalizeFuncCall(self,theFuncCall,parentStmt):
        '''turn a function call into a subroutine call
        returns the new temp created as return value for the new subroutine call'''
        DebugManager.debug(self.__recursionDepth*'|\t'+'converting function call "'+str(theFuncCall)+'" to a subroutine call')
        self.__recursionDepth += 1
        DebugManager.debug((self.__recursionDepth-1)*'|\t'+'|  creating new temp for the result of the subroutine that replaces "'+str(theFuncCall)+'":',newLine=False)
        (theNewTemp,newTempType,newTempTypeMods) = self.__newTemp(theFuncCall,parentStmt)
        newArgs = [theNewTemp]
        newSubName=''
        if is_intrinsic(theFuncCall.head):
            funcName=getGenericName(theFuncCall.head)
            newSubName = subroutinizedIntrinsics.makeName(funcName)
            argRanks=[]
            for arg in theFuncCall.args:
                argS=expressionShape(arg,self.__myUnit.symtab,parentStmt.lineNumber)                            
                if argS:
                    argRanks.append(len(argS))
                else:
                    argRanks.append(0)
            (d,t)=canonicalTypeClass(newTempType,newTempTypeMods)
            if (d):
                subroutinizedIntrinsics.markRequired(funcName,t,argRanks)
            else:
                for t in subroutinizedIntrinsics.typeList:
                    subroutinizedIntrinsics.markRequired(funcName,t,argRanks)
            if funcName in ('max','min') and len(theFuncCall.args)>2 :
                self.__recursionDepth -= 1
                return self.__canonicalizeFuncCall(self.__canonicalizeIntrinsicEllipsisRec(theFuncCall.head,theFuncCall.args),parentStmt)
        else:
            newSubName = subroutinizedIntrinsics.call_prefix + theFuncCall.head
        self.__myNewExecs.append(self.__canonicalizeSubCallStmt(\
            fs.CallStmt(newSubName,
                        theFuncCall.args+newArgs,
                        lineNumber=parentStmt.lineNumber,
                        label=False,
                        lead=parentStmt.lead)))
        DebugManager.debug((self.__recursionDepth-1)*'|\t'+'|_')
        self.__recursionDepth -= 1
        return theNewTemp

    def __hoistExpression(self,theExpression,parentStmt,paramName,forceIt=False):
        # function calls that are to be turned into subroutine calls
        # -> return the temp that carries the value for the new subcall
        # or alternatively if paramName is set return the construct
        # for a named parameter expression
        if isinstance(theExpression,fe.App) and \
           not isArrayReference(theExpression,self.__myUnit.symtab,parentStmt.lineNumber) and \
           self.shouldSubroutinizeFunction(theExpression,parentStmt):
            DebugManager.debug('it is a function call to be subroutinized')
            return self.__canonicalizeFuncCall(theExpression,parentStmt)
        # if argument is a variable reference then don't hoist
        if (not forceIt and
            (isinstance(theExpression,str) and fe.is_id(theExpression)) 
            or 
            (isinstance(theExpression,fe.App) and isArrayReference(theExpression,self.__myUnit.symtab,parentStmt.lineNumber))
            or
            isinstance(theExpression,fe.Sel)):
            return theExpression

        # Anything else: create an assignment to a temporary and return that temporary
        (theNewTemp,newTempType,newTempTypeMods) = self.__newTemp(theExpression,parentStmt)
        self.__myNewExecs.append(self.__canonicalizeAssignStmt(fs.AssignStmt(theNewTemp,
                                                                             theExpression,
                                                                             lineNumber=parentStmt.lineNumber,
                                                                             label=False,
                                                                             lead=parentStmt.lead)))
        if (paramName):
            return fe.NamedParam(paramName,theNewTemp)
        else: 
            return theNewTemp

    def __canonicalizeExpression(self,theExpression,parentStmt):
        '''Canonicalize an expression tree by recursively replacing function calls with subroutine calls
           returns an expression that replaces theExpression'''
        DebugManager.debug(self.__recursionDepth*'|\t'+'canonicalizing expression "'+str(theExpression)+'"',newLine=False)
        self.__recursionDepth += 1
        replacementExpression = theExpression
        # variable or constant -> do nothing
        if isinstance(theExpression, str):
            DebugManager.debug(', which is a string (should be a constant or a variable => no canonicalization necessary)')
        # application expressions
        elif isinstance(theExpression,fe.App):
            if isinstance(theExpression.head,fe.App):
                replacementExpression.head = self.__canonicalizeExpression(theExpression.head,parentStmt)
            # array reference -. do nothing
            elif isArrayReference(theExpression,self.__myUnit.symtab,parentStmt.lineNumber):
                DebugManager.debug(', which is an array reference (no canonicalization necessary)')
            # function calls to subroutinize -> subroutinize and recursively canonicalize args
            elif self.shouldSubroutinizeFunction(theExpression,parentStmt):
                DebugManager.debug(', it\'s a function call (subroutinized)')
                replacementExpression = self.__canonicalizeFuncCall(theExpression,parentStmt)
            # function calls that won't be subroutinized -> recursively canonicalize args
            else: 
                DebugManager.debug(', it\'s a function call (non-subroutinized)')
                replacementArgs = []
                for arg in theExpression.args:
                    replacementArgs.append(self.__canonicalizeExpression(arg,parentStmt))
                replacementHead = theExpression.head
                if not isinstance(theExpression.head,fe.Sel):
                    aSymtabEntry=self.__myUnit.symtab.lookup_name(theExpression.head)
                    # see if it is  a statement function and expand it
                    if (aSymtabEntry and isinstance(aSymtabEntry.entryKind,SymtabEntry.StatementFunctionEntryKind)):
                        parentStmt.beenModified = True
                        replacementExpression=self.__expandStmtFunExp(fe.App(replacementHead,replacementArgs))
                    # check whether we need to convert the function to the generic name (e.g. alog => log)
                    else:
                        if (is_intrinsic(theExpression.head) and theExpression.head.lower() != getGenericName(theExpression.head)) :
                            parentStmt.beenModified = True
                            replacementHead = getGenericName(theExpression.head)
                        replacementExpression = fe.App(replacementHead,replacementArgs)
        # Unary operation -> recursively canonicalize the sole subexpression
        elif isinstance(theExpression,fe.Unary):
            DebugManager.debug(', which is a unary op. with exp: "'+str(theExpression.exp)+'"')
            replacementExpression = theExpression.__class__(self.__canonicalizeExpression(theExpression.exp,parentStmt))
        # Binary operation -> recursively canonicalize both subexpressions
        elif isinstance(theExpression,fe.Ops):
            DebugManager.debug(', which is a binary op. with a1="'+str(theExpression.a1)+'", a2="'+str(theExpression.a2)+'"')
            a1Result = self.__canonicalizeExpression(theExpression.a1,parentStmt)
            a2result = self.__canonicalizeExpression(theExpression.a2,parentStmt)
            replacementExpression = fe.Ops(theExpression.op,a1Result,a2result)
        # Everything else...
        else:
            DebugManager.debug(', which is of type '+str(theExpression.__class__)+' that is assumed to require no canonicalization')
        DebugManager.debug((self.__recursionDepth-1)*'|\t'+'|_')
        self.__recursionDepth -= 1
        return replacementExpression

    def __canonicalizeSubCallStmt(self,aSubCallStmt):
        '''
        Canonicalize a subroutine call by hoisting all arguments
        (except simple variables) to temporaries.
        '''
        DebugManager.debug(self.__recursionDepth*'|\t'+'canonicalizing subroutine call statement "'+str(aSubCallStmt)+'"')
        self.__recursionDepth += 1
        # canonicalize each of the the expressions that serve as arguments
        replacementArgs = []
        # certain non-standard intrinsics (e.g. free) are subroutines rather than functions
        if (not isUsedNonStandard(aSubCallStmt.head)):
            for anArg in aSubCallStmt.args:
                paramName=None
                if isinstance(anArg,fe.NamedParam):
                    paramName=anArg.myId
                    anArg=anArg.myRHS
                DebugManager.debug((self.__recursionDepth - 1)*'|\t'+'|- argument "'+str(anArg)+'" ',newLine=False)
                argType=None
                argTypeMod=None
                try: 
                    argType = expressionType(anArg,self.__myUnit.symtab,aSubCallStmt.lineNumber)
                except InferenceError, e :
                    DebugManager.warning("cannot canonicalize argument >"+str(anArg)+"< parsed as "+repr(anArg)+" because: "+e.msg,aSubCallStmt.lineNumber)
                    replacementArgs.append(anArg)
                    continue
                # constant character expressions
                if argType is not None and (isinstance(argType.entryKind,TypetabEntry.CharacterEntryKind) or \
                        (isinstance(argType.entryKind,TypetabEntry.BuiltInEntryKind) and argType.entryKind.type_name=='character')):
                    if not self._hoistStringsFlag:
                        DebugManager.debug('is a string expression (which we aren\'t hoisting)')
                        replacementArgs.append(anArg)
                    elif isinstance(anArg,str) and self.__myUnit.symtab.lookup_name(anArg):
                        DebugManager.debug('is a string variable (which we aren\'t hoisting)')
                        replacementArgs.append(anArg)
                    elif isinstance(anArg,fe.App) and isArrayReference(anArg,self.__myUnit.symtab,aSubCallStmt.lineNumber):
                        DebugManager.debug('is a character array reference (which we aren\'t hoisting)')
                        replacementArgs.append(anArg)
                    else:
                        DebugManager.debug('is a string expression to be hoisted:',newLine=False)
                        replacementArgs.append(self.__hoistExpression(anArg,aSubCallStmt,paramName))
                # other constant expressions
                elif fe.isConstantExpression(anArg):
                    if not self._hoistConstantsFlag:
                        DebugManager.debug('is a constant expression (which we aren\'t hoisting)')
                        replacementArgs.append(anArg)
                    else:
                        DebugManager.debug('is a constant expression to be hoisted:',newLine=False)
                        replacementArgs.append(self.__hoistExpression(anArg,aSubCallStmt,paramName))
                # variables (with VariableEntry in symbol table) -> do nothing except for pointers to subroutinized functions and PARAMTERs 
                elif isinstance(anArg,str):
                    symtabEntry=self.__myUnit.symtab.lookup_name(anArg)
                    if (symtabEntry
                        and
                        (globalTypeTable.lookupTypeId(symtabEntry.typetab_id).isExternal()
                         or 
                         isinstance(symtabEntry.entryKind,SymtabEntry.FunctionEntryKind))):
                        if (function2subroutine.wasSubroutinized(symtabEntry.getScopePrefix(self.__myUnit)+anArg)) :
                            replacementArgs.append(function2subroutine.name_init+anArg); 
                            aSubCallStmt.beenModified=True
                            DebugManager.debug('is an identifier referring to a subroutinized function')
                        else :
                            DebugManager.warning('argument '+anArg+' in call to '+aSubCallStmt.head+' is classified as a function but we have not seen the definition and will assume it is not going to be subroutinized' ,lineNumber=aSubCallStmt.lineNumber)
                            replacementArgs.append(anArg)
                    elif (symtabEntry and symtabEntry.constInit and self._hoistConstantsFlag):
                        DebugManager.debug('is a PARAMETER identifier to be hoisted:',newLine=False)
                        replacementArgs.append(self.__hoistExpression(anArg,aSubCallStmt,paramName,True))
                    else : 
                        DebugManager.debug('is an identifier')
                        replacementArgs.append(anArg)
                # Array References -> do nothing
                elif isinstance(anArg,fe.App) and isArrayReference(anArg,self.__myUnit.symtab,aSubCallStmt.lineNumber):
                    DebugManager.debug('is an array reference')
                    replacementArgs.append(anArg)
                # everything else -> hoist and create an assignment to a temp variable
                else:
                    DebugManager.debug('is a nontrivial expression to be hoisted:',newLine=False)
                    replacementArgs.append(self.__hoistExpression(anArg,aSubCallStmt,paramName))
        else :
            DebugManager.warning('arguments in call to a non-standard intrinsic subroutine '+str(aSubCallStmt.head)+' are not hoisted',lineNumber=aSubCallStmt.lineNumber)
            replacementArgs=aSubCallStmt.args
        # replace aCallStmt with the canonicalized version
        replacementStatement = fs.CallStmt(aSubCallStmt.head,
                                           replacementArgs,
                                           lineNumber=aSubCallStmt.lineNumber,
                                           label=aSubCallStmt.label,
                                           lead=aSubCallStmt.lead,
                                           internal=aSubCallStmt.internal)
        DebugManager.debug((self.__recursionDepth-1)*'|\t'+'|_')
        self.__recursionDepth -= 1
        return replacementStatement

    def __canonicalizeAssignStmt(self,anAssignStmt):
        '''Canonicalize an assigment statement by removing function calls from the rhs'''
        DebugManager.debug(self.__recursionDepth*'|\t'+'canonicalizing assignment statement "'+str(anAssignStmt)+'"')
        self.__recursionDepth += 1
        replacementStatement = fs.AssignStmt(anAssignStmt.get_lhs(),
                                             self.__canonicalizeExpression(anAssignStmt.get_rhs(),anAssignStmt),
                                             lineNumber=anAssignStmt.lineNumber,
                                             label=anAssignStmt.label,
                                             lead=anAssignStmt.lead)
        DebugManager.debug((self.__recursionDepth-1)*'|\t'+'|_')
        self.__recursionDepth -= 1
        return replacementStatement

    def __canonicalizeIfNonThenStmt(self,anIfNonThenStmt):
        '''Canonicalize if stmt (without "then") by converting to an if-then construct and then recursively
           canonicalizing the test component and the conditionally executed statement'''
        # the replacement statement should be the endif
        DebugManager.debug(self.__recursionDepth*'|\t'+'canonicalizing if statement (without "then") "'+str(anIfNonThenStmt)+'" => replacing with an if-then statement')
        handleDoTermination=False
        if (anIfNonThenStmt.label 
            and 
            self.__myUnit.symtab.labelRefs.has_key(str(anIfNonThenStmt.label)) 
            and 
            any(map(lambda l: isinstance(l,fs.DoStmt),self.__myUnit.symtab.labelRefs[str(anIfNonThenStmt.label)]))):
            # DO terminated by IfNonThenStmt
            # if there is only one reference we can save it by moving the label to a new  END DO that follows  the replacement of anIfNonThenStmt : 
            if (len(self.__myUnit.symtab.labelRefs[str(anIfNonThenStmt.label)])==1) :
                handleDoTermination=True
            else : 
                e=CanonError('IF statement "'+str(anIfNonThenStmt)+'" terminates DO construct "'\
                             +str((filter(lambda l: isinstance(l,fs.DoStmt),self.__myUnit.symtab.labelRefs[str(anIfNonThenStmt.label)]))[0])\
                             +'" and cannot be converted to an IF construct because references to its label other than from the above DO construct exist; list of all references: ['\
                             +' ; '.join('line '+str(l.lineNumber)+": "+str(l) for l in self.__myUnit.symtab.labelRefs[str(anIfNonThenStmt.label)])\
                             +"]",
                             anIfNonThenStmt.lineNumber)   
                if CanonError._keepGoing:
                    DebugManager.warning(e.msg,e.lineNumber,DebugManager.WarnType.ifStmtToIfConstr)
                else:
                    raise e
        self.__recursionDepth += 1
        # first append the new IfThenStmt
        newLabel=(not handleDoTermination) and anIfNonThenStmt.label or None # if we handle the do termination the label moves to a new END DO 
        self.__myNewExecs.append(fs.IfThenStmt(self.__canonicalizeExpression(anIfNonThenStmt.test,anIfNonThenStmt),
                                 ifFormatStr=anIfNonThenStmt.ifFormatStr,
                                 thenFormatStr='then',
                                 lineNumber=anIfNonThenStmt.lineNumber,
                                 label=newLabel,
                                 lead=anIfNonThenStmt.lead))
        self.__recursionDepth -= 1
        # append the canonicalized version of the executable statement
        anIfNonThenStmt.stmt.lead = anIfNonThenStmt.lead+'  '
        self.__canonicalizeExecStmt(anIfNonThenStmt.stmt)
        self.__recursionDepth += 1
        # insert the endif statement as the replacement
        newEndIf=fs.EndifStmt(lead=anIfNonThenStmt.lead)
        replacementStatement=None
        if handleDoTermination:
            self.__myNewExecs.append(newEndIf)
            # new END DO with label that was on anIfNonThenStmt
            replacementStatement=fs.EnddoStmt(None,label=anIfNonThenStmt.label,lead=anIfNonThenStmt.lead)
        else:
            replacementStatement = newEndIf
        DebugManager.debug((self.__recursionDepth-1)*'|\t'+'|_')
        self.__recursionDepth -= 1
        return replacementStatement

    def __canonicalizeIfThenStmt(self,anIfThenStmt):
        '''Canonicalize if-then stmt by canonicalizing the test component
        returns a list of statements that replace anIfThenStmt'''
        DebugManager.debug(self.__recursionDepth*'|\t'+'canonicalizing if-then statement "'+str(anIfThenStmt)+'"')
        self.__recursionDepth += 1
        replacementStatement = fs.IfThenStmt(self.__canonicalizeExpression(anIfThenStmt.test,anIfThenStmt),
                                             lineNumber=anIfThenStmt.lineNumber,
                                             label=anIfThenStmt.label,
                                             lead=anIfThenStmt.lead)
        DebugManager.debug((self.__recursionDepth-1)*'|\t'+'|_')
        self.__recursionDepth -= 1
        return replacementStatement

    def __canonicalizeElseifStmt(self,anElseifStmt):
        '''Canonicalize anElseifStmt by canonicalizing the test component.  Returns a canonicalized ElseifStmt that replaces anElseifStmt'''
        DebugManager.debug(self.__recursionDepth*'|\t'+'canonicalizing elseif-then statement "'+str(anElseifStmt)+'"')
        self.__recursionDepth += 1
        newExecsLength = len(self.__myNewExecs)
        replacementStatement = fs.ElseifStmt(self.__canonicalizeExpression(anElseifStmt.test,anElseifStmt),
                                             lineNumber=anElseifStmt.lineNumber,
                                             label=anElseifStmt.label,
                                             lead=anElseifStmt.lead)
        if len(self.__myNewExecs) > newExecsLength: # this is the case iff some new statements were inserted
            e=CanonError('elseif test-component "'+str(anElseifStmt.test)+'" requires hoisting, but the placement of the extra assignment(s) is problematic.',anElseifStmt.lineNumber)
            if CanonError._keepGoing:
                DebugManager.warning(e.msg,e.lineNumber,DebugManager.WarnType.hoisting)
            else:
                raise e
        DebugManager.debug((self.__recursionDepth-1)*'|\t'+'|_')
        self.__recursionDepth -= 1
        return replacementStatement

    def __canonicalizeDoStmt(self,aDoStmt):
        '''
        Canonicalize aDoStmt statement by canonicalizing the loop start, end, and stride expressions.  Returns a canonicalized DoStmt that replaces aDoStmt.
        '''
        DebugManager.debug(self.__recursionDepth*'|\t'+'canonicalizing do statement "'+str(aDoStmt)+'"')
        self.__recursionDepth += 1
        replacementStatement = aDoStmt
        if aDoStmt.loopControl:
            replacementStart = self.__canonicalizeExpression(aDoStmt.loopControl.start,aDoStmt)
            newExecsLength = len(self.__myNewExecs)
            replacementStatement = fs.DoStmt(aDoStmt.doName,
                                             aDoStmt.doLabel,
                                             fe.LoopControl(aDoStmt.loopControl.var,
                                                            replacementStart,
                                                            self.__canonicalizeExpression(aDoStmt.loopControl.end,aDoStmt),
                                                            self.__canonicalizeExpression(aDoStmt.loopControl.stride,aDoStmt)),
                                             lineNumber=aDoStmt.lineNumber,
                                             label=aDoStmt.label,
                                             lead=aDoStmt.lead)
            if len(self.__myNewExecs) > newExecsLength: # this is the case iff loopControl.end or loopControl.stride required hoisting
                raise CanonError('Either loopControl.end "'+str(aDoStmt.loopControl.end)+'" or loopControl.stride "'+str(aDoStmt.loopControl.stride)+'" for DoStmt "'+str(aDoStmt)+'" requires hoisting, but the placement of the extra assignment(s) is problematic.',aDoStmt.lineNumber)
        DebugManager.debug((self.__recursionDepth-1)*'|\t'+'|_')
        self.__recursionDepth -= 1
        return replacementStatement

    def __canonicalizeWhileStmt(self,aWhileStmt):
        '''
        Canonicalize aWhileStmt statement by canonicalizing the test expression.  Returns a canonicalized while statement that replaces aWhileStmt.
        '''
        DebugManager.debug(self.__recursionDepth*'|\t'+'canonicalizing while statement "'+str(aWhileStmt)+'"')
        self.__recursionDepth += 1
        newExecsLength = len(self.__myNewExecs)
        replacementStatement = fs.WhileStmt(self.__canonicalizeExpression(aWhileStmt.testExpression,aWhileStmt),
                                            lineNumber=aWhileStmt.lineNumber,
                                            label=aWhileStmt.label,
                                            lead=aWhileStmt.lead)
        if len(self.__myNewExecs) > newExecsLength: # this is the case iff some new statements were inserted
            raise CanonError('while statement test expression "'+str(aWhileStmt.testExpression)+'" requires hoisting, but the placement of the extra assignment(s) is problematic.',aWhileStmt.lineNumber)
        DebugManager.debug((self.__recursionDepth-1)*'|\t'+'|_')
        self.__recursionDepth -= 1
        return replacementStatement

    def __canonicalizeSelectCaseStmt(self,aSelectCaseStmt):
        '''
        Canonicalize aSelectCaseStmt by canonicalizing the case expression.  Returns a canonicalized select case statement that replaces aSelectCaseStmt.
        '''
        DebugManager.debug(self.__recursionDepth*'|\t'+'canonicalizing select case statement "'+str(aSelectCaseStmt)+'"')
        self.__recursionDepth += 1
        replacementStatement = fs.SelectCaseStmt(self.__canonicalizeExpression(aSelectCaseStmt.caseExpression,aSelectCaseStmt),
                                                 lineNumber=aSelectCaseStmt.lineNumber,
                                                 label=aSelectCaseStmt.label,
                                                 lead=aSelectCaseStmt.lead)
        DebugManager.debug((self.__recursionDepth-1)*'|\t'+'|_')
        self.__recursionDepth -= 1
        return replacementStatement

    def __canonicalizeIOExpression(self,exp,parentStmt,paramName=None):
        '''
        Canonicalize an expression from an IO statement by hoisting any function calls
        '''
        DebugManager.debug(self.__recursionDepth*'|\t'+'canonicalizing an IO statement"'+str(parentStmt)+'"')
        self.__recursionDepth += 1
        newExp = exp
        if isinstance(exp,fe.App):
            appTypeId=appType(exp,self.__myUnit.symtab,parentStmt.lineNumber)
            if ((not isArrayReference(exp,self.__myUnit.symtab,parentStmt.lineNumber))
                and
                (not appTypeId in UnitCanonicalizer._ourPassiveTypeIds)):
                newExp = self.__hoistExpression(exp,parentStmt,paramName)
        elif isinstance(exp,fe.Ops):
            newExp = fe.Ops(exp.op,
                            self.__canonicalizeIOExpression(exp.a1,parentStmt),
                            self.__canonicalizeIOExpression(exp.a2,parentStmt))
        elif isinstance(exp,fe.ParenExp):
            newExp = fe.ParenExp(self.__canonicalizeIOExpression(exp.exp,parentStmt))
        elif isinstance(exp,fe.Umi):
            newExp = fe.Umi(self.__canonicalizeIOExpression(exp.exp,parentStmt))
        elif isinstance(exp,fe.Upl):
            newExp = fe.Upl(self.__canonicalizeIOExpression(exp.exp,parentStmt))
        elif isinstance(exp,fe.Not):
            newExp = fe.Not(self.__canonicalizeIOExpression(exp.exp,parentStmt))
        elif isinstance(exp,fe.MultiParenExp):
            newList = []
            for item in exp.expList:
                newList.append(self.__canonicalizeIOExpression(item,parentStmt))
            newExp = fe.MultiParenExp(newList)
        DebugManager.debug((self.__recursionDepth-1)*'|\t'+'|_')
        self.__recursionDepth -= 1
        return newExp

    def __canonicalizeIOStmt(self,anIOStmt):
        '''
        Canonicalize an IOStmt by canonicalizing the ioCtrlSpecList and the itemList. Returns a canonicalized IO statement that replaces anIOStmt.
        '''
        DebugManager.debug(self.__recursionDepth*'|\t'+'canonicalizing an IO statement"'+str(anIOStmt)+'"')
        self.__recursionDepth += 1
        replacementStmt = anIOStmt
        for item in replacementStmt.itemList:
            newExp = self.__canonicalizeIOExpression(item,replacementStmt)
            if newExp != item:
                index = replacementStmt.itemList.index(item)
                replacementStmt.itemList.insert(index,newExp)
                replacementStmt.itemList.remove(item)
                replacementStmt.modified = True
        DebugManager.debug((self.__recursionDepth-1)*'|\t'+'|_')
        self.__recursionDepth -= 1
        return replacementStmt

    def __expandStmtFunExp(self,anExp):
        newSon = anExp
        if isinstance(anExp,fe.App):
            for stmtFnStmt in self.__stmtFunctionStmts:
                newArgs = []
                for anArg in anExp.args:
                    newArgs.append(self.__expandStmtFunExp(anArg))
                if anExp.head == stmtFnStmt.name:
                    newSon = fe.ParenExp(stmtFnStmt.body)
                    # replace args
                    newSon = replaceArgs(str(newSon),stmtFnStmt.args,newArgs)
                else:
                    newSon = fe.App(anExp.head,newArgs)
        elif isinstance(anExp,fe.Ops):
            newSon = fe.Ops(anExp.op,
                            self.__expandStmtFunExp(anExp.a1),
                            self.__expandStmtFunExp(anExp.a2))
        elif isinstance(anExp,fe.ParenExp):
            newSon = fe.ParenExp(self.__expandStmtFunExp(anExp.exp))
        elif isinstance(anExp,fe.Umi):
            newSon = fe.Umi(self.__expandStmtFunExp(anExp.exp))
        elif isinstance(anExp,fe.Upl):
            newSon = fe.Upl(self.__expandStmtFunExp(anExp.exp))
        elif isinstance(anExp,fe.Not):
            newSon = fe.Not(self.__expandStmtFunExp(anExp.exp))
        elif isinstance(anExp,fe.MultiParenExp):
            newList = []
            for item in anExp.expList:
                newList.append(self.__expandStmtFunExp(item))
                newSon = fe.MultiParenExp(newList)
        return newSon
        

    def __expandStmtFunction(self,anExecStmt):
        if hasattr(anExecStmt,'_sons'):
            for aSon in anExecStmt.get_sons():
                theSon = getattr(anExecStmt,aSon)
                newSon = self.__expandStmtFunExp(theSon)
                anExecStmt.set_son(aSon,newSon)
        return anExecStmt

    def __canonicalizeExecStmt(self,anExecStmt):
        # We were previously working with the assumption that an original statement is modified as part of the canonicalization process
        # if and only if at least one new statement has been added.
        # This is not true, as for example we canonicalize y = alog(x) to y = log(x)
        # hence, we have added a flag "beenModified" which indicates that an expression should be replaced by the canonicalized version
        anExecStmt.beenModified = False
        newExecsLength = len(self.__myNewExecs) # store the current number of execs (to determine afterwards whether we've added some)
        replacementStatement = anExecStmt
        replacementStatement = self.__expandStmtFunction(replacementStatement)
        if isinstance(anExecStmt,fs.CallStmt):
            replacementStatement = self.__canonicalizeSubCallStmt(anExecStmt)
        elif isinstance(anExecStmt,fs.AssignStmt):
            replacementStatement = self.__canonicalizeAssignStmt(anExecStmt)
        elif isinstance(anExecStmt,fs.IfNonThenStmt):
            replacementStatement = self.__canonicalizeIfNonThenStmt(anExecStmt)
        elif isinstance(anExecStmt,fs.IfThenStmt):
            replacementStatement = self.__canonicalizeIfThenStmt(anExecStmt)
        elif isinstance(anExecStmt,fs.ElseifStmt):
            replacementStatement = self.__canonicalizeElseifStmt(anExecStmt)
        elif isinstance(anExecStmt,fs.DoStmt):
            replacementStatement = self.__canonicalizeDoStmt(anExecStmt)
        elif isinstance(anExecStmt,fs.WhileStmt):
            replacementStatement = self.__canonicalizeWhileStmt(anExecStmt)
        elif isinstance(anExecStmt,fs.SelectCaseStmt):
            replacementStatement = self.__canonicalizeSelectCaseStmt(anExecStmt)
        elif isinstance(anExecStmt,fs.IOStmt):
            replacementStatement = self.__canonicalizeIOStmt(anExecStmt)
        else:
            DebugManager.debug('Statement "'+str(anExecStmt)+'" is assumed to require no canonicalization')
        if self.__recursionDepth != 0:
            raise CanonError('Recursion depth did not resolve to zero when canonicalizing '+str(anExecStmt),anExecStmt.lineNumber)
        # determine whether a change was made
        if anExecStmt.beenModified or \
           len(self.__myNewExecs) > newExecsLength: # some new statements were inserted
            self.__myNewExecs.append(replacementStatement) # => replace anExecStmt with the canonicalized version
        else: # no new statements were inserted
            self.__myNewExecs.append(anExecStmt) # => leave anExecStmt alone

    def __canonicalizeFunctionDecls(self,aDecl,subroutineBlock):
        if isinstance(aDecl,fs.StmtFnStmt):
            self.__stmtFunctionStmts.append(aDecl)
        if self._keepFunctionDecl:
            self.__myNewDecls.append(aDecl)
        if isinstance(aDecl,fs.FunctionStmt):
            self.setFunctionBlockFlag(True)
            (self.__outParam,subroutineStmt) = function2subroutine.\
                                               convertFunctionOrEntryStmt(aDecl,self.__myUnit)
            subroutineBlock.append(subroutineStmt)
            self.__resultDecl = function2subroutine.\
                         createResultDecl(aDecl,self.__outParam)
            self.setCreateResultDeclFlag(True)
        elif not self._functionBlockFlag:
            if not self._keepFunctionDecl:
            	self.__myNewDecls.append(aDecl)
        elif isinstance(aDecl,fs.EndStmt):
            newEndStmt = fs.EndSubroutineStmt(lineNumber=aDecl.lineNumber,label=aDecl.label,lead=aDecl.lead)
            subroutineBlock.append(newEndStmt)
            self.__myNewDecls.extend(subroutineBlock)
            self.setFunctionBlockFlag(False)
        elif isinstance(aDecl,fs.Comments):
            if aDecl.rawline.strip() == '':
                pass
            else:
                subroutineBlock.append(aDecl)
        elif self._createResultDeclFlag \
                 and not isinstance(aDecl,fs.UseStmt):
            if self.__resultDecl is not None:
                subroutineBlock.append(self.__resultDecl)
                self.setCreateResultDeclFlag(False)
            elif isinstance(aDecl,fs.TypeDecl):
                (aDecl,resultDecl,resultDeclFlag) = function2subroutine.updateTypeDecl(\
                    aDecl,self.__outParam,self.__myNewDecls)
                self.setCreateResultDeclFlag(not resultDeclFlag)
                if resultDeclFlag:
                    subroutineBlock.append(resultDecl)
            if aDecl is not None:
                subroutineBlock.append(aDecl)
        else:
            subroutineBlock.append(aDecl)

    def __createFuncSubPairs(self):
        oldFuncnewSubPairs = []
        for aSubUnit in self.__myUnit.ulist:
            if isinstance(aSubUnit.uinfo,fs.SubroutineStmt) and \
                   aSubUnit.uinfo.name.startswith(function2subroutine.name_init):
                if aSubUnit.uinfo.name[0:6] != function2subroutine.name_init :
                    raise CanonError('Tried to strip "'+aSubUnit.uinfo.name[0:6]+'"' \
                                    +' from the beginning of "'+aSubUnit.uinfo.name+'"',aSubUnit.uinfo.lineNumber)
                DebugManager.debug('searching declarations to convert old function name "'+aSubUnit.uinfo.name[6:]+'"' \
                                  +' to new subroutine name "'+aSubUnit.uinfo.name+'"')
                oldFuncnewSubPairs.append([aSubUnit.uinfo.name[6:],
                                           aSubUnit.uinfo.name])
        return oldFuncnewSubPairs
        
    def __processInterfaceBlocks(self,oldFuncnewSubPairs):
        interfaceBlockFlag = False
        interfaceBlock = []
        self.__myNewDecls = []
        for aDecl in self.__myUnit.decls:
            # accumulate an interface block and process it
            if interfaceBlockFlag:
                interfaceBlock.append(aDecl)
                if isinstance(aDecl,fs.EndInterfaceStmt):
                    newInterfaceBlock = function2subroutine.\
                                        convertInterfaceBlock(interfaceBlock,oldFuncnewSubPairs,self._keepFunctionDecl)
                    self.__myNewDecls.extend(newInterfaceBlock)
                    interfaceBlockFlag = False
                    interfaceBlock = []
            elif isinstance(aDecl,fs.InterfaceStmt):
                interfaceBlockFlag = True
                interfaceBlock.append(aDecl)
            # if not part of the interface block, convert decls and add them
            else:
                (newDecl,modified) = function2subroutine.\
                                     convertFunctionDecl(aDecl,oldFuncnewSubPairs)
                if (not modified):
                    self.__myNewDecls.append(aDecl)
                else:
                    if (self._keepFunctionDecl):
                        self.__myNewDecls.append(aDecl)
                    self.__myNewDecls.append(newDecl)

    def __createNewSubroutine(self,aUnit,subroutineDecls):
        if isinstance(aUnit.uinfo,fs.FunctionStmt):
            DebugManager.debug(5*'-'+'>'+'called __createNewSubroutine ' \
                               + 'on unit "'+str(aUnit)+'",' \
                               + 'with subroutineDecls='+str(subroutineDecls)+',' \
                               +' with symtab "'+aUnit.symtab.debug()+'"')
            self.__processedFunctions.append(aUnit.uinfo.name)
            if self._keepFunctionDecl:
                newUnit = function2subroutine.\
                          convertFunction(aUnit,self.__myNewExecs,subroutineDecls)
                # if the unit has no parent, then it was the original unit.
                if aUnit.parent is None:
                    # create a parent
                    aUnit.parent = fortUnit.Unit()
                    # append new unit & original unit (already processed)
                    aUnit.parent.ulist.append(aUnit)
                    aUnit.parent.ulist.append(newUnit)
                    # return parent
                    aUnit = self.__myUnit.parent
                else:
                    aUnit.parent.ulist.append(newUnit)                    
            else:
                aUnit = function2subroutine.convertFunction(aUnit,self.__myNewExecs,subroutineDecls)
        return aUnit

    def __canonicalizeExecStmts(self,execList):
        DebugManager.debug('canonicalizing executable statements:')
        for anExecStmt in execList:
            DebugManager.debug('[Line '+str(anExecStmt.lineNumber)+']:')
            try:
                self.__canonicalizeExecStmt(anExecStmt)
            except InferenceError,e:
                raise CanonError('Caught InferenceError: '+e.msg,anExecStmt.lineNumber)
            except SymtabError,e: # add a lineNumber to SymtabErrors that don't have one
                e.lineNumber = e.lineNumber or anExecStmt.lineNumber
                raise e
            
    def __canonicalizeUseAndExternalStmts(self,aDecl):
        if (isinstance(aDecl,fs.UseStmt)):
            if isinstance(aDecl, fs.UseAllStmt):
                newRenameList=[]
                if aDecl.renameList:
                    for renameItem in aDecl.renameList:
                        scopedName=aDecl.moduleName+":"+renameItem.rhs
                        if function2subroutine.wasSubroutinized(scopedName): 
                            newRenameList.append(fs._PointerInit(function2subroutine.name_init+renameItem.lhs,
                                                                 function2subroutine.name_init+renameItem.rhs))
                            if (self._keepFunctionDecl):
                                newRenameList.append(renameItem)
                            aDecl.modified=True
                        else: 
                            newRenameList.append(renameItem)
                if (aDecl.modified):
                    aDecl.renameList=newRenameList
            elif isinstance(aDecl, fs.UseOnlyStmt):
                newOnlyList=[]
                for onlyItem in aDecl.onlyList:
                    if isinstance(onlyItem,fs._PointerInit):
                        if function2subroutine.wasSubroutinized(aDecl.moduleName+":"+onlyItem.rhs):
                            newOnlyList.append(fs._PointerInit(function2subroutine.name_init+onlyItem.lhs,
                                                               function2subroutine.name_init+onlyItem.rhs))
                            if (self._keepFunctionDecl):
                                newOnlyList.append(onlyItem)
                            aDecl.modified=True
                        else:
                            newOnlyList.append(onlyItem)
                    elif function2subroutine.wasSubroutinized(aDecl.moduleName+":"+onlyItem):
                        newOnlyList.append(function2subroutine.name_init+onlyItem)
                        if (self._keepFunctionDecl):
                            newOnlyList.append(onlyItem)
                        aDecl.modified=True
                    else: 
                        newOnlyList.append(onlyItem)
                if (aDecl.modified): 
                    aDecl.onlyList=newOnlyList
        elif (isinstance(aDecl,fs.ExternalStmt)):
            newProcedureNames=[]
            for p in aDecl.procedureNames:
                if (function2subroutine.wasSubroutinized(p)):
                    newProcedureNames.append(function2subroutine.name_init+p)
                    aDecl.modified=True
                else:
                    newProcedureNames.append(p)
            if (aDecl.modified):
                aDecl.procedureNames=newProcedureNames
                    

    def canonicalizeUnit(self):
        '''Recursively canonicalize \p aUnit'''
        DebugManager.debug(('+'*55)+' Begin canonicalize unit <'+str(self.__myUnit.uinfo)+'> '+(55*'+'))
        DebugManager.debug('local '+self.__myUnit.symtab.debug())
        DebugManager.debug('subunits (len ='+str(len(self.__myUnit.ulist))+'):')
        if (not self.__myUnit.parent and self.__myUnit.uinfo): # would have been added in the parent
            lead=self.__myUnit.uinfo.lead or ''
            addIt=False
            if self.__myUnit.ulist:
                addIt=True # if we have subunits add it regardless
            # try to get a lead and see if we have non-comment execs 
            ncExecsIter=itertools.ifilter(lambda l: not isinstance(l, fs.Comments),self.__myUnit.execs)
            try : 
                lead=ncExecsIter.next().lead
                addIt=True # made it here, i.e. we have execs and need to add it
                # see if we can get a better lead from non-comment decls 
                ncDeclsIter=itertools.ifilter(lambda l: not isinstance(l, fs.Comments),self.__myUnit.decls)
                try : 
                    lead=ncDeclsIter.next().lead
                except StopIteration, e:
                    pass # doesn't matter 
            except StopIteration, e: # no exec statements, no reason to add it here. 
                pass # still we may have to add it because of subunits
            if (addIt and not UnitCanonicalizer._overloadingMode):
                self.__myUnit.decls.insert(0, #always insert as the first decl to avoid ordering problems
                                           fs.UseAllStmt(moduleName=subroutinizedIntrinsics.getModuleName(),
                                                         renameList=None,
                                                         lead=lead))
        newList = []
        for subUnit in self.__myUnit.ulist:
            DebugManager.debug(5*'%'+'>'+'canon.canonicalizeUnit: ' \
                              +'canonicalizing subunit '+str(subUnit))
            # if the unit is new, as a result of function transformation,
            # skip processing
            if isinstance(subUnit.uinfo,fs.SubroutineStmt) and \
                     subUnit.uinfo.name.startswith(function2subroutine.name_init):
                DebugManager.debug(5*'%'+'>'+'\t skipping this subunit because we generated it')
                newList.append(subUnit)
            elif subUnit.uinfo.name in self.__processedFunctions:
                DebugManager.debug(5*'%'+'>'+'\t skipping this subunit because we already processed it')
                newList.append(subUnit)
            else:
                newUnit = UnitCanonicalizer(subUnit).canonicalizeUnit()
                newList.append(newUnit)
        self.__myUnit.ulist = newList
        
        subroutineBlock = []    
        for aDecl in self.__myUnit.decls:
            self.__canonicalizeUseAndExternalStmts(aDecl)
            self.__canonicalizeFunctionDecls(aDecl,subroutineBlock)

        ## replace the declaration statements for the unit
        if not isinstance(self.__myUnit.uinfo,fs.FunctionStmt):
            self.__myUnit.decls = self.__myNewDecls
            subroutineDecls = []
        else:
            subroutineDecls = self.__myNewDecls

        self.__myNewDecls = [] # empty it out, the following line may add new declarations
        self.__canonicalizeExecStmts(self.__myUnit.execs)
        # set the leading whitespace for the new declarations and add them to the unit
        for aDecl in self.__myNewDecls:
            aDecl.lead = self.__myUnit.uinfo.lead+'  '
        if not isinstance(self.__myUnit.uinfo,fs.FunctionStmt):
            self.__myUnit.decls.extend(self.__myNewDecls)
        subroutineDecls.extend(self.__myNewDecls)

        # replace the executable statements for the unit
        if not isinstance(self.__myUnit.uinfo,fs.FunctionStmt):
            self.__myUnit.execs = self.__myNewExecs

        # for function units, also create a corresponding subroutine
        self.__myUnit = self.__createNewSubroutine(self.__myUnit,subroutineDecls)
        
        # build list of old function/new subroutine name pairs 
        oldFuncnewSubPairs = self.__createFuncSubPairs()
        # accumulate an interface block and process it
        self.__processInterfaceBlocks(oldFuncnewSubPairs)

        self.__myUnit.decls = self.__myNewDecls
            
        DebugManager.debug(('+'*54)+' End canonicalize unit <'+str(self.__myUnit.uinfo)+'> '+(54*'+')+'\n\n')

        return self.__myUnit


