'''
Combinator based assemblers for lists

In general, an assembler has this type:

list -> (val, list)

where list is the initial input list, and the result list
is the 'remainder' after the assembly

'''

class _pat(object):
    'add functionality to pattern functions by making them callable objects'

    def __init__(self,fn):
        self.fn   = fn
        self.post = []

    def __call__(self,*args,**kwargs):
        (v,rst) = self.fn(*args,**kwargs)
        for f in self.post:
            v = f(v)
        return (v,rst)

    def addpost(self,*fns):
        self.post.extend(fns)
        return self

from flatten import flatten

class AssemblerException(Exception):
    '''class for exception for failure to assemble'''
    def __init__(self,msg,rest):
        self.msg  = msg
        self.rest = rest

    def __str__(self):
        errString='\nERROR: AssemblerError: parser failed :'+str(self.msg)
        return (errString)

def pred(p):
    '''produce an assembler based on predicate p
    if p(s.next()), then return that value, otherwise AssemblerException
    '''
    def asm(s):
        if (len(s) == 0):
            raise AssemblerException("empty assembly",s)
        v = s[0]
        if p(v):
            return (v,s[1:])
        raise AssemblerException("predicate failure",s)

    return _pat(asm)

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

    return _pat(asm)

def seq(*asms):
    '''assembler that produces a sequence of assemblies'''

    def asm(s):
        s_save = s
        sloc = s
        rv = []
        try:
            for a in asms:
                v,sloc = a(sloc)
                rv.append(v)
            return (rv,sloc)

        except AssemblerException,excp:
            msg  = excp.msg + "->seq failure"
            rest = s_save

            raise AssemblerException(msg,rest)

    return _pat(asm)

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

    return _pat(asm)

def treat(a,*ff):
    '''Given an assembler a, and a function f, apply f to the
    assembler a return value, and return the value of the application
    as the return value of the treated assembler.
    NOTE: supplied for back compatibility. New code should use the
          addpost method
    '''
    def trt(s):
        (v,rst) = a(s)
        for f in ff:
            v = f(v)
        return (v,rst)

    return _pat(trt)

def plus(a):
    '''given an assembler a, return the Kleene '+' operation.
    Kleene '+' = seq(a,star(a)), but the return value should still
    be a list of values
    '''
    return treat(seq(a,star(a)),lambda x: [x[0]] + x[1])

def vgen(a,src):
    '''for a given assembler a, and source stream src
    vgen is a returns generator that yields a stream of a-assemblies
    from src, until src is exhausted
    '''
    rst = src
    while True:
        try:
            (v,rst) = a(rst)
            yield v
        except AssemblerException:
            break

def zo1(a):
    '''implement the '?' operator of classical regexp
    engines. That is, 0 or 1 occurrences of a
    '''
    def asm(s):
        try:
            (v,r) = a(s)
            return ([v],r)
        except AssemblerException:
            return ([],s)

    return _pat(asm)

def lit(s):
    '''a recognizer for a literal string
    NOTE: s is in lower case, and the match occurs
    whenever the element in question and s are equal
    in a case-insensitive manner
    '''
    return pred(lambda x:x.lower()==s)

def cslist(a):
    '''for a given assembler a, create an assembler that will
    assemble a list of comma-separated elements of a
    Example:
    suppose we have an assembler ays = pred(lambda x: x == 'a')
    That is, ays recognizes the letter a
    then cslist(ays) will recognize:
        "'a','a','a','a',...."
    which is a comma-separated list of letter 'a'
    '''
    def process(e):
        rv = []
        for (v,lst) in e:
            rv.append(v)
            for (dc,v) in lst:
                rv.append(v)
        return rv
    
    comma = pred(lambda x:x == ',')
    return treat(zo1(seq(a,star(seq(comma,a)))),process)
