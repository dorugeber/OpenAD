'''add methods to fort stmts to build units
'''

import string
import copy
from _Setup import *

from PyUtil.typetab import TypetabEntry
from PyUtil.symtab import Symtab,SymtabEntry,SymtabError, GenericInfo, FormalArgs, globalTypeTable
from PyUtil.debugManager import DebugManager

import PyFort.fortStmts     as fs
import PyFort.fortExp       as fe

class InterfaceInfo:
    def __init__(self,name,parent):
        self.name=name # the interface name
        self.parent=parent # interfaces can be nested
        self._in_procedureFuncDecl=None # if we are in an function decl inside of the interface 

def typesep(dd,default_dims):
    '''return name and dimensions for a given decl entry
    a type declaration will either be a simple var (string)
    or an App expression
    '''
    _helper = lambda e: isinstance(e,str) and (e,default_dims) or (e.head,e.args)

    d = dd.lhs

    if isinstance(d,fe.Ops): return _helper(d.a1)
    return _helper(d)

def _beginDrvdTypeDefn(aDrvdTypeDefn,curr):
    'derived type definition -- record type in symbol table and set the name on the unit'
    localSymtab = curr.val.symtab
    theSymtabEntry = localSymtab.lookup_name_local(aDrvdTypeDefn.name)
    access=None # DrvdTypeDefn currently cannot parse qualifiers
    curr.val._in_drvdType=aDrvdTypeDefn.name
    if theSymtabEntry: # already in symtab, shouldn't happen  
        theSymtabEntry.enterEntryKind(SymtabEntry.DerivedTypeEntryKind(aDrvdTypeDefn.base_type))
    else :
        newSymtabEntry = SymtabEntry(SymtabEntry.DerivedTypeEntryKind(aDrvdTypeDefn.base_type),
                                     dimensions=None,
                                     length=None,
                                     origin='local',
                                     access=access)
        typetab_id=globalTypeTable.enterNamedType(curr.val._in_drvdType,curr.val.symtab)    
        newSymtabEntry.typetab_id=typetab_id
        DebugManager.debug('defn "'+str(aDrvdTypeDefn)+'" NOT already present in symbol table => adding '+str(newSymtabEntry.debug(aDrvdTypeDefn.name)))
        localSymtab.enter_name(aDrvdTypeDefn.name,newSymtabEntry)
    # add to type table & set typetab_id
    return aDrvdTypeDefn

def _typebound_proc_block(aContainsStmt, curr):
    'if a contains statement occurs in a derived type definition, then there is a type-bound procedure block, and fs.ContainsStmt is a decl'
    if curr.val._in_drvdType:
        return True
    return False

def _endDrvdTypeDefn(aEndDrvdTypeDefnStmt,curr):
    'derived type definition end  -- unset the name on the unit'
    if (curr.val._drvdTypeDefaultAccess):
        prefix=curr.val._in_drvdType+':'
        lenPrefix=len(prefix)
        for name,entry in curr.val.symtab.ids.items():
            if (len(name)>lenPrefix and name[:lenPrefix]==prefix):
                entry.setDefaultAccess(curr.val._drvdTypeDefaultAccess)
    curr.val._in_drvdType=None
    return aEndDrvdTypeDefnStmt

