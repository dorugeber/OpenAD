from Setup     import *
from unittest  import *
from PyUtil.buf_iter  import *
from itertools import *

class C1(TestCase):
    def test1(self):
        'create a bufiter from a list'
        a_ = self.assert_
        ae = self.assertEquals

        b  = buf_iter([1,2,3,4])

        x,y = b.next(),b.next()
        ae(x,1)
        ae(y,2)
        ae(b.lookahead,[])

    def test2(self):
        'test putback'
        a_ = self.assert_
        ae = self.assertEquals

        b  = buf_iter([1,2,3,4])

        x,y = b.next(),b.next()
        b.putback([x])
        ae(b.lookahead,[1])

class C2(TestCase):
    def test1(self):
        'buf_iter(file) ops'
        ae = self.assertEquals

        ff      = buf_iter(open_t('f.tmp.f'))
        (x,x,x) = ff.next(),ff.next(),ff.next()
        ae(x,'       function tmp(x,y)\n')

        c       = ff.putback(['c  Surprise!\n'])
        (c1,c2) = c.next(),c.next()
        ae(c1,'c  Surprise!\n')
        ae(c2,'       integer x\n')

        c.putback(['c More Stuff\n','c comes here\n'])
        (c1,c2) = list(islice(ff,2))
        ae(c1,'c More Stuff\n')
        ae(c2,'c comes here\n')

        c1 = 0
        for t in ff: c1 += 1
        ae(c1,3)

        c1 = 0
        for t in c: c1 += 1
        ae(c1,0)

class C3(TestCase):
    def test1(self):
        'buf_iter(file) ops, making sure that buf_iter is idempotent'
        ae = self.assertEquals

        ff      = buf_iter(open_t('f.tmp.f'))
        ff      = buf_iter(ff)
        (x,x,x) = ff.next(),ff.next(),ff.next()
        ae(x,'       function tmp(x,y)\n')

        c       = ff.putback(['c  Surprise!\n'])
        (c1,c2) = c.next(),c.next()
        ae(c1,'c  Surprise!\n')
        ae(c2,'       integer x\n')

        c.putback(['c More Stuff\n','c comes here\n'])
        (c1,c2) = list(islice(ff,2))
        ae(c1,'c More Stuff\n')
        ae(c2,'c comes here\n')

        c1 = 0
        for t in ff: c1 += 1
        ae(c1,3)

        c1 = 0
        for t in c: c1 += 1
        ae(c1,0)

s1 = makeSuite(C3)

suite = asuite(C1,C2,C3)

if __name__ == "__main__":
    sys.exit(runit(suite))
