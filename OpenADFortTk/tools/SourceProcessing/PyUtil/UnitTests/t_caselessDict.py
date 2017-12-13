from Setup import *
from unittest import *
from PyUtil.caselessDict import *

class C1(TestCase):
    def test1(self):
        'store under key "foo", retrieve under key "Foo"'
        ae = self.assertEquals
        a_ = self.assert_

        t1 = caselessDict()
        t1['foo'] = 'bar'
        ae(t1['Foo'],'bar')
        ae(t1['FOO'],'bar')

    def test2(self):
        'build dict using kw'
        ae = self.assertEquals
        a_ = self.assert_
        t1 = caselessDict(FOO='baz')
        ae(t1['foo'],'baz')

    def test3(self):
        'buid dict using item pairs'
        ae = self.assertEquals
        a_ = self.assert_
        t1 = caselessDict(('foo','bar'),('BAZ','foob'))
        ae(t1['fOO'],'bar')
        ae(t1['baz'],'foob')

s1 = asuite(C1)

suite = asuite(C1)

if __name__ == '__main__':
    sys.exit(runit(suite))