def _processTypedeclStmt(aTypeDeclStmt,curr):
    'type declaration -- record type in symbol table'
    localSymtab = curr.val.symtab
    newType = (aTypeDeclStmt.__class__,aTypeDeclStmt.mod)
    newLength = None
    DebugManager.debug('[Line '+str(aTypeDeclStmt.lineNumber)+']: '+sys._getframe().f_code.co_name+' for '+repr(aTypeDeclStmt)+' with default dimensions '+str(aTypeDeclStmt.dimension))
    access=None
    inDrvdTypeDefn=curr.val._in_drvdType
    for anAttribute in aTypeDeclStmt.attrs :
        if isinstance(anAttribute,str) and anAttribute.lower() in Symtab.ourSpecificAccessKWs :
            access= anAttribute.lower()
    for aDecl in aTypeDeclStmt.decls:
        DebugManager.debug('\tProcessing decl '+repr(aDecl)+' ... ',newLine=False)
        (name,newDimensions) = typesep(aDecl,aTypeDeclStmt.dimension)
        if inDrvdTypeDefn:
            name=inDrvdTypeDefn+":"+name
        try:
            # set the length for character statements
            if (aTypeDeclStmt.kw_str == 'character'):
                newLength = aTypeDeclStmt.mod and aTypeDeclStmt.mod[0] \
                                               or 1
                # extract the name and length for character declarations such as "character foo*7"
                if (isinstance(aDecl,fs._NoInit) or isinstance(aDecl,fs._AssignInit)):
                    if (isinstance(aDecl.lhs,fe.Ops) and aDecl.lhs.op == '*'):
                        DebugManager.debug('(recognized as a character statement with asterisk length specification) ... ',newLine=False)
                        name = aDecl.lhs.a1
                        newLength = aDecl.lhs.a2
            theSymtabEntry = localSymtab.lookup_name_local(name)
            if isinstance(aDecl,fs._AssignInit):
                theTmpDeclStmt=aTypeDeclStmt.__class__(aTypeDeclStmt.get_mod(),aTypeDeclStmt.get_attrs(),[aDecl.lhs])
            else:
                theTmpDeclStmt=aTypeDeclStmt.__class__(aTypeDeclStmt.get_mod(),aTypeDeclStmt.get_attrs(),[aDecl])
            if theSymtabEntry: # already in symtab -> enter new information (taking exception to any conflicts)
                DebugManager.debug('decl "'+str(aDecl)+'" already present in local symbol table as '+str(theSymtabEntry.debug(name)))
                theSymtabEntry.enterType(newType,localSymtab)
                if (theSymtabEntry.dimensions and (newDimensions is None)):
                    theTmpDeclStmt.dimension=theSymtabEntry.dimensions
                else:
                    theSymtabEntry.enterDimensions(newDimensions)
                theSymtabEntry.enterLength(newLength)
                if (isinstance(aDecl,fs._PointerInit) or isinstance(aDecl,fs._AssignInit) and localSymtab.isConstInit(aDecl.rhs)):
                    theSymtabEntry.enterConstInit(localSymtab.getConstInit(aDecl.rhs))
                if inDrvdTypeDefn:
                    theSymtabEntry.enterDrvdTypeName(inDrvdTypeDefn)
                # for function/subroutine entries, also update this information in the parent symbol table
                if localSymtab.parent and theSymtabEntry.entryKind.__class__ in (SymtabEntry.FunctionEntryKind,SymtabEntry.SubroutineEntryKind) and not (theSymtabEntry.origin=='external'):
                    parentSymtabEntry=localSymtab.parent.lookup_name_local(name)
                    if (not parentSymtabEntry):
                        localSymtab.replicateEntry(name,'local',name,localSymtab.parent,replicatingUp=True)
                        DebugManager.debug('[Line '+str(aTypeDeclStmt.lineNumber)+']: new PARENT unit symtab entry (see above)')
                    else:
                        localSymtab.augmentParentEntry(theSymtabEntry,parentSymtabEntry,name)            
                        DebugManager.debug('[Line '+str(aTypeDeclStmt.lineNumber)+']: updated PARENT unit symtab entry '+parentSymtabEntry.debug(name))
                typetab_id = globalTypeTable.getType(theTmpDeclStmt,localSymtab)
                theSymtabEntry.typetab_id=typetab_id
            else: # no symtab entry -> create one
                if aTypeDeclStmt.parameter:
                    newSymtabEntry = SymtabEntry(SymtabEntry.VariableEntryKind(),
                                                 type=newType,
                                                 dimensions=newDimensions,
                                                 length=newLength,
                                                 origin='local',
                                                 access=access)
                else:
                    newSymtabEntry = SymtabEntry(SymtabEntry.GenericEntryKind(),
                                                 type=newType,
                                                 dimensions=newDimensions,
                                                 length=newLength,
                                                 origin='local',
                                                 access=access)
                if (isinstance(aDecl,fs._PointerInit) or isinstance(aDecl,fs._AssignInit) and localSymtab.isConstInit(aDecl.rhs)):
                    newSymtabEntry.enterConstInit(localSymtab.getConstInit(aDecl.rhs))
                elif aTypeDeclStmt.parameter and isinstance(aDecl,fs._AssignInit):
                    newSymtabEntry.enterConstInit(aDecl.rhs)
                if inDrvdTypeDefn:
                    newSymtabEntry.enterDrvdTypeName(inDrvdTypeDefn)
                localSymtab.enter_name(name,newSymtabEntry)
                typetab_id = globalTypeTable.getType(theTmpDeclStmt,localSymtab)
                newSymtabEntry.typetab_id=typetab_id
                DebugManager.debug('decl "'+str(aDecl)+'" NOT already present in symbol table => adding '+str(newSymtabEntry.debug(name)))
            unitSymbolEntry=localSymtab.lookup_name_level(curr.val.name())[0]
            if (unitSymbolEntry and isinstance(unitSymbolEntry.entryKind,SymtabEntry.FunctionEntryKind) and  unitSymbolEntry.genericInfo and unitSymbolEntry.genericInfo.genericName):
                genericSymbolEntry=localSymtab.lookup_name(unitSymbolEntry.genericInfo.genericName)
                if (genericSymbolEntry is None):
                    raise SymtabError('cannot find generic '+unitSymbolEntry.genericInfo.genericName+' for specific '+curr.val.name()+' entry: '+unitSymbolEntry.debug(curr.val.name()))
                if (genericSymbolEntry.genericInfo is None or (not curr.val.name().lower() in genericSymbolEntry.genericInfo.resolvableTo)):
                    raise SymtabError('no info available for specific '+curr.val.name()+' in generic entry: '+genericSymbolEntry.debug(unitSymbolEntry.genericInfo.genericName))
                specificFormalArgs=localSymtab.lookup_name(unitSymbolEntry.genericInfo.genericName).genericInfo.resolvableTo[curr.val.name().lower()]
                if (specificFormalArgs is None):
                    raise SymtabError('no arguments set for specific '+curr.val.name()+' under generic '+ unitSymbolEntry.genericInfo.genericName+' entry: '+localSymtab.lookup_name(unitSymbolEntry.genericInfo.genericName).debug(unitSymbolEntry.genericInfo.genericName))
                if name.lower() in specificFormalArgs:
                    se=localSymtab.lookup_name_local(name)
                    specificFormalArgs[name.lower()]=(se.typetab_id,se.dimensions)
                    DebugManager.debug('recorded type in '+str(id(specificFormalArgs))+str(specificFormalArgs))
        except SymtabError,e: # add lineNumber and symbol name to any SymtabError we encounter
            e.lineNumber = e.lineNumber or aTypeDeclStmt.lineNumber
            e.symbolName = e.symbolName or name
            raise e
    return aTypeDeclStmt

