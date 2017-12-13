from _Setup import *

from PyUtil.debugManager import DebugManager
from PyUtil.caselessDict import caselessDict as cDict
from PyUtil.arrayboundstab import ArrayBoundsTab
from PyUtil.characterlentab import CharacterLenTab

from PyFort.intrinsic import getBuiltInTypes
from PyFort.fortStmts import DrvdTypeDecl, DrvdTypeDefn, CharacterStmt,RealStmt,_NoInit,UPClassStmt,ExternalStmt
from PyFort.fortExp import Ops,App

class TypetabError(Exception):
    '''exception for errors that occur when using the type table'''
    def __init__(self,msg,theType=None,entry=None,lineNumber=None):
        self.msg = msg
        self.theType = theType
        self.entry = entry
        self.lineNumber = lineNumber

    def __str__(self):
        errString='\nERROR: TypetabError at line '+str(self.lineNumber)+':'+str(self.msg)
        if self.theType:
            typeStr = self.theType.debug() or '<type class unknown>'
            errString+='\nFor type '+typeStr
        return (errString)

class Typetab(object):
    '''class to manage built-in types, arrays & user-defined types'''

    def __init__(self):
        self.ids = dict() # string for the key and TypetabEntry for the value
        self.intrinsicIdToTypeMap=dict() # typetab id for the key and (fortStmt Type,Kind) pair for value
        self.intrinsicTypeToIdMap=dict() # type name for key and typetab id for value
        self.type_counter=1
        self.__enterBuiltInTypes()
        self.arrayBoundsTab=ArrayBoundsTab()
        self.charLenTab=CharacterLenTab()
        self.currentUnitTypeEntries=set()   # type ids of type entries added during the processing of the current unit

    def debug(self):
        return  'TypeTab:[\n'+'\n'.join(map(lambda l: '\t'+l.debug(),self.ids.values()))+'\n]'

    # traverse type table (after symtab traversal to mark all referenced typetab entries) and verify that all remaining types are global and referenced
    def verifyTypeEntries(self):
        for typeEntry in self.ids.values():
            # verify that all user-created type entries in type table are valid
            if not typeEntry.isGlobalTypeEntry():			    
                raise TypetabError('Non-global type in type table: ', theType=typeEntry)
            elif not isinstance(typeEntry.entryKind, TypetabEntry.BuiltInEntryKind) and not typeEntry.referenced:
                raise TypetabError('This type is never referenced by the symbol table: ', theType=typeEntry)

    def lookupTypeId(self,typeid):
        '''check for typeid in the type table'''
        DebugManager.debug('Typetab.lookupTypeId called on "'+str(typeid)+'"')
        if typeid in self.ids:
            return self.ids[typeid]
        else:
            return None

    def removeTypeEntry(self,typeid):
        DebugManager.debug(self.__class__.__name__+":"+sys._getframe().f_code.co_name
                           +' called on '+self.ids[typeid].debug())
        del self.ids[typeid]

    def cleanUpUnitTypeEntries(self):
        for aTypeEntryId in self.currentUnitTypeEntries:
            typeEntry=self.lookupTypeId(aTypeEntryId)
            if typeEntry and not typeEntry.isGlobalTypeEntry():
                self.removeTypeEntry(aTypeEntryId)
        self.currentUnitTypeEntries.clear()

    def intrinsicTypeNameToEntry(self,type_name):
        '''get typeid by intrinisic type name'''
        return self.lookupTypeId(self.intrinsicTypeToIdMap[type_name])

    def __getBuiltInName(self,theType,localSymtab):
        '''get the built-in type name from the given type & symtab'''
        from PyFort.inference import guessBytes
        if (theType.kw=='doubleprecision'):
            numBytes=guessBytes((theType.__class__,theType.get_mod()),localSymtab,0)
            typeName='real_'+str(numBytes)
        elif (theType.kw=='doublecomplex'):
            numBytes=guessBytes((theType.__class__,theType.get_mod()),localSymtab,0)
            typeName='complex_'+str(numBytes)
        elif (theType.kw in ['character','external','logical']):
            return theType.kw
        else:
            numBytes=guessBytes((theType.__class__,theType.get_mod()),localSymtab,0)
            typeName=theType.kw+'_'+str(numBytes)
        return typeName

    def __getDimensions(self,theType,localSymtab):
        '''get the dimensions of the declaration from given type and symtab'''
        if theType.dimension:
            return theType.dimension
        elif len(theType.get_decls())>0:
            theDecl=theType.get_decls()[0]
            if isinstance(theDecl,_NoInit):
                if isinstance(theDecl.lhs,App):
                    return theDecl.lhs.args
            elif isinstance(theDecl,App):
                return theDecl.args
        return None

    def __getTypeKind(self,theType,localSymtab):
        '''get the TypetabEntry kind'''
        # do the special case first:
        if isinstance(theType,ExternalStmt):
            return TypetabEntry.BuiltInEntryKind
        if theType.allocatable:
            return TypetabEntry.AllocatableEntryKind
        if theType.pointer:
            if self.__getDimensions(theType,localSymtab):
                return TypetabEntry.ArrayPointerEntryKind
            elif isinstance(theType,DrvdTypeDecl) or isinstance(theType,DrvdTypeDefn):
                return TypetabEntry.NamedTypePointerEntryKind
            elif isinstance(theType,CharacterStmt) and len(theType.get_mod())>0:
                return TypetabEntry.CharacterPointerEntryKind
            else:
                # assume BuiltInPointerEntryKind; type name will be checked later
                return TypetabEntry.BuiltInPointerEntryKind
        else:
            if self.__getDimensions(theType,localSymtab):
                return TypetabEntry.ArrayEntryKind
            elif isinstance(theType,DrvdTypeDecl) or isinstance(theType,DrvdTypeDefn):
                return TypetabEntry.NamedTypeEntryKind
            elif isinstance(theType,CharacterStmt) and len(theType.get_mod())>0:
                return TypetabEntry.CharacterEntryKind
            else:
                # assume BuiltInEntryKind; type name will be checked later
                return TypetabEntry.BuiltInEntryKind

    def __enterBuiltInTypes(self):
        '''populate the type table with the built-in types'''
        for aTypePair in getBuiltInTypes():
            if aTypePair[0].kw=='doubleprecision':
                entryKind=TypetabEntry.BuiltInEntryKind('real_8')
            elif aTypePair[0].kw=='doublecomplex':
                entryKind=TypetabEntry.BuiltInEntryKind('complex_16')
            elif aTypePair[0].kw in ['character','external','logical']:
                entryKind=TypetabEntry.BuiltInEntryKind(aTypePair[0].kw)
            else:
                entryKind = TypetabEntry.BuiltInEntryKind(aTypePair[0].kw+'_'+str(aTypePair[1]))
            newEntry = TypetabEntry(entryKind,self.type_counter)
            self.intrinsicIdToTypeMap[self.type_counter]=aTypePair
            self.intrinsicTypeToIdMap[entryKind.type_name]=self.type_counter
            self.ids[self.type_counter]=newEntry
            self.type_counter += 1

    def enterArrayType(self,baseTypeID,dimensions,entryKind):
        '''create a new TypetabEntry for the array type & add it to the type table'''
        if entryKind==TypetabEntry.AllocatableEntryKind:
            # AllocatableEntryKind
            newEntry=TypetabEntry(TypetabEntry.AllocatableEntryKind(baseTypeID,len(dimensions)),self.type_counter)
        elif entryKind==TypetabEntry.ArrayEntryKind:
            # ArrayEntryKind
            arrayid=self.arrayBoundsTab.getArrayBoundsId(dimensions)
            newEntry=TypetabEntry(TypetabEntry.ArrayEntryKind(arrayid,baseTypeID),self.type_counter)
        else:
            # ArrayPointerEntryKind
            newEntry=TypetabEntry(TypetabEntry.ArrayPointerEntryKind(baseTypeID),self.type_counter)
        self.ids[self.type_counter]=newEntry
        self.type_counter += 1
        self.currentUnitTypeEntries.add(newEntry.typetab_id)
        return newEntry

    def enterNamedType(self,symbolName,localSymtab):
        '''create a new TypetabEntry for the user-defined type & add it to the type table'''
        newTypetabKind=TypetabEntry.NamedTypeEntryKind(symbolName,localSymtab,None)
        newEntry=TypetabEntry(newTypetabKind,self.type_counter)
        self.ids[self.type_counter]=newEntry
        self.type_counter+=1
        self.currentUnitTypeEntries.add(newEntry.typetab_id)
        return newEntry.typetab_id

    # get a NamedType entry (or NamedTypePointer) by symbolName & symtab
    # if it's not present already, add it
    def __getNamedType(self,symbolName,localSymtab):
        '''get the named type entry typeid for the given symbolName and symtab'''
        kindEntryMatches = [e for e in self.ids.values() if isinstance(e.entryKind,TypetabEntry.NamedTypeEntryKind) or isinstance(e.entryKind,TypetabEntry.NamedTypePointerEntryKind)]
        if len(kindEntryMatches)!=0:
            # check symbol name
            kindEntryMatches = [e for e in kindEntryMatches if (e.entryKind.symbolName==symbolName)]
            match = [e for e in kindEntryMatches if ((e.entryKind.localSymtab==None) or (e.entryKind.localSymtab==localSymtab))]
            if match:
                return match[0].typetab_id
        # if no match, add a new type entry
        # if every base type is added as it is searched, then the type entry here does not have a base type.
        return self.enterNamedType(symbolName,localSymtab)

    # newType: pair  (type class,type modifier) => pass in only type class (don't need type mod)?
    # enter the type in the type table and return the typetab_id
    def __enterNewType(self,theType,localSymtab):
        '''create a TypetabEntry for the given type decl and add it to the type table'''
        DebugManager.debug('Typetab.__enterNewType called on "'+str(theType)+'"')
        typeKind=self.__getTypeKind(theType,localSymtab)
        newEntry=None
        if typeKind==TypetabEntry.NamedTypeEntryKind:
            symbolName=theType.get_mod()[0]
            baseTypeId=None
            if localSymtab is not None:
                (symtabEntry,baseSymtab)=localSymtab.lookup_name_level(symbolName)
                if symtabEntry:
                    baseName=symtabEntry.entryKind.base_type
                    baseTypeId=self.__getNamedType(baseName,baseSymtab)
            kind=TypetabEntry.NamedTypeEntryKind(symbolName,localSymtab,baseTypeId)
            newEntry=TypetabEntry(kind,self.type_counter)
        elif typeKind==TypetabEntry.ArrayEntryKind:
            # check that type is defined in table & get typeid
            baseType = theType.__class__(theType.get_mod(),[],[])
            typeID = self.getType(baseType,localSymtab)
            newEntry=self.enterArrayType(typeID,self.__getDimensions(theType,localSymtab),typeKind)
            return newEntry.typetab_id
        elif typeKind==TypetabEntry.ArrayPointerEntryKind:
            # need to look up pointed-to type and check for typeid match
            baseType = theType.__class__(theType.get_mod(),[],[])
            baseType.dimension = self.__getDimensions(theType,localSymtab)
            typeID = self.getType(baseType,localSymtab)
            newEntry=self.enterArrayType(typeID,self.__getDimensions(theType,localSymtab),typeKind)
            return newEntry.typetab_id
        elif typeKind==TypetabEntry.BuiltInPointerEntryKind:
            # need to look up pointed-to type and check for typeid match
            baseType = theType.__class__(theType.get_mod(),[],[])
            baseType.dimension = self.__getDimensions(theType,localSymtab)
            typeID = self.getType(baseType,localSymtab)
            kind=typeKind(typeID)
            newEntry=TypetabEntry(kind,self.type_counter)
        elif typeKind==TypetabEntry.NamedTypePointerEntryKind:
            baseTypeId=None
            if isinstance(theType,DrvdTypeDecl):
                symbolName=theType.get_mod()[0]
                (symtabEntry,baseSymtab)=localSymtab.lookup_name_level(symbolName)
                if symtabEntry:
                    base_type=symtabEntry.entryKind.base_type
                    baseTypeId=self.__getNamedType(base_type,baseSymtab)
            elif isinstance(theType,DrvdTypeDefn):
                symbolName=theType.name
                (symtabEntry,baseSymtab)=localSymtab.lookup_name_level(theType.base_type)
                baseTypeId=self.__getNamedType(theType.base_type,baseSymtab)
            kind=TypetabEntry.NamedTypePointerEntryKind(symbolName,localSymtab,baseTypeId)
            newEntry=TypetabEntry(kind,self.type_counter)
        elif typeKind==TypetabEntry.CharacterEntryKind:
            charLenId = self.charLenTab.getCharLen(theType.get_mod()[0])
            kind=TypetabEntry.CharacterEntryKind(charLenId)
            newEntry=TypetabEntry(kind,self.type_counter)
        elif typeKind==TypetabEntry.AllocatableEntryKind:
            baseType = theType.__class__(theType.get_mod(),[],[])
            typeID = self.getType(baseType,localSymtab)
            newEntry=self.enterArrayType(typeID,self.__getDimensions(theType,localSymtab),typeKind)
            return newEntry.typetab_id
        elif typeKind==TypetabEntry.BuiltInEntryKind:
            # these should already be added, unless it's a character with a length specifier
            raise TypetabError('attempting to incorrectly add a Built In type to the type table.',str(theType))
        else:
            raise TypetabError('unrecognized typeKind: ',str(theType))
        self.ids[self.type_counter]=newEntry
        self.type_counter += 1
        return newEntry.typetab_id

    # newType: pair  (type class,type modifier) => pass in only type class (don't need type mod)?
    # deal with pointer issue
    # check if theType is BuiltIn, NamedType or Array
    # if type is not present, add it
    # filter for same # derefs and entrykind
    # then filter results again for match
    # if not: return None
    # else: return typeid
    def lookupType(self,theType,localSymtab):
        '''attempt to match the given type decl to one in the type table and return the typeid or None'''
        DebugManager.debug('Typetab.lookupType called on "'+str(theType)+'"')
        typeKind=self.__getTypeKind(theType,localSymtab)
        kindEntryMatches = [e for e in self.ids.values() if isinstance(e.entryKind,typeKind)]
        if len(kindEntryMatches)==0:
            return None
        else:
            if typeKind==TypetabEntry.BuiltInEntryKind:
                builtInName=self.__getBuiltInName(theType,localSymtab)
                match = [e for e in kindEntryMatches if (e.entryKind.type_name==builtInName)]
                if match:
                    # return associated key
                    return match[0].typetab_id
            elif typeKind==TypetabEntry.ArrayPointerEntryKind or typeKind==TypetabEntry.BuiltInPointerEntryKind:
                # need to look up pointed-to type and check for typeid match
                baseType = theType.__class__(theType.get_mod(),[],[])
                baseType.dimension = self.__getDimensions(theType,localSymtab)
                typeID = self.lookupType(baseType,localSymtab)
                if typeID is not None:
                    match = [e for e in kindEntryMatches if (e.entryKind.typetab_id==typeID)]
                    if match:
                        return match[0].typetab_id                
            elif typeKind==TypetabEntry.NamedTypeEntryKind or typeKind==TypetabEntry.NamedTypePointerEntryKind:
                # check symbol name
                # newType is DrvdTypeDecl or DrvdTypeDefn
                if isinstance(theType,DrvdTypeDecl):
                    kindEntryMatches = [e for e in kindEntryMatches if (e.entryKind.symbolName==theType.get_mod()[0])]
                elif isinstance(theType,DrvdTypeDefn):
                    kindEntryMatches = [e for e in kindEntryMatches if (e.entryKind.symbolName==theType.name)]
                # check scope
                match = [e for e in kindEntryMatches if ((e.entryKind.localSymtab==None) or (e.entryKind.localSymtab==localSymtab))]
                if match:
                    return match[0].typetab_id
            elif typeKind==TypetabEntry.ArrayEntryKind:
                # check that type is defined in table
                baseType = theType.__class__(theType.get_mod(),[],[])
                typeID = self.lookupType(baseType,localSymtab)
                if typeID:
                    typeMatches = [e for e in kindEntryMatches if (e.entryKind.typetab_id==typeID)]
                    if typeMatches:
                        # check arrayid
                        arrayid=self.arrayBoundsTab.lookupArrayBounds(self.__getDimensions(theType,localSymtab))
                        if arrayid:
                            match = [e for e in typeMatches if (e.entryKind.arrayid==arrayid)]
                            if match:
                                return match[0].typetab_id
            elif typeKind==TypetabEntry.CharacterEntryKind:
                charId = self.charLenTab.lookupCharLen(theType.get_mod()[0])
                if charId:
                    charMatch = [e for e in kindEntryMatches if (e.entryKind.charLenId==charId)]
                    if charMatch:
                        return charMatch[0].typetab_id
            elif typeKind==TypetabEntry.AllocatableEntryKind:
                # check that type is defined in table
                baseType = theType.__class__(theType.get_mod(),[],[])
                typeID = self.lookupType(baseType,localSymtab)
                if typeID:
                    typeMatches = [e for e in kindEntryMatches if (e.entryKind.typetab_id==typeID)]
                    if typeMatches:
                        # check that rank matches
                        match = [e for e in typeMatches if (e.entryKind.rank==len(self.__getDimensions(theType,localSymtab)))]
                        if match:
                            return match[0].typetab_id
            else:
                raise TypetabError("type of argument not recognized",theType=theType)
        return None

    # get the type id; if it is not already in the table, add it
    def getType(self,theType,localSymtab):
        '''look up the given type decl in the type table, or add it if necessary, and return the typeid'''
        DebugManager.debug('Typetab.getType called on "'+str(theType)+'"')
        if isinstance(theType,UPClassStmt):
            # unlimited polymorphic entity=> not declared to have a type
            return None
        typeid = self.lookupType(theType,localSymtab)
        if not typeid:
            newType=self.__enterNewType(theType,localSymtab)
            self.currentUnitTypeEntries.add(newType)
            return newType
        return typeid

    # get the type entry; if it is not already in the table, add it
    def getTypeEntry(self,theType,localSymtab):
        '''look up the given type decl in the type table, or add it if necessary, and return the type entry'''
        DebugManager.debug('Typetab.getTypeEntry called on "'+str(theType)+'"')
        typeid=self.getType(theType,localSymtab)
        return globalTypeTable.lookupTypeId(typeid)

    # type equivalence between two entries; Do not match array bounds exactly, only rank.
    # used for comparing formal arguments to dummy arguments
    def equivalence(self,entry1,entry2):
        '''compare two entries & return true if their types are equivalent'''
        if not (entry1.entryKind.keyword==entry2.entryKind.keyword):
            return False
        for aSon in entry1.entryKind._sons:
            theEntry1Son = getattr(entry1.entryKind,aSon)
            theEntry2Son = getattr(entry2.entryKind,aSon)
            if aSon is 'arrayid':
                # match rank only
                array1Entry = self.arrayBoundsTab.lookupArrayId(theEntry1Son)
                array2Entry = self.arrayBoundsTab.lookupArrayId(theEntry2Son)
                if array1Entry.rank != array2Entry.rank:
                    return False
            elif theEntry1Son != theEntry2Son:
                return False
        return True

