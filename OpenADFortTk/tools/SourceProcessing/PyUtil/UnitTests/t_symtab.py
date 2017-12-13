from Setup import *
from unittest import *
from PyUtil.symtab import Symtab, SymtabEntry

from PyFort.fortStmts import RealStmt,IntegerStmt

Symtab.setTypeDefaults((RealStmt,[]),(IntegerStmt,[]))

class C1(TestCase):
    topSE=SymtabEntry(SymtabEntry.GenericEntryKind())
    fooSE=SymtabEntry(SymtabEntry.GenericEntryKind())
    def setUp(self):
        t = Symtab()
        t.enter_name('TOP',C1.topSE)

        ts = Symtab(t)
        ts.enter_name('fOo',C1.fooSE)
        self.ts = ts

    def test1(self):
        'local lookup'
        ts = self.ts
        ae(ts.lookup_name('foo'),C1.fooSE)

    def test2(self):
        'remote lookup'
        ts = self.ts
        ae(ts.lookup_name('top'),C1.topSE)

    def test3(self):
        'no entry'
        ts = self.ts
        a_(not ts.lookup_name('bar'))

s1 = asuite(C1)

suite = asuite(C1)

if __name__ == '__main__':
    sys.exit(runit(suite))