def _processDimensionStmt(aDimensionStmt,curr):
    localSymtab = curr.val.symtab
    DebugManager.debug('[Line '+str(aDimensionStmt.lineNumber)+']: stmt2unit._processDimensionStmt('+str(aDimensionStmt)+') with symbol table '+str(localSymtab))
    for aDimensionSpec in aDimensionStmt.lst:
        try:
            theSymtabEntry = localSymtab.lookup_name_local(aDimensionSpec.arrayName)
            if theSymtabEntry:
                DebugManager.debug('\tvariable "'+aDimensionSpec.arrayName+'" already present in local symbol table as '+theSymtabEntry.debug(aDimensionSpec.arrayName))
                theSymtabEntry.enterDimensions(tuple(aDimensionSpec.arraySpec))
            else:
                newSymtabEntry = SymtabEntry(SymtabEntry.VariableEntryKind(),
                                             dimensions=tuple(aDimensionSpec.arraySpec),
                                             origin='local')
                DebugManager.debug('\tvariable "'+aDimensionSpec.arrayName+'" NOT already present in symbol table -- adding '+newSymtabEntry.debug(aDimensionSpec.arrayName))
                localSymtab.enter_name(aDimensionSpec.arrayName,newSymtabEntry)
        except SymtabError,e: # add lineNumber and symbol name to any SymtabError we encounter
            e.lineNumber = e.lineNumber or aDimensionStmt.lineNumber
            e.symbolName = e.symbolName or aDimensionSpec.arrayName
            raise e
    return aDimensionStmt

def _processParameterStmt(aParameterStmt,curr):
    localSymtab = curr.val.symtab
    DebugManager.debug('[Line '+str(aParameterStmt.lineNumber)+']: stmt2unit._processParameterStmt('+str(aParameterStmt)+') with symbol table '+str(localSymtab))
    for aNamedParam in aParameterStmt.namedParamList:
        try:
            theSymtabEntry = localSymtab.lookup_name_local(aNamedParam[0])
            if theSymtabEntry:
                DebugManager.debug('\tvariable "'+aNamedParam[0]+'" already present in local symbol table as '+theSymtabEntry.debug(aNamedParam[0]))
                theSymtabEntry.enterConstInit(aNamedParam[2])
            else:
                # try implicit typing
                implicitLocalType=localSymtab.implicit[aNamedParam[0][0]]
                if implicitLocalType: # we handle the error condition below
                    theSymtabEntry = SymtabEntry(SymtabEntry.VariableEntryKind(),
                                                 type=implicitLocalType)
                    DebugManager.warning(sys._getframe().f_code.co_name+' implicit typing: '+str(implicitLocalType)+' '+str(aNamedParam),aParameterStmt.lineNumber,DebugManager.WarnType.implicit)
                    theSymtabEntry.enterConstInit(aNamedParam[2])
                else:
                    raise SymtabError("Parameter statement uses a variable not found in symtab",symbolName=aNamedParam[0],lineNumber=aParameterStmt.lineNumber)
        except SymtabError,e: # add lineNumber and symbol name to any SymtabError we encounter
            e.lineNumber = e.lineNumber or aParameterStmt.lineNumber
            e.symbolName = e.symbolName or aNamedParam[0]
            raise e
    return aParameterStmt

def _processExternalStmt(anExternalStmt,curr):
    localSymtab = curr.val.symtab
    DebugManager.debug('[Line '+str(anExternalStmt.lineNumber)+']: stmt2unit._processExternalStmt: called on "'+str(anExternalStmt)+'" with localSymtab '+str(localSymtab))
    for aProcedureName in anExternalStmt.procedureNames:
        try:
            theSymtabEntry = localSymtab.lookup_name(aProcedureName)
            if not theSymtabEntry: # first guess - assume it is an external subroutine
                newSymtabEntry = SymtabEntry(SymtabEntry.SubroutineEntryKind(),typetab_id=globalTypeTable.getType(anExternalStmt,localSymtab),
                                             origin='external')
                localSymtab.enter_name(aProcedureName,newSymtabEntry)
                DebugManager.debug('\tprocedure NOT already present in symbol table -- adding '+newSymtabEntry.debug(aProcedureName))
            else:
                DebugManager.debug('\tprocedure already has SymtabEntry'+theSymtabEntry.debug(aProcedureName))
                # if the entry has a type, we know it's a function
                newEntryKind = theSymtabEntry.typetab_id and SymtabEntry.FunctionEntryKind \
                                                    or SymtabEntry.SubroutineEntryKind
                theSymtabEntry.enterEntryKind(newEntryKind())
        except SymtabError,e: # add lineNumber and symbol name to any SymtabError we encounter
            e.lineNumber = e.lineNumber or anExternalStmt.lineNumber
            e.symbolName = e.symbolName or aProcedureName
            raise e
    return anExternalStmt

