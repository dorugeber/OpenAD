import sys

from _Setup import *

from PyUtil.assembler import *
from PyUtil.buf_iter import buf_iter
from PyUtil.debugManager import DebugManager
from PyUtil.flatten import flatten
from PyUtil.symtab import Symtab
from PyUtil.errors import ParseError

import PyFort.fortStmts as fs
from PyFort.fortFile import Ffile
from PyFort.fortParse import parse_stmts,parse_cmnt
import PyFort.module_handler as module_handler

import PyFort.stmt2unit

def instance_pred(*class_list):
    '''define a predicate that checks if argument is an instance of 1 of the
    classes listed in class_list
    '''
    def the_pred(x):
        for c in class_list:
            if isinstance(x,c):
                return True
        return False
    return the_pred

def install_pat(cur):

    def _show(s):
        print 'looking at:',s
        return s

    def declAction(cur):
        def _declAction(self):
            return self.decl2unitAction(cur)
        return _declAction

    def execAction(cur):
        def _execAction(self):
            return self.exec2unitAction(cur)
        return _execAction

    def outOfOrder(cur):
        def _outOfOrder(self):
            raise ParseError(self.lineNumber,str(self),fs.Exec,'obsolete language feature: non-executable statement following an executable statement (recommended practice: move the statement up to the declarations section)')
            return
        return _outOfOrder
    
    _ustart    = lambda s: s.is_ustart() # set in fortStmts, true for block data, module, function, subroutine, program
    _uend      = lambda s: s.is_uend() # set in fortStmts, true for the respective end statements

    startunit  = treat(pred(_ustart))
    endunit    = pred(_uend)
    cblk       = pred(lambda s: s.is_comment()) # set in fortStmts, true for Comments only
    ulist_pre  = pred(lambda s: s.is_contains()) # set in fortStmts, true for ContainsStmt only
    # for the following: is_decl is set in fortStmts and true for all subclasses of Decl except for ContainsStmt and EndStmt when not in an interface
    # the "action" refers to the decl2unitAction of a Statement which is set in stmt2unit; this - however - is executed only when is_decl is true
    adecl      = treat(pred(lambda s: s.is_decl(cur)),declAction(cur))
    aexec      = treat(pred(lambda s: s.is_exec()),execAction(cur)) # is exec is set in fortStmts, true for Exec and all subclasses except for EndStmt and its subclasses
    oooDecl    = treat(pred(lambda s: s.is_decl(cur)),outOfOrder(cur))
    # tuple of shorter names for the following:
    (c,u,d,x,o,n,e) = (cblk,      # comment block
                       startunit, # unit start
                       adecl,     # declarations
                       aexec,     # executable stmts
                       oooDecl,   # out of order declaration, to provide diagnostic
                       ulist_pre, # contains being a predicate for subunits
                       endunit)   # unit end

    fmod = cur.module_handler

    def handle_mod(aUnit):
        '''
        collect all the modules along the way
        '''
        if isinstance(aUnit.uinfo,fs.ModuleStmt):
            fmod.add_module(aUnit.uinfo.name,aUnit)
        return aUnit

    def cmst(p): return treat(seq(zo1(c),p),flatten)

    def _ul(s):
        'eta expansion for recursive patterns'
        return uu(s)

    uh    = treat(seq(zo1(c),u),cur.uhead)         # comments and unit start, invoke uhead -> uinfo -> <stmt>.unit_entry
    udcl  = treat(star(cmst(d)),flatten,cur.udecl) # comments and declarations. invoke udecl
    uexc  = treat(star(cmst(x)),flatten,cur.uexec) # comments and executable statements
    odecl = treat(star(cmst(o)),flatten)           # comments and out of order declarations
    cnth  = treat(cmst(n),cur.ucont)               # comments and CONTAINS, invoke ucont -> makes a new sub unit
    cntl  = treat(star(_ul),cur.ulist)             # recursive descent into sub units
    uctn  = zo1(seq(cnth,cntl))                    # contains block   
    uend  = treat(cmst(e),cur.uend,handle_mod)     # comments and unit end, invoke uend (makes new Unit) and handle_mod

    uu    = treat(seq(uh,udcl,uexc,odecl,uctn,uend),lambda s:cur.fval) # apply cur.fval to the full assembly for a unit
    ucm   = treat(c,cur.ucm,lambda s:cur.fval)     # apply ucm and cur.fval to the comments in c
    return disj(uu,ucm)                            # it is either a unit or some comments

def _symtab_of(v):
    return v and v.symtab or None

