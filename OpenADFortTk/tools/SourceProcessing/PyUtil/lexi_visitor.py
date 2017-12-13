'''
common routines for installing visitor/walker methods
in classes
'''

from PyUtil.fval import Fval

class LexiVisitor(object):
    '''
    generic visitor function with setup and clr methods
    '''
    def __init__(self,lexi,attr='visitor'):
        '''install all components of lexi into designated
        attribute of designated classes
        '''
        self.lexi = lexi
        self.attr = attr

    def setup(self):
        '''for each element of the lexi, assign designated
        attribute
        '''
        for (cls,meth) in self.lexi:
            setattr(cls,self.attr,meth)

    def clr(self):
        'undo setup: delete the method attr for each class in the lexi'
        for (cls,meth) in self.lexi:
            if hasattr(cls,self.attr): delattr(cls,self.attr)
        pass

class LexiPrimVisitor(LexiVisitor):
    def setup(self):
        'store Fval into attr (to avoid conversion to method)'
        for (cls,fn) in self.lexi:
            setattr(cls,self.attr,Fval(fn))

class MultiLexiVisitor(object):
    'multiple visitors: do all init/clear at once'
    def __init__(self,*visitors):
        'save list of visitors'
        self.visitors = visitors

    def setup(self):
        'setup each visitor'
        for v in self.visitors:
            v.setup()

    def clr(self):
        'setup each visitor'
        for v in self.visitors:
            v.clr()