def _processCommonStmt(aCommonStmt,curr):
    localSymtab = curr.val.symtab
    DebugManager.debug('[Line '+str(aCommonStmt.lineNumber)+']: '+sys._getframe().f_code.co_name+' called on "'+repr(aCommonStmt)+'" with localSymtab '+localSymtab.debug())
    commonTypeId=None
    updateDeclTypeList=[]
    for aDecl in aCommonStmt.declList:
        try:
            aDeclName=""
            if isinstance(aDecl,fe.App):
                aDeclName=aDecl.head
            else:
                aDeclName=aDecl
            theSymtabEntry = localSymtab.lookup_name(aDeclName)
            if not theSymtabEntry:
                # symtab entry will be added by declaration
                newSymtabEntry = SymtabEntry(SymtabEntry.VariableEntryKind(),
                                             origin=Symtab._ourCommonScopePrefix+aCommonStmt.name)
                localSymtab.enter_name(aDeclName,newSymtabEntry)
                if isinstance(aDecl,fe.App):
                    newSymtabEntry.enterDimensions(aDecl.args)
                if commonTypeId:
                    newSymtabEntry.typetab_id=commonTypeId
                else:
                    updateDeclTypeList.append(newSymtabEntry)
                DebugManager.debug('\tcommon variable NOT already present in symbol table -- adding '+newSymtabEntry.debug(aDeclName))
            else:
                DebugManager.debug('\tcommon variable already has SymtabEntry'+theSymtabEntry.debug(aDeclName))
                theSymtabEntry.enterEntryKind(SymtabEntry.VariableEntryKind())
                if isinstance(aDecl,fe.App):
                    theSymtabEntry.enterDimensions(aDecl.args)
                theSymtabEntry.updateOrigin(Symtab._ourCommonScopePrefix+aCommonStmt.name)
                commonTypeId=theSymtabEntry.typetab_id
        except SymtabError,e: # add lineNumber and symbol name to any SymtabError we encounter
            e.lineNumber = e.lineNumber or aCommonStmt.lineNumber
            e.symbolName = e.symbolName or aDeclName
            raise e
    if commonTypeId:
        for symtabEntry in updateDeclTypeList:
            dimensions=symtabEntry.lookupDimensions()
            if dimensions:
                newTypeEntry=globalTypeTable.enterArrayType(commonTypeId,dimensions,TypetabEntry.ArrayEntryKind)
                symtabEntry.typetab_id=newTypeEntry.typetab_id
            else:
                symtabEntry.typetab_id=commonTypeId
    return aCommonStmt

def _assign2stmtfn(anAssignmentStmt,curr):
    'convert assign stmt to stmtfn, and enter in unit symtab'
    DebugManager.debug('[Line '+str(anAssignmentStmt.lineNumber)+']: converting '+str(anAssignmentStmt)+' to stmt fn')
    newStmtFn = fs.StmtFnStmt(anAssignmentStmt.lhs.head,
                              anAssignmentStmt.lhs.args,
                              anAssignmentStmt.rhs,
                              lineNumber=anAssignmentStmt.lineNumber,
                              label=anAssignmentStmt.label,
                              lead=anAssignmentStmt.lead)
    newStmtFn.rawline = anAssignmentStmt.rawline
    newSymtabEntry = SymtabEntry(SymtabEntry.StatementFunctionEntryKind(),
                                 origin='local')
    curr.val.symtab.enter_name(anAssignmentStmt.lhs.head,newSymtabEntry)
    return newStmtFn

def _is_stmt_fn(s,cur):
    'determine if assignment s is a statement function, based on "unit" symtab'
    DebugManager.debug('checking assignment '+str(s)+' for stmt fn')
    lhs  = s.lhs
    look = cur.val.symtab.lookupDimensions
    return isinstance(lhs,fe.App) and isinstance(lhs.head,str) and not look(lhs.head)

reportedMissingModules=set()
def _use_module(aUseStmt,cur):
    '''
    incorporate the used module symbol table into the current unit symbol table
    issue a warning if the module has not been seen yet
    '''
    DebugManager.debug('[Line '+str(aUseStmt.lineNumber)+']: stmt2unit._use_module() for '+str(aUseStmt)+': with symtab '+str(cur.val.symtab)+' and parent symtab '+str(cur.val.symtab.parent))
    module_unit = cur.module_handler.get_module(aUseStmt.moduleName)
    if module_unit:
        DebugManager.debug('updating '+str(cur.val.symtab)+') with module "'+aUseStmt.moduleName+'", which has unit '+str(module_unit),newLine=False)
        try:
            if isinstance(aUseStmt,fs.UseAllStmt):
                DebugManager.debug(' where we are using ALL')
                cur.val.symtab.update_w_module_all(module_unit,aUseStmt.renameList)
            elif isinstance(aUseStmt,fs.UseOnlyStmt):
                DebugManager.debug(' where we are using ONLY '+str(aUseStmt.onlyList))
                cur.val.symtab.update_w_module_only(module_unit,aUseStmt.onlyList)
        except KeyError,e:
            raise SymtabError('KeyError for key "'+str(e)+'"' \
                             +' while updating '+cur.val.symtab.debug() \
                             +' according to use statement "'+str(aUseStmt)+'"',
                              symbolName=str(e),
                              entry=None,
                              lineNumber=aUseStmt.lineNumber)
    else:
	global reportedMissingModules
        if not (aUseStmt.moduleName.lower() in reportedMissingModules) :
            reportedMissingModules.add(aUseStmt.moduleName.lower())
            DebugManager.warning('definition for module '+aUseStmt.moduleName+' not seen in the input.',aUseStmt.lineNumber,DebugManager.WarnType.noDefinition)
    return aUseStmt

def _setAccess(anAccessStmt,cur):
    ''' set the access attributes '''
    DebugManager.debug('[Line '+str(anAccessStmt.lineNumber)+']: stmt2unit._setAccess() for '+str(anAccessStmt))
        
    accessAttr=anAccessStmt.__class__.kw
    if (not anAccessStmt.vlist):
        if (cur.val._in_drvdType):
            cur.val._drvdTypeDefaultAccess=accessAttr # and process it at the end of rhe derived type definition
        else: 
            cur.val.symtab.setDefaultAccess(accessAttr)
    else: 
        for v in anAccessStmt.vlist:
            if(cur.val._in_drvdType):
                v=cur.val._in_drvdType+':'+v
            theEntry=cur.val.symtab.lookup_name(v)
            if (theEntry):
                theEntry.setSpecificAccess(accessAttr)
            else: # forward access declaration
                theEntry=SymtabEntry(SymtabEntry.GenericEntryKind(),
                                     access=accessAttr)
                cur.val.symtab.enter_name(v,theEntry)
    return anAccessStmt
    
