from Setup              import *
from unittest           import *
from PyUtil.l_assembler import *

from PyFort.op_prec import OpPrec

def mkexp(e):
    (a,oplst) = e
    for (op,e2) in oplst:
        a = '(%s %s %s)' % (op,a,mkexp(e2))
    return a

def scan(s):
    import PyFort.fortScan as fs
    return filter(lambda x: x != ' ',fs.scan1.scan(s)[0])


class C1(TestCase):
    def setUp(self):
        optbl = [(list('+-'),1),
                 (list('*/'),2),
                 (['**'],3),
                 ]
        E1 = OpPrec(pred(lambda v: v in list('abcdefghijklmnopqrstuvwxyz')),
                    optbl,
                    ['**'])
        E1 = treat(E1,mkexp)

        self.E1 = E1

    def test1(self):
        'exp = simple var'

        ae = self.assertEquals
        a_ = self.assert_
        E1 = self.E1

        s     = 'x'
        (v,r) = E1(scan(s))
        ae(v,s)
        ae(r,[])

    def test2(self):
        'exp = x*y + a*b'

        ae = self.assertEquals
        a_ = self.assert_
        E1 = self.E1

        s     = 'x*y + a*b'
        (v,r) = E1(scan(s))
        ae(v,'(+ (* x y) (* a b))')
        ae(r,[])

    def test3(self):
        'exp = a ** b ** c'

        ae = self.assertEquals
        a_ = self.assert_
        E1 = self.E1

        s     = 'a ** b ** c'
        (v,r) = E1(scan(s))
        ae(v,'(** a (** b c))')
        ae(r,[])

    def test4(self):
        'exp = x * y**c + k*y**d + l'

        ae = self.assertEquals
        a_ = self.assert_
        E1 = self.E1

        s     = 'x * y**c + k*y**d + l'
        (v,r) = E1(scan(s))
        ae(v,'(+ (+ (* x (** y c)) (* k (** y d))) l)')
        ae(r,[])

    def test5(self):
        'exp = x * y *'

        ae = self.assertEquals
        a_ = self.assert_
        E1 = self.E1

        s     = 'x * y *'
        (v,r) = E1(scan(s))
        ae(v,'(* x y)')
        ae(r,['*'])

s1    = makeSuite(C1)
suite = asuite(C1)

if __name__ == '__main__':
    sys.exit(runit(suite))
