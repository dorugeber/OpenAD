from Setup import *
from unittest import *
from PyUtil.symtab import Symtab, SymtabEntry
from PyUtil.typetab import Typetab, TypetabEntry

import PyFort.fortStmts as fs
import PyFort.fortExp as fe

Symtab.setTypeDefaults((fs.RealStmt,[]),(fs.IntegerStmt,[]))

class C1(TestCase):
    topSE=SymtabEntry(SymtabEntry.GenericEntryKind())
    fooSE=SymtabEntry(SymtabEntry.GenericEntryKind())
    def setUp(self):
        self.typetab = Typetab()
        s = Symtab()
        s.enter_name('TOP',C1.topSE)
        ts = Symtab(s)
        ts.enter_name('foo',C1.fooSE)
        self.symtab=ts        

    def test1(self):
        '''look up built in type integer'''
        integerStmt=fs.IntegerStmt([],[],['foo'])
        typeid = self.typetab.lookupType(integerStmt,self.symtab)
        ae(typeid,5)

    def test2(self):
        '''lookup built in type real'''
        realStmt=fs.RealStmt([],[],['foo'])
        typeid = self.typetab.lookupType(realStmt,self.symtab)
        ae(typeid,7)

    def test3(self):
        '''look up built in type double'''
        doubleStmt=fs.DoubleStmt([],[],['foo'])
        typeid = self.typetab.lookupType(doubleStmt,self.symtab)
        ae(typeid,8)

    def test4(self):
        '''look up built in type complex'''
        complexStmt=fs.ComplexStmt([],[],['foo'])
        typeid = self.typetab.lookupType(complexStmt,self.symtab)
        ae(typeid,9)

    def test5(self):
        '''look up built in type double complex'''
        doubleComplexStmt=fs.DoubleCplexStmt([],[],['foo'])
        typeid = self.typetab.lookupType(doubleComplexStmt,self.symtab)
        ae(typeid,10)

    def test6(self):
        '''look up built in type logical'''
        logicalStmt=fs.LogicalStmt([],[],['foo'])
        typeid = self.typetab.lookupType(logicalStmt,self.symtab)
        ae(typeid,2)

    def test7(self):
        '''look up built in type character'''
        characterStmt=fs.CharacterStmt([],[],['foo'])
        typeid = self.typetab.lookupType(characterStmt,self.symtab)
        ae(typeid,1)

    def test8(self):
        '''test named type entry'''
        drvdType=fs.DrvdTypeDecl(['t'],[],[fs._NoInit('bar')])
        typeid = self.typetab.getType(drvdType,self.symtab)
        ae(typeid,12)

    def test9(self):
        '''test named type lookup'''
        drvdType=fs.DrvdTypeDecl(['t'],[],[fs._NoInit('bar')])
        typeid1 = self.typetab.getType(drvdType,self.symtab)
        typeid2 = self.typetab.lookupType(drvdType,self.symtab)
        ae(typeid1,typeid2)

    def test10(self):
        '''test array type entry and lookup'''
        arrayStmt=fs.RealStmt([],[fe.App('dimension',[10])],['foo'])
        typeid1 = self.typetab.getType(arrayStmt,self.symtab)
        typeid2 = self.typetab.lookupType(arrayStmt,self.symtab)
        ae(typeid1,typeid2)
        
    def test11(self):
        '''test built in type pointer entry and lookup'''
        integerPtrStmt=fs.IntegerStmt([],['pointer',],['foo'])
        typeid1 = self.typetab.getType(integerPtrStmt,self.symtab)
        typeid2 = self.typetab.lookupType(integerPtrStmt,self.symtab)
        ae(typeid1,self.typetab.type_counter-1)
        ae(typeid1,typeid2)

    def test12(self):
        '''test named type pointer entry and lookup'''
        drvdType=fs.DrvdTypeDecl(['t'],[],[fs._NoInit('bar')])
        self.typetab.getType(drvdType,self.symtab)
        drvdTypePtr=fs.DrvdTypeDecl(['t'],['pointer'],[fs._NoInit('foo')])
        typeid1 = self.typetab.getType(drvdTypePtr,self.symtab)
        typeid2 = self.typetab.lookupType(drvdTypePtr,self.symtab)
        ae(typeid1,self.typetab.type_counter-1)
        ae(typeid1,typeid2)

    def test13(self):
        '''test array type pointer entry and lookup'''
        arrayStmt=fs.RealStmt([],['pointer',fe.App('dimension',[10])],['foo'])
        typeid1 = self.typetab.getType(arrayStmt,self.symtab)
        typeid2 = self.typetab.lookupType(arrayStmt,self.symtab)
        ae(typeid1,typeid2)

    def test14(self):
        '''test type equivalence, which matches type, kind & rank'''
        arrayStmt1=fs.RealStmt([],[fe.App('dimension',[10,2])],['foo'])
        arrayStmt2=fs.RealStmt([],[fe.App('dimension',[10,4])],['foo'])
        typeid1 = self.typetab.getType(arrayStmt1,self.symtab)
        typeid2 = self.typetab.getType(arrayStmt2,self.symtab)
        typeEntry1 = self.typetab.lookupTypeId(typeid1)
        typeEntry2 = self.typetab.lookupTypeId(typeid2)
        equivalentBool = self.typetab.equivalence(typeEntry1,typeEntry2)
        ae(True,equivalentBool)

suite = asuite(C1)

if __name__ == '__main__':
    sys.exit(runit(suite))