def _unit_entry(self,cur):
    '''enter a subroutine or function into:
       1. The local symtab for the object
       2. The unit symtab
       3. The parent of the unit (if there is one)
    '''
    DebugManager.debug('[Line '+str(self.lineNumber)+']: stmt2unit._unit_entry() for '+str(self))
    if (cur.val.nestLevel==2) : 
        DebugManager.warning('Open64 front-end handling of doubly nested module procedures is fragile; check >'+self.name+'< for correct handling.',self.lineNumber,DebugManager.WarnType.nesting)
    currentSymtab = cur.val.symtab
    if (currentSymtab.parent and (self.name in currentSymtab.parent.ids)) :
        mpSymtabEntry=currentSymtab.parent.ids[self.name]
        if (isinstance(mpSymtabEntry.entryKind,SymtabEntry.ProcedureEntryKind)):
            # this is the definition of a previously declared module procedure
            entry = self.makeSymtabEntry(currentSymtab)
            mpSymtabEntry.entryKind=entry.entryKind
            mpSymtabEntry.typetab_id=entry.typetab_id
            currentSymtab.enter_name(self.name,entry)
        else: 
            # this is some forward declaration as e.g. in public/private statement
            entry = self.makeSymtabEntry(currentSymtab)
            currentSymtab.enter_name(self.name,entry)
            DebugManager.debug('[Line '+str(self.lineNumber)+']: new unit symtab entry '+entry.debug(self.name))
            # update the parent info
            currentSymtab.augmentParentEntry(entry,mpSymtabEntry,self.name)
            DebugManager.debug('[Line '+str(self.lineNumber)+']: updated parent symtab entry '+mpSymtabEntry.debug(self.name))       
    else: # nothing exists in parent
        entry = self.makeSymtabEntry(currentSymtab)
        currentSymtab.enter_name(self.name,entry)
        DebugManager.debug('[Line '+str(self.lineNumber)+']: new unit symtab entry '+entry.debug(self.name))
        if currentSymtab.parent:
            currentSymtab.replicateEntry(self.name,"local",self.name,currentSymtab.parent,replicatingUp=True)
            DebugManager.debug('[Line '+str(self.lineNumber)+']: new PARENT unit symtab entry (see above)')
    DebugManager.debug('[Line '+str(self.lineNumber)+']: stmt2unit._unit_entry() for '+str(self)+': with symtab '+str(currentSymtab)+' with parent symtab '+str(currentSymtab.parent))
    if (isinstance(self,fs.FunctionStmt)): 
        cur.val._in_functionDecl=self
    if (isinstance(self,fs.SubroutineStmt)): 
        cur.val._in_subroutineDecl=self
    return self

def _unit_exit(self,cur):
    '''exit a subroutine or function
    '''
    DebugManager.debug('[Line '+str(self.lineNumber)+']: stmt2unit._unit_exit() for '+str(self))
    parentSymtabEntry=None
    theSymtabEntry=None
    theArgs=None
    if cur.val._in_functionDecl:
        theSymtabEntry=cur.val.symtab.lookup_name(cur.val._in_functionDecl.name)
        if cur.val.symtab.parent:
            parentSymtabEntry=cur.val.symtab.parent.lookup_name(cur.val._in_functionDecl.name)
        theArgs=cur.val._in_functionDecl.args
        if (theSymtabEntry.typetab_id is None and cur.val._in_functionDecl.result):
            # try to get the type from the result symbol
            theResultEntry=cur.val.symtab.lookup_name(cur.val._in_functionDecl.result)
            if (theResultEntry):
                theSymtabEntry.copyAndEnterType(theResultEntry.typetab_id)
                if parentSymtabEntry:  # update the copy in the parent
                    parentSymtabEntry.copyAndEnterType(theResultEntry.typetab_id)
        else:
            if parentSymtabEntry: # update the copy in the parent
                parentSymtabEntry.copyAndEnterType(theSymtabEntry.typetab_id)
        cur.val._in_functionDecl=None
    if cur.val._in_subroutineDecl:
        theSymtabEntry=cur.val.symtab.lookup_name(cur.val._in_subroutineDecl.name)
        if cur.val.symtab.parent:
            parentSymtabEntry=cur.val.symtab.parent.lookup_name(cur.val._in_subroutineDecl.name)
        theArgs=cur.val._in_subroutineDecl.args
        cur.val._in_subroutineDecl=None
    # handle the arguments list:
    theSymtabEntry.specificFormalArgs=FormalArgs()
    if parentSymtabEntry:
        parentSymtabEntry.specificFormalArgs=FormalArgs()
    for pos,arg in enumerate(theArgs):
        argSymtabEntry=cur.val.symtab.lookup_name(arg)
        if (not argSymtabEntry):
            argSymtabEntry=SymtabEntry(SymtabEntry.GenericEntryKind())
            cur.val.symtab.enter_name(arg,argSymtabEntry)
        argSymtabEntry.origin="dummy"
        theSymtabEntry.specificFormalArgs.args[arg]=(pos,argSymtabEntry)
        if (parentSymtabEntry):
            parentSymtabEntry.specificFormalArgs.args[arg]=(pos,copy.deepcopy(cur.val.symtab.lookup_name(arg)))
    return self

def _module_exit(stmt,cur):
    '''exit a module
    '''
    DebugManager.debug('[Line '+str(stmt.lineNumber)+']: '+sys._getframe().f_code.co_name+' for '+str(stmt))
    cur.val.symtab.markTypesAsReferenced(True)
    return stmt

