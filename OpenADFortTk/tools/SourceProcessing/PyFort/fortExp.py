'''
Expression parser for fortran expressions:
  use rec decent
'''
import re,copy

from _Setup import *

from PyIR.mutable_tree import _Mutable_T

from PyUtil.l_assembler import *
from PyUtil.flatten import flatten

import PyFort.fortScan  as fs
from PyFort.op_prec import OpPrec

_id_re     = re.compile(fs.id_re,re.I | re.X)
_flonum_re = re.compile(fs.flonum_re,re.I | re.X)
_int_re    = re.compile(fs.int_re,re.I | re.X)
_q_re      = re.compile(fs.q_re,re.I | re.X)
_qq_re     = re.compile(fs.qq_re,re.I | re.X)
_num_re    = re.compile(fs.flonum_re + '|' + fs.int_re,re.I | re.X)

_quote_set   = set(['"',"'"])
_logicon_set = set(['.true.','.false.'])
_optbl       = (([':']     ,1),
                (['.eqv.',
                  '.neqv.'],2),
                (['.or.']  ,3),
                (['.and.'] ,4),
                (['.le.',
                  '.lt.',
                  '.eq.',
                  '.ne.',
                  '.ge.',
                  '.gt.',
                  '<',
                  '<=',
                  '>',
                  '>=',
                  '==',
                  '/=']   ,5),
                (['//']   ,6),
                (['+','-'],7),
                (['*','/'],8),
                (['**']   ,9),
                )

_unary_ops   = ['-',
                '+',
                '.not.',
                ]

def is_op(op):
    '''
    Check to see if op is in the _optbl

    This routine is only necessary for
    unit test to check if all ops are covered
    '''
    op_l = op.lower()
    for (lst,prec) in _optbl:
        if op_l in lst:
            return True
    return False

_whitespace = False

def setWhitespace(useWhitespace):
    _whitespace = useWhitespace    

def copyExp(exp):
    '''handle base cases for custom deepcopy'''
    if isinstance(exp,list):
        return map(copyExp,exp)
    if isinstance(exp,tuple):
        return tuple(map(copyExp,exp))
    elif isinstance(exp,str) or isinstance(exp,bool) or exp is None:
        return exp
    else:
        return copy.deepcopy(exp)

class _Exp(_Mutable_T):
    'base class for Expression trees'
    _sons = []
    def get_sons(self):
        return self._sons
    def set_son(self,theSon,newSon):
        setattr(self,theSon,newSon)
        self.modified = True
    def __deepcopy__(self,memo={}):
        '''cheaper deepcopy implementation for copying expressions'''
        newSons=map(lambda aSon: copyExp(getattr(self,aSon)),self.get_sons())
        return self.__class__(*newSons)
    pass

class App(_Exp):
    'application expressions (ie f(x))'
    _sons = ['head','args']
    def __init__(self,head,args):
        self.head = head
        self.args = args

    def __repr__(self):
        return 'App(%s,%s)' % (repr(self.head),repr(self.args))

    def __str__(self):
        return '%s(%s)' % (str(self.head),
                           ','.join([str(l) for l in self.args]))
    def map(self,fn):
        return App(self.head,[fn(a) for a in self.args])

class NamedParam(object):
    '''
    Class to implement named paramter expressions i.e. call foo(arg=10)
    these may occur either in a parameter statement or as an argument to a function or subroutine call
    '''
    _sons = ['myId','myRHS']

    def __init__(self,anId,aRHS):
        self.myId = anId
        self.myRHS = aRHS
        self.accessed = False

    def __repr__(self):
        return 'NamedParam(%s,%s)' % (self.myId,repr(self.myRHS))

    def __str__(self):
        rstr=str(self.myId)
        if _whitespace:
            rstr+=' = '
        else:
            rstr+='='
        rstr+=str(self.myRHS)
        return rstr

    def get_sons(self):
        self.accessed = True
        return self._sons

    def set_son(self,theSon,newSon):
        setattr(self,theSon,newSon)
        self.modified = True

    def __deepcopy__(self,memo={}):
        return NamedParam(self.myId,self.myRHS)
class Sel(_Exp):
    'selection expressions like foo(i,j)%k'

    _sons = ['head','proj']
    
    def __init__(self,head,proj):
        self.head = head
        self.proj = proj

    def __repr__(self):
        return 'Sel(%s,%s)' % (repr(self.head),repr(self.proj))

    def __str__(self):
        return '%s%%%s' % (str(self.head),str(self.proj))

    def map(self,fn):
        return Sel(fn(self.head),fn(self.proj))

    def lower(self):
        return Sel(self.head.lower(),self.proj.lower())

