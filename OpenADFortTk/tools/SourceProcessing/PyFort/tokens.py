'''
Token types as classes
'''
from _Setup import *

class Token(object):
    def __init__(self,s):
        self.val = s
    def __str__(self):
        return self.val
    def __repr__(self):
        return "%s(%s)" %(self.__class__.__name__,self.val)

class Const(Token):
    pass

class IntConst(Const):
    pass

class RealConst(Const):
    pass

class QQstringConst(Const):
    pass

class QstringConst(Const):
    pass

class ComplexConst(Const):
    pass

class Ident(Token):
    pass

class Idem(Token):
    pass