class TypetabEntry(object):
    'class to manage type table entries'

    class GenericEntryKind(object):
        keyword = 'unknown'
        def __init__(self):
            self.globalTypeEntry=False

        def setTypeEntryToGlobal(self):
            self.globalTypeEntry=True

    class BuiltInEntryKind(GenericEntryKind):
        'class to manage built-in types such as integer, real, etc.'
        keyword = 'builtin'
        _sons = ['type_name']

        def __init__(self,type_name):
            self.type_name=type_name  # name of built-in type (e.g. integer) (use type class here?) only if not a pointer
            self.globalTypeEntry=True

        def debug(self):
            return '[BuiltInEntryKind: '+str(self.type_name)+']'

    class CharacterEntryKind(BuiltInEntryKind):
        'class to manage character types'
        _sons = ['type_name','charLenId']

        def __init__(self,charLenId):
            TypetabEntry.GenericEntryKind.__init__(self)
            self.charLenId=charLenId # id for the character length entry in the characterlentab
            TypetabEntry.BuiltInEntryKind.__init__(self,'character')
            self.globalTypeEntry=False

        def debug(self):
            return '[CharacterEntryKind; '+str(self.type_name)+', lenId='+str(self.charLenId)+(self.globalTypeEntry and ', isGlobal' or '')+']'

    class BuiltInPointerEntryKind(GenericEntryKind):
        'class to manage pointers to built-in entry types'
        keyword = 'BIpointer'
        _sons = ['typetab_id']

        def __init__(self,typetab_id):
            TypetabEntry.GenericEntryKind.__init__(self)
            self.typetab_id=typetab_id        # type id of built-in type pointed to

        def debug(self):
            return '[BuiltInPointerEntryKind; typeid of built-in type pointed to: '+str(self.typetab_id)+']'

    class NamedTypeEntryKind(GenericEntryKind):
        'class to manage named types'
        keyword = 'namedtype'
        _sons = ['localSymtab']

        def __init__(self,symbolName,localSymtab,baseTypeId=None,polymorphic=False):
            TypetabEntry.GenericEntryKind.__init__(self)
            self.symbolName=symbolName        # symbolName of named type
            self.localSymtab=localSymtab      # scope named type is defined in
            self.baseTypeId=baseTypeId        # baseType, if this is a type which extends another named type

        def getBaseTypeEntry(self):
            return self.baseType

        def debug(self):
            returnString='[NamedTypeEntryKind; symbolName:'+str(self.symbolName)
            if self.localSymtab:
                returnString+=', localSymtab where the type is defined: ' + str(id(self.localSymtab))
            else:
                returnString+=', localSymtab is None'
            returnString+=(self.globalTypeEntry and ', isGlobal' or '')+']'    
            return returnString

    class NamedTypePointerEntryKind(GenericEntryKind):
        'class to manage pointers to named types'
        keyword = 'NTpointer'
        _sons = ['symbolName','localSymtab']

        def __init__(self,symbolName,localSymtab,baseTypeId):
            TypetabEntry.GenericEntryKind.__init__(self)
            self.symbolName=symbolName        # symbolName of named type
            self.localSymtab=localSymtab      # scope the symbolName is defined in
            self.baseTypeId=baseTypeId        # baseType, if this is a type which extends another named type

        def debug(self):
            returnString='[NamedTypePointerEntryKind; symbolName:'+str(self.symbolName)
            if self.localSymtab:
                returnString+=', localSymtab where the type is defined: '+ str(id(self.localSymtab))
            else:
                returnString+=', localSymtab is None'
            returnString+=(self.globalTypeEntry and ', isGlobal' or '')+']'    
            return returnString

    class ArrayEntryKind(GenericEntryKind):
        'class to manage array types'
        keyword = 'array'
        _sons = ['arrayid','typetab_id']

        def __init__(self,arrayid,typetab_id):
            TypetabEntry.GenericEntryKind.__init__(self)
            self.arrayid=arrayid      # array id for array table where dimension information is stored
            self.typetab_id=typetab_id        # typeid of BI or NT array type

        def getArrayBounds(self):
            arrayTabEntry=globalTypeTable.arrayBoundsTab.lookupArrayId(self.arrayid)
            arrayBounds=[]
            for dimEntry in arrayTabEntry.dimArray:
                arrayBounds.append(Ops(':',dimEntry.lower,dimEntry.upper))
            return arrayBounds
            
        def getArrayRank(self):
            arrayTabEntry=globalTypeTable.arrayBoundsTab.lookupArrayId(self.arrayid)
            return arrayTabEntry.rank

        def debug(self):
            return '[ArrayEntryKind; arrayid='+str(self.arrayid)+\
                                    ', base typetab_id='+str(self.typetab_id) +(self.globalTypeEntry and ', isGlobal' or '')+']'

    class ArrayPointerEntryKind(GenericEntryKind):
        'class to manage pointers to array types'
        keyword = 'ARpointer'
        _sons = ['typetab_id']

        def __init__(self,typetab_id):
            TypetabEntry.GenericEntryKind.__init__(self)
            self.typetab_id=typetab_id             # type id of array type of target

        def debug(self):
            return '[ArrayPointerEntryKind; Type id of array type of target: '+str(self.typetab_id)+(self.globalTypeEntry and ', isGlobal' or '')+']'

        def getArrayTypeEntry(self):
            return globalTypeTable.lookupTypeId(self.typetab_id)

    class AllocatableEntryKind(GenericEntryKind):
        'class to manage allocatable types'
        keyword = 'allocatable'
        _sons = ['typetab_id','rank']

        def __init__(self,typetab_id,rank):
            TypetabEntry.GenericEntryKind.__init__(self)
            self.typetab_id=typetab_id # type id of base type
            self.rank=rank             # rank

        def debug(self):
            return '[AllocatableEntryKind; Type id of allocatable base type: '+str(self.typetab_id)+';rank: '+str(self.rank)+(self.globalTypeEntry and ', isGlobal' or '')+']'

    def __init__(self,entryKind,typetab_id):
        self.entryKind = entryKind # some instance of self.GenericEntryKind
        self.typetab_id=typetab_id # typeid in type table for this TypeTabEntry
        self.referenced=False # sanity check that all types are referenced by a symtab entry

    def setTypeEntryToGlobal(self):
        self.entryKind.setTypeEntryToGlobal()

    def isGlobalTypeEntry(self):
        return self.entryKind.globalTypeEntry

    def setReferenced(self):
        self.referenced=True

    def getBaseTypeId(self):
        'return the id for the base type of the pointer, allocatable, or array entry kind'
        DebugManager.debug('TypetabEntry.getBaseTypeId called on type: '+self.debug())
        if isinstance(self.entryKind,TypetabEntry.ArrayEntryKind):
            return self.entryKind.typetab_id
        elif isinstance(self.entryKind,TypetabEntry.ArrayPointerEntryKind):
            return self.entryKind.getArrayTypeEntry().getBaseTypeId()
        elif isinstance(self.entryKind,TypetabEntry.BuiltInPointerEntryKind):
            return self.entryKind.typetab_id
        elif isinstance(self.entryKind,TypetabEntry.AllocatableEntryKind):
            return self.entryKind.typetab_id
        else:
            return self.typetab_id

    def getBaseTypeEntry(self):
        'return the type entry for the base type of the pointer, allocatable, or array entry kind'
        DebugManager.debug('TypetabEntry.getBaseTypeEntry called on type: '+self.debug())
        return globalTypeTable.lookupTypeId(self.getBaseTypeId())

    def debug(self):
        return '[TypetabEntry('+str(id(self))+') -> entryKind='+self.entryKind.debug()+\
                                         ', typetab_id='+str(self.typetab_id)+\
                                         ']'

    def isExternal(self):
        return (isinstance(self.entryKind,TypetabEntry.BuiltInEntryKind) and self.entryKind.type_name=='external')
    
global globalTypeTable
globalTypeTable=Typetab()
