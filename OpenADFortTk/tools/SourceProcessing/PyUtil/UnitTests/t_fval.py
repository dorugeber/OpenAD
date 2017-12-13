from Setup import *
from unittest import *

from PyUtil.fval import Fval

class X(object):pass

class C1(TestCase):
    def test1(self):
        'Fval object call'
        ae = self.assertEquals
        a_ = self.assert_
        v1 = Fval(lambda x,y: x+y)
        ae(v1(3,4),7)
    def test2(self):
        'store Fval in a class, call it'
        ae = self.assertEquals
        a_ = self.assert_
        X.f = Fval(lambda x,y:x + y)
        ae(X.f('foo','bar'),'foobar')
    def test3(self):
        'store Fval in a instance, call it'
        ae = self.assertEquals
        a_ = self.assert_

        xx   = X()
        xx.f = Fval(lambda x,y:x + y)
        ae(xx.f(3,4),7)

s1 = makeSuite(C1)
suite = asuite(C1)

if __name__ == "__main__":
    sys.exit(runit(suite))
