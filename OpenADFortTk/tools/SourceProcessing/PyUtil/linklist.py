'''
(single) linked lists. Different from python 'lists' because
need real links. Want to implement mutation operations with
nodes having efficient delete, splice methods:
   eg self.del() will use self._prev to adjust links
      self.splice(a,b,c,d...) will create a linked list of nodes,
       and use self._prev to adjust pointers

Nil singleton is created by creating 1 instance of local __Nil class
'''
from PyUtil.struct import Struct

class __Nil(object):
    'effectively, the nil object'
    def set_prev(self,node): pass
    def rm_prev(self): pass
    def nilp(self): return True
    def rm(self): pass
    def repl(self,*v,**kw): pass
    def splice(self,*v,**kw):pass
    def mutate(self):
        'nothing happens here'
        pass
    def __repr__(self): return 'Nil'

class Node(object):
    def __init__(self,val,lst):
        '''like lisp "cons": takes a val and a node:
        returns a node = (val . lst) in lisp notation
        '''
        self.val  = val
        self.link = lst

    def __repr__(self): return 'Node(%s)' % repr(self.val)
    def __str__(self):  return 'Node(%s)' % str(self.val)

    def nilp(self):return False

    def set_prev(self,node):
        'set the _prev field of node'
        self._prev = node

    def rm_prev(self):
        'remove the _prev attribute from a node'
        del self._prev

    def rm(self):
        'remove self from the list'
        self._prev.link = self.link
    
    def repl(self,v):
        v.link = self.link
        self._prev.link = v

    def splice(self,*vals):
        p = self._prev
        l = self.link
        p.link = l
        for v in vals:
            nn     = Node(v,l)
            p.link = nn
            p      = nn

    def mutate(self,oth=None):
        '''expects that mutator and _prev attrs are set:
              applies mutator to self
              sets _prev.link to new self ??
              sets self.link._prev to new self
              deletes self._prev
        '''
        self.mutator(oth)
        self.link.set_prev(self)
        self.rm_prev()

Nil = __Nil()

class LList(object):
    '''creates a list head:
    This exists only so that mutate can be the major op of lists
    '''
    def __init__(self,*args):
        'create a list from a seq of args'
        p = self
        p.link = Nil
        for n in args:
            v = Node(n,Nil)
            p.link = v
            p = v

    def nilp(self): return self.link.nilp()

    @staticmethod
    def from_iter(iter):
        'create LList from single iterator'
        rv = LList()
        p  = rv
        for n in iter:
            v       = Node(n,Nil)
            p.link  = v
            p       = v
        return rv

    def __iter__(self):
        'LList implements next, so is self-iterating'
        return self.nextn()
    
    def nextn(self):
        '''start with 1st node of the LList, cdr down the list
        stop iteration is node is Nil
        '''
        p = self.link
        while(True):
            if p.nilp():
                raise StopIteration
            yield p
            p = p.link

    def mutate(self,mutator,arg=None):
        '''
        set up the mutator (if it needs it)
        'cdr' down the list, mutating each node
        clr mutator (if it needs it)
        '''
        mutator.setup()
        cur = self.link
        if not cur.nilp():
            cur._prev = self
        while not cur.nilp():
            cur.mutate(arg)
            cur = cur.link
        mutator.clr()

    def __cdrn__(self,fn):
        if self.nilp(): return 'Nil'

        return 'LList(%s)' % ','.join([fn(n) for n in self])
        
    def __repr__(self): return self.__cdrn__(repr)

    def __str__(self):  return self.__cdrn__(str)

import fval as fv

class _LLMutator(object):
    def __init__(self,*a,**kw): pass
    def setup(self,*a,**kw): pass
    def clr(self,*a,**kw):   pass
    def mutate(self,*a,**kw) : pass

def vmutate(self,arg=None):
    '''use the value mutating function, but pass self so that
    list mutating actions take place there
    '''
    if not arg: arg = Struct()
    arg.lines = self
    rv = self.val.mutate(arg)
    return rv

class LLMutatorVal(_LLMutator):
    def __init__(self,vmutator,*a,**kw):
        self.vm    = vmutator
        self.vm_a  = a
        self.vm_kw = kw

    def setup(self):
        self.vm.setup(*self.vm_a,**self.vm_kw)
        Node.mutator = vmutate

    def clr(self):
        self.vm.restore(*self.vm_a,**self.vm_kw)
        del Node.mutator

class LLMutatorTriv(_LLMutator):
    def __init__(self,fn):
        self.triv = fn
    def setup(self):
        Node.mutator = self.triv
    def clr(self):
        del Node.mutator