class _AtomH(object):
    'helper class, captures the args of app or selection'
    _sons = ['arglist']

    def __init__(self,arglist):
        self.arglist = arglist

    def make(self):
        return (self.__class__.constr,self.arglist)

class _AppH(_AtomH):
    constr = App

class _SelH(_AtomH):
    constr = Sel

class Slice(_Exp):
    pass
    
class Rslice(Slice):

    _sons = ['arg']

    def __init__(self,e):
        self.arg = e
    def __repr__(self):
        return 'Rslice(%s)' % repr(self.arg)
    def __str__(self):
        return ':%s' % str(self.arg)

class Lslice(Slice):

    _sons = ['arg']

    def __init__(self,e):
        self.arg = e
    def __repr__(self):
        return 'Lslice(%s)' % repr(self.arg)
    def __str__(self):
        return '%s:' % str(self.arg)

class Zslice(Slice):
    def __init__(self): pass
    def __repr__(self):
        return 'Zslice()'
    def __str__(self):
        return ':'

class MultiParenExp(_Exp):
    'parenthesized expression with a comma-separated list like (a,b)'
    _sons = ['expList']
    
    def __init__(self,expList):
        self.expList = expList

    def __repr__(self):
        return 'MultiParenExp(%s)' % (repr(self.expList),)

    def __str__(self):
        return '(%s)' % (','.join([str(e) for e in self.expList]))

    def map(self,fn):
        return MultiParenExp([fn(self.exp) for e in self.expList])

class Unary(_Exp):
    _sons = ['exp']

class Umi(Unary):
    'unary minus'
    def __init__(self,exp):
        self.exp = exp

    def __repr__(self):
        return 'Umi(%s)' % (repr(self.exp),)

    def __str__(self):
        return '-%s' % (str(self.exp),)

    def map(self,fn):
        return Umi(fn(self.exp))

class Upl(Unary):
    'unary plus'
    def __init__(self,exp):
        self.exp = exp

    def __repr__(self):
        return 'Upl(%s)' % (repr(self.exp),)

    def __str__(self):
        return '+%s' % (str(self.exp),)

    def map(self,fn):
        return Upl(fn(self.exp))

class Not(Unary):
    'unary not'

    def __init__(self,exp):
        self.exp = exp

    def __repr__(self):
        return 'Not(%s)' % (repr(self.exp),)

    def __str__(self):
        return '.not. %s' % (str(self.exp),)

    def map(self,fn):
        return Not(fn(self.exp))

class ParenExp(Unary):
    'parenthesized expression'
    def __init__(self,exp):
        self.exp = exp

    def __repr__(self):
        return 'ParenExp(%s)' % (repr(self.exp),)

    def __str__(self):
        return '(%s)' % (str(self.exp),)

    def map(self,fn):
        return ParenExp(fn(self.exp))

class Ops(_Exp):
    'some sequence of binops'
    _sons = ['op','a1','a2']

    def __init__(self,op,a1,a2):
        self.op = op
        self.a1 = a1
        self.a2 = a2
    
    def __repr__(self):
        return 'Ops(%s,%s,%s)' % (repr(self.op),repr(self.a1),repr(self.a2),)

    def __str__(self):
        if _whitespace:
            return '%s %s %s' % (str(self.a1),
                                 str(self.op),
                                 str(self.a2))
        else:
            return '%s%s%s' % (str(self.a1),
                               str(self.op),
                               str(self.a2))

    def map(self,fn):
        return Ops(self.op,fn(self.a1),fn(self.a2))


def is_id(t):
    return _id_re.match(t)

def is_int(t):
    return _int_re.match(t)

def is_const(t):
    ''' is \param t a literal constant'''
    t1 = t.lower()
    return t1[0] in _quote_set or t1 in _logicon_set or _num_re.match(t1)

def isConstantExpression(anExpression):
    ''' is \param anExpression an expression of literal constants'''
    if isinstance(anExpression,str):
        return is_const(anExpression)
    elif isinstance(anExpression,int):
        return True
    elif isinstance(anExpression,Unary):
        return isConstantExpression(anExpression.exp)
    elif isinstance(anExpression,Ops):
        return (isConstantExpression(anExpression.a1) and isConstantExpression(anExpression.a2))
    else:
        return False

def _lc_const(t):
    t1 = t.lower()
    if t1 in _logicon_set:
        return t1
    return t

def is_unary(t):
    t1 = t.lower()
    return t1 in _unary_ops

def _squeeze(exp_list):
    ''' turn an expression list into an actual list
    with no embedded commas or lists
    '''
    (e1,rest) = exp_list
    return [e1] + [e2 for (dc,e2) in rest]

def _mkapp_e_r(aPatternMatchResult):
    'return an Rslice object'
    (colon,e) = aPatternMatchResult
    return Rslice(e)