class Unit(object):
    '''unit object
    cmnt       unit comment block (may not exist)
    uinfo      unit type (module,subroutine,function,blockdata,program)
    decls      list of declaration statements (including comments)
    execs      list of executable statements (including comments)
    contains   contains list (including comments)
    ulist      unit list (the units "contain"ed within this one)
    end        end list (including comments)
    symtab     hierachical symbol table
    fmod       ???? (fmod = cur.module_handler)
    _in_iface  whether or not we are currently in an interface block
    _in_drvdType the name of the derived type whose definition block we are currently processing
    _in_functionDecl this will be set to the FunctionDeclStmt if a function is being processed
    '''

    def __init__(self,parent=None,fmod=None):
        'create a unit'
        self.cmnt      = None
        self.uinfo     = None
        self.decls     = []
        self.execs     = []
        self.contains  = []
        self.ulist     = []
        self.end       = []
        self.parent    = parent
	if parent:
		self.nestLevel = parent.nestLevel+1
        else:
		self.nestLevel = 0
        self.symtab    = Symtab(_symtab_of(parent))
        self.fmod      = fmod
        self._in_iface = None # this would be set to an InterfaceInfo instance if we are in an interface
        self._in_drvdType = None # this would be set to the name of the derived type being processed
        self._drvdTypeDefaultAccess = None # this would be set for access while the derived type is being processed
        self._in_functionDecl = None # this will be set to the FunctionDeclStmt if a function is being processed
        self._in_subroutineDecl = None # this will be set to the SubroutineDeclStmt if a subroutine is being processed
        DebugManager.debug('new unit created: '+str(self)+', new symtab being created = '+str(self.symtab))

    def name(self):
        return self.uinfo.name

    def printit(self,out=sys.stdout):
        if self.cmnt:
            print >> out,self.cmnt.flow(),
        if self.uinfo:
            print >> out,self.uinfo.flow(),
        for aDeclStmt in self.decls:
            print >> out,aDeclStmt.flow(),
        for anExecStmt in self.execs:
            print >> out,anExecStmt.flow(),
        for aContainsEntry in self.contains:
            print >> out,aContainsEntry.flow(),
        for aSubUnit in self.ulist:
            aSubUnit.printit(out)
        if self.end:
            for anEndListEntry in self.end:
                print >> out,anEndListEntry.flow(),

    # print everything but the comments and executable statements to \p out
    def printDecls(self,out=sys.stdout):
        if self.uinfo:
            print >> out,self.uinfo.flow(),
        for aDeclStmt in self.decls:
            if not aDeclStmt.is_comment() :
                print >> out,aDeclStmt.flow(),
        for aContainsEntry in self.contains:
            print >> out,aContainsEntry.flow(),
        for aSubUnit in self.ulist:
            aSubUnit.printDecls(out)
        if self.end:
            for anEndListEntry in self.end:
                print >> out,anEndListEntry.flow(),

# end class Unit

class _curr(object):
    '''helper object:
    embodies the notion of "current" object being built
    incrementally via the subpatterns in a seq pattern
    '''
    def __init__(self,module_handler=None):
        self.val            = Unit(None,module_handler)
        self.fval           = None  
        self.module_handler = module_handler

    def uhead(self,s):
        self.uinfo(s[1])
        if s[0]:
            self.ucomm(s[0][0])
        return self.val

    def ucomm(self,s):
        self.val.cmnt = s
        return self.val

    def uinfo(self,s):
        self.val.uinfo = s
        s.unit_entry(self)
        return self.val

    def udecl(self,s):
        self.val.decls = s
        return self.val

    def uexec(self,s):
        self.val.execs = s
        return self.val

    def uend(self,s):
        u = self.val
        p = u.parent
        s[-1].unit_exit(self)
        u.end = s
        self.fval = u
        self.val  = Unit(p,self.module_handler)
        return u

    def ucont(self,s):
        u = self.val
        u.contains = s
        self.val = Unit(u,self.module_handler)
        return u

    def ulist(self,l):
        p = self.val.parent
        self.val = p
        self.val.ulist = l
        return self.val

    def ucm(self,cmnt):
        u         = self.val
        p         = u.parent
        u.cmnt    = cmnt
        self.fval = u
        self.val  = None # no need for Unit(p), as this must be last
        return u

def fortUnitIterator(fileName,inputFormat=None):
    '''
    returns an iterator over units found in the given file named fileName;
    the units are assembled based on install_pat
    and buf_iter provides the source iterator from which the units are assembled
    '''
    return vgen(install_pat(_curr(module_handler.ourModuleHandler)),
                buf_iter(Ffile.file(fileName,parse_cmnt,parse_stmts,\
                                    inputFormat).lines))

if __name__ == '__main__':
    from _Setup.testit import *

    fn0 = 'units1.f90'
    i0  = fortUnitIterator(fname_t(fn0),True)
    fn1 = 'units2.f90'
    i1  = fortUnitIterator(fname_t(fn1),True)
    fn2 = 'units3.f90'
    i2  = fortUnitIterator(fname_t(fn2),True)
    fn3 = 'units4.f90'
    i3  = fortUnitIterator(fname_t(fn3),True)
    fn4 = 'units5.f90'
    i4  = fortUnitIterator(fname_t(fn4),True)
    fn5 = 'units6.f90'
    i5  = fortUnitIterator(fname_t(fn5),True)

    fn  = 'if.f90'
    ii  = fortUnitIterator(fname_t(fn),True)
