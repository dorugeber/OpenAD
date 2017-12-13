from _Setup import *
from PyUtil.l_assembler import *
from PyUtil.l_assembler import _pat

class _OpPrec(object):
    '''create an operator precedence expression parser
    from an atom assembler, an operator table, and a list
    of the operators that are right associative
    '''

    def __init__(self,atom,optbl,right=()):
        '''The operator precedence assembler needs 3 data elements:

           1)atom  = the base assembler (typically vars & constants)

           2)optbl = the operator precedence table:
                     a sequence of pairs of the form (oplst,prec)
                     where oplst is a sequence of operator tokens,
                     and prec is the integer precedence level for
                     each op in the oplst

           3)right = sequence of operators that are right associative
        '''
        self.atom  = atom
        self.right = right
        self.opset = [l for (ops,p) in optbl for l in ops]

        self.prec  = dict()
        for (s,p) in optbl:
            for op in s:
                self.prec[op.lower()] = p

    def __call__(self,s):
        '''OpPrec objects should function like an assembler function
        So, it must be callable.
        The effect should be to call the bound method self.exp(0) on
        an input stream
        '''
        return self.exp(0)(s)

    def is_op(self,e):
        return e.lower() in self.opset

    def q(self,op):
        qq = self.prec[op.lower()]
        if op in self.right:
            return qq
        else:
            return qq+1

    def op(self,p):
        return pred(lambda e:self.is_op(e) and self.prec[e.lower()] >= p)

    def opseq(self,p):
        'special dependent sequence'

        def asm(s):
            s_save = s
            rv = []
            try:
                (v,s) = self.op(p)(s)
                rv.append(v)
                (v,s) = self.exp(self.q(v))(s)
                rv.append(v)
                return (rv,s)
            except AssemblerException,excp:
                msg = excp.msg + '->special opseq failure'
                raise AssemblerException(msg,s_save)

        return asm

    def exp(self,p):
        return seq(self.atom,star(self.opseq(p)))

def OpPrec(atom,optbl,right=()):
    'wrapper function to make _OpPrec class into a proper l_assembler pattern object'
    tmp = _OpPrec(atom,optbl,right)
    return _pat(tmp)