def _mkapp_z(aPatternMatchResult):
    'return a Zslice object'
    return Zslice()

def _mkapp_e_l(aPatternMatchResult):
    'return an Lslice object'
    (e,colon) = aPatternMatchResult
    return Lslice(e)

def _mk_namedParamExp(a):
    'return named parameter binding'
    return NamedParam(a[0],a[2])

def _mkapp1(a):
    '''turn a recognized app into an App object'''
    (h,dc1,el,dc2) = a
    return App(h,_squeeze(el))

def _mkapp0(a):
    '''turn a recognized app with empty arglist into an App object'''
    (h,dc1,dc2) = a
    return App(h,[])

def _mksel(a):
    '''turn a recognized sel into a Sel object'''
    (head,dc,proj) = a
    return Sel(head,proj)

def _mkapp_ch(a):
    'turn a chain of apps into a single app'
    (h,lst) = a
    for l in lst:
        (cls,arg) = l.make()
        h = cls(h,arg)
    return h

def _mksel(a):
    'turn recognized selection into a Sel object'
    (h,dc,p) = a
    return Sel(h,p)

def _mkumi(um):
    'turn a recognized unary minus into a Umi object'
    (dc,e) = um
    return Umi(e)

def _mkupl(um):
    'turn a recognized unary plus into a Upl object'
    (dc,e) = upl
    return Upl(e)

def _mkunot(unot):
    (dc,e) = unot
    return Not(e)

_unary_tbl = {'+'    : Upl,
              '-'    : Umi,
              '.not.': Not,
              }

def _mkunary(e):
    (op,ee) = e
    return _unary_tbl[op.lower()](ee)

def _mkparen(parn):
    'turn a recognized parenthesized expression into a paren object'
    (dc,e,dc2) = parn
    return ParenExp(e)

def _makeMultiParen(formMultiParen):
    'turn a recognized multiple parenthesized expression into a MultiParenExp object'
    (leftParen,expList,rightParen) = formMultiParen
    return MultiParenExp(expList)

def _mkexp(e):
    'convert a recognized exp into an Ops object'
    (a,oplst) = e
    for (op,a2) in oplst:
        a = Ops(op,a,_mkexp(a2))
    return a

id      = pred(is_id)
const   = pred(is_const)
const   = treat(const,_lc_const)
unary   = pred(is_unary)

def atom0(scan):
    '''eta expansion, since letrec unavail in python'''
    return disj(NamedParmExp,id,const,unaryExp,paren,formMultiParen,ArrayConstructor.form)(scan)

def atom(scan):

    _app1_trail = seq(lit('('),cslist(_appExp),lit(')'))
    _app1_trail = treat(_app1_trail,lambda a: _AppH(a[1]))

    _app0_trail = seq(lit('('),lit(')'))
    _app0_trail = treat(_app0_trail,lambda a: _AppH([]))

    _sel_trail  = seq(lit('%'),id)
    _sel_trail  = treat(_sel_trail,lambda a: _SelH(a[1]))

    _app_trail  = disj(_app1_trail,_app0_trail,_sel_trail)

    p0 = seq(atom0,star(_app_trail))
    p0 = treat(p0,_mkapp_ch)

    return p0(scan)

'''
FIXME:
   duplicated *_trail code, cannot factor out easily due to fwd ref
'''

def lv_exp(scan):
    'proper syntax for an lvalue'

    _app1_trail = seq(lit('('),cslist(_appExp),lit(')'))
    _app1_trail = treat(_app1_trail,lambda a: _AppH(a[1]))

    _app0_trail = seq(lit('('),lit(')'))
    _app0_trail = treat(_app0_trail,lambda a: _AppH([]))

    _sel_trail  = seq(lit('%'),id)
    _sel_trail  = treat(_sel_trail,lambda a: _SelH(a[1]))

    _app_trail  = disj(_app1_trail,_app0_trail,_sel_trail)

    p0 = seq(id,star(_app_trail))
    p0 = treat(p0,_mkapp_ch)

    return p0(scan)

Exp = OpPrec(atom,_optbl,('**',))
Exp = treat(Exp,_mkexp)

NamedParmExp = seq(id,lit('='),Exp)
NamedParmExp = treat(NamedParmExp,_mk_namedParamExp)

StarExp  = lit('*')

NamedParmExpWithStar = seq(id,lit('='),disj(Exp,StarExp))
NamedParmExpWithStar = treat(NamedParmExpWithStar,_mk_namedParamExp)

ExpList = seq(disj(NamedParmExp,Exp),star(seq(lit(','),disj(NamedParmExp,Exp))))

_appExpR = seq(lit(':'),Exp)
_appExpR = treat(_appExpR,_mkapp_e_r)

