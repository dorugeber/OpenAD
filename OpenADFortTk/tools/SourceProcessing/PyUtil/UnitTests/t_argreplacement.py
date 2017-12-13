from Setup import *
from unittest import *
from PyUtil.argreplacement import *

class C1(TestCase):
    def test1(self):
        'replacement with possibility of two-step changes for x'
        ae = self.assertEquals
        a_ = self.assert_
        t1 = replaceArgs("x%d+y%d", ['x', 'y'], ['ad_prp_1', fe.App('x',['2'])])
        ae(t1,"ad_prp_1%d+x(2)%d")

    def test2(self):
        'replacement with possibility of mismatch of an arg as  a substring in another arg'
        ae = self.assertEquals
        a_ = self.assert_
        t1 = replaceArgs("x%d+xx%d", ['x', 'xx'], ['a', 'b'])
        ae(t1,"a%d+b%d")

s1 = asuite(C1)

suite = asuite(C1)

if __name__ == '__main__':
    sys.exit(runit(suite))

