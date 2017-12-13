'''
Mixin for mapping functions for any
construction that has a seq of 'lines'
'''

def ident(l,*args,**kws) : return [l]
def noop(l,*args,**kws)  : pass
def ident1(l,*args,**kws): return l

_debug = False

class _Mappable(object):
    '''This mixin defines the various mapping hooks for
    elements.
    '''
    mapc = noop
    map1 = ident1
    map  = ident

class _Map(object):
    '''
    Mixin adds mapc (side effect only)
               map1 (single element return)
               map  (multi-element return)
    mapping functions expect to take an object with a
    sequence of lines, and apply various functions to them

    '''
    def mapc(self,lexi,*args,**kws):
        '''mapping routine for side effects only:
        calls function mapc for all lines.
        returns an iterator of 'None', but each time in the loop,
        NOTE: mapc may be defined differently for each class.

        Usually use this as:
           for dontcare in f1.mapc(lexi,arg1,...):
               do something with arg1, ...

        pure side effect may be accomplished with:
           for dc in f1.mapc(lexi,arg1,...): pass
    
        '''
        for (cls,meth) in lexi:
            cls.mapc = meth
        for l in self.lines:
            l.mapc(*args,**kws)
            yield None
        for (cls,meth) in lexi:
            cls.mapc = noop

    def map1(self,lexi,*args,**kws):
        '''mapping routine that does not splice:
        calls function map for each line.
        the map1 routine returns an iterator for a single value
        functions that are entries in the lexi return a single
        value.
        
        Typical use:
            for l in f1.map1(lexi,arg1,...,):
                do_something_with_l
        
        '''
        for (cls,meth) in lexi:
            cls.map1 = meth
        for l in self.lines:
            yield l.map1(*args,**kws)
        for (cls,meth) in lexi:
            cls.map1 = ident1

    def map(self,lexi,*args,**kws):
        '''mapping routine that splices:
        calls function map for each line.
        the map routine returns an iterator for a single value
        The difference between map and map1 is that the
        functions that are mapped (ie, entries in the lexi)
        return a *sequence* of values

        Typical use:
            for l in f1.map(lexi,...arg1...):
                do_something_with(l)
        
        '''
        for (cls,meth) in lexi:
            cls.map = meth
        for l in self.lines:
            for ll in l.map(*args,**kws):
                if _debug:
                    print 'about to yield:',ll
                yield ll
        for (cls,meth) in lexi:
            cls.map = ident