def _generic_exit(stmt,cur):
    '''exit something - check if it is a module
    '''
    DebugManager.debug('[Line '+str(stmt.lineNumber)+']: '+sys._getframe().f_code.co_name+' for '+str(stmt))
    if (isinstance(cur.val.uinfo,fs.ModuleStmt)):
        return _module_exit(stmt,cur)
    return stmt

def _implicit(self,cur):
    '''update the implicit table
    '''
    alphabet=string.ascii_lowercase
    for (type_spec,letter_spec) in self.lst: 
        for e in letter_spec:
            if isinstance(e,fe.Ops): # something like 'q-t' for which we get Ops('-','q','t')
                for letter in alphabet[ord(e.a1.lower())-ord(alphabet[0]):ord(e.a2.lower())-ord(alphabet[0])+1]:
                    cur.val.symtab.implicit[letter] = type_spec
            else:
                cur.val.symtab.implicit[e] = type_spec
    DebugManager.debug('[Line '+str(self.lineNumber)+']: stmt2unit._implicit() implicit table is now '+str(cur.val.symtab.implicit)+str(cur.val.symtab))
    return self

def _implicit_none(self,cur):
    cur.val.symtab.implicit_none()
    DebugManager.debug('[Line '+str(self.lineNumber)+']: stmt2unit._implicit_none() implicit table is now '+str(cur.val.symtab.implicit)+str(cur.val.symtab))
    return self

def _processEntry(self,cur):
    '''
    add the entry name to 
       1. The local symtab for the object
       2. The unit symtab
       3. The parent of the unit (if there is one)
    '''   
    currentSymtab = cur.val.symtab
    if (currentSymtab.parent and (self.name in currentSymtab.parent.ids))  :
        # this must be the definition of a previously  declared module procedure
        mpSymTabEntry=currentSymtab.parent.ids[self.name]
        if (not isinstance(mpSymTabEntry.entryKind,SymtabEntry.ProcedureEntryKind)
            or
            mpSymTabEntry.genericInfo is None):
            raise SymtabError('parent symbol is not a module procedure')
        entry = SymtabEntry(currentSymtab.lookup_name(cur.val.uinfo.name).entryKind)
        mpSymTabEntry.entryKind=entry.entryKind
        mpSymTabEntry.typetab_id=entry.typetab_id
        entry.genericInfo=mpSymTabEntry.genericInfo
        currentSymtab.enter_name(self.name,entry)
    else: 
        entry = SymtabEntry(currentSymtab.lookup_name(cur.val.uinfo.name).entryKind)
        currentSymtab.enter_name(self.name,entry)
        DebugManager.debug('[Line '+str(self.lineNumber)+']: new unit symtab entry '+entry.debug(self.name))
        if currentSymtab.parent:
            currentSymtab.replicateEntry(self.name,"local",self.name,currentSymtab.parent,replicatingUp=True)
            DebugManager.debug('[Line '+str(self.lineNumber)+']: new PARENT unit symtab entry (see above)')
    DebugManager.debug('[Line '+str(self.lineNumber)+']: stmt2unit._processEntry() for '+str(self)+': with symtab '+str(currentSymtab)+' with parent symtab '+str(currentSymtab.parent))
    if (isinstance(self,fs.FunctionStmt)): 
        cur.val._in_functionDecl=self
    return self    
    

def _beginProcedureUnit(aProcedureDeclStmt,cur):
    '''
    called for function/subroutine statements within an interface block
    '''
    localSymtab = Symtab(cur.val.symtab)
    DebugManager.debug('[Line '+str(aProcedureDeclStmt.lineNumber)+']: stmt2unit._beginProcedureUnit:' \
                      +' called for '+aProcedureDeclStmt.__class__.__name__+': "'+str(aProcedureDeclStmt)+'"' \
                      +' changing from current symtab "'+str(cur.val.symtab)+'"' \
                      +' to local symtab "'+str(localSymtab)+'"')
    entry = aProcedureDeclStmt.makeSymtabEntry(localSymtab)
    localSymtab.enter_name(aProcedureDeclStmt.name,entry)
    cur.val.symtab.enter_name(aProcedureDeclStmt.name,entry)
    cur.val.symtab = localSymtab
    if (isinstance(aProcedureDeclStmt,fs.FunctionStmt)): 
        cur.val._in_iface._in_procedureFuncDecl=aProcedureDeclStmt
    return aProcedureDeclStmt

def _endProcedureUnit(anEndProcedureStmt,cur):
    '''
    called for function/subroutine end statements within an interface block
    '''
    if cur.val._in_iface._in_procedureFuncDecl:
        theSymtabEntry=cur.val.symtab.lookup_name(cur.val._in_iface._in_procedureFuncDecl.name)
        if (theSymtabEntry.typetab_id is None and cur.val._in_iface._in_procedureFuncDecl.result):
            # try to get the tupe from the result symbol
            theResultEntry=cur.val.symtab.lookup_name(cur.val._in_iface._in_procedureFuncDecl.name)
            if (theResultEntry):
                theSymtabEntry.copyAndEnterType(theResultEntry.typetab_id)
        cur.val._in_iface._in_procedureFuncDecl=None         
    if cur.val.symtab.parent :
        DebugManager.debug('[Line '+str(anEndProcedureStmt.lineNumber)+']: stmt2unit._endProcedureUnit:' \
                          +' called on "'+str(anEndProcedureStmt)+'"' \
                          +' ACTION: on unit '+str(cur.val)+' reverting from symtab "'+str(cur.val.symtab)+'"' \
                                       +' to parent symtab "'+str(cur.val.symtab.parent)+'"')
        cur.val.symtab = cur.val.symtab.parent
    else :
        raise SymtabError('stmt2unit._endProcedureUnit('+str(anEndProcedureStmt)+'):' \
                         +' cannot revert from symtab "'+str(cur.val.symtab)+'"' \
                         +' to parent symtab, because there is no parent symtab.',
                          symbolName=None,
                          entry=None,
                          lineNumber=anEndProcedureStmt.lineNumber)
    return anEndProcedureStmt

