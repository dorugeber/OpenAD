from Setup    import *
from unittest import *
from useparse import *

#from PyUtil.debugManager import DebugManager
#DebugManager.setVerbose(True)

from PyUtil.symtab import Symtab, SymtabEntry, globalTypeTable

from PyFort.inference import expressionShape
from PyFort.fortStmts import IntegerStmt,RealStmt,DimensionStmt
from PyFort.fortStmts import _Prec,_Kind,_F77Len
from PyFort.fortExp   import Ops

from useparse  import *

Symtab.setTypeDefaults((fortStmts.RealStmt,[]),(fortStmts.IntegerStmt,[]))
theSymtab = Symtab()

class ShapeOpsExpressions(TestCase):
    'test shape inference for simple combinations of binary ops for constants and variables'

    def test0(self):
        'scalar * array'
        ae = self.assertEquals

        theTypeId = globalTypeTable.getType(RealStmt([],[],['x']),theSymtab)
        theSymtab.enter_name('x',SymtabEntry(SymtabEntry.VariableEntryKind, type=RealStmt,typetab_id=theTypeId))
        stmt = 'real, dimension(2,3) :: y'
        ps = pps(stmt)
        theTypeId = globalTypeTable.getType(ps,theSymtab)
        theSymtab.enter_name('y', SymtabEntry(SymtabEntry.VariableEntryKind,
                                              type=RealStmt,
                                              dimensions=['2','3'],
                                              typetab_id=theTypeId))
        e1 = ep('x * y')
        ae(expressionShape(e1,theSymtab,lineNumber=0),
           (['2','3']))

    def test1(self):
        'scalar * subscripted array'
        ae = self.assertEquals

        theTypeId = globalTypeTable.getType(RealStmt([],[],['x']),theSymtab)
        theSymtab.enter_name('x',SymtabEntry(SymtabEntry.VariableEntryKind, type=RealStmt,typetab_id=theTypeId))
        stmt = 'real, dimension(2,3) :: y'
        ps = pps(stmt)
        theTypeId = globalTypeTable.getType(ps,theSymtab)
        theSymtab.enter_name('y', SymtabEntry(SymtabEntry.VariableEntryKind,
                                              type=RealStmt,
                                              dimensions=['2','3'],
                                              typetab_id=theTypeId))
        e1 = ep('x * y(1,:)')
        ae(expressionShape(e1,theSymtab,lineNumber=0),
           ['3'])


    def test2(self):
        'scalar * fixed array segment'
        ae = self.assertEquals

        theTypeId = globalTypeTable.getType(RealStmt([],[],['x']),theSymtab)
        theSymtab.enter_name('x',SymtabEntry(SymtabEntry.VariableEntryKind, type=RealStmt,typetab_id=theTypeId))
        stmt = 'real, dimension(2,3) :: y'
        ps = pps(stmt)
        theTypeId = globalTypeTable.getType(ps,theSymtab)
        theSymtab.enter_name('y', SymtabEntry(SymtabEntry.VariableEntryKind,
                                              type=RealStmt,
                                              dimensions=['2','3'],
                                              typetab_id=theTypeId))
        e1 = ep('x * y(1,1:2)')
        ae(str(expressionShape(e1,theSymtab,lineNumber=0)),
           str([Ops(':','1','2')]))

    def test3(self):
        'scalar * range w open ubound'
        ae = self.assertEquals

        theTypeId = globalTypeTable.getType(RealStmt([],[],['x']),theSymtab)
        theSymtab.enter_name('x',SymtabEntry(SymtabEntry.VariableEntryKind, type=RealStmt,typetab_id=theTypeId))
        stmt = 'real, dimension(2,3) :: y'
        ps = pps(stmt)
        theTypeId = globalTypeTable.getType(ps,theSymtab)
        theSymtab.enter_name('y', SymtabEntry(SymtabEntry.VariableEntryKind,
                                              type=RealStmt,
                                              dimensions=['2','3'],
                                              typetab_id=theTypeId))
        e1 = ep('x * y(1,2:)')
        ae(str(expressionShape(e1,theSymtab,lineNumber=0)),
           str([Ops(':','2','3')]))

    def test4(self):
        'scalar * range w open lbound'
        ae = self.assertEquals

        theTypeId = globalTypeTable.getType(RealStmt([],[],['x']),theSymtab)
        theSymtab.enter_name('x',SymtabEntry(SymtabEntry.VariableEntryKind, type=RealStmt,typetab_id=theTypeId))
        stmt = 'real, dimension(2,3) :: y'
        ps = pps(stmt)
        theTypeId = globalTypeTable.getType(ps,theSymtab)
        theSymtab.enter_name('y', SymtabEntry(SymtabEntry.VariableEntryKind,
                                              type=RealStmt,
                                              dimensions=['2','3'],
                                              typetab_id=theTypeId))
        e1 = ep('x * y(1,:2)')
        ae(str(expressionShape(e1,theSymtab,lineNumber=0)),
           str([Ops(':','1','2')]))

    def test5(self):
        'scalar * range w open lbound and dimension range'
        ae = self.assertEquals

        theTypeId = globalTypeTable.getType(RealStmt([],[],['x']),theSymtab)
        theSymtab.enter_name('x',SymtabEntry(SymtabEntry.VariableEntryKind, type=RealStmt,typetab_id=theTypeId))
        stmt = 'real, dimension(2:4,-1:2) :: y'
        ps = pps(stmt)
        theTypeId = globalTypeTable.getType(ps,theSymtab)
        theSymtab.enter_name('y', SymtabEntry(SymtabEntry.VariableEntryKind,
                                              type=RealStmt,
                                              dimensions=[Ops(':','2','4'),Ops(':','-1','2')],
                                              typetab_id=theTypeId))
        e1 = ep('x * y(:3,1)')
        ae(str(expressionShape(e1,theSymtab,lineNumber=0)),
           str([Ops(':','2','3')]))

suite = asuite(ShapeOpsExpressions)

if __name__ == '__main__':
    sys.exit(runit(suite))

