from _Setup import *

from PyUtil.typetab import TypetabEntry
from PyFort.inference import expressionType, makeDeclFromName
from PyFort.fortStmts import CommonStmt, getVarName
from PyUtil.typetab import globalTypeTable               

##
# reusable definitions related to the active type
#

class ActiveTypeHelper(object):
    
    ##
    # set something here for the unit tests
    #
    _replacement_type = 'active' 

    ##
    # is a dictionary because we have an abstract and a replacement active type
    #
    _activeTypeId={}
    
    @staticmethod
    def setPlaceholderActiveType(activeTypeName):
        actactiveTypeName=activeTypeName.lower()
        if (not activeTypeName in ActiveTypeHelper._activeTypeId): 
            ActiveTypeHelper._activeTypeId[activeTypeName]=globalTypeTable.enterNamedType(activeTypeName, None)
            entry=globalTypeTable.lookupTypeId(ActiveTypeHelper._activeTypeId[activeTypeName])
            entry.setReferenced()
            entry.setTypeEntryToGlobal()
        
    @staticmethod
    def getActiveTypeId(activeTypeName):
        activeTypeName=activeTypeName.lower()
        if (not activeTypeName in ActiveTypeHelper._activeTypeId):
            for k,v in globalTypeTable.ids.items():
                if (isinstance(v.entryKind,TypetabEntry.NamedTypeEntryKind) \
                    and \
                    v.entryKind.symbolName.lower()==activeTypeName):
                    ActiveTypeHelper._activeTypeId[activeTypeName]=k
                    break
            if (not activeTypeName in ActiveTypeHelper._activeTypeId):
                raise TypeError("no active type found")
        return ActiveTypeHelper._activeTypeId[activeTypeName]

    @staticmethod
    def isActive(aUnit,Exp,lineNumber,activeTypeName):
        varName=getVarName(Exp,lineNumber)
        activeTypeName=activeTypeName.lower()
        expType=expressionType(varName,aUnit.symtab,lineNumber)
        if isinstance(expType.entryKind,TypetabEntry.ArrayEntryKind):
            expType=expType.getBaseTypeEntry()
        if isinstance(expType.entryKind,TypetabEntry.NamedTypeEntryKind):
            if (expType.entryKind.symbolName.lower()==activeTypeName):
                return True
        return False
    
    @staticmethod
    def __activateTypetabEntry(typeEntry,activeTypeName):
        ''' recursively descend through the type entries to find a Built-in base type to be changed to the active type '''
        baseEntry=typeEntry.getBaseTypeEntry()
        if (baseEntry.typetab_id==typeEntry.typetab_id):
            raise TypeError("cannot activate variable of non-built-in type "+typeEntry.debug())
        if isinstance(baseEntry.entryKind,TypetabEntry.BuiltInEntryKind):
            typeEntry.entryKind.typetab_id=ActiveTypeHelper.getActiveTypeId(activeTypeName)
        else:
            ActiveTypeHelper.__activateTypetabEntry(baseEntry)
    
    @staticmethod
    def activateSymtabEntry(aUnit,var,activeTypeName):
        ''' change a symbols type information to the active type '''
        if (not ActiveTypeHelper.isActive(aUnit,var, 0, activeTypeName)):
                symTabEntry=aUnit.symtab.lookup_name(var)
                expTypeEntry=globalTypeTable.lookupTypeId(symTabEntry.typetab_id)
                if isinstance(expTypeEntry.entryKind,TypetabEntry.BuiltInEntryKind):
                    symTabEntry.typetab_id=ActiveTypeHelper.getActiveTypeId(activeTypeName)
                else:
                    ActiveTypeHelper.__activateTypetabEntry(expTypeEntry,activeTypeName)
    
    @staticmethod
    def getCommonDecls(aUnit,activeTest,cTriples):
        ''' collect common block declarations if there are any active variables in the common block '''
        for decl in aUnit.decls:
            if isinstance(decl,CommonStmt):
                activeVars=set()
                commonVarDecls=set()
                for var in decl.declList:
                    if activeTest(var,decl):
                        activeVars.add(var.lower())
                    commonVarDecls.add(makeDeclFromName(aUnit.symtab,var,decl.lineNumber))
                # avoid duplications and skip common blocks with no active variables
                if activeVars and not decl.name.lower() in cTriples:
                    cTriples[decl.name.lower()]=(decl,commonVarDecls,activeVars)