def _beginInterface(anInterfaceStmt,cur):
    if (anInterfaceStmt.name): 
        cur.val.symtab.enter_name(anInterfaceStmt.name,SymtabEntry(SymtabEntry.InterfaceEntryKind()))
    cur.val._in_iface = InterfaceInfo(anInterfaceStmt.name,cur.val._in_iface)
    DebugManager.debug('[Line '+str(anInterfaceStmt.lineNumber)+']: stmt2unit._beginInterface('+str(anInterfaceStmt)+')')
    if (anInterfaceStmt.name): 
        # collect all the procedurenames in a mock symtab...
        localSymtab = Symtab(cur.val.symtab)
        cur.val.symtab = localSymtab
    # local attribute added on to convey the name to _endInterface
    return anInterfaceStmt

def _processProcedureStmt(aProcedureStmt,curr):
    localSymtab = curr.val.symtab
    DebugManager.debug('[Line '+str(aProcedureStmt.lineNumber)+']: stmt2unit._processProcedureStmt: called on "'+str(aProcedureStmt)+'" with localSymtab '+str(localSymtab))
    for aProcedureName in aProcedureStmt.procedureList:
        try:
            theSymtabEntry = localSymtab.lookup_name(aProcedureName)
            if not theSymtabEntry:
                newSymtabEntry = SymtabEntry(SymtabEntry.ProcedureEntryKind())
                localSymtab.enter_name(aProcedureName,newSymtabEntry)
                DebugManager.debug('\t_processProcedureStmt: module procedure NOT already present in symbol table -- adding '+newSymtabEntry.debug(aProcedureName))
            else:
                DebugManager.debug('\t_processProcedureStmt: module procedure already has SymtabEntry'+theSymtabEntry.debug(aProcedureName))
                # if the entry has a type, we know it's a function
                newEntryKind = theSymtabEntry.typetab_id and SymtabEntry.FunctionEntryKind \
                                                    or SymtabEntry.ProcedureEntryKind
                theSymtabEntry.enterEntryKind(newEntryKind())
                theGenericSymtabEntry=localSymtab.lookup_name(curr.val._in_iface.name)
                if (theGenericSymtabEntry.genericInfo):
                    if(theGenericSymtabEntry.genericInfo.genericName!=curr.val._in_iface.name.lower()):
                        raise SymtabError('mismatched generic name')
                    if (not (aProcedureName in theGenericSymtabEntry.genericInfo.resolvableTo.keys())):
                        theGenericSymtabEntry.genericInfo.resolvableTo[aProcedureName]=theSymtabEntry
                else :
                    theGenericSymtabEntry.genericInfo=GenericInfo()
                    theGenericSymtabEntry.genericInfo.genericName=curr.val._in_iface.name.lower()
                    theGenericSymtabEntry.genericInfo.resolvableTo[aProcedureName]=theSymtabEntry
        except SymtabError,e: # add lineNumber and symbol name to any SymtabError we encounter
            e.lineNumber = e.lineNumber or aProcedureStmt.lineNumber
            e.symbolName = e.symbolName or aProcedureName
            raise e
    return aProcedureStmt

def _endInterface(anEndInterfaceStmt,cur):
    # get all the procedurenames from the mock symtab...
    mockSymtab=cur.val.symtab
    if (cur.val._in_iface.name) : 
        ifName=cur.val._in_iface.name
        cur.val.symtab = cur.val.symtab.parent
        genericSymtabEntry = cur.val.symtab.lookup_name(ifName)
        for name in mockSymtab.ids.keys():
            try:
                specificSymtabEntry = cur.val.symtab.lookup_name(name)
                if not specificSymtabEntry:
                    specifcSymtabEntry = SymtabEntry(SymtabEntry.ProcedureEntryKind())
                    cur.val.symtab.enter_name(name,specifcSymtabEntry)
                    DebugManager.debug('\tmodule procedure NOT already present in symbol table -- adding '+specifcSymtabEntry.debug(name))
                else:
                    DebugManager.debug('\tmodule procedure already has SymtabEntry'+specificSymtabEntry.debug(name))
                    # if the entry has a type, we know it's a function
                    newEntryKind = specificSymtabEntry.typetab_id and SymtabEntry.FunctionEntryKind \
                                   or SymtabEntry.ProcedureEntryKind
                    specificSymtabEntry.enterEntryKind(newEntryKind())
                genericSymtabEntry.addResolveName(name,specifcSymtabEntry)
            except SymtabError,e: # add lineNumber and symbol name to any SymtabError we encounter
                e.lineNumber = e.lineNumber or aProcedureStmt.lineNumber
                e.symbolName = e.symbolName or aProcedureName
                raise e
    cur.val._in_iface = cur.val._in_iface.parent
    DebugManager.debug('[Line '+str(anEndInterfaceStmt.lineNumber)+']: stmt2unit._endInterface('+str(anEndInterfaceStmt)+')')
    return anEndInterfaceStmt

def _processDoLabels(aStmt,curr):
    if (aStmt.doLabel):
        curr.val.symtab.enterLabelRef(aStmt.doLabel,aStmt)
    return aStmt

