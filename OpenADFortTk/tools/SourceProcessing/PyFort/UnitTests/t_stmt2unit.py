from Setup import *
from unittest import *

from PyUtil.symtab import Symtab, globalTypeTable
from PyFort.inference import expressionType

from PyFort.stmt2unit import *
from PyFort.stmt2unit import _implicit

from useparse import *

Symtab.setTypeDefaults((fs.RealStmt,[]),(fs.IntegerStmt,[]))

class U(object):
    def __init__(self):
        self.symtab = Symtab()

class _curr(object):
    def __init__(self):
        self.val = U()

class C1(TestCase):
    def test1(self):
        'implicit action'

        cur = _curr()

        # s0 = pps('parameter (special=2)')
        # s1 = pps('implicit integer(special) (a-f)')
        # sr = _implicit(s1,cur)
        v = cur.val.symtab
        # s2 = pps('integer(special) a')
        # t = (s2.__class__,s2.mod)
    
        # t1 = expressionType('foo',v,0)
        # ae(repr(t1),repr(t))
        
        s2 = pps('real a')
        t = (s2.__class__,s2.mod)
    
        t1 = expressionType('zoo',v,0)
        ae(t1,globalTypeTable.lookupTypeId(globalTypeTable.lookupType(s2,v)))

s1 = makeSuite(C1)
suite = asuite(C1)

if __name__ == "__main__":
    sys.exit(runit(suite))

