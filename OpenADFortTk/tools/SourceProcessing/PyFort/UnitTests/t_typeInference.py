from Setup    import *
from unittest import *
from useparse import *

from PyUtil.symtab import Symtab,globalTypeTable
from PyUtil.typetab import TypetabEntry

from PyFort.inference import expressionType,_TypeContext,_kw2type,_lenfn 
import PyFort.fortStmts as fortStmts
from PyFort.fortStmts import LogicalStmt,CharacterStmt,IntegerStmt,RealStmt,DoubleStmt,ComplexStmt,DoubleCplexStmt
from PyFort.fortStmts import _Prec,_Kind,_F77Len

Symtab.setTypeDefaults((fortStmts.RealStmt,[]),(fortStmts.IntegerStmt,[]))
theSymtab = Symtab()


class TypeUtils(TestCase):
    '''Typing, and misc xform utilities
    '''
    def test1(self):
        '_kw2type'
        self.assertEquals(_kw2type('real'),fortStmts.RealStmt)
        self.assertEquals(_kw2type('doubleprecision'),fortStmts.DoubleStmt)
        self.assertEquals(_kw2type('integer'),fortStmts.IntegerStmt)
        self.assertEquals(_kw2type('logical'),fortStmts.LogicalStmt)

    def test2(self):
        '_lenfn'
        self.assertEquals(str(_lenfn(15)),'*15')

class TypeConstants(TestCase):
    def test0(self):
        'constants - numerical values without modifiers'
        self.assertEquals(_TypeContext(0,theSymtab)._constantType(ep('3.787')),
                          globalTypeTable.lookupTypeId(globalTypeTable.intrinsicTypeToIdMap['real_4']))
        self.assertEquals(_TypeContext(0,theSymtab)._constantType(ep('3.787D00')),
                          globalTypeTable.lookupTypeId(globalTypeTable.intrinsicTypeToIdMap['real_8']))
        self.assertEquals(_TypeContext(0,theSymtab)._constantType(ep('3')),
                          globalTypeTable.lookupTypeId(globalTypeTable.intrinsicTypeToIdMap['integer_4']))

    # def test1(self):
    #     'constants - numerical values with modifiers'

    #     (type,typeModList) = _TypeContext(0,theSymtab)._constantType(ep('3.787_foo'))
    #     typeMod = typeModList[0]
    #     self.assertEquals(type,fortStmts.RealStmt)
    #     self.assert_(isinstance(typeMod,_Kind))
    #     self.assertEquals(typeMod.mod,'foo')

    #     (type,typeModList) = _TypeContext(0,theSymtab)._constantType(ep('0_w2f__i8'))
    #     typeMod = typeModList[0]
    #     self.assertEquals(type,fortStmts.IntegerStmt)
    #     self.assert_(isinstance(typeMod,_Kind))
    #     self.assertEquals(typeMod.mod,'w2f__i8')

    def test2(self):
        'constants - logical values'
        self.assertEquals(_TypeContext(0,theSymtab)._constantType(ep('.true.')),
                          globalTypeTable.lookupTypeId(globalTypeTable.intrinsicTypeToIdMap['logical']))

    def test3(self):
        'constants - strings'
        constTypeEntry = _TypeContext(0,theSymtab)._constantType(ep(r"'food'"))
        self.assert_(isinstance(constTypeEntry.entryKind,TypetabEntry.CharacterEntryKind))
        typeMod=globalTypeTable.charLenTab.lookupCharLenId(constTypeEntry.entryKind.charLenId).charLenExp
        self.assert_(isinstance(typeMod,_F77Len))
        self.assertEquals(typeMod.len,'4')

class TypeOpsExpressions(TestCase):
    'test type inference for simple combinations of binary ops for constants and variables'

    def test0(self):
        'Typing of various binary operation expressions over constants and implicitly types variables'
        ae = self.assertEquals

        e1 = ep('x * y')
        ae(expressionType(e1,theSymtab,lineNumber=0).typetab_id,7)

        e1 = ep('5.11d0 * 4.77d0')
        ae(expressionType(e1,theSymtab,lineNumber=0).typetab_id,8)

        e1 = ep('i + 4')
        ae(expressionType(e1,theSymtab,lineNumber=0).typetab_id,5)

        e1 = ep('z + 5.11d0 * 4.77d0')
        ae(expressionType(e1,theSymtab,lineNumber=0).typetab_id,8)

        e1 = ep('x * 5.11d0 + i * 4.77')
        ae(expressionType(e1,theSymtab,lineNumber=0).typetab_id,8)

def _gt(decl):
    'generate type reps f decl strings, using parser'
    stmt = pps(decl + ' x')
    return stmt

class TypeMerging(TestCase):
    'Merge types to find the supremum type'
    def test00(self):
        'Merge unmodified real with None'
        t = (fortStmts.RealStmt,[])
        self.assertEquals(_TypeContext(0,theSymtab)._typemerge([],
                                                              t),
                          t)

    def test01(self):
        'Merge modified real with None'
        t = (fortStmts.RealStmt,[_Prec('4')])
        self.assertEquals(_TypeContext(0,theSymtab)._typemerge([],
                                                                t),
                          t)

    def test1(self):
        'Merging of types in order to yield the correct supremum'
        ae = self.assertEquals

        t1 = globalTypeTable.getTypeEntry(_gt('real'),theSymtab)
        t2 = globalTypeTable.getTypeEntry(_gt('real*4'),theSymtab)
        t3 = globalTypeTable.getTypeEntry(_gt('real*8'),theSymtab)
        t4 = globalTypeTable.getTypeEntry(_gt('double precision'),theSymtab)
        t5 = globalTypeTable.getTypeEntry(_gt('complex'),theSymtab)
        t6 = globalTypeTable.getTypeEntry(_gt('integer'),theSymtab)

        ae(_TypeContext(0,theSymtab)._typemerge([],t1),t1)
        ae(_TypeContext(0,theSymtab)._typemerge([t2],t1),t2)
        ae(_TypeContext(0,theSymtab)._typemerge([t1,t1,t1],t2),t1)
        ae(_TypeContext(0,theSymtab)._typemerge([t1,t2,t1],t1),t2)
        ae(_TypeContext(0,theSymtab)._typemerge([t1,t2,t1,t3],t1),t3)
        ae(_TypeContext(0,theSymtab)._typemerge([t6,t6,t6,t1],t1),t1)
        ae(_TypeContext(0,theSymtab)._typemerge([t3,t4,],t1),t4)
        ae(_TypeContext(0,theSymtab)._typemerge([t1,t2,t3,t4,t5,t6],t1),t5)


suite = asuite(TypeUtils,TypeConstants,TypeOpsExpressions,TypeMerging)

if __name__ == '__main__':
    sys.exit(runit(suite))