def _processSimpleGotoLabels(aStmt,curr):
    if (aStmt.targetLabel):
        curr.val.symtab.enterLabelRef(aStmt.targetLabel,aStmt)
    return aStmt

def _processGotoLabels(aStmt,curr):
    for l in  aStmt.labelList:
        curr.val.symtab.enterLabelRef(l,aStmt)
    return aStmt

def _processArithmIfLabels(aStmt,curr):
    for l in  aStmt.labelTriple:
        curr.val.symtab.enterLabelRef(l,aStmt)
    return aStmt
def _processSons(aStmt,curr):
    if aStmt.stmt.is_exec():
        aStmt.stmt.exec2unitAction(curr)
    return aStmt

# hooks used ONLY IN THIS MODULE:
def _makeFunctionSymtabEntry(aFunctionStmt,localSymtab):
    if aFunctionStmt.ty is not None:
        typeId=globalTypeTable.getType(aFunctionStmt.ty[0](aFunctionStmt.ty[1],[],[]),localSymtab)
    else: 
        typeId=None
    return SymtabEntry(SymtabEntry.FunctionEntryKind(),typetab_id=typeId)
fs.FunctionStmt.makeSymtabEntry   = _makeFunctionSymtabEntry
def _makeSubroutineSymtabEntry(self,localSymtab):
    return SymtabEntry(SymtabEntry.SubroutineEntryKind())
fs.SubroutineStmt.makeSymtabEntry = _makeSubroutineSymtabEntry

# all hooks below are used by fortUnit:
# no-op for everybody as a baseline to be overridden: 
fs.GenStmt.unit_entry             = lambda s,*rest,**kw: s  # on start of a unit
fs.GenStmt.unit_exit              = lambda s,*rest,**kw: s  # on exit of a unit
fs.GenStmt.decl2unitAction        = lambda s,*rest,**kw: s  # for statements that have is_decl returning true
fs.GenStmt.exec2unitAction        = lambda s,*rest,**kw: s  # for statements that have is_exec returning true

# subroutine / function definitions are units 
fs.SubroutineStmt.unit_entry      = _unit_entry         # start definition 
fs.EndSubroutineStmt.unit_exit    = _unit_exit          # end definition 
fs.FunctionStmt.unit_entry        = _unit_entry         # start definition  
fs.EndFunctionStmt.unit_exit      = _unit_exit          # end definition
fs.EndModuleStmt.unit_exit        = _module_exit        # end of a module definition
fs.EndStmt.unit_exit              = _generic_exit       # figure out what it is we are ending

# a derived type definition is a pseudo sub unit;
# the symbol table contents is merged with the enclosing
# unit's symbol table. 
fs.DrvdTypeDefn.decl2unitAction       = _beginDrvdTypeDefn  # start definition
fs.ContainsStmt.is_decl               = _typebound_proc_block
fs.EndDrvdTypeDefn.decl2unitAction    = _endDrvdTypeDefn    # end definition


# the following are all related to declaration statements,
# the hooks update the symbol table
fs.DimensionStmt.decl2unitAction      = _processDimensionStmt
fs.ParameterStmt.decl2unitAction      = _processParameterStmt
fs.ExternalStmt.decl2unitAction       = _processExternalStmt
fs.TypeDecl.decl2unitAction           = _processTypedeclStmt
fs.CommonStmt.decl2unitAction         = _processCommonStmt
fs.UseStmt.decl2unitAction            = _use_module
fs.PrivateStmt.decl2unitAction        = _setAccess
fs.PublicStmt.decl2unitAction         = _setAccess
fs.ImplicitNone.decl2unitAction       = _implicit_none
fs.ImplicitStmt.decl2unitAction       = _implicit
fs.ProcedureStmt.decl2unitAction      = _processProcedureStmt # always in an interface

fs.InterfaceStmt.decl2unitAction      = _beginInterface       # sets unit.val._in_iface which affects is_decl return for some statement classes
# the is_decl implementation looks at the value of unit.val._in_iface for the following classes;
# i.e. the following hooks are called only when unit.val._in_iface is set, that is, because we are in an interface
# we are looking at a declaration of a function or subroutine and not at a definition. 
fs.SubroutineStmt.decl2unitAction     = _beginProcedureUnit   # start declaration 
fs.EndSubroutineStmt.decl2unitAction  = _endProcedureUnit     # end declaration   
fs.FunctionStmt.decl2unitAction       = _beginProcedureUnit   # start declaration 
fs.EndFunctionStmt.decl2unitAction    = _endProcedureUnit     # end declaration 
fs.EndInterfaceStmt.decl2unitAction   = _endInterface         # unsets unit.val._in_iface which affects is_decl return for some statement classes

# special case turns an Exec statement into a Decl statement:
# _is_stmt_fn determines if an assignment is a statement function and if yes
# then is_decl will return yes and what was an executable AssignStmt will be
# turned into a StmtFnStmt instance 
fs.AssignStmt.is_decl             = _is_stmt_fn 
fs.AssignStmt.decl2unitAction     = _assign2stmtfn

fs.DoStmt.exec2unitAction           = _processDoLabels
fs.SimpleGotoStmt.exec2unitAction   = _processSimpleGotoLabels
fs.ComputedGotoStmt.exec2unitAction = _processGotoLabels
fs.AssignedGotoStmt.exec2unitAction = _processGotoLabels
fs.ArithmIfStmt.exec2unitAction     = _processArithmIfLabels
fs.IfNonThenStmt.exec2unitAction    = _processSons
fs.EntryStmt.exec2unitAction        = _processEntry
