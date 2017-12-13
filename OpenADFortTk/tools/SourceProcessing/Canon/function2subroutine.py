'''
Function to Subroutine conversion
'''

from _Setup import *
from optparse import OptionParser

from PyFort.fortUnit import Unit,fortUnitIterator
from PyUtil.debugManager import DebugManager
from PyUtil.symtab import Symtab,SymtabError, SymtabEntry
import PyFort.fortStmts as fs
import PyFort.fortExp as fe
import copy

class FunToSubError(Exception):
    '''exception for errors that occur during the transformation of function unit definitions to subroutine definitions'''
    def __init__(self,msg):
        self.msg  = msg

    def __str__(self):
        errString='\nERROR: FunToSubError: '+str(self.msg)
        return (errString)

name_init = 'oad_s_'

###
# we track all converted names with scope qualifier
#
ourConvertedScopedNameList = []

def wasSubroutinized(scopedName):
    return (scopedName.lower() in ourConvertedScopedNameList)

ourReferencedBeforeSeenScopedNameList=[]

def trackAsReferencedBeforeSeen(scopedName):
    global ourReferencedBeforeSeenScopedNameList
    if (not (scopedName.lower() in ourReferencedBeforeSeenScopedNameList)):
        ourReferencedBeforeSeenScopedNameList.append(scopedName)

def wasReferencedBeforeSeen(scopedName):
    return (scopedName.lower() in ourReferencedBeforeSeenScopedNameList)

def createTypeDecl(type_kw,mod,attrs,outParam,aLead):
    DebugManager.debug(10*'-'+'>'+'called function2subroutine.createTypeDecl ' \
                     + 'with type keyword "'+type_kw+'",' \
                     +' mod = "'+str(mod)+'",' \
                     +' attrs = "'+str(attrs)+'",' \
                     +' outParam = "'+str(outParam)+'",'\
                     +' lead = "'+str(aLead)+'"')
    newAttrs = attrs # should be deepcopied already, if necessary
    newAttrs.append(fe.App('intent',['out']))
    # look up the class in the kwBuiltInTypesTbl and invoke the ctor which has the same signature for all type classes
    if (type_kw in fs.kwBuiltInTypesTbl.keys()): 
        return (fs.kwBuiltInTypesTbl[type_kw])(mod=mod,attrs=newAttrs,decls=[outParam],lead=aLead)
    else : # must be derived type
        return fs.DrvdTypeDecl(mod=mod,attrs=newAttrs,decls=[outParam],lead=aLead)

def convertFunctionDecl(aDecl,oldFuncnewSubPairs):
    DebugManager.debug(10*'-'+'>'+'called function2subroutine.convertFunctionDecl ' \
                       + 'on declaration statement "'+str(aDecl)+'"' \
                     +' with oldFuncnewSubPairs = "'+str(oldFuncnewSubPairs)+'"')
    newDecl = copy.deepcopy(aDecl)
    modified = False
    if hasattr(newDecl,"_sons"): # list of attribute names of Decl instances
        for anAttrName in newDecl.get_sons():
            anAttrValue = getattr(newDecl,anAttrName)
            if isinstance(anAttrValue,list):
                if isinstance(newDecl,fs.VarAttrib):
                    newAttrValue = []
                    for anOldNewPair in oldFuncnewSubPairs :
                        for anAttrValuePart in anAttrValue:
                            if (isinstance(anAttrValuePart,str) and anOldNewPair[0].lower()==anAttrValuePart.lower()):
                                newAttrValue.append(anOldNewPair[1])
                                modified = True
                                break
                    newDecl.set_son(anAttrName,newAttrValue)
                else:
                    for anOldNewPair in oldFuncnewSubPairs :
                        for anAttrValuePart in anAttrValue:
                            if (isinstance(anAttrValuePart,str) and anOldNewPair[0].lower()==anAttrValuePart.lower()):
                                anAttrValue.remove(anAttrValuePart)
                                anAttrValue.append(anOldNewPair[1])
                                newDecl.modified = True
                                modified = True
                                break                    
            else : # not a list 
                for anOldNewPair in oldFuncnewSubPairs :
                    if (isinstance(anAttrValue,str) and anAttrValue.lower() == anOldNewPair[0].lower()) :
                        newDecl.set_son(anAttrName,anOldNewPair[1])
                        modified = True
    return (newDecl,modified)

