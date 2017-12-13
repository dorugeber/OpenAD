'''
Primitive program representation:
   program = linked list of stmts
   added wrinkle: you can modify (or delete) the statements in question
   or generate new statements to be added
   
'''
from _Setup import *
import PyUtil.linklist as L
import sys

class Prog1(object):

    def __init__(self,builder,stmt_iter):
        '''Takes a builder object, and a stmt_iter,
        installs the builder components (functions) as
        build functions for the stmts.
        then for each stmt, the visitor is called with the
        prog1 object as the arg to the builder/visitor
        '''
        builder.setup()
        self.stmts = L.LList()
        self.prev  = self.stmts
        for n in stmt_iter:
            n.build(self)
        builder.clr()

    def stmt_iter(self):
        return iter(self.stmts)
    
    def attach(self,*vals):
        '''attach vals to prog1 object
        self.prev = current attachment point
        '''
        p = self.prev
        for v in vals:
            ln     = L.Node(v,L.Nil)
            p.link = ln
            p      = ln
        self.prev = p

    def printit(self,out=sys.stdout):
        'print stmts to a file obj out'

        for s in self.stmt_iter():
            s = s.val
            if not hasattr(s,'rawline'): s.flow()
            print >> out,s.rawline,

    def writeit(self,fname):
        try:
          ff = open(fname,'w')
        except IOError:
          msg="Error cannot open file named: "+name
          raise UserError(msg)
        self.printit(ff)
        ff.close()

        
