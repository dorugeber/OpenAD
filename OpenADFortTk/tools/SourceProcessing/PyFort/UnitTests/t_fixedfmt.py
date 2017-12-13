from unittest  import *
from copy      import copy as ocp
from copy      import deepcopy as docp

from Setup     import *
from PyFort.fixedfmt  import *
from PyFort.fixedfmt  import _comment_p
from cStringIO import StringIO

class preds(TestCase):
    s1 = '''
c This is a comment
'''
    s2 = '''

'''
    s3 = '''
      x(10) = y * 14
'''
    s4 = '''
     +x = y * 10 + 'qqq' // 'rrr'
'''
    s5 = '''
10    foo = sin(bar)
'''
    s6 = '''
c $openad foob bar glurg
'''
    s7 = '''
c $openad$ foob bar glurg
'''
    (s1,s2,s3,s4,s5,s6,s7) = [ ll[1:] for ll in (s1,s2,s3,s4,s5,s6,s7)]

    def setUp(self):
        pass

    def tearDown(self):
        pass

    def test1(self):
        '''s1 is a comment'''
        s1 = preds.s1
        self.assert_(_comment_p(s1))

    def test2(self):
        '''s1 is not a stmt or continuation'''
        s1 = preds.s1
        self.assert_(not (stmt_p(s1) or cont_p(s1)))

    def test3(self):
        '''s2 (blank) is a comment and not a stmt nor a continuation'''
        s2 = preds.s2
        self.assert_(_comment_p(s2) and not (stmt_p(s2) or cont_p(s2)))

    def test4(self):
        '''s3 is a stmt, and nothing else'''
        s3 = preds.s3
        self.assert_(stmt_p(s3) and not (_comment_p(s3) or cont_p(s3)))

    def test5(self):
        '''s4 is a continuation, and nothing else'''
        s4 = preds.s4
        self.assert_(cont_p(s4) and not (_comment_p(s4) or stmt_p(s4)))

    def test6(self):
        '''s5 is a stmt, and nothing else'''
        s5 = preds.s5
        self.assert_(stmt_p(s5) and not (_comment_p(s5) or cont_p(s5)))

class C2(TestCase):

    def test1(self):
        'stmt pattern'
        f1 = file(fname_t('f1.f'))
        stmt = fixedfmt.a_stmt
        (v,rst) = stmt(buf_iter(f1))
        ae(v,['      subroutine foo(x)\n', []])

    def test2(self):
        'vgen(stmt)'
        f1 = file(fname_t('f1.f'))
        stmt = fixedfmt.a_stmt
        ll   = vgen(stmt,buf_iter(f1))
        lll  = list(ll)
        ae(len(lll),5)
        ae(lll[2],['        x = 5.0\n', []])

    def test3(self):
        'fjoin'
        f1 = file(fname_t('f1.f'))
        stmt = docp(fixedfmt.a_stmt)
        stmt.addpost(fixedfmt.fjoin)
        ll   = vgen(stmt,buf_iter(f1))
        lll  = list(ll)
        (r,jl,ic) = lll[1]
        a_(ic)
        ae(jl,'        x = x + 5 + 2 * x + sin(x+2.0)')

    def test4(self):
        'cblk'
        f1 = file(fname_t('f2.f'))
        cblk = plus(fixedfmt.comm)
        (v,rst) = cblk(buf_iter(f1))
        ae(v,['c\n', 'c\n', 'c  actual comment lines\n', '\n', '\n'])

s = asuite(C2)

suite = asuite(preds,C2)

if __name__ == "__main__":
    sys.exit(runit(suite))
