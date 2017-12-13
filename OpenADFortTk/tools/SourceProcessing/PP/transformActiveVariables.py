from PyUtil.debugManager import DebugManager

from PP.activeModuleHelper import addReferenceIfNeeded, getActiveModuleUse
from PyFort.fortUnit import fortUnitIterator
from PyFort.intrinsic import is_intrinsic,is_inquiry,getGenericName
import PyFort.fortStmts as fs
import PyFort.fortExp as fe
from PP.activeTypeHelper import ActiveTypeHelper
from PyFort.inference import expressionType, makeDeclFromName
from PyUtil.caselessDict import caselessDict

class TransformError(Exception):
    '''Exception for errors that occur during active variable transformation'''
    def __init__(self,msg,lineNumber):
        self.msg = msg
        self.lineNumber = lineNumber

    def __str__(self):
        errString='\nERROR: TransformError at line '+str(self.lineNumber)+':'+str(self.msg)
        return (errString)

class TransformActiveVariables(object):
    'class to perform rudimentary tranformations of active variables on non-transformed files'

    _replacement_type = 'active' 

    @staticmethod
    def setReplacementType(replacementType):
        TransformActiveVariables._replacement_type = replacementType

    # info for common blocks which contain active variables
    _cTriples = caselessDict()
    _cNames = set()
    
    # populates the commonBlocks array with the names of all common blocks which contain active variables
    @staticmethod
    def getActiveVars(fileName,inputFormat=None):
        for aUnit in fortUnitIterator(fileName,inputFormat):
            ActiveTypeHelper.getCommonDecls(aUnit, 
                                            TransformActiveVariables(aUnit).__isActiveNamedType, 
                                            TransformActiveVariables._cTriples)

    def __init__(self, aUnit):
        self.__myUnit = aUnit
        self._varsToActivate=set()
        self.__newDecls=[]

    def __isActiveNamedType(self,Exp,lineNumber):
        return ActiveTypeHelper.isActive(self.__myUnit, Exp, lineNumber, TransformActiveVariables._replacement_type)

    def __isActive(self,Exp,parentStmt):
        varName=fs.getVarName(Exp,parentStmt.lineNumber)
        if (self.__isActiveNamedType(varName,parentStmt.lineNumber) or varName.lower() in self._varsToA146ctivate):
            return True
        return False

    # recursively transforms an expression if it contains a variable in
    # the activeVars array by adding %v to it
    # if variable is implicitly declared, create an active declaration for it & add it to list of newDecls
    def __transformActiveVariableReferences(self,Exp,parentStmt):
        DebugManager.debug('TransformActiveVariables.__transformActiveVariableReferences called on "'+str(Exp)+'"')
        if isinstance(Exp,list) :
            Exp = [self.__transformActiveVariableReferences(s,parentStmt) for s in Exp]
        elif isinstance(Exp,fe.App) and isinstance(Exp,str) and is_intrinsic(Exp.head) and not is_inquiry(Exp.head):
            Exp.head=getGenericName(Exp.head)
            Exp.args=[self.__transformActiveVariableReferences(arg,parentStmt) for arg in Exp.args]
        elif isinstance(Exp,fe.App):
            try:
                if not is_inquiry(Exp.head):
                    if (self.__isActive(Exp,parentStmt)):
                        Exp = fe.Sel(Exp,"v")
                        parentStmt.modified = True
                    if hasattr(Exp,'args'):
                        Exp.args=[self.__transformActiveVariableReferences(arg,parentStmt) for arg in Exp.args]
                        parentStmt.modified=True
            except:
                pass            
        elif isinstance(Exp,fe.Sel):
            try:
                if (self.__isActive(Exp.head ,parentStmt) and not Exp.proj.lower()=="d"):
                    Exp = fe.Sel(Exp,"v")
                    parentStmt.modified = True
                if hasattr(Exp,'args'):
                    Exp.args=[self.__transformActiveVariableReferences(arg,parentStmt) for arg in Exp.args]
                    parentStmt.modified=True
            except:
                pass  
        elif isinstance(Exp,str):
            try:
                if self.__isActive(Exp,parentStmt):
                    Exp = fe.Sel(Exp,"v")
                    parentStmt.modified = True
                if hasattr(Exp,'args'):
                    Exp.args=[self.__transformActiveVariableReferences(arg,parentStmt) for arg in Exp.args]
                    parentStmt.modified=True
            except:
                pass
        elif hasattr(Exp, "_sons"):
            for aSon in Exp.get_sons() :
                theSon = getattr(Exp,aSon)
                newSon = self.__transformActiveVariableReferences(theSon,parentStmt)
                Exp.set_son(aSon,newSon)
        DebugManager.debug('TransformActiveVariables.__transformActiveVariableReferences returning '+str(Exp))
        return Exp
        
    def __activateCommonBlockDecls(self):
        # get list of vars which are equivalenced to active variables (and therefore need to be active)
        for aDecl in self.__myUnit.decls:
            if (isinstance(aDecl,fs.TypeDecl)):
                for var in aDecl.decls:
                    var=fs.getVarName(var,aDecl.lineNumber).lower()
                    symTabEntry=self.__myUnit.symtab.lookup_name(var)
                    if (symTabEntry.origin and "common" in symTabEntry.origin):
                        cbName=symTabEntry.origin.split(':')[1]
                        if cbName in TransformActiveVariables._cTriples: 
                            if not var.lower() in map(lambda l:l.lower(),TransformActiveVariables._cTriples[cbName][0].declList):
                                raise TransformError("variable "+var+" of common block "+cbName+" not found in the transformaed version of the common block")
                            if (var.lower() in TransformActiveVariables._cTriples[cbName][2]): 
                                self._varsToActivate.add(var.lower())

    def __activateThroughEquivalence(self):
        activeVarCount=len(self._varsToActivate)
        while(activeVarCount!=len(self._varsToActivate)):
            for aDecl in self.__myUnit.decls:
                if (isinstance(aDecl,fs.EquivalenceStmt)):
                    for nlist in aDecl.nlists:
                        newList = set([])
                        active = False
                        for item in nlist:
                            for var in item:
                                # if one variable is active, they all need to be active;
                                # add non-active vars to a list of vars to be activated
                                if self.__isActiveNamedType(var,aDecl.lineNumber):
                                    active=True
                                # if equivalenced to an active variable from a common block, the variable will
                                # be in localActiveVars
                                elif fs.getVarName(var,aDecl.lineNumber).lower() in self._varsToActivate:
                                    active=True
                                newList.add(fs.getVarName(var,aDecl.lineNumber).lower())
                        if len(newList)>0 and active:
                            self._varsToActivate.update(newList)
            activeVarCount=len(self._varsToActivate)

    def __removeActiveFromTypeDecl(self,theDecl):
        dropList=[]
        for var in theDecl.get_decls():
            strippedVar=fs.getVarName(var,theDecl.lineNumber).lower()
            if strippedVar in self._varsToActivate:
                dropList.append(var)
        map(theDecl.decls.remove,dropList)
        if dropList:
            theDecl.modified=True # triggers redoing the rawline

    # helper function for createActiveTypeDecls
    # adds a new active declaration to the list of declarations
    def __insertNewActiveTypeDecl(self,activeDecls,oldDecl,oldDeclList):
        if len(activeDecls)>0:
            newDecl=fs.DrvdTypeDecl([self._replacement_type],[],activeDecls,lead=oldDecl.lead)
            self.__newDecls.append(newDecl)
        if len(oldDeclList)>0:
            oldDecl.modified=True
            self.__newDecls.append(oldDecl)

    def __changeTypeDecls(self):
        ''' drop the variables to be activated from the passive declaration statements and create 
        new declaration statements with the active type'''
        lead=None
        for aDecl in self.__myUnit.decls:
            if isinstance(aDecl,(fs.TypeDecl,fs.DimensionStmt)):
                if aDecl.lead:
                    lead=aDecl.lead
                self.__removeActiveFromTypeDecl(aDecl)
                if (aDecl.decls): # still something to declare
                    self.__newDecls.append(aDecl)
            else:
                self.__newDecls.append(aDecl)
        for var in self._varsToActivate:
            ActiveTypeHelper.activateSymtabEntry(self.__myUnit, var, TransformActiveVariables._replacement_type)
            nDecl=makeDeclFromName(self.__myUnit.symtab,var,0)
            if lead:
                nDecl.lead=lead
            self.__newDecls.append(nDecl)
        
    def transformUnit(self):
        '''transform all decl and exec statements in the file if they contain a variable to be activated '''
        DebugManager.debug(('+'*55)+' transforming active varioables in unit <'+str(self.__myUnit.uinfo)+'> '+(55*'+'))
        DebugManager.debug('local '+self.__myUnit.symtab.debug())
        DebugManager.debug('subunits (len = '+str(len(self.__myUnit.ulist))+'):')

        for subUnit in self.__myUnit.ulist :
            DebugManager.debug(str(subUnit))
            TransformActiveVariables(subUnit).transformUnit()

        # add active module
        addReferenceIfNeeded(self.__myUnit,self.__newDecls,getActiveModuleUse())

        self.__activateCommonBlockDecls()        
        self.__activateThroughEquivalence()
        
        self.__changeTypeDecls()
        self.__myUnit.decls=self.__newDecls

        for anExec in self.__myUnit.execs:
            DebugManager.debug('TransformActiveVariables.transformUnit: '\
                              +'processing exec statement "'+str(anExec)+'"',
                               lineNumber=anExec.lineNumber)
            if isinstance(anExec,fs.AllocateStmt) or \
               isinstance(anExec,fs.DeallocateStmt) : continue
            elif hasattr(anExec, "_sons"):
                for aSon in anExec.get_sons() :
                    theSon = getattr(anExec,aSon)
                    if isinstance(theSon,fs.AllocateStmt) or isinstance(theSon,fs.DeallocateStmt) :
                        continue
                    newSon = self.__transformActiveVariableReferences(theSon,anExec)
                    anExec.set_son(aSon,newSon)
            DebugManager.debug('TransformActiveVariables.transformUnit: resulting exec statement: "'+str(anExec)+'"')
        DebugManager.debug('TransformActiveVariables.transformUnit: finished transforming exec statements for this unit.  execs = '+str(self.__myUnit.execs))
        return self.__myUnit
