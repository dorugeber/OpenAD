'''
Test the free format stuff by actually reading files
'''
from Setup     import *
from unittest  import *
from PyFort.fortFile  import Ffile
from PyFort.fortLine  import *
from PyFort.fortStmts import *
from PyFort.fortStmts import _F90ExplLen,_Star,_NoInit,_Kind,_ExplKind
from useparse  import *


class T1(TestCase):

    def test1(self):
        'simple free format string'

        ae = self.assertEquals
        a_ = self.assert_
        l1 = preclip('''
  logical subroutine(x,y, &
      & z)
''')
        flow.setInputFormat('free')
        flow.setOutputFormat('free')
        f1 = Ffile.here(l1,True)
        ll = list(f1.lines)
        ae(len(ll),1)
        a_(isinstance(ll[0],fline))
        ae(ll[0].line,'logical subroutine(x,y,  z)')
        ae(ll[0].rawline,'logical subroutine(x,y,  z)')

    def test2(self):
        'continued free format string with ! comments -- KNOWN TO FAIL (internal comments are not currently being preserved, see https://trac.mcs.anl.gov/projects/openAD/ticket/187)'

        ae = self.assertEquals
        a_ = self.assert_
        l1 = preclip('''
  logical subroutine(x,y, &
      & z) ! test it
''')
        f1 = Ffile.here(l1,True)
        ll = list(f1.lines)
        ae(len(ll),1)
        a_(isinstance(ll[0],fline))
        ae(ll[0].line,'logical subroutine(x,y,  z) ')
        ae(ll[0].rawline,'logical subroutine(x,y,  z) ')

    def test3(self):
        'uncontinued free format string with ! comments -- KNOWN TO FAIL (internal comments are not currently being preserved, see https://trac.mcs.anl.gov/projects/openAD/ticket/187)'

        ae = self.assertEquals
        a_ = self.assert_
        l1 = preclip('''
function foo(x,y) ! test it  
''')
        f1 = Ffile.here(l1,True)
        ll = list(f1.lines)
        ae(len(ll),1)
        a_(isinstance(ll[0],fline))
        ae(ll[0].line,'function foo(x,y) ')
        ae(ll[0].rawline,'function foo(x,y) ')

    def test4(self):
        'decl with ! comment'

        ae = self.assertEquals
        a_ = self.assert_
        l1 = 'real :: x(:) ! here comes the comment'
        f1 = Ffile.here(l1,True)
        ll = list(f1.lines)
        ae(repr(pps(ll[0].line)),repr(RealStmt([],[],[_NoInit(App('x',[':']))])))

    def test5(self):
        'continued statement with goto'
        ae = self.assertEquals
        a_ = self.assert_
        l1 = preclip('''
 1900 IF (ABSSB.GE.APMWHD(20,IPMP).OR.ABS(WB).GE.APMWHD(19,IPMP))  GO TO&!PFLW.169
         2000 !PFLW.170
''')
        flow.setInputFormat('free')
        flow.setOutputFormat('free')
        f1 = Ffile.here(l1,True)
        ll = list(f1.lines)
        ae(len(ll),1)
        a_(isinstance(ll[0],fline))
	compLine='IF (ABSSB.GE.APMWHD(20,IPMP).OR.ABS(WB).GE.APMWHD(19,IPMP))  GO TO 2000 '
        ae(ll[0].line,compLine)

s1 = asuite(T1)
suite = asuite(T1)

if __name__ == '__main__':
    sys.exit(runit(suite))
