#
# mutator classes and utilities
#

from _Setup import *
from PyUtil.fval import Fval

def _noop(s,*a,**kw):
    'mutator _noop primitive'
    pass

def _prim_m(s,*a,**kw):
    'non-node noop'
    return s

class LexiMutator(object):
    'mutator defined by lexicon (ie by cases)'
    def __init__(self,lexi,prim=_prim_m):
        self.lexi = lexi
        self.prim = Fval(prim)

    def setup(self):
        for (cls,fn) in self.lexi:
            cls.mutator = fn
        _Mutable_T.prim_mutate = self.prim

    def restore(self):
        for (cls,dc) in self.lexi:
            del cls.mutator
        _Mutable_T.mutator = _noop
        _Mutable_T.prim_mutate = Fval(_prim_m)

    clr = restore

class FnMutator(object):
    'mutator defined as a single function'
    def __init__(self,fn,prim=_prim_m):
        self.fn   = fn
        self.prim = Fval(prim)

    def setup(self):
        _Mutable_T.mutator = self.fn
        _Mutable_T.prim_mutate = self.prim

    def restore(self):
        _Mutable_T.mutator = _noop
        _Mutable_T.prim_mutate = Fval(_prim_m)
#
# paths to tree objects
#
class __Path(object):
    '''abstract path class:
    when a value is mutated to a new node class(!), then
    the path must be reassigned the new value.
    '''
class _Spath(object):
    '''a scalar path: base object + attribute name are sufficient
    '''
    def __init__(self,base,attr):
        self.base = base
        self.attr = attr

    def set(self,val):
        'set a value for a scalar path'
        setattr(self.base,self.attr,val)

class _Lpath(object):
    '''a list path object: base object + attribute + index
    '''
    def __init__(self,base,attr,idx):
        self.base = base
        self.attr = attr
        self.idx  = idx

    def set(self,val):
        'set a value for an element of a list'
        m = self.base.sonv(self.attr)
        m[self.idx] = val
#
# Mutable tree support stuff
#
def is_leaf(v):
    '''a node is a leaf if:
       1) it is a token  OR
       2) it has an empty sons list
    '''
    return isinstance(v,str) or (hasattr(v,'sons') and not v.sons)

def _prim(n):
    'if "node" n has no mutate method, then it is primitive'
    return not hasattr(n,'mutate')

class _Mutable_T(object):
    '''Support mutable tree data structures:
    expect a sons element in the class that describes what
    attributes hold subtrees.
    if one of the subtree attributes is list valued, then
    list
    '''

    def putpth(self,pth):
        self._pth = pth
        return self

    def clrpth(self):
        del self._pth
        return self

    def prune(self,v=True):
        self._prune = v
        return self._prune
    
    def pruneq(self):
        return hasattr(self,'_prune') and self._prune
            
    def repl(self,val):
        'replace self with val: use pth to link to new val'
        self._pth.set(val)

    def sonv(self,son):
        '''synonym for getattr'''
        return getattr(self,son)

    def idx_son_val(self,son,idx):
        '''get an element from a list-valued son'''
        tmp = getattr(self,son)
        return tmp[idx]

    def subtrees(self):
        'create a list of (path,subtree) pairs for a given node'
        rv = []
        for a in self._sons:
            son_val = self.sonv(a)
            if isinstance(son_val,list):
               rv.extend([(_Lpath(self,a,i),self.idx_son_val(a,i))
                          for i in range(len(son_val))])
            else:
                rv.append((_Spath(self,a),son_val))
        return rv

    mutator     = _noop
    prim_mutate = Fval(_prim_m)

    def mapp(self,*arg):
        'like mapc, but the mappor may be deleted'
        self.mappor(*arg)

    def visit(self,*arg):
        '''primarily operate on arg'''
        rv = self.visitor(*arg)
        if not (self.pruneq() or is_leaf(self)):
            for (pth,n) in self.subtrees():
                if _prim(n):
                    self.prim_visit(n,pth,*arg)
                else:
                    n.putpth(pth).visit(*arg)
                    n.clrpth()
        # FIXME: clear the _prune thing?
        return rv

    def mutate(self,oth=None):
        '''The mutation walker:
        apply the mutator to self, THEN
        mutate all subtrees
        '''
        rv = self.mutator(oth)
        if not (self.pruneq() or is_leaf(self)):
            for (pth,n) in self.subtrees():
                if _prim(n):
                    self.prim_mutate(n,pth,oth)
                else:
                    n.putpth(pth).mutate(oth)
                    n.clrpth()

def mutate(top,mutator,oth=None):
    '''mutate the substructure of the top level object (top).
    Use the mutator function to do it.
    !!! NOTE: This is not quite the same as the method. The
    mutator is NOT applied to the toplevel object
    '''
    mutator.setup()
    for (pth,n) in top.subtrees():
        if _prim(n):
            top.prim_mutate(n,pth,oth)
        else:
            n.putpth(pth).mutate(oth)
            n.clrpth()
    mutator.restore()
    return top
