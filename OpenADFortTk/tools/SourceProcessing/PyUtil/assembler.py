'''
Combinator based assemblers for generators (streams)

In general, an assembler has this type:

stream -> (val, stream)

where stream is the initial input stream, and the result stream
is the 'remainder' after the assembly

'''

from _Setup import *

from PyUtil.flatten   import flatten
from PyUtil.buf_iter  import buf_iter

class pat(object):
    def __init__(self,fn):
        self.fn   = fn
        self.post = []
        self.pre  = []

    def __call__(self,*args,**kwargs):
        for f in self.pre:
            f()
        (v,rst) = self.fn(*args,**kwargs)
        for f in self.post:
            v = f(v)
        return (v,rst)

    def addpost(self,*fns):
        self.post.extend(fns)
        return self

    def addpre(self,*fns):
        self.pre.extend(fns)
        return self

class AssemblerException(Exception):
    '''exception for failure to assemble'''
    def __init__(self,msg,rest):
        self.msg  = msg
        self.rest = rest

    def __str__(self):
        errString='\nERROR: AssemblerError: parser failed :'+str(self.msg)
        return (errString)

def pred(p):
    '''produce an assembler based on predicate p
    if p(stream.next()), then return that value, otherwise AssemblerException
    '''
    def asm(s):
        try:
            v = s.next()
        except StopIteration:
            raise AssemblerException('Empty Assembly',buf_iter(iter([])))
        try:
            if p(v):
                return (v,s)
        except:
            pass
        raise AssemblerException('Predicate Failure',s.putback([v]))

    return pat(asm)

any = pred(lambda x:True)

def star(a):
    '''assembler that repeatedly applies asm a to the stream

    NOTE: 0 applications is ok (still assembles)
    '''
    def asm(s):
        rv = []
        sloc = s
        try:
            while(True):
                v,sloc = a(sloc)
                rv.append(v)
        except AssemblerException,excp:
            return (rv,excp.rest)
    return pat(asm)

def seq(*asms):
    '''assembler that produces a sequence of assemblies'''

    def asm(s):
        sloc = s
        rv = []
        try:
            for a in asms:
                v,sloc = a(sloc)
                rv.append(v)
            return rv,sloc

        except AssemblerException,excp:
            msg  = excp.msg + "->seq failure"
            rest = excp.rest.putback(flatten(rv))
            raise AssemblerException(msg,rest)

    return pat(asm)

def disj(*asms):
    '''assembler that produces 1st valid assembly from a list of
    assemblers'''

    def asm(s):
        for a in asms:
            try:
                return a(s)

            except AssemblerException,excp:
                s = excp.rest

        raise AssemblerException('disj failure',s)

    return pat(asm)

def treat(a,*ff):
    '''Given an assembler a, and a list of functions *ff, apply each function in the list
    to the assembler a return value, and return the value of the applications
    as the return value of the treated assembler.
    '''
    def trt(s):
        (v,rst) = a(s)
        for f in ff:
            v = f(v)
        return (v,rst)

    return pat(trt)

#    return a.addpost(f)

def pretreat(a,*ff):
    'before invoking an assembler a, call all the functions in the list'

    def pretrt(s):
        for f in ff:
            f()
        return a(s)

    return pat(pretrt)

def plus(a):
    '''given an assembler a, return the Kleene '+' operation.
    Kleene '+' = seq(a,star(a)), but the return value should still
    be a list of values
    '''
    return treat(seq(a,star(a)),lambda x: [x[0]] + x[1])

def zo1(a):
    '''implement the '?' operator of classical regexp
    engines. That is, 0 or 1 occurrences of a
    '''
    def asm(s):
        try:
            (v,r) = a(s)
            return ([v],r)
        except AssemblerException,excp:
            return ([],excp.rest)

    return pat(asm)

# if mult_stmts is true, vgen is called on something which may have
# multiple statements per a-assembly because the original file may have
# had multiple statements per line (and thus parse_stmts would have returned
# a list of parsed statements, instead of just a single one)
# in this case, the returns generator must yield each element in the list.
def vgen(a,src,mult_stmts=False):
    '''for a given assembler a, and source stream src
    vgen is a returns generator that yields a stream of a-assemblies
    from src, until src is exhausted
    '''
    rst = src
    while True:
        try:
            (v,rst) = a(rst)
            if mult_stmts:
                if isinstance(v,list):
                    for elt in v:
                        yield elt
                else:
                    yield v
            else:
                yield v
        except AssemblerException, e:
            try:
                from PyFort.fortUnit import Unit
                something = e.rest.next()
                msg="unable to assemble contents of a unit "
                if (isinstance(something,Unit)):
                    msg+="named "+str(something.uinfo.name)
                else:
                    msg+="starting with "+str(something)
                msg+="\nexception is: "+e.msg
                raise AssemblerException(msg,e.rest)
            except StopIteration, se:
                pass
            break

if __name__ == '__main__':

    from _Setup.testit import *