def convertInterfaceBlock(oldInterfaceBlock,oldFuncnewSubPairs,keepFunctionDef):
    newInterfaceBlock = []
    createdNewBlock = False
    for aDecl in oldInterfaceBlock:
        if isinstance(aDecl,fs.ProcedureStmt):
            for aModProcDecl in oldInterfaceBlock:
                (newDecl,modified) = convertFunctionDecl(aModProcDecl,oldFuncnewSubPairs)
                if modified:
                    createdNewBlock = True
                newInterfaceBlock.append(newDecl)
            if createdNewBlock:
                if not isinstance(newInterfaceBlock[0],fs.InterfaceStmt):
                    raise FunToSubError(sys._getframe().f_code.co_name+": error transforming interface block")
                # rename interface
                old_name = newInterfaceBlock[0].get_name()
                new_name = name_init+old_name
                newInterfaceBlock[0].set_name(new_name)
                if newInterfaceBlock[-1].get_name():
                    newInterfaceBlock[-1].set_name(new_name)
                if (keepFunctionDef):
                    newInterfaceBlock.extend(oldInterfaceBlock)
                global ourConvertedScopedNameList
                ourConvertedScopedNameList.append(old_name.lower())
            return newInterfaceBlock
    for aDecl in oldInterfaceBlock: # not a block with module procedures
        (newDecl,modified) = convertFunctionDecl(aDecl,oldFuncnewSubPairs)
        newInterfaceBlock.append(aDecl)
        if modified:
            newInterfaceBlock.append(newDecl)
    return newInterfaceBlock

def convertFunctionOrEntryStmt(theStmt,theUnit):
    DebugManager.debug(10*'-'+'>'+'called function2subroutine.convertFunctionOrEntryStmt on '+str(theStmt))
    if (not (isinstance(theStmt,fs.FunctionStmt) or isinstance(theStmt,fs.EntryStmt))):
        raise FunToSubError(sys._getframe().f_code.co_name+': convertFunctionOrEntryStmt called for '+str(theStmt))
    if theStmt.result is None:
        outParam = fs._NoInit(theStmt.name.lower())
    else:
        outParam = fs._NoInit(theStmt.result.lower())
    args=[]
    for arg in theStmt.args:
        symtabEntry=theUnit.symtab.lookup_name(arg)
        if (symtabEntry and isinstance(symtabEntry.entryKind,SymtabEntry.FunctionEntryKind) and symtabEntry.origin=="dummy") :
            args.append(fe.copyExp(name_init+arg.lower()))
        else:
            args.append(fe.copyExp(arg))
    args.append(outParam)
    name = name_init+theStmt.name.lower()
    if isinstance(theStmt,fs.FunctionStmt):
        convertedStmt = fs.SubroutineStmt(name,args,theStmt.qualifiers,lead=theStmt.lead)
    else:
        convertedStmt = fs.EntryStmt(name,args,lead=theStmt.lead)
        global ourConvertedScopedNameList
        scopedName=''
        funUnitParent=theUnit.parent
        while (funUnitParent) : 
                    scopedName=funUnitParent.uinfo.name+':'+scopedName
                    funUnitParent=funUnitParent.parent
        scopedName+=theStmt.name
        ourConvertedScopedNameList.append(scopedName.lower())
        if (wasReferencedBeforeSeen(scopedName)):
            raise FunToSubError(sys._getframe().f_code.co_name+': function '+theStmt.name+' was referenced before we saw this definition and the reference was not subroutinized; try to move up the definition before the reference')
    return (outParam,convertedStmt)

def createResultDecl(functionStmt,outParam):
    DebugManager.debug(10*'-'+'>'+'called function2subroutine.createResultDecl ' \
                     + 'on function statement "'+str(functionStmt)+'"' \
                     +' with out parameter "'+str(outParam)+'"')
    if functionStmt.ty is not None:
        (type_name,mod) = functionStmt.ty
        newDecl = createTypeDecl(type_name.kw,mod,[],outParam,functionStmt.lead)
        return newDecl
    return None

def updateResultDecl(decl,outParam):
    DebugManager.debug(10*'-'+'>'+'called function2subroutine.updateResultDecl ' \
                     + 'on declaration statement "'+str(decl)+'",' \
                     +' outParam = "'+str(outParam)+'"')
    if (str(outParam) == decl) or \
           (hasattr(decl,'lhs') and (str(outParam) == decl.lhs)):
        return True
    else:
        return False

