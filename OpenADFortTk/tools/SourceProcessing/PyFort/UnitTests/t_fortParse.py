from Setup import *

from unittest      import *
from PyFort.fortFile import Ffile
from PyFort.fortParse import parse_stmts,parse_cmnt
from itertools     import *
import PyFort.fortStmts as fs

class C1(TestCase):
    def test1(self):
        'parse file f3.f'

        f1   = Ffile.file(fname_t('f3.f'),c_action=parse_cmnt,s_action=parse_stmts)
        ok_s = (fs.Comments,
                fs.SubroutineStmt,
                fs.Comments,
                fs.AssignStmt,
                fs.Comments,
                fs.AssignStmt,
                fs.AssignStmt,
                fs.AssignStmt,
                fs.IfStmt,
                fs.AssignStmt,
                fs.AssignStmt,
                fs.WriteStmt,
                fs.OpenStmt,
                fs.FormatStmt,
                fs.Comments,
                fs.DoStmt,
                fs.AssignStmt,
                fs.EnddoStmt,
                fs.Comments,
                fs.EndStmt,
                )
        for (l,c) in izip(f1.lines,ok_s):
            a_(isinstance(l,c))

    def test2(self):
        'parse file f3a.f'

        f1   = Ffile.file(fname_t('f3a.f'),c_action=parse_cmnt,s_action=parse_stmts)
        ok_s = (fs.SubroutineStmt,
                fs.Comments,
                fs.AssignStmt,
                fs.EndStmt,
                )
        for (l,c) in izip(f1.lines,ok_s):
            a_(isinstance(l,c))

s1 = makeSuite(C1)

suite = asuite(C1)

if __name__ == "__main__":
    sys.exit(runit(suite))
