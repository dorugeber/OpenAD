'''
Test list stuff

'''
from Setup import *
from unittest import *
from itertools import *

from PyUtil.linklist import LList,LLMutatorVal,LLMutatorTriv

class C1(TestCase):
    def test1(self):
        'make empty list'
        ae = self.assertEquals
        a_ = self.assert_
        v = LList()
        a_(v.nilp())
    def test2(self):
        'make nonempty list'
        ae = self.assertEquals
        a_ = self.assert_
        v  = LList(1,2,3,4,5)
        a_(not v.nilp())
        for (a,b) in izip(xrange(5),v):
            ae(a+1,b.val)

    def test3(self):
        'simple mutation removal'
        @LLMutatorTriv
        def mu1(self,*a):
            if self.val == 4:
                self.rm()
        ae = self.assertEquals
        a_ = self.assert_
        v  = LList(1,2,3,4,5)
        v.mutate(mu1)
        for (a,b) in izip([1,2,3,5],v):
            ae(a,b.val)

    def test4(self):
        'simple mutation splice'
        ae = self.assertEquals
        a_ = self.assert_

        @LLMutatorTriv
        def mu1(self,oth):
            'insert a few items'
            if self.val == 4:
                self.splice(3.5,self.val,4.5)

        v  = LList(1,2,3,4,5)
        v.mutate(mu1)
        for (a,b) in izip([1,2,3,3.5,4,4.5,5],v):
            ae(a,b.val)

s1 = asuite(C1)

suite = asuite(C1)

if __name__ == "__main__":
    sys.exit(runit(suite))