_appExpL = seq(Exp,lit(':'))
_appExpL = treat(_appExpL,_mkapp_e_l)

_appExpZ = lit(':')
_appExpZ = treat(_appExpZ,_mkapp_z)

_appExp = disj(_appExpR,_appExpL,_appExpZ,NamedParmExp,Exp)

app1      = seq(id,lit('('),ExpList,lit(')'))
app1      = treat(app1,_mkapp1)

app0      = seq(id,lit('('),lit(')'))
app0      = treat(app0,_mkapp0)

app       = disj(app1,app0)

sel       = seq(id,lit('%'),id)
sel       = treat(sel,_mksel)

unaryExp  = seq(unary,atom)
unaryExp  = treat(unaryExp,_mkunary)

paren     = seq(lit('('),Exp,lit(')'))
paren     = treat(paren,_mkparen)

formMultiParen = seq(lit('('),
                 cslist(Exp),
                 lit(')'))
formMultiParen = treat(formMultiParen,_makeMultiParen)

class LoopControl(_Exp):
    '''loop control part of DO statements, data-implied-do, and array constructor'''
    _sons = ['var','start','end','stride']
    # form of loop control is:
    #  [,] scalar-integer-variable-name = scalar-integer-expression , scalar-integer-expression [, scalar-integer-expression]
    form = seq(id,               # 0 = var
               lit('='),         #
               Exp,              # 2 = start
               lit(','),         #
               Exp,              # 4 = end
               zo1(seq(lit(','), # 5 = stride
                       Exp)))
    form = treat(form, lambda x: LoopControl(x[0],x[2],x[4],x[5] and x[5][0][1] or None))

    def __init__(self,var,start,end,stride):
        self.var = var
        self.start = start
        self.end = end
        self.stride = stride # optional

    def __str__(self):
        optionalStrideStr = self.stride and ','+str(self.stride) \
                                         or ''
        return '%s = %s,%s%s' %  (self.var,
                                  str(self.start),
                                  str(self.end),
                                  optionalStrideStr)

    def __repr__(self):
        return '%s(%s,%s,%s,%s)' % (self.__class__.__name__,
                                    repr(self.var),
                                    repr(self.start),
                                    repr(self.end),
                                    repr(self.stride))


class ArrayConstructorImpliedDo(_Exp) :
    _sons = ['valueList','loopControl']

    # form: ( ac-value-list , ac-do-variable = scalar-integer-expression , &
    #         scalar-integer-expression [ , scalar-integer-expression
    form = seq(lit('('),         #
               Exp,              # 1
               lit(','),         #
               LoopControl.form, # 3
               lit(')'))
    form = treat(form, lambda x: ArrayConstructorImpliedDo(x[1],x[3]))

    def __init__(self,valueList,loopControl) :
        self.valueList = valueList
        self.loopControl = loopControl

    def __repr__(self) :
        return '%s(%s,%s)' % (self.__class__.__name__,
                              repr(self.valueList),
                              repr(self.loopControl))

    def __str__(self):
       #return '( %s ,%s )' % (','.join([str(aValue) for aValue in self.valueList]),
       #                       str(loopControl))
        return '( %s,%s )' % (str(self.valueList), str(self.loopControl))


class ArrayConstructor(_Exp):
    '''assigning an entire array at once, like this: a = (/ 2, 3, 5, 7, 11, 13, 17 /)
       or like this : a=[ 2, 3, 5, 7, 11, 13, 17 ]'''
    _sons = ['valueList']

    formValue = disj(Exp,
                     ArrayConstructorImpliedDo.form)
    formOld = seq(lit('('),          # 0
                  lit('/'),          # 1
                  cslist(formValue), # 2 = valueList
                  lit('/'),          # 3
                  lit(')'))          # 4
    formNew = seq(lit('['),          # 0 
                  cslist(formValue), # 1 = valueList
                  lit(']'))          # 2
    form = disj(formOld,formNew)
    form = treat(form, lambda x: len(x)==5 and ArrayConstructor(x[2]) or ArrayConstructor(x[1]))

    def __init__(self,valueList):
        self.valueList = valueList

    def __repr__(self):
        return '%s(%s)' % (self.__class__.__name__,repr(self.valueList))

    def __str__(self):
        return '(/' + ','.join([str(aValue) for aValue in self.valueList]) + '/)'


# utility list
#
app_id    = disj(app,id)
app_id_l  = seq(app_id,star(seq(lit(','),app_id)))
app_id_l  = treat(app_id_l,_squeeze)

#
# simple subtree replacement function
#
def subst(a,pat,repl):
    if pat(a):
        return repl(a)
    elif isinstance(a,str):
        return a
    elif not a._sons:
        return a
    else:
        return a.map(lambda x:subst(x,pat,repl))
