from Setup import *
from _Setup import *
from unittest import *
from useparse import *

from PyFort.fortExp import _optbl
import PyFort.fortExp as fe

from mutable_tree import _Mutable_T,LexiMutator,mutate
#
# nodes
#
class _Node(_Mutable_T):
    'abstract node class'
    _sons = ()
    def __repr__(self):
        c = self.__class__.__name__
        args = ','.join([repr(getattr(self,s)) for s in self._args])
        return '%s(%s)' % (c,args)

class A(_Node):
    'simulate function app'
    _args = ('name','args')
    _sons = ('args',)

    def __init__(self,name,args):
        self.name = name
        self.args = args

class B(_Node):
    'simulate binary op'
    _args = ('op','a1','a2')
    _sons = ('a1','a2')

    def __init__(self,op,a1,a2):
        self.op = op
        self.a1 = a1
        self.a2 = a2

class P(_Node):
    'simulate projection (% operator in F90)'
    _args = ('head','sel')
    _sons = ('head',)

    def __init__(self,head,sel):
        self.head = head
        self.sel  = sel

class S(_Node):
    'simulate subroutine call'
    
    _args = ('name','args')
    _sons = ('args',)
    
    def __init__(self,name,args):
        self.name = name
        self.args = args

def _old_m1(self,pth,oth):
    '_V mutator'
    rv = _MC()
    if self.name == '_V':
        nv = P(self.args[0],'v')
        pth.set(nv)
        rv.prune = True
        return rv
    else:
        return rv

def m1(self,oth):
    '_V mutator'
    if self.name == '_V':
        nv = P(self.args[0],'v')
        self.repl(nv)
        self.prune(True)

class C1(TestCase):
    'mutators on A,B,P,S nodes'
    def test1(self):
        '_V mutator on S node'
        ae = self.assertEquals
        a_ = self.assert_
        mu = LexiMutator(((A,m1),))

        #call foo(_V(a) + _V(b),g(_V(x) + V(y))
        vv = S('foo',[B('+',A('_V',['a']),A('_V',['b'])),
                      A('g',[B('+',A('_V',['x']),A('_V',['y']))])])
        o  = eval(repr(vv))
        mutate(vv,mu)
        ae(repr(vv),
           repr(S('foo',[B('+',P('a','v'),P('b','v')),
                         A('g',[B('+',P('x','v'),P('y','v'))])])))

class C2(TestCase):
    def test1(self):
        "binary slice op ':' op has sons"
        ae = self.assertEquals
        a_ = self.assert_
        v = ep('xxx:yyy')
        a_(hasattr(v,'_sons'))

def _mkt2fn(s):
    def __test(self):
        ae = self.assertEquals
        a_ = self.assert_
        v  = ep('xxx %s yyy' %s)
        a_(hasattr(v,'_sons'))
    __test.__doc__ = "op '%s' has sons" % s

    return __test

__c = 2
for b in ([x for (o,p) in _optbl for x in o][1:]):
    setattr(C2,'test%d' % __c,_mkt2fn(b))
    __c += 1

def xmv(self,*dc):
    'mutate __value__ calls'
    if self.head == '__value__':
        nv = fe.Sel(self.args[0],'v')
        self.repl(nv)
        self.prune(True)

def xa(self,*dc):
    'mutate __value__ and __deriv__ calls'
    if self.head == '__value__':
        nv = fe.Sel(self.args[0],'v')
        self.repl(nv)
        self.prune(True)
    if self.head == '__deriv__':
        nv = self.args[0]
        self.repl(nv)
        self.prune(True)

class C3(TestCase):
    'mutate with some bona fide Exp nodes'
    def test1(self):
        'change a __value__node'
        ae = self.assertEquals
        a_ = self.assert_

        mu = LexiMutator([(fe.App,xmv)])
        vv = ep('foo(__value__(h(i,j)))')
        mutate(vv,mu)

        ae(str(vv),'foo(h(i,j)%v)')

    def test2(self):
        'change both __value__ and __deriv__ nodes'
        mu = LexiMutator([(fe.App,xa)])
        ss = pps('__value__(x%kk(3,i+j)%l) = 4 * __deriv__(y%l%mm)')
        mutate(ss,mu)
        self.assertEquals(str(ss),'x%kk(3,i+j)%l%v = 4*y%l%mm')

s1 = makeSuite(C3)
suite = asuite(C1,C2,C3)

if __name__ == "__main__":
    sys.exit(runit(suite))