def updateTypeDecl(aDecl,outParam,declList):
    DebugManager.debug(10*'-'+'>'+'called function2subroutine.updateTypeDecl ' \
                     + 'on declaration statement "'+str(aDecl)+'",' \
                     +' outParam = "'+str(outParam)+'",' \
                     +' declList = "'+str(declList)+'"')
    resultDeclCreated = False
    newDecl=None
    declCopy = copy.deepcopy(aDecl)
    if (len(declCopy.decls) == 1) and \
           updateResultDecl(declCopy.get_decls()[0],outParam):
        newDecl = createTypeDecl(declCopy.kw,declCopy.get_mod(),declCopy.get_attrs(),outParam,declCopy.lead)
        declCopy = None
        resultDeclCreated = True
    else:
        for decl in declCopy.get_decls():
            if updateResultDecl(decl,outParam):
                # deep copy attrs so declCopy attrs aren't modified
                newDecl = createTypeDecl(declCopy.kw,declCopy.get_mod(),fe.copyExp(declCopy.get_attrs()),outParam,declCopy.lead)
                declCopy.decls.remove(decl)
                declCopy.modified = True
                resultDeclCreated = True
    return (declCopy,newDecl,resultDeclCreated)

def convertFunction(functionUnit,newExecs,newDecls):
    '''converts a function unit definition to a subroutine unit definition'''
    DebugManager.debug(10*'-'+'>'+'called function2subroutine.convertFunction ' \
                     + 'on function unit statement "'+str(functionUnit)+'",' \
                     + 'with newDecls='+str(newDecls)+',' \
                     +' with symtab "'+functionUnit.symtab.debug()+'"')
    newSubUnit = Unit(parent=functionUnit.parent,fmod=functionUnit.fmod)
    (outParam,newSubUnit.uinfo) = convertFunctionOrEntryStmt(functionUnit.uinfo,functionUnit)
    newSubUnit.cmnt = functionUnit.cmnt
    newSubUnit.contains = functionUnit.contains
    newSubUnit.ulist = functionUnit.ulist
    global ourConvertedScopedNameList
    scopedName=''
    funUnitParent=functionUnit.parent
    while (funUnitParent) : 
        scopedName=funUnitParent.uinfo.name+':'+scopedName
        funUnitParent=funUnitParent.parent
    scopedName+=functionUnit.uinfo.name
    if (wasReferencedBeforeSeen(scopedName)):
        raise FunToSubError(sys._getframe().f_code.co_name+': function '+theStmt.name+' was referenced before we saw this definition and the reference was not subroutinized; try to move up the definition before the reference')
    ourConvertedScopedNameList.append(scopedName.lower())
    newList = []
    for subUnit in newSubUnit.ulist:
        # no need to process function statements, since they have already been
        # recursively processed in canon. => drop function stmts & keep other sub units
        if not isinstance(subUnit.uinfo,fs.FunctionStmt):
            newList.append(subUnit)
    newSubUnit.ulist = newList

    resultDecl = createResultDecl(functionUnit.uinfo,outParam)
    if resultDecl is not None:
        funTypeFound = True
    else:
        funTypeFound = False

    # iterate over decls for functionUnit
    for aDecl in newDecls:
        if not funTypeFound and isinstance(aDecl,fs.TypeDecl):
            (aDecl,resultDecl,funTypeFound) = updateTypeDecl(aDecl,outParam,newSubUnit.decls)
        if aDecl is not None:
            newSubUnit.decls.append(aDecl)

    if resultDecl is not None:
        if (newSubUnit.decls and (not isinstance(newSubUnit.decls[-1],fs.Comments))):
            resultDecl.lead = newSubUnit.decls[-1].lead
        # append declaration for new out parameter
        newSubUnit.decls.append(resultDecl)
        
    # iterate over execs for functionUnit
    for anExec in newExecs:
        if isinstance(anExec,fs.EntryStmt):
            (anEntryOutParam,entryStmt) = convertFunctionOrEntryStmt(anExec,functionUnit)
            newSubUnit.execs.append(entryStmt)
        else:
            newSubUnit.execs.append(anExec)

    # iterate over end stmts for functionUnit
    for endStmt in functionUnit.end:
        newEndStmts = []
        if isinstance(endStmt,fs.EndStmt):
            newEndStmt=fs.EndSubroutineStmt(newSubUnit.uinfo.name,lineNumber=endStmt.lineNumber,\
                                  label=endStmt.label,lead=endStmt.lead)
            newEndStmts.append(newEndStmt)
        else:
            newEndStmts.append(endStmt)
    newSubUnit.end = newEndStmts    

    return newSubUnit
