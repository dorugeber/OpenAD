#### FIXME: functions returning derived types !!! ###
#### function parsing in general: distinguish 'real function' (a var decl) from 'real function foo()'

'''
Various Fortran statement types and Fortran parsing functions
'''

from _Setup import *

from PyIR.mapper       import _Mappable
from PyIR.mutable_tree import _Mutable_T

from PyUtil.chomp        import chomp
from PyUtil.errors  import ParseError, LogicError
from PyUtil.l_assembler import AssemblerException as ListAssemblerException
from PyUtil.debugManager import DebugManager

from PyFort.fortExp      import *
from PyFort.fixedfmt     import fixedfmt
import PyFort.flow as flow
import itertools
import copy

class __FakeUnit(object):
    def __init__(self):
        self._in_iface = False

_non = __FakeUnit()

class _TypeMod(_Mutable_T):
    'modifier for type declaration'
    _sons = ['mod']

    def __init__(self,e):
        self.mod = e

    def __repr__(self):
        return '%s(%s)' % (self.__class__.__name__,repr(self.mod),)

    def __str__(self):
        return self.pat % str(self.mod)

    def get_sons(self):
        return self._sons

class _Star(_Mutable_T):
    'Utility modifier type for character data'
    _sons = []
    def __init__(self):
        pass
    def __repr__(self):
        return '_Star()'
    def __str__(self):
        return '(*)'

class _FLenMod(_Mutable_T):
    'generic fortran char len type'
    _sons = ['len']
    def __init__(self,len):
        self.len = len
    def __repr__(self):
        return '%s(%s)' % (self.__class__.__name__,repr(self.len))
    def __str__(self):
        return self.pat_ % str(self.len)
    
    def get_sons(self):
        return self._sons

class _F90ExplLen(_FLenMod):
    'utility modifier for explicit len in F90 char data'
    pat_ = '(len=%s)'

class _F90Len(_FLenMod):
    'Utility modifier for character data, F90 style'
    pat_ = '(%s)'

    def _separate_implicit_list(self):
        app = self.len
        return ([],app)

class _F77Len(_FLenMod):
    'Utility modifier for character data, F77 style'
    pat_ = '*%s'
    def _separate_implicit_list(self):
        app = self.len
        return ([_F77Len(app.head)],app.args)

class _DimensionSpecifier(object):
    '''generic dimension array specifier for dimension attributes or dimension statements'''

    form = seq(lit('('),
               cslist(disj(lit('*'),
                           Exp)),
               lit(')'))
    form = treat(form,lambda x: x[1])

class _DimensionAttribute(object):
    '''dimension attribute for type declarations'''
    form = seq(lit('dimension'),
               _DimensionSpecifier.form)
    form = treat(form,lambda x: App(x[0],x[1]))

class _DimensionArraySpec(_Mutable_T):
    'dimension array name and array specifier for dimension statements'
    _sons=['arrayName','arraySpec']

    form = seq(id,
               _DimensionSpecifier.form)
    form = treat(form,lambda x: _DimensionArraySpec(x[0],x[1]))

    def __init__(self, arrayName, arraySpec,lineNumber=0):
        self.arrayName = arrayName
        self.arraySpec = arraySpec
        self.lineNumber = lineNumber

    def __repr__(self):
        return '%s(%s,%s)' % (self.__class__.__name__,repr(self.arrayName),repr(self.arraySpec))

    def __str__(self):
        return '%s(%s)' % (self.arrayName,','.join(str(l) for l in self.arraySpec))

    def get_sons(self):
        return self._sons

class _KindTypeMod(_TypeMod):
    def __deepcopy__(self,memo={}):
        if isinstance(self.mod,str):
            return self.__class__(self.mod)
        else:
            return self.__class__(copy.deepcopy(self.mod))

class _Prec(_KindTypeMod):
    pat = '*%s'

    def _separate_implicit_list(self):
        app = self.mod
        return ([_Prec(app.head)],app.args)

class _Kind(_KindTypeMod):
    pat = '(%s)'

    def _separate_implicit_list(self):
        l = self.mod
        return ([],self.mod)

class _ExplKind(_KindTypeMod):
    pat = '(kind = %s)'

prec = seq(lit('*'),pred(is_int))
prec = treat(prec,lambda a:_Prec(a[1]))

# kind = seq(lit('('),cslist(Exp),lit(')')) #### KILL THIS??
kind = seq(lit('('),Exp,lit(')'))
kind = treat(kind,lambda a:_Kind(a[1]))

explKind = seq(lit('('),lit('kind'),lit('='),Exp,lit(')'))
explKind = treat(explKind,lambda a:_ExplKind(a[3]))

starmod  = seq(lit('('),lit('*'),lit(')'))
starmod  = treat(starmod,lambda l: _Star())

lenmod   = disj(Exp,starmod)
f77mod   = seq(lit('*'),lenmod)
f77mod   = treat(f77mod,lambda l: _F77Len(l[1]))

f90mod   = seq(lit('('),disj(lit('*'),Exp),lit(')'))
f90mod   = treat(f90mod,lambda l: _F90Len(l[1]))

explLen  = seq(lit('('),lit('len'),lit('='),Exp,lit(')'))

id_l     = treat(id,str.lower)
_typeid  = disj(lit('real'),
                lit('integer'),
                lit('logical'),
                lit('character'),
                lit('complex'),
                lit('doubleprecision'),
                lit('doublecomplex'),
                )

pstd = seq(_typeid,
         zo1(disj(prec,explKind,kind)),
         )

pchar = seq(lit('character'),
         zo1(disj(f77mod,f90mod,explLen)),
         )

type_pat = disj(pchar,pstd)

def _get_class(p):
    return (kwtbl[p[0].lower()],p[1])

type_pat_sem = treat(type_pat,_get_class)

def _name2class(name): return kwtbl[name.lower()]

def _ta_listify(asm):
    rv = []
    for (dc,item) in asm:
        rv.append(item)
    return rv

typeAttributeExpression = disj(_DimensionAttribute.form,
                               Exp)

type_attr_list = star(seq(lit(','),
                          typeAttributeExpression))
type_attr_list = treat(type_attr_list,_ta_listify)

class _Init(object):
    'general f90 init object'

class _NoInit(_Init):
    'no initialization'
    def __init__(self,lhs): self.lhs = lhs
    def __repr__(self): return '_NoInit(%s)' % repr(self.lhs)
    def __str__(self): return str(self.lhs)
    def __deepcopy__(self,memo={}):
        return _NoInit(self.lhs)
class _PointerInit(_Init):
    'pointer initialization'
    _sons = ['lhs','rhs']

    def __init__(self,lhs,rhs):
        self.lhs = lhs
        self.rhs = rhs
    def __repr__(self):
        return '_PointerInit(%s,%s)' % (repr(self.lhs),
                                        repr(self.rhs))
    def __str__(self):
        return '%s => %s' % (str(self.lhs),
                             str(self.rhs))
    def get_sons(self):
        return self._sons

    def set_son(self,theSon,newSon):
        oldSon = getattr(self,theSon)
        if newSon is not oldSon:
            setattr(self,theSon,newSon)
            self.modified = True

    def __deepcopy__(self,memo={}):
        return _PointerInit(self.lhs,self.rhs)
class _AssignInit(_Init):
    'normal assignment-style initialization'
    _sons = ['lhs','rhs']

    def __init__(self,lhs,rhs):
        self.lhs = lhs
        self.rhs = rhs
        if isinstance(rhs,ArrayConstructor):
            TypeDecl.assignInitArrConst = True
    def __repr__(self):
        return '_AssignInit(%s,%s)' % (repr(self.lhs),
                                       repr(self.rhs))
    def __str__(self):
        return '%s = %s' % (str(self.lhs),
                             str(self.rhs))

    def get_sons(self):
        return self._sons

    def set_son(self,theSon,newSon):
        oldSon = getattr(self,theSon)
        if newSon is not oldSon:
            setattr(self,theSon,newSon)
            self.modified = True

    def __deepcopy__(self,memo={}):
        return _AssignInit(self.lhs,self.rhs)

def _handle_init(asm):

    (lhs,init) = asm
    if init:
        init = init[0]
        if init[0] == '=>':
            return _PointerInit(lhs,init[1])
        else:
            return _AssignInit(lhs,init[1])
    else:
        return _NoInit(lhs)

init_spec = zo1(seq(disj(lit('=>'),lit('=')),Exp))

star_range = seq(Exp,lit(':'),lit('*'))
star_range = treat(star_range,lambda x: Ops(':',x[0],'*'))

upper_bound_range = seq(lit(':'),Exp)
upper_bound_range = treat(upper_bound_range, lambda x: Ops(':','',x[1]))

lower_bound_range = seq(Exp,lit(':'))
lower_bound_range = treat(lower_bound_range, lambda x: Ops(':',x[0],''))

var_w_dim = seq(id,zo1(seq(lit('('),cslist(disj(star_range,lit('*'),lit(':'),lower_bound_range,Exp)),lit(')'))))
var_w_dim = treat(var_w_dim,lambda x: x[1] and App(x[0],x[1][0][1]) or x[0])

decl_item = seq(var_w_dim,init_spec)
decl_item = treat(decl_item,_handle_init)

char_override  = seq(lit('*'),Exp)

char_decl_     = seq(var_w_dim,zo1(char_override))
char_decl_     = treat(char_decl_,lambda x: x[1] and Ops('*',x[0],x[1][0][1]) or x[0])

char_decl_item = seq(char_decl_,init_spec)
char_decl_item = treat(char_decl_item,_handle_init)

def typestr(raw):
    (tyid,mod) = raw
    return kwtbl[tyid].kw_str + (mod and str(mod[0]) or '')

def typestr2(raw):
    (tycls,mod) = raw
    return tycls.kw_str + (mod and str(mod[0]) or '')

class GenStmt(_Mappable,_Mutable_T):
    _sons = []

    def __init__(self,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        """initializes a generic statement object
          formatting (leading whitespace or line breaks)
        lineNumber: contains the line number the statement was
          originally on
        label: contains a statement's numeric label, if there was one
        lead:contains the line lead, excluding the 6 leading spaces
          for fixed format (if input was in fixed format)
        internal: a list of internal comments
        accessed: determines whether or not the statement has been accessed and
          potentially modified, since the rawline is only updated
          when a statement is accessed
        """
        self.rawline = ''
        self.lineNumber = lineNumber
        self.label = label
        self.lead = lead
        self.internal = internal
        self.rest = rest
        self.modified = False

    @classmethod
    def parse(cls,ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        return cls(scan,lineNumber)

    def is_exec(self,unit=_non): return False
    def is_decl(self,unit=_non): return False
    def is_ustart(self,unit=_non): return False
    def is_uend(self,unit=_non): return False
    def is_contains(self,unit=_non): return False
    def is_comment(self,unit=_non): return False

    def __str__(self):
        return self.rawline

    def set_rawline(self,newRawline):
        self.rawline = newRawline

    def get_rawline(self):
        '''updates the rawline and returns it'''
        return self.rawline.strip()

    def get_sons(self):
        return self._sons

    def __deepcopy__(self,memo={}):
        '''cheaper deepcopy implementation for copying statements'''
        newSons=map(lambda l: copyExp(getattr(self,l)),self.get_sons())
        newStmt=self.__class__(*newSons)
        newStmt.lead=self.lead
        newStmt.rawline=self.rawline
        if (DebugManager.check()):
            if (repr(self)!=repr(newStmt)):
                raise LogicError(sys._getframe().f_code.co_name+" does not replicate identically; original: "+repr(self)+" vs copied: "+repr(newStmt))
            if (str(self).lower()!=str(newStmt).lower()):
                raise LogicError(sys._getframe().f_code.co_name+" does not unparse (caser insensitive) identically; original: "+str(self)+" vs copied: "+str(newStmt))
        return newStmt

class Skip(GenStmt):
    def __init__(self):
        self.scan = []

class Comments(GenStmt):
    def __init__(self,rawline,lineNumber=0):
        self.rawline = rawline
        self.lineNumber = lineNumber

    def __repr__(self):
        return 'Comments(blk)'

    def viz(self):
        return 'Comments(%s)' % self.rawline

    def flow(self):
        """formats a comment for printing, depending on the format
        (free or fixed) of the output, while preserving all leading
        or trailing blank lines"""
        if self.rawline.strip() == '':
            return self.rawline
        formattedOutput = ''
        lead_len = len(self.rawline) - len(self.rawline.lstrip())
        if lead_len > 0:
            num_lines = self.rawline[:lead_len].count('\n')
            formattedOutput += '\n'*num_lines
        lines = self.rawline.strip().splitlines()
        if (flow.outputFormat == 'free'):
            for line in lines:
                formattedOutput += '!'+flow.flow_comment(line[1:])
        else:
            for line in lines:
                formattedOutput += 'C'+flow.flow_comment(line[1:])

        end_len = len(self.rawline)-len(self.rawline.rstrip())
        if end_len > 0:
            num_lines = self.rawline[len(self.rawline)-end_len+1:].count('\n')
            formattedOutput += '\n'*num_lines
        return formattedOutput

    def is_comment(self,unit=_non): return True

    def __deepcopy__(self,memo={}):
        return Comments(self.rawline,self.lineNumber)

def comment_bl(*comlines):
    return Comments('\n'.join(['c '+ chomp(s) for s in comlines])+'\n')

class NonComment(GenStmt):

    def __init__(self,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        GenStmt.__init__(self,lineNumber,label,lead)
        self.internal = internal
        self.rest = rest
        self.rawline = str(self)
        self.modified = False
            
    def __repr__(self):
        return '%s(%s)' % (self.__class__.__name__,
                           ','.join([repr(self.__dict__[aSon]) for aSon in self._sons]))

    def __str__(self):
        return self.__class__.kw_str

    def flow(self):
        """formats a statement for printing by concatenating the label
        string, the lead, the rawline (updated if necessary) and six
        leading spaces (if fixed format), and adding the necessary line
        breaks (depending on fixed or free format output)"""
        labelStr = self.label and ' ' + ('%-4d' % self.label) + ' ' \
                               or ''
        if (flow.outputFormat=='fixed') and len(labelStr) != 6:
            labelStr = labelStr + (6 - len(labelStr)) * ' '
            formattedOutput = flow.flow_line(labelStr + self.lead + self.get_rawline()) + '\n'
        else:
            formattedOutput = flow.flow_line(labelStr + self.lead + self.get_rawline()) + '\n'
        for aComment in self.internal:
            newComment=Comments(aComment).flow()
            formattedOutput+=newComment
        return formattedOutput

    def same_level(self,parsed):
        parsed.label = False
        parsed.lead    = self.lead
        return parsed.flow()

    def indent(self,n):
        self.lead += ' ' * n
        return self.flow()

    def basic_line(self):
        self.label = False
        self.lead   = ''
        self.flow()
        return self

    def same(self,targ):
        self.same_level(targ)
        targ.lineNumber = self.lineNumber
        targ.label = self.label
        targ.lead = self.lead
        return targ.flow()

    def clone_fmt(self,src):
        self.label = False
        self.lead   = src.lead
        self.flow()
        return self

    def get_sons(self):
        '''returns the list of sons for this statement, and also marks the statement as accessed.'''
        return self._sons

    def set_son(self,theSon,newSon):
        oldSon = getattr(self,theSon)
        if newSon is not oldSon:
            setattr(self,theSon,newSon)
            self.modified = True

    def get_rawline(self):
        '''returns the rawline for this statement.
           If the statement has been changed (indicated by the flag self.modified), then the rawline is updated before being returned.'''
        if self.modified:
            old_rawline = self.rawline
            self.rawline = str(self)
            for string in self.rest:
                if string.lower() not in self.rawline.lower():
                    raise ParseError(self.lineNumber,self.rawline,"The parser dropped the substring "+string+" from the rawline. The original statement was '"+old_rawline+"'")
        self.modified = False
        return self.rawline.strip()

class Decl(NonComment):
    def __init__(self,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        NonComment.__init__(self,lineNumber,label,lead,internal,rest)

    def is_decl(self,unit=_non): return True

def attrstr(l):
    if not l: return ''
    return ','+ ','.join([str(ll) for ll in l])

def ditemstr(l):
    return ','.join([str(ll) for ll in l])

def itemstr(l):
    return ''.join([str(ll) for ll in l])

class TypeDecl(Decl):
    '''
    The double colon "::" is optional when parsing, but we have chosen to always include it when printing
    '''
    ## This is the first appearance of the kw and kw_str members; the kw is a string with no spaces (like 'doubleprecision').
    kw = '__unknown__'
    ## the kw_str may have spaces (like with 'double precision').
    kw_str = ''
    mod = None
    decls = []
    assignInitArrConst = False
    _sons = ['mod','attrs','decls']
    spec=type_pat
    
    @classmethod
    def parse(cls,ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        p0 = seq(TypeDecl.spec,
                 type_attr_list,
                 zo1(lit('::')),
                 cslist(decl_item))
        (v,r) = p0(scan)
        ((typ,mod),attrs,dc,decls) = v
        return cls(mod,attrs,decls,lineNumber,rest=r)

    def __init__(self,mod,attrs,decls,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.mod   = mod
        self.attrs = attrs
        self.decls = decls
        Decl.__init__(self,lineNumber,label,lead,internal,rest)
        if TypeDecl.assignInitArrConst:
            self.modified = True
            TypeDecl.assignInitArrConst=False
        self.parameter=False
        self.dimension=None
        self.pointer=False
        self.allocatable=False
        if self.attrs is not None:
            for anAttr in self.attrs:
                if isinstance(anAttr,App):
                    if anAttr.head.lower()=='dimension':
                        self.dimension=anAttr.args
                elif anAttr.lower()=='parameter':
                    self.parameter=True
                elif anAttr.lower()=='pointer':
                    self.pointer=True
                elif anAttr.lower()=='allocatable':
                    self.allocatable=True

    def __repr__(self):
        return '%s(%s,%s,%s)' % (self.__class__.__name__,
                                 repr(self.mod),
                                 repr(self.attrs),
                                 repr(self.decls))

    def __str__(self):
        modstr = ''
        if self.mod:
            modstr = str(self.mod[0])
        attr_str = ''
        if self.attrs:
            attr_str = ','+','.join([str(a) for a in self.attrs])
        return '%s%s%s :: %s' % (self.__class__.kw_str,
                                 modstr,
                                 attr_str,
                                 ','.join([str(d) for d in self.decls]))

    def add_dimension(self,dimensions):
        self.dimension=dimensions
        self.attrs.append(fe.App('dimension',dimensions))

    def addPointerAttr(self):
        self.pointer=True
        self.attrs.append('pointer')

    def get_mod(self):
        return self.mod

    def set_mod(self,newMod):
        if self.mod != newMod:
            self.mod = newMod
            self.modified = True

    def get_attrs(self):
        return self.attrs

    def set_attrs(self,newAttrs):
        self.attrs = newAttrs
        self.modified = True
    
    def get_decls(self):
        return self.decls

    def set_decls(self,newDecls):
        self.decls = newDecls
        self.modified = True

class DrvdTypeDecl(TypeDecl):
    '''
    Derived type declarations are treated as declarations of type "type,"
     with a modifier that is the name of the type.
    '''
    kw     = 'type'
    kw_str = kw

    spec=seq(lit(kw),
             lit('('),
             id,
             lit(')'))
    
    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        p0 = seq(DrvdTypeDecl.spec,
                 type_attr_list,
                 zo1(lit('::')),
                 cslist(decl_item))
        p0 = treat(p0,lambda l: DrvdTypeDecl([l[0][2]],l[1],l[3],lineNumber=lineNumber))
        (v,r) = p0(scan)
        v.rest=r
        return v

    def __str__(self):
        attr_str = ''
        if self.attrs:
            attr_str = ','+','.join([str(a) for a in self.attrs])
        return '%s(%s)%s :: %s' % (self.__class__.kw_str,
                                   str(self.mod[0]),
                                   attr_str,
                                   ','.join([str(d) for d in self.decls]))

class ClassStmt(DrvdTypeDecl):
    '''
    used to declare polymorphic entities of specified type "type," or an unlimited polymorphic entity
    modifier is the name of the type.
    '''
    kw     = 'class'
    kw_str = kw

    spec=seq(lit(kw),
             lit('('),
             id,
             lit(')'))
    
    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        if scan[2]=='*':
            return UPClassStmt.parse(ws_scan,lineNumber)
        p0 = seq(ClassStmt.spec,
                 type_attr_list,
                 zo1(lit('::')),
                 cslist(decl_item))
        p0 = treat(p0,lambda l: ClassStmt([l[0][2]],l[1],l[3],lineNumber=lineNumber))
        (v,r) = p0(scan)
        v.rest=r
        return v

class UPClassStmt(ClassStmt):
    '''
    unlimited polymorphic entity
    used to declare polymorphic entities of specified type "type," or an unlimited polymorphic entity
    modifier is the name of the type.
    '''
    kw     = 'class'
    kw_str = kw

    spec=seq(lit(kw),
             lit('('),
             lit('*'),
             lit(')'))
    
    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        p0 = seq(UPClassStmt.spec,
                 type_attr_list,
                 zo1(lit('::')),
                 cslist(decl_item))
        p0 = treat(p0,lambda l: UPClassStmt([l[0][2]],l[1],l[3],lineNumber=lineNumber))
        (v,r) = p0(scan)
        v.rest=r
        return v

class DrvdTypeDefn(Decl):
    '''
    derived type definition (start)
    '''
    kw_str = 'derivedDefn'
    kw     = kw_str
    _sons  = ['name']

    def __init__(self,name,base_type=None,access_spec=None,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.name = name
        self.base_type=base_type
        self.access_spec=access_spec
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return 'DrvdTypeDefn(%s)' % repr(self.name)

    def __str__(self):
        return 'type %s' % str(self.name)
    
    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        access_spec=disj(lit('public'),lit('private'))
        p0    = treat(seq(lit('type'),
                          zo1(seq(lit(','),lit('extends'),lit('('),id,lit(')'))),
                          zo1(seq(lit(','),access_spec)),
                          zo1(lit('::')),id),
                      lambda l: DrvdTypeDefn(l[4],l[1] and l[1][0][3] or None,l[2] and l[2][0][1] or None,lineNumber=lineNumber))
        (v,r) = p0(scan)
        v.rest = r
        return v

class InterfaceStmt(Decl):
    kw = 'interface'
    kw_str = kw
    _sons =['name']

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        formInterfaceStmt = seq(lit('interface'),
                                zo1(id))
        ((interfaceKeyword,interfaceName),rest) = formInterfaceStmt(scan)
        name = interfaceName and interfaceName[0] \
                              or None
        return InterfaceStmt(name,lineNumber,rest)

    def __init__(self,l,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.name = l
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return 'InterfaceStmt(%s)' % self.name

    def __str__(self,whitespace=False):
        if self.name:
            return 'interface %s' % self.name
        else:
            return 'interface'

    def get_name(self):
        return self.name

    def set_name(self,newName):
        if self.name != newName:
            self.name = newName
            self.modified = True

class ProcedureStmt(Decl):
    kw = 'procedure'
    kw_str = kw
    _sons = ['hasModuleKeyword','procedureList']
    optKwPrefix='module'
    
    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        formprocedureStmt = seq(zo1(lit(ProcedureStmt.optKwPrefix)),    #0 - module keyword (optional)
                                lit(ProcedureStmt.kw), #1
                                cslist(id))            #2 - procedureList
        formprocedureStmt = treat(formprocedureStmt, lambda x : ProcedureStmt(x[0] and True or False, x[2],lineNumber))
        (theParsedStmt,rest) = formprocedureStmt(scan)
        theParsedStmt.rest = rest
        return theParsedStmt

    def __init__(self,hasModuleKeyword,procedureList,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.hasModuleKeyword = hasModuleKeyword
        self.procedureList = procedureList
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return '%s(%s,%s)' % (self.__class__.__name__,
                              repr(self.hasModuleKeyword),
                              repr(self.procedureList))

    def __str__(self):
        moduleKeywordStr = self.hasModuleKeyword and ProcedureStmt.optKwPrefix+' ' \
                                                  or ''
        return '%s%s %s' % (moduleKeywordStr,
                            self.__class__.kw_str,
                            ','.join([str(aProcedureItem) for aProcedureItem in self.procedureList]))


class TypePseudoStmt(GenStmt):
    '''
    type keyword signals *either* declaration or definition
    '''
    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        if scan[1] == '(': return DrvdTypeDecl.parse(scan,lineNumber)
        return DrvdTypeDefn.parse(scan,lineNumber)

class PUstart(Decl):
    def is_decl(self,unit=_non):  return True
    def is_ustart(self,unit=_non): return True

class PointerStmt(Decl):
    kw = 'pointer'
    kw_str = kw

    @staticmethod
    def parse(ws_scan,lineNumber) :
        scan = filter(lambda x: x != ' ',ws_scan)
        form = seq(lit(PointerStmt.kw)) # 0 = stmt_name
        (id,rest) = form(scan)
        return PointerStmt(lineNumber=lineNumber,rest=rest)

    def __init__(self,stmt_name=kw_str,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

class TargetStmt(Decl):
    kw = 'target'
    kw_str = kw

    @staticmethod
    def parse(ws_scan,lineNumber) :
        scan = filter(lambda x: x != ' ',ws_scan)
        form = seq(lit(TargetStmt.kw)) # 0 = stmt_name
        (id,rest) = form(scan)
        return TargetStmt(lineNumber=lineNumber,rest=rest)

    def __init__(self,stmt_name=kw_str,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

class Exec(NonComment):
    ''' base class for all executable statements'''
    def __init__(self,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        NonComment.__init__(self,lineNumber,label,lead,internal,rest)
        
    def is_exec(self,unit=_non): return True

    def __repr__(self):
        return '%s(%s)' % (self.__class__.__name__,
                           ','.join([repr(getattr(self,aSon)) for aSon in self._sons]))

class Leaf(Exec):
    "special Exec that doesn't have components"

    @classmethod
    def parse(cls,ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        return cls(lineNumber)

    def __init__(self,lineNumber=0,label=False,lead='',internal=[],rest=[],*dc):
        Exec.__init__(self,lineNumber,label,lead,internal,rest)
            
    def __repr__(self):
        return '%s()' % self.__class__.__name__

class DeclLeaf(Decl):
    "special Decl that has no components"

    @classmethod
    def parse(cls,ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        return cls(lineNumber)

    def __init__(self,lineNumber=0,label=False,lead='',internal=[],rest=[],*dc,**dc2):
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self): return '%s()' % self.__class__.__name__

class PUend(Leaf):

    def __init__(self,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        Leaf.__init__(self,lineNumber,label,lead,internal,rest)

class IfPUstart(DeclLeaf):
    kw_str = '(If)prog_unit_start'

class IfPUend(DeclLeaf):
    kw_str = '(If)prog_unit_end'

class SequenceStmt(DeclLeaf):
    kw_str = 'sequence'
    kw=kw_str
    
class BlockdataStmt(PUstart):
    kw = 'blockdata'
    kw_str = 'block data'
    _sons = ['name']

    @staticmethod
    def parse(ws_scan,lineNumber) :
        scan = filter(lambda x: x != ' ',ws_scan)
        form = seq(lit(BlockdataStmt.kw_str), # 0 = stmt_name
                   id)                        # 1 = blockdataname
        ((stmt_name,blockdataname),rest) = form(scan)
        return BlockdataStmt(blockdataname,lineNumber=lineNumber,rest=rest)

    def __init__(self,blockdataname,stmt_name=kw_str,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.name = blockdataname
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

class CommonStmt(Decl):
    kw = 'common'
    kw_str = kw
    _sons = ['name','declList']

    @classmethod
    def parse(cls,ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        theStmt=None
        rm=None
        try : # try the blank with no space between the slashes
            stmtPatn = seq(lit(CommonStmt.kw),lit('//'),cslist(Exp))
            stmtMaker = treat(stmtPatn,lambda l: cls('',l[2]))
            (theStmt,rm)=stmtMaker(scan)
        except: # named or blank with spaces between the slashes
            stmtPatn = seq(lit(CommonStmt.kw),lit('/'),zo1(id),lit('/'),cslist(Exp))
            stmtMaker = treat(stmtPatn,lambda l: cls((len(l[2]) and l[2][0] or ''),l[4]))
            (theStmt,rm)=stmtMaker(scan)
	theStmt.lineNumber=lineNumber
        theStmt.rest=rm
        return theStmt

    def __init__(self,name,declList=[],lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.name = name
        self.declList = declList
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return self.__class__.__name__+'('+self.name+','+repr(self.declList)+')'

    def __str__(self):
        return self.kw + '/%s/ %s' % \
              (self.name,','.join(str(item) for item in self.declList))

class _ImplicitDoConstruct(object):
    '''implicit do construct for DATA statements'''
    _sons = ['object','loopControl']
    # data-implied-do object is one of
    #  array-element
    #  scalar-structure-component
    #  data-implied-do
    form = seq(lit('('),         # 0
               app,              # 1 = app
               lit(','),         # 2
               LoopControl.form, # 3 = loopControl
               lit(')'))         # 4
    form = treat(form, lambda x: _ImplicitDoConstruct(x[1],x[3]))

    #  form of data-implied-do:
    # ( data-implied-do-object-list , named-scalar-integer-variable = scalar-integer-expression , scalar-integer-expression [ , scalar-integer-expression ] )
    form = seq(lit('('),         # 0
               disj(app,         # 1 = object
                    form),
               lit(','),         # 2
               LoopControl.form, # 3 = loopControl
               lit(')'))         # 4
    form = treat(form, lambda x: _ImplicitDoConstruct(x[1],x[3]))

    def __init__(self,object,loopControl):
        self.object = object
        self.loopControl = loopControl

    def __str__(self):
        return '(%s, %s)' % (str(self.object),str(self.loopControl))

    def __repr__(self):
        return '%s(%s,%s)' % (self.__class__.__name__,repr(self.object),repr(self.loopControl))


class DataStmt(Decl):
    kw = 'data'
    kw_str = kw
    _sons = ['objectValuePairList']

    @staticmethod
    def parse(ws_scan,lineNumber):
        ''' 
        \todo we do not cover the full range of the DATA stmt syntax.
        among the exceptions are: 
         - from the dataObject pattern we subobject patterns combining App with Sel 
        '''
        scan = filter(lambda x: x != ' ',ws_scan)
        dataObjectListPatn=cslist(disj(_ImplicitDoConstruct.form,app1,id))
        dataObjectValuePairListPatn=cslist(seq(dataObjectListPatn,lit('/'),cslist(Exp),lit('/')))
        dataObjectValuePairList=treat(dataObjectValuePairListPatn,lambda l:[(le[0],le[2]) for le in l])
        formDataStmt = seq(lit(DataStmt.kw),dataObjectValuePairList)           #
        formDataStmt = treat(formDataStmt, lambda x: DataStmt(x[1],stmt_name=x[0],lineNumber=lineNumber))
        (theParsedStmt,rest) = formDataStmt(scan)
        theParsedStmt.rest=rest
        return theParsedStmt

    def __init__(self,objectValuePairList,stmt_name=kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.objectValuePairList = objectValuePairList
        self.stmt_name = stmt_name
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self):
        def dumpObjectValuePair(thePair):
            rstr=','.join(str(o) for o in thePair[0])
            rstr+=' /'
            rstr+=','.join(str(v) for v in thePair[1])
            rstr+='/'
            return rstr
        return '%s %s' % (self.stmt_name,','.join(map(dumpObjectValuePair,self.objectValuePairList)))

    def __repr__(self):
        return self.__class__.__name__ + \
               '(' + \
               ','.join([repr(aSon) for aSon in self._sons]) + \
               ')'

class EndInterfaceStmt(DeclLeaf):
    'End of interface block'
    kw    = 'endinterface'
    kw_str = 'end interface'
    _sons =['name']

    @staticmethod
    def parse(ws_scan,lineNumber) :
        scan = filter(lambda x: x != ' ',ws_scan)
        form = seq(lit(EndInterfaceStmt.kw), # 0 = stmt_name
                   zo1(id)) # interface name
        ((endInterfaceKeyword,interfaceName),rest) = form(scan)
        name = interfaceName and interfaceName[0] or None
        return EndInterfaceStmt(name,lineNumber=lineNumber,rest=rest)

    def __init__(self,name=None,stmt_name=kw_str,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.name=name
        DeclLeaf.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return 'EndInterfaceStmt(%s)' % self.name

    def __str__(self,whitespace=False):
        if self.name:
            return self.kw_str+' %s' % self.name
        else:
            return self.kw_str

    def get_name(self):
        return self.name

    def set_name(self,newName):
        if self.name != newName:
            self.name = newName
            self.modified = True

class EndDrvdTypeDefn(DeclLeaf):
    'end of a derived type definition'
    kw     = 'endtype'
    kw_str = 'end type'

    @staticmethod
    def parse(ws_scan,lineNumber) :
        scan = filter(lambda x: x != ' ',ws_scan)
        form = seq(lit(EndDrvdTypeDefn.kw)) # 0 = stmt_name
        (id,rest) = form(scan)
        return EndDrvdTypeDefn(lineNumber=lineNumber,rest=rest)

    def __init__(self,stmt_name=kw_str,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        DeclLeaf.__init__(self,lineNumber,label,lead,internal,rest)    

class VarAttrib(Decl):
    _sons  = ['vlist']

    @classmethod
    def parse(cls,ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        p0 = seq(lit(cls.kw),zo1(lit('::')),cslist(id))

        ((dc,sep,vlist),r) = p0(scan)
        if vlist:
            return cls(vlist,lineNumber,rest=r)
        else:
            return cls(r,lineNumber,rest=r)

    def __init__(self,vlist,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.vlist = vlist
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return '%s(%s)' % (self.__class__.__name__, repr(self.vlist))

    def __str__(self):
        s_key = self.kw_str
        rem   = ''
        if self.vlist:
            rem = ' :: %s' % ','.join([str(v) for v in self.vlist])
        return s_key+rem

class PrivateStmt(VarAttrib):
    kw     = 'private'
    kw_str = kw
    
    def __init__(self,vlist,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        VarAttrib.__init__(self,vlist,lineNumber,label,lead,internal,rest)

class PublicStmt(VarAttrib):
    kw     = 'public'
    kw_str = kw

    def __init__(self,vlist,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        VarAttrib.__init__(self,vlist,lineNumber,label,lead,internal,rest)

class ContainsStmt(DeclLeaf):
    kw     = 'contains'
    kw_str = kw

    def is_contains(self,unit=_non): return True
    def is_decl(self,unit=_non): return False
    
class ImplicitNone(DeclLeaf):
    kw     = 'implicitnone'
    kw_str = 'implicit none'

def _extract_imp_elts(type_pair):
    '''hack to extract the list of implicit letters from
    the absorbed type modifier in the case of type*Exp or
    type(Explist)
    '''
    (cls,m) = type_pair
    m = m[0]
    (nmod,implst) = m._separate_implicit_list()
    return ((cls,nmod),implst)

class ImplicitStmt(Decl):
    kw = 'implicit'
    kw_str = kw
    _sons = ['lst']

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        try: 
            implNone = seq(lit(ImplicitStmt.kw),lit('none'))
            implNone = treat(implNone,ImplicitNone)
            (theImplNone,r) = implNone(scan)
            theImplNone.rest=r
            theImplNone.lineNumber=lineNumber
            return theImplNone
        except:
            letter_spec_list=seq(lit('('),cslist(Exp),lit(')'))
            letter_spec_list=treat(letter_spec_list,lambda l: l[1]) # extract the list 
            type_spec_with_mods=disj(treat(DrvdTypeDecl.spec,lambda l:(DrvdTypeDecl,[l[2]])),
                                     treat(CharacterStmt.spec,lambda l:(CharacterStmt,l[1])),
                                     treat(TypeDecl.spec,_get_class))
            # For the other cases the type is the keyword the
            # parser handles the types consisting of two keywords
            # here we have to do it explicitly:
            dblp = treat(seq(lit('double'),lit('precision')),lambda l:'doubleprecision')
            # get the pair of type class and empty mod list
            type_spec_wo_mods=disj(treat(dblp,lambda l:(DoubleStmt,[])),
                                   treat(_typeid,lambda l:_get_class((l,[]))))
            typeLetterListPair=disj(seq(type_spec_with_mods,letter_spec_list),
                                    seq(type_spec_wo_mods,letter_spec_list))
            implSpec=seq(lit(ImplicitStmt.kw),cslist(typeLetterListPair))
            (kw,list),rest=implSpec(scan)
            return ImplicitStmt(list,lineNumber,rest=rest)

    def __init__(self,lst,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        # list of pairs that are (type-spec,letter-spec-list)
        # where type-spec is a pair (<TypeClass>,[<mods>])
        self.lst  = lst 
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return self.__class__.__name__+'(%s)' % repr(self.lst)

    def __str__(self):
        def dumpPair(p):
            rstr=p[0][0].kw_str
            if len(p[0][1]):
                rstr+=str(p[0][1][0])
            return rstr+' ('+','.join([str(l) for l in p[1]])+')'  
        rstr=ImplicitStmt.kw+' '
        rstr+=','.join(map(dumpPair,self.lst))
        return rstr

class EquivalenceStmt(Decl):
    kw = 'equivalence'
    _sons = ['nlists']

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        nlist = seq(lit('('),
                    cslist(Exp),
                    lit(')'))
        nlist = treat(nlist,lambda l:l[1])
        stmt = seq(lit(EquivalenceStmt.kw),cslist(nlist))
        ([equivalence,nlists],rm) = stmt(scan)
        return EquivalenceStmt(nlists,lineNumber,rest=rm)

    
    def __init__(self,nlists,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.nlists = nlists
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self):
        declStrList = map(lambda nlist: '('+','.join(str(item) for item in nlist)+')',self.nlists)
        return '%s %s' % (self.kw,','.join(declStrList))
        
namedParamPatn = seq(id,lit('='),Exp)

class ParameterStmt(Decl):
    _sons = ['namedParamList']
    kw = 'parameter'
    kw_str = kw

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        p0 = seq(lit(ParameterStmt.kw),
                 lit('('),
                 cslist(namedParamPatn),
                 lit(')'))
        p0 = treat(p0,lambda l:ParameterStmt(l[2],lineNumber))
        (v,r) = p0(scan)
        v.rest=r
        return v
   
    def __init__(self,namedParamList,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.namedParamList = namedParamList
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return 'ParamterStmt(%s)' % ','.join([repr(aNamedParam) for aNamedParam in self.namedParamList])

    def __str__(self):
        return 'parameter (%s)' % ','.join([itemstr(aNamedParam) for aNamedParam in self.namedParamList])

class SaveStmt(Decl):
    'Save statement'
    kw    = 'save'
    kw_str = kw

    @staticmethod
    def parse(ws_scan,lineNumber) :
        scan = filter(lambda x: x != ' ',ws_scan)
        form = seq(lit(SaveStmt.kw)) # 0 = stmt_name
        (id,rest) = form(scan)
        return SaveStmt(lineNumber=lineNumber,rest=rest)

    def __init__(self,stmt_name=kw_str,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

class StmtFnStmt(Decl):
    _sons = ['name','args','body']

    def __init__(self,name,args,body,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.name = name
        self.args = args
        self.body = body
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return 'StmtFnStmt(%s,%s,%s)' % (repr(self.name),
                                         repr(self.args),
                                         repr(self.body))
    def __str__(self):
        return '%s(%s) = %s' % (str(self.name),
                                ','.join([str(l) for l in self.args]),
                                str(self.body))

    def get_name(self):
        return self.name

    def set_name(self,newName):
        if self.name != newName:
            self.name = newName
            self.modified = True

    def get_args(self):
        return self.args

    def set_args(self,newArgs):
        self.args = newArgs
        self.modified = True

    def get_body(self):
        return self.body

    def set_body(self,newBody):
        if self.body != newBody:
            self.body = newBody
            self.modified = True

class ExternalStmt(Decl):
    _sons = ['procedureNames']
    kw = 'external'
    kw_str = kw

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        formExternalStmt = seq(lit(ExternalStmt.kw),
                               zo1(lit('::')),
                               cslist(id))
        ((externalKeyword,doubleColon,procedureNames),rest) = formExternalStmt(scan)
        return ExternalStmt(procedureNames,lineNumber,rest=rest)

    def __init__(self,procedureNames,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.procedureNames = procedureNames
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return self.__class__.__name__+'('+repr(self.procedureNames)+')'

    def __str__(self):
        return self.kw+' '+','.join([str(aProcedureName)
                                     for aProcedureName in
                                     self.procedureNames])

class AllocatableStmt(Decl):
    _sons = ['lst']
    kw = 'allocatable'
    kw_str = kw

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        formAllocatableStmt = seq(lit(AllocatableStmt.kw),
                               zo1(lit('::')),
                               cslist(id))
        ((allocatableKeyword,doubleColon,lst),rest) = formAllocatableStmt(scan)
        return AllocatableStmt(lst,lineNumber,rest=rest)

    def __init__(self,lst,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.lst = lst
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return self.__class__.__name__+'('+repr(self.lst)+')'

    def __str__(self):
        return self.kw+' '+','.join([str(aName) for aName in self.lst])

class CharacterStmt(TypeDecl):
    kw = 'character'
    kw_str = kw

    # build up the spec: 
    _starmod  = seq(lit('('),lit('*'),lit(')'))
    _starmod  = treat(_starmod,lambda l: _Star())
    
    _lenmod   = disj(seq(lit('('),Exp,lit(')')),pred(is_int),_starmod)
    _f77mod   = seq(lit('*'),_lenmod)
    _f77mod   = treat(_f77mod,lambda l: _F77Len(l[1]))
    
    _f90mod   = seq(lit('('),disj(lit('*'),Exp),lit(')'))
    _f90mod   = treat(_f90mod,lambda l: _F90Len(l[1]))
    
    _explLen  = seq(lit('len'),
                    lit('='),
                    disj(Exp,
                         lit('*')))
    
    _explLen  = treat(_explLen,lambda l: _F90ExplLen(l[2]))
    
    _explKind = seq(lit('kind'),
                    lit('='),
                    Exp)
    
    _explKind = treat(_explKind,lambda l: _ExplKind(l[2]))
    
    _explList = seq(lit('('),
                    cslist(disj(_explLen,_explKind)), 
                    lit(')'))
    
    _explList = treat(_explList,lambda l: l[1])
    
    _modOpts  = zo1(disj(_f77mod,_f90mod,_explList))
    
    # the first two are just single things while the last thing is a list
    # we need to remove one level of list nesting: 
    _modOpts  = treat(_modOpts,lambda l: len(l) and isinstance(l[0],list) and l[0] or l)
    spec=seq(lit(kw),
             _modOpts)
    
    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        p1 = seq(CharacterStmt.spec,type_attr_list,zo1(lit('::')),
                 cslist(char_decl_item))
        try: 
          (((dc,mod),attrs,dc1,decls),rest) = p1(scan)
        except AssemblerException,e:
          raise ParseError(lineNumber,scan,'character variable declaration')  

        return CharacterStmt(mod,attrs,decls,dc,lineNumber,rest=rest)

    def __init__(self,mod,attrs,decls,stmt_name=kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.stmt_name = stmt_name
        TypeDecl.__init__(self,mod,attrs,decls,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return 'CharacterStmt(%s,%s,%s)' % (repr(self.mod),repr(self.attrs),repr(self.decls))

    def __str__(self):
        modstr = ''
        if self.mod:
            if (len(self.mod)==1 and isinstance(self.mod[0],_F77Len) or isinstance(self.mod[0],_F90Len)):
                modstr=str(self.mod[0])
            else:
                modstr = '('+','.join([str(a)[1:-1] for a in self.mod])+')'
        
        attr_str = ''
        if self.attrs:
            attr_str = ','+','.join([str(a) for a in self.attrs])
            
        return '%s%s%s :: %s' % (self.stmt_name,modstr,
                                 attr_str,
                                 ','.join([str(d) for d in self.decls]))

class IntrinsicStmt(Decl):
    'Intrinsic stmt'
    kw    = 'intrinsic'
    kw_str = kw

    @staticmethod
    def parse(ws_scan,lineNumber) :
        scan = filter(lambda x: x != ' ',ws_scan)
        form = seq(lit(IntrinsicStmt.kw)) # 0 = stmt_name
        (id,rest) = form(scan)
        return IntrinsicStmt(lineNumber=lineNumber,rest=rest)

    def __init__(self,stmt_name=kw_str,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

class IncludeLine(Decl):
    '''
    Include line; the fortran standard says it is not a statement but a processor directive
    after parsing it; it is immediately  replaced by the contents of the included file
    '''
    kw    = 'include'
    kw_str = kw

    @staticmethod
    def parse(ws_scan,lineNumber) :
        scan = filter(lambda x: x != ' ',ws_scan)
        form = seq(lit(IncludeLine.kw)) # 0 = stmt_name
        (id,rest) = form(scan)
        return IncludeLine(lineNumber=lineNumber,rest=rest)

    def __init__(self,stmt_name=kw_str,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

class RealStmt(TypeDecl):
    kw = 'real'
    kw_str = kw

    def __init__(self,mod,attrs,decls,stmt_name=kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.stmt_name = stmt_name
        TypeDecl.__init__(self,mod,attrs,decls,lineNumber,label,lead,internal,rest)

class ComplexStmt(TypeDecl):
    kw = 'complex'
    kw_str = kw

    def __init__(self,mod,attrs,decls,stmt_name=kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.stmt_name = stmt_name
        TypeDecl.__init__(self,mod,attrs,decls,lineNumber,label,lead,internal,rest)

class IntegerStmt(TypeDecl):
    kw = 'integer'
    kw_str = kw

    def __init__(self,mod,attrs,decls,stmt_name=kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.stmt_name = stmt_name
        TypeDecl.__init__(self,mod,attrs,decls,lineNumber,label,lead,internal,rest)

class LogicalStmt(TypeDecl):
    kw = 'logical'
    kw_str = kw

    def __init__(self,mod,attrs,decls,stmt_name=kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.stmt_name = stmt_name
        TypeDecl.__init__(self,mod,attrs,decls,lineNumber,label,lead,internal,rest)

class DoubleStmt(TypeDecl):
    kw     = 'doubleprecision'
    kw_str = 'double precision'

    def __init__(self,mod,attrs,decls,stmt_name=kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.stmt_name = stmt_name
        TypeDecl.__init__(self,mod,attrs,decls,lineNumber,label,lead,internal,rest)

class DoubleCplexStmt(TypeDecl):
    kw     = 'doublecomplex'
    kw_str = 'double complex'

    def __init__(self,mod,attrs,decls,stmt_name=kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.stmt_name = stmt_name
        TypeDecl.__init__(self,mod,attrs,decls,lineNumber,label,lead,internal,rest)

class DimensionStmt(Decl):
    kw = 'dimension'
    kw_str = kw
    _sons = ['lst']

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)

        p1 = seq(lit(DimensionStmt.kw),
                 zo1(lit('::')),
                 cslist(_DimensionArraySpec.form))
        ((dc,sep,lst),rest) = p1(scan)
        return DimensionStmt(lst,dc,lineNumber,rest=rest)

    def __init__(self,lst,stmt_name=kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.lst = lst
        self.stmt_name = stmt_name
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return '%s(%s)' % (self.__class__.__name__,repr(self.lst))

    def __str__(self):
        return '%s %s' % (self.stmt_name,','.join([str(l) for l in self.lst]))

    def get_decls(self):
        return self.lst

    def set_decls(self,newLst):
        self.lst = newLst
        self.modified = True

class IntentStmt(Decl):
    'Intent stmt'
    kw    = 'intent'
    kw_str = kw

    @staticmethod
    def parse(ws_scan,lineNumber) :
        scan = filter(lambda x: x != ' ',ws_scan)
        form = seq(lit(IntentStmt.kw)) # 0 = stmt_name
        (id,rest) = form(scan)
        return IntentStmt(lineNumber=lineNumber,rest=rest)

    def __init__(self,stmt_name=kw_str,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

class OptionalStmt(Decl):
    'Optional stmt'
    kw    = 'optional'
    kw_str = kw

    @staticmethod
    def parse(ws_scan,lineNumber) :
        scan = filter(lambda x: x != ' ',ws_scan)
        form = seq(lit(OptionalStmt.kw)) # 0 = stmt_name
        (id,rest) = form(scan)
        return OptionalStmt(lineNumber=lineNumber,rest=rest)

    def __init__(self,stmt_name=kw_str,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

class NamelistStmt(Decl):
    'Namelist stmt'
    kw    = 'namelist'
    kw_str = kw
    _sons = ['namVarListPairs']

    @staticmethod
    def parse(ws_scan,lineNumber) :
        scan = filter(lambda x: x != ' ',ws_scan)
        namVarListPair = seq(lit('/'),
                             id,         # 1 = namlist
                             lit('/'),
                             cslist(id)) # 3 = varlist
        namVarListPair = treat(namVarListPair, lambda x:(x[1],x[3]))

        form = seq(lit(NamelistStmt.kw_str), # 0 = stmt_name
                   star(namVarListPair))     # 1 = namVarListPairs
        ((name,namVarListPairs),rest) = form(scan)
        return NamelistStmt(namVarListPairs,stmt_name=name,lineNumber=lineNumber,rest=rest)

    def __init__(self,namVarListPairs,stmt_name=kw_str,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.namVarListPairs = namVarListPairs
        Decl.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self):
        nameVarGroups = ''
        for pair in self.namVarListPairs:
            pair = '/%s/ %s' % (pair[0],','.join(pair[1]))
            if nameVarGroups == '':
                nameVarGroups = pair
            else:
                nameVarGroups += ' '+pair
        return '%s %s' % (self.kw_str,nameVarGroups)

class FuncOrSubStmt(PUstart):
    ourQualifiers=['recursive','pure','elemental']
    ourQualifierLits=map(lit,ourQualifiers)    

class SubroutineStmt(FuncOrSubStmt):
    kw = 'subroutine'
    kw_str = kw
    utype_name = kw
    _sons = ['name','args','qualifiers']

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        subroutinePrefix=star(disj(*FuncOrSubStmt.ourQualifierLits))
        p1 = seq(subroutinePrefix,
                 lit(SubroutineStmt.kw),
                 id,
                 zo1(seq(lit('('),cslist(id),lit(')')))
                 )
        ((qualifiers,theKW,name,args),rst) = p1(scan)
        if args:
            args = args[0][1]
        if (not qualifiers):
            qualifiers=None
        return SubroutineStmt(name,args,qualifiers,lineNumber=lineNumber,rest=rst)

    def __init__(self,name,args,qualifiers=None,stmt_name=kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.name = name
        self.args = args
        self.qualifiers=qualifiers
        self.stmt_name = stmt_name
        PUstart.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return '%s(%s,%s,%s)' % (self.__class__.__name__,
                                 repr(self.name),
                                 repr(self.args),
                                 repr(self.qualifiers))
    def __str__(self):
        rStr=''
        if (self.qualifiers):
            rStr+=' '.join(self.qualifiers)+' '
        return '%s%s %s(%s)' % (rStr,self.stmt_name,self.name,
                                ','.join([str(d) for d in self.args]))

class ProgramStmt(PUstart):
    kw = 'program'
    kw_str = kw
    utype_name = kw
    _sons = ['name']

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        p1 = seq(lit(ProgramStmt.kw),
                 id)
        ((dc,name),rest) = p1(scan)
        return ProgramStmt(name,dc,lineNumber=lineNumber,rest=rest)

    def __init__(self,name,stmt_name=kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.name = name
        self.stmt_name = stmt_name 
        PUstart.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return '%s(%s)' % (self.__class__.__name__,repr(self.name))

    def __str__(self):
        return '%s %s' % (self.stmt_name,self.name)

class FunctionStmt(FuncOrSubStmt):
    kw = 'function'
    kw_str = kw
    utype_name = kw
    _sons = ['ty','name','args','result','qualifiers']

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        drvdTypeSpec=seq(lit('type'), lit('('), id, lit(')'))
        drvdTypeSpec=treat(drvdTypeSpec,lambda l: DrvdTypeDecl([l[2]],[],[],lineNumber=lineNumber))
        functionPrefix=star(disj(type_pat_sem,drvdTypeSpec,*FuncOrSubStmt.ourQualifierLits))
        p1 = seq(functionPrefix,
                 lit(FunctionStmt.kw),
                 id,
                 lit('('),
                 cslist(id),
                 lit(')'),
                 zo1(seq(lit('result'),
                         lit('('),
                         id,
                         lit(')'))))
        ((prefix,dc,name,dc1,args,dc2,resultstuff),rest) = p1(scan)
        type=None
        qualifiers=None
        if (prefix):
            typeIter=itertools.ifilter(lambda l: not(l in FunctionStmt.ourQualifiers),prefix)
            try:
                type=typeIter.next()
            except StopIteration, e:
                pass
            qualifiers=filter(lambda l: (l in FunctionStmt.ourQualifiers),prefix)
            if (not qualifiers): # empty list
                qualifiers=None
        result = resultstuff and resultstuff[0][2] \
                              or None
        return FunctionStmt(type,name,args,result,qualifiers,lineNumber,rest=rest)

    def __init__(self,ty,name,args,result,qualifiers=None,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        '''
        typ = None

        if ty:
            (type_name,mod) = ty[0]
            type_class = _name2class(type_name)
            typ        = (type_class,mod)
        '''
        self.ty = ty
        self.name = name
        self.args = args
        self.result = result
        self.qualifiers=qualifiers
        PUstart.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        typeRepr = self.ty and '('+self.ty[0].__name__+','+repr(self.ty[1])+')' \
                            or None
        resultRepr = self.result and repr(self.result) \
                                  or None
        return '%s(%s,%s,%s,%s,%s)' % (self.__class__.__name__,
                                       typeRepr,
                                       repr(self.name),
                                       repr(self.args),
                                       resultRepr,
                                       repr(self.qualifiers))

    def __str__(self):
        typePrefix = self.ty and (typestr2(self.ty)+' ') \
                              or ''
        resultStr = self.result and ' result('+str(self.result)+')' \
                                 or ''
        rStr=''
        if (self.qualifiers):
            rStr+=' '.join(self.qualifiers)+' '
        return '%s%s%s %s(%s)%s' % (rStr,
                                    typePrefix,
                                    self.kw,
                                    str(self.name),
                                    ','.join([str(l) for l in self.args]),
                                    resultStr)

class ModuleStmt(PUstart):
    kw = 'module'
    kw_str = kw
    utype_name = kw
    _sons = ['name']

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        p1 = seq(lit(ModuleStmt.kw),
                 id)
        ((dc,name),rest) = p1(scan)
        return ModuleStmt(name,lineNumber,rest=rest)

    def __init__(self,name,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.name = name 
        PUstart.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return '%s(%s)' % (self.__class__.__name__,
                              repr(self.name))
    def __str__(self):
        return 'module %s' % self.name

class UseStmt(Decl):
    kw     = 'use'
    kw_str = kw

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        # forms of renameItem: (we currently support only the first)
        # [ local-name => ] module-entity-name
        # [ OPERATOR ( local-defined-operator ) => ] OPERATOR ( module-defined-operator )
        formRenameItem = seq(id,
                             lit('=>'),
                             id)
        formRenameItem = treat(formRenameItem, lambda x: _PointerInit(x[0],x[2]))

        formModuleNature = seq(zo1(seq(lit(','),
                                       id)),
                               lit('::'))
        formModuleNature = treat(formModuleNature, lambda x: x[0] and x[0][0][1] or None)

        # use statement with NO ONLY:
        # USE [[ , module-nature ] :: ] module-name [ , rename-list ]
        # module-nature is one of [intrinsic,non_intrinsic]
        formUseAllStmt = seq(lit(UseStmt.kw),
                             zo1(formModuleNature),
                             id,
                             zo1(seq(lit(','),
                                     cslist(formRenameItem))))
        formUseAllStmt = treat(formUseAllStmt, lambda x: UseAllStmt(x[2],x[3] and x[3][0][1] or None,x[1] or None,x[0],lineNumber=lineNumber))

        # forms of onlyItem:
        # generic-name
        # OPERATOR ( module-defined-operator )    (not currently supported)
        # ASSIGNMENT ( = )                        (not currently supported)
        # dtio-generic-spec                       (not currently supported)
        # module-entity-name
        # rename
        formOnlyItem = disj(formRenameItem,
                            id)

        # use statement WITH ONLY:
        # USE [[ , module-nature ] :: ] module-name , ONLY : [ only-list ]
        formUseOnlyStmt = seq(lit(UseStmt.kw),
                              zo1(formModuleNature),
                              id,
                              lit(','),
                              lit('only'),
                              lit(':'),
                              cslist(formOnlyItem))
        formUseOnlyStmt = treat(formUseOnlyStmt, lambda x: UseOnlyStmt(x[2],x[6],x[1] or None, x[0],lineNumber=lineNumber))

        (theParsedStmt,rest) = disj(formUseOnlyStmt,formUseAllStmt)(scan)
        theParsedStmt.rest = rest
        return theParsedStmt

class UseAllStmt(UseStmt):
    _sons  = ['moduleName','renameList','moduleNature']

    def __init__(self,moduleName,renameList,moduleNature=None,stmt_name=UseStmt.kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.moduleName = moduleName
        self.renameList = renameList
        self.moduleNature = moduleNature
        self.stmt_name = stmt_name
        UseStmt.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self):
        renameListStr = self.renameList and ', '+\
                        ','.join([str(aRenameItem)
                                  for aRenameItem in self.renameList]) \
                                         or ''
        return self.stmt_name+' '+str(self.moduleName)+renameListStr

class UseOnlyStmt(UseStmt):
    _sons  = ['moduleName','onlyList','moduleNature']

    def __init__(self,moduleName,onlyList,moduleNature=None,stmt_name=UseStmt.kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.moduleName = moduleName
        self.onlyList = onlyList
        self.moduleNature = moduleNature
        self.stmt_name = stmt_name
        UseStmt.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self):
        return self.stmt_name+' '+str(self.moduleName)+', only: '+\
               ','.join([str(anOnlyItem) for anOnlyItem in self.onlyList])

class EntryStmt(Exec):
    '''
    entry stmt; the standard lists it as a non-executable but 
    the unit parser pattern (fortUnit) knows only the two groups declaration and executable 
    statements which don't mix so this one has to be treated 
    as if it was an executable 
    '''
    kw    = 'entry'
    kw_str = kw
    _sons = ['name','args','result']

    @staticmethod
    def parse(ws_scan,lineNumber) :
        scan = filter(lambda x: x != ' ',ws_scan)
        entryPatn = seq(lit(EntryStmt.kw),
                        id,
                        zo1(seq(lit('('),
                                cslist(id),
                                lit(')'))),
                        zo1(seq(lit('result'),
                                lit('('),
                                id,
                                lit(')'))))
        ((theKW,name,args,result),rest) = entryPatn(scan)
        result = result and result[0][2] or None
        args = args and args[0][1] or None
        theStmt=EntryStmt(name,args,result,lineNumber=lineNumber,rest=rest)
        DebugManager.warning('statement >'+str(theStmt)+'< implies unstructured control flow',lineNumber,DebugManager.WarnType.controlFlow)
        return theStmt

    def __init__(self,name,args=None,result=None,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.name=name
        self.args=args
        self.result=result
        Exec.__init__(self,lineNumber,label,lead,internal,rest)
        
    def __repr__(self):
        return self.__class__.__name__+('(%s,%s.%s)' % (self.name,repr(self.args),repr(self.result)))
        
    def __str__(self):
        rStr=self.kw_str+' '+self.name
        if self.args:
            rStr+=' ( '+','.join(map(str,self.args))+' ) '
        if self.result:
            rStr+=' result ( '+self.result+' )'
        return rStr

class ExitStmt(Exec):
    kw = 'exit'
    kw_str = kw
    _sons = ['optionalDoConstructName']

    @staticmethod
    def parse(ws_scan,lineNumber) :
        scan = filter(lambda x: x != ' ',ws_scan)
        form = seq(lit(ExitStmt.kw), # 0 = stmt_name
                   zo1(id))          # 1 = optionalDoConstructName
        form = treat(form, lambda x: ExitStmt(x[1] and x[1][0] or None,
                                              x[0]))
        (theParsedStmt,rest) = form(scan)
        theParsedStmt.rest = rest
        return theParsedStmt 

    def __init__(self,optionalDoConstructName,stmt_name=kw_str,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.optionalDoConstructName = optionalDoConstructName
        self.stmt_name = stmt_name
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self) :
        optionalDoConstructStr = self.optionalDoConstructName and ' '+self.optionalDoConstructName \
                                                               or '' 
        return '%s%s' % (self.stmt_name,optionalDoConstructStr)


class EnddoStmt(Exec):
    kw = 'enddo'
    kw_str = 'end do'
    _sons = ['doConstructName']

    @staticmethod
    def parse(ws_scan,lineNumber) :
        scan = filter(lambda x: x != ' ',ws_scan)
        form = seq(lit(EnddoStmt.kw), # 0 = stmt_name
                   zo1(id))           # 1 = doConstructName
        form = treat(form, lambda x: EnddoStmt(x[1] and x[1][0] or None))
        (theParsedStmt,rest) = form(scan)
        theParsedStmt.rest = rest
        return theParsedStmt 

    def __init__(self,doConstructName,stmt_name=kw_str,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.doConstructName = doConstructName
        self.stmt_name = stmt_name
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self) :
        optionalDoConstructStr = self.doConstructName and ' '+self.doConstructName \
                                                       or '' 
        return '%s %s' % (self.stmt_name,optionalDoConstructStr)


class CycleStmt(Exec) :
    kw = 'cycle'
    kw_str = kw
    _sons = ['doConstructName']

    @staticmethod
    def parse(ws_scan,lineNumber) :
        scan = filter(lambda x: x != ' ',ws_scan)
        form = seq(lit(CycleStmt.kw), # 0 = stmt_name
                   zo1(id))           # 1 = doConstructName
        form = treat(form, lambda x: CycleStmt(x[1] and x[1][0] or None,
                                               x[0]))
        (theParsedStmt,rest) = form(scan)
        theParsedStmt.rest = rest
        return theParsedStmt 

    def __init__(self,doConstructName,stmt_name=kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.doConstructName = doConstructName
        self.stmt_name = stmt_name
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self) :
        optionalDoConstructStr = self.doConstructName and ' '+self.doConstructName \
                                                       or '' 
        return '%s%s' % (self.stmt_name,optionalDoConstructStr)


class CallStmt(Exec):
    kw = 'call'
    kw_str = kw
    _sons = ['head','args']

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        prefix = seq(lit(CallStmt.kw),disj(app,sel,id))
        ((dc,a),rst) = prefix(scan)
        if (isinstance(a,App)):
            return CallStmt(a.head,a.args,dc,lineNumber,rest=rst)
        elif isinstance(a,Sel):
            return CallStmt(a,[],dc,lineNumber,rest=rst)
        else:
            return CallStmt(a,[],dc,lineNumber,rest=rst)

    def __init__(self,head,args,stmt_name=kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.head = head
        self.args = args
        self.stmt_name = stmt_name
        Exec.__init__(self,lineNumber,label,lead,internal,rest)
        
    def __repr__(self):
        return 'CallStmt(%s,%s)' % (repr(self.head),
                                    repr(self.args),)

    def __str__(self):
        return '%s %s(%s)' % (self.stmt_name,str(self.head),
                                ','.join([str(l) for l in self.args]))

    def get_head(self):
        return self.head

    def set_head(self,newHead):
        if self.head != newHead:
            self.head = newHead
            self.modified = True

    def get_args(self):
        return self.args

    def set_args(self,newArgs):
        self.args = newArgs
        self.modified = True

class AssignStmt(Exec):
    _sons = ['lhs','rhs']

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        if (not '=' in scan):
            raise AssemblerException(scan,lineNumber)
        lhsEq   = seq(lv_exp,lit('='))
        ((l,dc),rst) = lhsEq(scan)
        ((r),rst) = Exp(rst)
        return AssignStmt(l,r,lineNumber,rest=rst)

    def __init__(self,lhs,rhs,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.lhs  = lhs
        self.rhs  = rhs
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return 'AssignStmt(%s,%s)' % (repr(self.lhs),repr(self.rhs))

    def __str__(self):
        return '%s = %s' % (str(self.lhs),str(self.rhs))

    def get_lhs(self):
        return self.lhs

    def set_lhs(self,newLhs):
        if lhs != newLhs:
            self.lhs = newLhs
            self.modified = True

    def get_rhs(self):
        return self.rhs

    def set_rhs(self,newRhs):
        if rhs != newRhs:
            self.rhs = newRhs
            self.modified = True

class PointerAssignStmt(Exec):
    _sons = ['lhs','rhs']

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        formPointerAssignStmt = seq(lv_exp,
                                    lit('=>'),
                                    Exp)
        ((lhs,assignSymbol,rhs),rst) = formPointerAssignStmt(scan)
        return PointerAssignStmt(lhs,rhs,lineNumber,rest=rst)

    def __init__(self,lhs,rhs,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.lhs  = lhs
        self.rhs  = rhs
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return 'PointerAssignStmt(%s,%s)' % (repr(self.lhs),repr(self.rhs))

    def __str__(self):
        return '%s => %s' % (str(self.lhs),str(self.rhs))


class IOStmt(Exec):
    _sons = ['ioCtrlSpecList','itemList','kwString']

    def __init__(self,ioCtrlSpecList,itemList,kwString,lineNumber,label,lead,internal,rest):
        self.kwString = kwString # the actual string as given in the program
        self.ioCtrlSpecList=ioCtrlSpecList
        self.itemList=itemList
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def get_itemList(self):
        return self.itemList

    def set_itemList(self,newItemList):
        self.itemList = newItemList
        self.modified = True

class SimpleSyntaxIOStmt(IOStmt):

    @classmethod
    def parse(cls,ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        ioStmtPatn = seq(lit(cls.kw),disj(lit('*'),Exp),zo1(seq(lit(','),cslist(Exp))))
        ioStmtExtractList = treat(ioStmtPatn,lambda l: (l[0],l[1],len(l[2]) and l[2][0][1] or []))
        ((kwString,format,itemList),rest) = ioStmtExtractList(scan)
        return cls([format],itemList,kwString,lineNumber,rest=rest)

    def __init__(self,ioCtrlSpecList,itemList,kwString,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        IOStmt.__init__(self,ioCtrlSpecList,itemList,kwString,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return '%s(%s,%s)' % (self.__class__.__name__,repr(self.ioCtrlSpecList),repr(self.itemList))

    def __str__(self):
        rstr=self.kwString+' '+str(self.ioCtrlSpecList[0])
        if self.itemList:
            rstr+=','+','.join(map(str,self.itemList))
        return '%s' % (rstr)

class PrintStmt(SimpleSyntaxIOStmt):
    kw = 'print'
    kw_str = kw

    def __init__(self,ioCtrlSpecList,itemList,kwString=kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        SimpleSyntaxIOStmt.__init__(self,ioCtrlSpecList,itemList,kwString,lineNumber,internal,rest)

class ComplexSyntaxIOStmt(IOStmt):

    @classmethod
    def parseAll(cls,ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        ioStmt = seq(lit(cls.kw),
                     lit('('),
                     cslist(disj(lit('*'),NamedParmExpWithStar,Exp)),
                     lit(')'),
                     cslist(Exp))
        ([kwString,lbracket,ioCtrlSpecList,rbracket,itemList],rest) = ioStmt(scan)

        return cls(ioCtrlSpecList,itemList,kwString,lineNumber=lineNumber,rest=rest)

    def __init__(self,ioCtrlSpecList,itemList,kwString,lineNumber,label,lead,internal,rest):
        IOStmt.__init__(self,ioCtrlSpecList,itemList,kwString,lineNumber,label,lead,internal,rest)

    def __str__(self):
        return '%s(%s) %s' % (self.kwString,
                              ','.join(map(str,self.ioCtrlSpecList)),
                              ','.join(map(str,self.itemList)))

class SimpleReadStmt(SimpleSyntaxIOStmt):
    ''' 
    syntax variant that has only format but not a full ioCtrlSpecList; 
    its parse method is only called as a fallback on failure of ReadStmt.parse 
    '''
    kw = 'read'
    kw_str = kw

    def __init__(self,format,itemList,kwString=kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        SimpleSyntaxIOStmt.__init__(self,format,itemList,kwString,lineNumber,label,lead,internal,rest)

class ReadStmt(ComplexSyntaxIOStmt):
    kw = 'read'
    kw_str = kw
    
    @staticmethod
    def parse(ws_scan,lineNumber):
        try : 
            return ReadStmt.parseAll(ws_scan,lineNumber)
        except ListAssemblerException,e:
            return SimpleReadStmt.parse(ws_scan,lineNumber)

    def __init__(self,ioCtrlSpecList,itemList,kwString=kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        IOStmt.__init__(self,ioCtrlSpecList,itemList,kwString,lineNumber,label,lead,internal,rest)
    
class WriteStmt(ComplexSyntaxIOStmt):
    kw = 'write'
    kw_str = kw

    @staticmethod
    def parse(ws_scan,lineNumber):
        return WriteStmt.parseAll(ws_scan,lineNumber)

    def __init__(self,ioCtrlSpecList,itemList,kwString=kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        ComplexSyntaxIOStmt.__init__(self,ioCtrlSpecList,itemList,kwString,lineNumber,label,lead,internal,rest)

class FormatStmt(Exec):
    kw = 'format'
    kw_str = kw

#     @staticmethod
#     def parse(ws_scan,lineNumber):
#         scan = filter(lambda x: x != ' ',ws_scan)
#         formFormatStmt = seq(lit(FormatStmt.kw),
#                              lit('('),
#                              cslist(Exp),
#                              lit(')'))
#         ((stmt_name,lparen,editlist,rparen),rst) = formFormatStmt(scan)
#         return FormatStmt(editlist,lineNumber)
    
#     def __init__(self,editlist,lineNumber=0,label=False,lead='',internal=[],rest=[]):
#         self.editlist = editlist
#         Exec.__init__(self,lineNumber,label,lead,internal,rest)
        
#     def __str__(self):
#         return '%s(%s)' % (self.kw_str,','.join(self.editlist))

class StopStmt(Exec):
    kw = 'stop'
    kw_str = kw
    _sons = ['msg']

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        formStopStmt = seq(lit(StopStmt.kw),
                           zo1(disj(const,pred(is_int))))
        ((stmt_name,msg),rst) = formStopStmt(scan)
        if len(msg) > 0:
            return StopStmt(msg[0],lineNumber)
        else:
            return StopStmt(lineNumber=lineNumber)

    def __init__(self,msg='',lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.msg = msg
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self):
        return '%s %s' % (self.kw,self.msg)

class ReturnStmt(Leaf):
    kw = 'return'
    kw_str = kw
    _sons = ['ordinal']

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        formReturnStmt = seq(lit(ReturnStmt.kw),
                             zo1(pred(is_int)))
        ((stmt_name,ordinal),rst) = formReturnStmt(scan)
        if len(ordinal) > 0:
            return ReturnStmt(ordinal[0],lineNumber)
        return ReturnStmt(lineNumber=lineNumber)

    def __init__(self,ordinal=-1,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.ordinal = ordinal
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self):
        if self.ordinal >= 0:
            return '%s %d' % (self.kw,self.ordinal)
        else:
            return self.kw

class IfStmt(Exec):
    kw = 'if'
    kw_str = kw

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        prefix = seq(lit(IfStmt.kw),lit('('),Exp,lit(')'))
        theStmt=None
        try: #arithmetic if
            arithIfPatn=seq(prefix,pred(is_int),lit(','),pred(is_int),lit(','),pred(is_int))
            (((ifLit,dc1,expr,dc2),l1,c1,l2,c2,l3),rest)=arithIfPatn(scan)
            theStmt=ArithmIfStmt(expr,(l1,l2,l3),ifLit,lineNumber,rest=rest)
        except: 
            try: # if ( ) then
                thenPatn=seq(prefix,lit('then'))
                (((ifLit,dc1,expr,dc2),thenLit),rest)=thenPatn(scan)
                theStmt=IfThenStmt(expr,ifLit,thenLit,lineNumber,rest=rest)
            except: # if ( )  statement   no  then 
                ((ifLit,dc1,expr,dc2),rest)=prefix(scan)
                wsIdx=-1 # find index where rest starts in ws_scan
                while wsIdx<(len(scan)-len(rest)):
                    wsIdx=ws_scan[wsIdx+1:].index(rest[0])+wsIdx+1
                subStmt=_kw_parse(ws_scan[wsIdx:],lineNumber)
                theStmt=IfNonThenStmt(expr,subStmt,ifLit,lineNumber)
        return theStmt
    
class ArithmIfStmt(IfStmt): #arithmetic if 
    _sons = ['expr', 'labelTriple','ifFormatStr']

    def __init__(self,expr,labelTriple,ifFormatStr=IfStmt.kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.expr = expr
        self.labelTriple = labelTriple
        self.ifFormatStr = ifFormatStr
        IfStmt.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return self.__class__.__name__+'(%s,%s)' % (repr(self.expr),repr(self.labelTriple))

    def __str__(self):
        return '%s (%s) %s' % (self.kw_str,str(self.expr),','.join(self.labelTriple))
                   
class IfThenStmt(IfStmt):
    _sons = ['test','ifFormatStr','thenFormatStr']

    def __init__(self,test,ifFormatStr=IfStmt.kw,thenFormatStr='then',lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.test = test
        self.ifFormatStr = ifFormatStr
        self.thenFormatStr = thenFormatStr
        IfStmt.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return 'IfThenStmt(%s)' % (repr(self.test),)

    def __str__(self):
        return '%s (%s) %s' % (self.ifFormatStr,str(self.test),self.thenFormatStr)

class IfNonThenStmt(IfStmt):
    _sons = ['test','stmt','ifFormatStr']

    def __init__(self,test,stmt,ifFormatStr=IfStmt.kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.test = test
        self.stmt = stmt
        self.ifFormatStr = ifFormatStr
        IfStmt.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return self.__class__.__name__+'(%s,%s)' % (repr(self.test),
                                                    repr(self.stmt))

    def __str__(self):
        return '%s (%s) %s' % (self.ifFormatStr,str(self.test),str(self.stmt))

class ElseifStmt(Exec):
    kw = 'elseif'
    kw_str = kw
    _sons = ['test']

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        prefix = seq(lit(ElseifStmt.kw),lit('('),Exp,lit(')'),lit('then'))

        ((dc0,dc1,e,dc2,dc3),rest) = prefix(scan)
        return ElseifStmt(e,dc0,dc3,lineNumber,rest=rest)

    def __init__(self,e,stmt_name=kw,stmt_name2='then',lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.test = e
        self.stmt_name = stmt_name
        self.stmt_name2 = stmt_name2
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return 'ElseifStmt(%s)' % (repr(self.test),)

    def __str__(self):
        return '%s (%s) %s' % (self.stmt_name,str(self.test),self.stmt_name2)
    
class ElseStmt(Leaf):
    kw = 'else'
    kw_str = kw

class WhereStmt(Exec):
    ''' WHERE ( logical-expression ) array-assignment-statement'''
    kw = 'where'
    kw_str = kw
    _sons = ['conditional','assignment']

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        lhs = lv_exp

        formAssign = seq(lv_exp,lit('='),Exp)
        formAssign = treat(formAssign,lambda x:AssignStmt(x[0],x[2],lineNumber=lineNumber))

        formWhereStmt = seq(lit(WhereStmt.kw),
                            lit('('),
                            Exp,
                            lit(')'),
                            zo1(formAssign))
        ((whereKW,oPeren,conditional,cPeren,assignment),rest) = formWhereStmt(scan)
        assignment = assignment and assignment[0] \
                                 or None
        return WhereStmt(conditional,assignment,lineNumber,rest=rest)

    def __init__(self,conditional,assignment,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.conditional = conditional
        self.assignment = assignment
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self):
        assignStr = self.assignment and ' '+str(self.assignment) \
                                     or ''
        return '%s (%s)%s' % (self.kw,
                               str(self.conditional),
                               assignStr)

class ElsewhereStmt(Leaf):
    kw = 'elsewhere'
    kw_str = kw

class EndWhereStmt(Leaf):
    kw = 'endwhere'
    kw_str = 'end where'

class EndifStmt(Leaf):
    kw = 'endif'
    kw_str = 'end if'

class EndStmt(PUend):
    kw = 'end'
    kw_str = kw

    def is_uend(self,unit=_non): return True
    def is_decl(self,unit=_non):
        return unit.val._in_iface
    def is_exec(self,unit=_non): return False

    def __init__(self,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        PUend.__init__(self,lineNumber,label,lead,internal,rest)
        
class ComplexEndStmt(EndStmt):
    _sons = ['name']

    @staticmethod
    def parse(ws_scan,lineNumber,kw,SubClass):
        scan = filter(lambda x: x != ' ',ws_scan)
        formEndStmt = seq(lit(kw),
                          zo1(id))
        ((stmt_name,name),rst) = formEndStmt(scan)
        if name == []:
            return SubClass(lineNumber=lineNumber)
        else:
            return SubClass(name[0],lineNumber)

    def __init__(self,name='',lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.name = name
        EndStmt.__init__(self,lineNumber,label,lead,internal,rest)
        

    def __str__(self):
        return '%s %s' % (self.kw_str,self.name)
    

class EndPseudoStmt(GenStmt):
    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        if len(scan) >= 2 and scan[1].lower() == 'interface':
            return EndInterfaceStmt.parse(scan,lineNumber,rest=rest)
        return EndStmt.parse(scan,lineNumber,rest=rest)


class EndModuleStmt(ComplexEndStmt):
    kw =  'endmodule'
    kw_str = 'end module'

    @staticmethod
    def parse(ws_scan,lineNumber):
        return ComplexEndStmt.parse(ws_scan,lineNumber,EndModuleStmt.kw,EndModuleStmt)

    def __init__(self,name='',lineNumber=0,label=False,lead='',internal=[],rest=[]):
        ComplexEndStmt.__init__(self,name,lineNumber,label,lead,internal,rest)

class EndProgramStmt(ComplexEndStmt):
    kw    = 'endprogram'
    kw_str = 'end program'

    @staticmethod
    def parse(ws_scan,lineNumber):
        return ComplexEndStmt.parse(ws_scan,lineNumber,EndProgramStmt.kw,EndProgramStmt)

    def __init__(self,name='',lineNumber=0,label=False,lead='',internal=[],rest=[]):
        ComplexEndStmt.__init__(self,name,lineNumber,label,lead,internal,rest)

class EndFunctionStmt(ComplexEndStmt):
    'End of function block'
    kw    = 'endfunction'
    kw_str = 'end function'

    @staticmethod
    def parse(ws_scan,lineNumber):
        return ComplexEndStmt.parse(ws_scan,lineNumber,EndFunctionStmt.kw,EndFunctionStmt)

    def __init__(self,name='',lineNumber=0,label=False,lead='',internal=[],rest=[]):
        ComplexEndStmt.__init__(self,name,lineNumber,label,lead,internal,rest)

class EndSubroutineStmt(ComplexEndStmt):
    'End of subroutine block'
    kw    = 'endsubroutine'
    kw_str = 'end subroutine'

    @staticmethod
    def parse(ws_scan,lineNumber):
        return ComplexEndStmt.parse(ws_scan,lineNumber,EndSubroutineStmt.kw,EndSubroutineStmt)

    def __init__(self,name='',lineNumber=0,label=False,lead='',internal=[],rest=[]):
        ComplexEndStmt.__init__(self,name,lineNumber,label,lead,internal,rest)

class DoStmt(Exec):
    #FIXME: optional comma isn't handled
    '''
    [do-construct-name :] do [label] [loop control]
    '''
    kw = 'do'
    kw_str = kw
    _sons = ['doName','doLabel','loopControl','doFormatStr']

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        formDoName = seq(id,
                         lit(':'))
        formDoName = treat(formDoName, lambda x: x[0])
        formDoStmt = seq(zo1(formDoName),        # 0 = doName
                         lit(DoStmt.kw),         # 1 = theDoKeyword
                         zo1(pred(is_int)),      # 2 = doLabel
                         zo1(LoopControl.form))  # 3 = loopControl
        formDoStmt = treat(formDoStmt, lambda x: DoStmt(x[0] and x[0][0] or None,
                                                        x[2] and x[2][0] or None,
                                                        x[3] and x[3][0] or None,
                                                        x[1]))
        (theParsedStmt,rest) = formDoStmt(scan)
        theParsedStmt.rest = rest
        theParsedStmt.lineNumber=lineNumber
        return theParsedStmt 

    def __init__(self,doName,doLabel,loopControl,doFormatStr=kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.doName = doName
        self.doLabel = doLabel
        self.loopControl = loopControl
        self.doFormatStr = doFormatStr
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self):
        doNameString = self.doName and str(self.doName)+': ' \
                                    or ''
        doLabelString = self.doLabel and ' '+str(self.doLabel) \
                                      or ''
        loopControlString = self.loopControl and ' '+str(self.loopControl) \
                                              or ''
        return '%s%s%s%s' % (doNameString,self.doFormatStr,doLabelString,loopControlString)


class WhileStmt(Exec):
    #FIXME: optional construct name, label, and comma are not handled
    '''
    [do-construct-name : ] DO [ label ] [ , ] while ( scalar-logical-expression )
    '''
    _sons = ['testExpression']
    kw = 'dowhile'
    kw_str = 'do while'

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        formWhileStmt = seq(lit(WhileStmt.kw),
                            lit('('),
                            Exp,
                            lit(')'))
        ((theDoWhileKeyword,openPeren,theTestExpression,closePeren),rest) = formWhileStmt(scan)
        return WhileStmt(theTestExpression,theDoWhileKeyword,lineNumber,rest=rest)

    def __init__(self,testExpression,stmt_name=kw_str,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.testExpression = testExpression
        self.stmt_name = stmt_name 
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self):
        return 'do while (%s)' % str(self.testExpression)


class ContinueStmt(Leaf):
    kw = 'continue'
    kw_str = kw


class SelectTypeStmt(Exec):
    #FIXME: optional case construct name 
    '''
    [type-construct-name :] select type (type-expression)
    '''
    _sons = ['typeExpression']
    kw = 'selecttype'
    kw_str = 'select type'

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        formTypeSelector = disj(seq(id,lit('=>'),id),Exp)
        formSelectTypeStmt = seq(lit(SelectTypeStmt.kw),
                                 lit('('),
                                 formTypeSelector,
                                 lit(')'))
        try:
            ((selectTypeKeyword,openParen,typeExpression,closeParen),rest) = formSelectTypeStmt(scan)
        except ListAssemblerException,e:
            raise ParseError(lineNumber,scan,'Select Type statement')
        return SelectTypeStmt(typeExpression,selectTypeKeyword,lineNumber,rest=rest)

    def __init__(self,typeExpression,stmt_name=kw_str,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.typeExpression = typeExpression
        self.stmt_name = stmt_name
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self):
        return '%s (%s)' % (self.stmt_name,str(self.typeExpression))

class TypeIsStmt(Exec):
    #FIXME: optional case construct name 
    '''
    type is (type-name)
    '''
    _sons = []
    kw = 'typeis'
    kw_str = 'type is'

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        formTypeIsStmt = seq(lit(TypeIsStmt.kw),
                             lit('('),
                             id,
                             lit(')'))
        try:
            ((typeIsKeyword,lparen,type_name,rparen),rest) = formTypeIsStmt(scan)
        except ListAssemblerException,e:
            raise ParseError(lineNumber,scan,'case default statement')
        return TypeIsStmt(type_name,lineNumber,rest=rest)

    def __init__(self,type_name,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.type_name=type_name
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self):
        return 'type is ('+self.type_name+')'

    def __repr__(self):
        return 'TypeIsStmt(%s)' % (repr(self.type_name),)

class ClassIsStmt(Exec):
    #FIXME: optional case construct name 
    '''
    class is (class-name)
    '''
    _sons = []
    kw = 'classis'
    kw_str = 'class is'

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        formClassIsStmt = seq(lit(ClassIsStmt.kw),
                             lit('('),
                             id,
                             lit(')'))
        try:
            ((classIsKeyword,lparen,class_name,rparen),rest) = formClassIsStmt(scan)
        except ListAssemblerException,e:
            raise ParseError(lineNumber,scan,'case default statement')
        return ClassIsStmt(class_name,lineNumber,rest=rest)

    def __init__(self,class_name,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.class_name=class_name
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self):
        return 'class is ('+self.class_name+')'

    def __repr__(self):
        return 'ClassIsStmt(%s)' % (repr(self.class_name),)

class SelectCaseStmt(Exec):
    #FIXME: optional case construct name 
    '''
    [case-construct-name :] select case (case-expression)
    '''
    _sons = ['caseExpression']
    kw = 'selectcase'
    kw_str = 'select case'

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        formSelectCaseStmt = seq(lit(SelectCaseStmt.kw),
                                 lit('('),
                                 Exp,
                                 lit(')'))
        try:
            ((selectCaseKeyword,openPeren,caseExpression,closePeren),rest) = formSelectCaseStmt(scan)
        except ListAssemblerException,e:
            raise ParseError(lineNumber,scan,'Select Case statement')
        return SelectCaseStmt(caseExpression,selectCaseKeyword,lineNumber,rest=rest)

    def __init__(self,caseExpression,stmt_name=kw_str,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.caseExpression = caseExpression
        self.stmt_name = stmt_name
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self):
        return '%s (%s)' % (self.stmt_name,str(self.caseExpression))

class EndSelectCaseStmt(Leaf):
    #FIXME: optional case construct name 
    '''
    end select [case-construct-name]
    '''
    kw = 'endselect'
    kw_str = 'end select'

class CaseDefaultStmt(Exec):
    #FIXME: optional case construct name 
    '''
    case default [case-construct-name]
    '''
    _sons = []
    kw = 'casedefault'
    kw_str = 'case default'

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        formCaseDefaultStmt = seq(lit(CaseDefaultStmt.kw))
        try:
            ((caseDefaultKeyword),rest) = formCaseDefaultStmt(scan)
        except ListAssemblerException,e:
            raise ParseError(lineNumber,scan,'case default statement')
        return CaseDefaultStmt(lineNumber,rest=rest)

    def __init__(self,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self):
        return 'case default'

class CaseRangeListStmt(Exec):
    #FIXME: optional case construct name 
    '''
    case (case-value-range-list) [case-construct-name]
    '''
    _sons = ['caseRangeList']
    kw = 'case'
    kw_str = kw

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        formRangeItem = disj(lower_bound_range,
                             upper_bound_range,
                             Exp)
        formCaseRangeListStmt = seq(lit(CaseRangeListStmt.kw),
                                    lit('('),
                                    cslist(formRangeItem),
                                    lit(')'))
        try:
            ((caseKeyword,openPeren,caseRangeList,closePeren),rest) = formCaseRangeListStmt(scan)
        except ListAssemblerException,e:
            raise ParseError(lineNumber,scan,'case range list statement')
        return CaseRangeListStmt(caseRangeList,caseKeyword,lineNumber,rest=rest)

    def __init__(self,caseRangeList,stmt_name=kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.caseRangeList = caseRangeList
        self.stmt_name = stmt_name
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self):
        return '%s (%s)' % (self.stmt_name,
                            ','.join([str(range) for range in self.caseRangeList]))

class GotoStmt(Exec):
    kw = 'goto'
    kw_str = 'go to'

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        theStmt=None
        try : 
            simplePatn=seq(lit(GotoStmt.kw),pred(is_int))
            ((thekw,theLabel),rest)=simplePatn(scan)
            theStmt=SimpleGotoStmt(theLabel,lineNumber,rest=rest)
        except :
            try: 
                computedPatn=seq(lit(GotoStmt.kw),lit('('),cslist(pred(is_int)),lit(')'),zo1(lit(',')),Exp)
                ((kwS,pl,labelList,pr,optComma,theExpr),rest)=computedPatn(scan)
                theStmt=ComputedGotoStmt(labelList,theExpr,lineNumber,rest=rest)
            except :
                assignedPatn=seq(lit(GotoStmt.kw),id,zo1(seq(zo1(lit(',')),lit('('),cslist(pred(is_int)),lit(')'))))
                assignedExtract=treat(assignedPatn,lambda l:(l[1],
                                                             len(l[2]) # have the optional label list 
                                                             and 
                                                             l[2][0][2] # extract it
                                                             or [])) # without the optional label list
                ((var,labelList),rst)=assignedExtract(scan)
                theStmt=AssignedGotoStmt(var,labelList,lineNumber,rest=rst)
        return theStmt
    
class SimpleGotoStmt(GotoStmt):
    _sons = ['targetLabel']

    def __init__(self,targetLabel,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.targetLabel = targetLabel
        Exec.__init__(self,lineNumber,label,lead,internal,rest)
        
    def __str__(self):
        return self.__class__.kw_str+' '+self.targetLabel

class AssignedGotoStmt(GotoStmt):
    _sons = ['var','labelList']
    
    def __init__(self,var,labelList,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.labelList = labelList
        self.var = var
        Exec.__init__(self,lineNumber,label,lead,internal,rest)
        
    def __str__(self):
        rStr=self.__class__.kw_str+' '+self.var
        if self.labelList:
            rStr+=' ('+','.join(self.labelList)+')'
        return rStr
                   
class ComputedGotoStmt(GotoStmt):
    _sons = ['labelList','expr']

    def __init__(self,labelList,expr,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.labelList = labelList
        self.expr = expr
        Exec.__init__(self,lineNumber,label,lead,internal,rest)
        
    def __str__(self):
        return self.__class__.kw_str+' ('+','.join(l for l in self.labelList)+') '+str(self.expr)

class AllocateStmt(Exec):
    '''
    For the time being, we do not require proper parsing of allocate and deallocate statements.
    We have commented out this (incomplete) implementation of the parsing, because it covers many but not all of the cases.
    '''
    kw = 'allocate'
    kw_str = kw
    _sons = ['argList','statVariable','statFormatStr','allocateFormatStr']

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        formAllocateStmt = seq(lit(AllocateStmt.kw), # 0 = allocateFormatStr
                               lit('('),             # 1
                               cslist(Exp),          # 2 = argList
                               lit(')'))
        formAllocateStmt = treat(formAllocateStmt, lambda x: AllocateStmt(x[2],allocateFormatStr=x[0],lineNumber=lineNumber))
        if scan[-4].lower() == 'stat':
            newScan = scan[0:-5]
            newScan.append(')')
            (theParsedStmt,rest) = formAllocateStmt(newScan)
            theParsedStmt.statFormatStr = scan[-4]
            theParsedStmt.statVariable = scan[-2]
            theParsedStmt.rest = rest
            return theParsedStmt
        else:
            (theParsedStmt,rest) = formAllocateStmt(scan)
            theParsedStmt.rest = rest
            return theParsedStmt

    def __init__(self,argList,statVariable=None,statFormatStr='stat',allocateFormatStr=kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.argList = argList
        self.statVariable = statVariable
        self.statFormatStr = statFormatStr
        self.allocateFormatStr = allocateFormatStr
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self):
        statVarStr = self.statVariable and ',stat='+self.statVariable \
                                   or ''
        return '%s(%s%s)' % (self.allocateFormatStr,
                             ','.join([str(arg) for arg in self.argList]),
                             statVarStr)

class DeallocateStmt(Exec):
    kw = 'deallocate'
    kw_str = kw
    _sons = ['argList','deallocateFormatStr']

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        formDeallocateStmt = seq(lit(DeallocateStmt.kw),
                                 lit('('),
                                 cslist(Exp),
                                 lit(')'))
        ((deallocKeyword,oParen,argList,cParen),rest) = formDeallocateStmt(scan)
        return DeallocateStmt(argList,deallocKeyword,lineNumber,rest=rest)

    def __init__(self,argList,deallocateFormatStr=kw,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.argList = argList
        self.deallocateFormatStr = deallocateFormatStr
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self):
        return '%s(%s)' % (self.kw,','.join([str(arg) for arg in self.argList]))

class NullifyStmt(Exec):
    kw = 'nullify'
    kw_str = kw
    _sons = ['ptrObjList']

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        formNullifyStmt = seq(lit(NullifyStmt.kw),
                              lit('('),
                              cslist(Exp),
                              lit(')'))
        ((stmt_name,lparen,ptrObjList,rparen),rst) = formNullifyStmt(scan)
        return NullifyStmt(ptrObjList,lineNumber,rest=rst)

    def __init__(self,ptrObjList,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.ptrObjList = ptrObjList
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self):
        return '%s (%s)' % (self.kw,','.join(str(item) for item in self.ptrObjList))
        

class BackspaceStmt(Exec):
    kw = 'backspace'
    kw_str = kw
    _sons = ['unitspec','params']

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        unitSpec = seq(zo1(seq(lit('unit'),lit('='))),
                       int)
        formBackspaceStmt = seq(lit(BackspaceStmt.kw),
                                lit('('),
                                unitSpec,
                                zo1(seq(lit(','),
                                        cslist(disj(seq(lit('err'),lit('='),Exp),
                                                    seq(lit('iostat'),lit('='),Exp))))),
                                lit(')'))
        
        ((stmt_name,lparen,unitspec,params,rparen),rst) = formBackspaceStmt(scan)
        return BackspaceStmt(unitspec,params,lineNumber,rest=rst)

    def __init__(self,unitspec,params=[],lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.unitspec = unitspec
        if len(params) > 0:
            self.params = params[0][1]
        else: self.params = []
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self):
        if isinstance(self.unitspec,list):
            unitspec = ''.join(self.unitspec)
        else:
            unitspec = self.unitspec
        if self.params == []:
            return '%s (%s)' % (self.kw,unitspec)            
        paramList = map(lambda param: ''.join(str(elt) for elt in param),self.params)
        return '%s (%s)' % (self.kw,unitspec+','+','.join(paramlist))

class DeletedAssignStmt(Exec):
    ''' 
    old style ASSIGN statement which was deleted in the F95 standard
    we parse it so we complain about it 
    '''
    kw = 'assign'
    kw_str = kw
    to = 'to'
    _sons = ['assignedLabel','var','kwString','toString']

    @staticmethod
    def parse(ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        patn = seq(lit(DeletedAssignStmt.kw),pred(is_int),lit('to'),id)
        ((kwString,assignedLabel,toString,var),rst)=patn(scan)
        return DeletedAssignStmt(assignedLabel,var,kwString,toString,lineNumber,rest=rst)

    def __init__(self,assignedLabel,var,kwString=kw,toString=to,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.assignedLabel=assignedLabel
        self.var=var
        self.kwString=kwString
        self.toString=toString
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __repr__(self):
        return self.__class__.__name__+'('+self.assignedLabel+','+self.var+')'

    def __str__(self):
        return '%s %s %s %s' % (self.kwString,self.assignedLabel,self.toString,self.var)

class BuiltinExec(Exec):
    _sons = ['paramsDict']

    @classmethod
    def parse(cls,ws_scan,lineNumber):
        scan = filter(lambda x: x != ' ',ws_scan)
        theParams=dict((i,None) for i in cls.paramNames)
        theParsedStmt=None
        try:
            # without parenthesis
            formStmt = seq(lit(cls.kw),disj(id,pred(is_int)))
            ((builtinKeyword,unitSpec),rest) = formStmt(scan)
            theParams[cls.paramNames[0]]=unitSpec
            theParsedStmt=cls(theParams,lineNumber)
        except:
            # with parenthesis
            aNamedParam=treat(namedParamPatn,lambda p: NamedParam(p[0].lower(),p[2]))
            formStmt = seq(lit(cls.kw),lit('('),cslist(disj(aNamedParam,Exp)),lit(')'))
            ((stmt_name,lparen,params,rparen),rest)=formStmt(scan)
            if isinstance(params,list):
                for pos,p in enumerate(params):
                    if isinstance(p,NamedParam):
                        theParams[p.myId]=p
                    else:
                        theParams[cls.paramNames[pos]]=p
            else:
                theParams[cls.paramNames[0]]=params
            theParsedStmt=cls(theParams,lineNumber)
        theParsedStmt.rest = rest
        return theParsedStmt
        
    def __init__(self,aParamsDict,lineNumber=0,label=False,lead='',internal=[],rest=[]):
        self.paramsDict = aParamsDict
        Exec.__init__(self,lineNumber,label,lead,internal,rest)

    def __str__(self):
        rstr=self.kw+"("
        pCount=0
        for p in self.__class__.paramNames:
            if (p in self.paramsDict.keys() and self.paramsDict[p]) :
                if (pCount):
                    rstr+=","
                pCount+=1
                rstr+=str(self.paramsDict[p])
        rstr+=')'
        return rstr

    def __repr__(self):
        reprStr=self.__class__.__name__
        params = []
        for p in self.__class__.paramNames:
            if (p in self.paramsDict.keys() and self.paramsDict[p]):
                params.append(repr(self.paramsDict[p]))
        return self.__class__.__name__+'(%s)' % ','.join(params)

class RewindStmt(BuiltinExec):
    kw = 'rewind'
    kw_str = kw
    paramNames=['unit','err','iostat']


class OpenStmt(BuiltinExec):
    kw = 'open'
    kw_str = kw
    paramNames=['unit',
                'access',
                'action',
                'blank',
                'delim',
                'err',
                'file',
                'form',
                'iostat',
                'pad',
                'position',
                'recl',
                'status']

class CloseStmt(BuiltinExec):
    kw = 'close'
    kw_str = kw
    paramNames=['unit',
                'iostat',
                'err',
                'status']

class InquireStmt(BuiltinExec):
    kw = 'inquire'
    kw_str = kw
    paramNames=['unit',
                'access',
                'action',
                'blank',
                'delim',
                'direct',
                'err',
                'exist',
                'file',
                'form',
                'formatted',
                'iostat',
                'name',
                'named',
                'nextrec',
                'number',
                'opened',
                'pad',
                'postion',
                'read',
                'readwrite',
                'recl',
                'sequential',
                'unformatted',
                'write']

kwBuiltInTypesTbl= dict(
    character       = CharacterStmt,
    complex         = ComplexStmt,
    doublecomplex   = DoubleCplexStmt,
    doubleprecision = DoubleStmt,
    integer         = IntegerStmt,
    logical         = LogicalStmt,
    real            = RealStmt
    )

kwtbl = dict(assign          = DeletedAssignStmt,
             blockdata       = BlockdataStmt,
             common          = CommonStmt,
             data            = DataStmt,
             implicit        = ImplicitStmt,
             equivalence     = EquivalenceStmt,
             parameter       = ParameterStmt,
             private         = PrivateStmt,
             public          = PublicStmt,
             sequence        = SequenceStmt,
             type            = TypePseudoStmt,
             interface       = InterfaceStmt,
             contains        = ContainsStmt,
             save            = SaveStmt,
             goto            = GotoStmt,
             external        = ExternalStmt,
             allocatable     = AllocatableStmt,
             intrinsic       = IntrinsicStmt,
             include         = IncludeLine,
             dimension       = DimensionStmt,
             subroutine      = SubroutineStmt,
             program         = ProgramStmt,
             function        = FunctionStmt,
             procedure       = ProcedureStmt,
             module          = ModuleStmt,
             format          = FormatStmt,
             entry           = EntryStmt,
             call            = CallStmt,
             open            = OpenStmt,
             close           = CloseStmt,
             read            = ReadStmt,
             write           = WriteStmt,
             stop            = StopStmt,
             elseif          = ElseifStmt,
             endif           = EndifStmt,
             end             = EndStmt,
             endinterface    = EndInterfaceStmt,
             endtype         = EndDrvdTypeDefn,
             endmodule       = EndModuleStmt,
             endprogram      = EndProgramStmt,
             endfunction     = EndFunctionStmt,
             endsubroutine   = EndSubroutineStmt,
             endblockdata    = EndStmt,
             endwhere        = EndWhereStmt,
             do              = DoStmt,
             enddo           = EnddoStmt,
             dowhile         = WhileStmt,
             selectcase      = SelectCaseStmt,
             endselect       = EndSelectCaseStmt,
             casedefault     = CaseDefaultStmt,
             case            = CaseRangeListStmt,
             selecttype      = SelectTypeStmt,
             typeis          = TypeIsStmt,
             classis          = ClassIsStmt,
             intent          = IntentStmt,
             optional        = OptionalStmt,
             allocate        = AllocateStmt,
             deallocate      = DeallocateStmt,
             inquire         = InquireStmt,
             nullify         = NullifyStmt,
             backspace       = BackspaceStmt,
             namelist        = NamelistStmt,
             )

kwtbl.update(kwBuiltInTypesTbl)

for kw in ('if','continue','return','else','print','use','cycle','exit','rewind','where','elsewhere','format','pointer','target','class'):
    kwtbl[kw] = globals()[kw.capitalize() + 'Stmt']
    
lhs    = disj(app,id)
assign = seq(lhs,lit('='),Exp)


import kw_multi

def sqz(n,mutable):
    'return the squeezed kw, and as a side effect, change the scan'

    rv = ''.join([l.lower() for l in mutable[0][0:n]])
    mutable[0][0:n] = [rv]
    return rv

def parse(ws_scan,lineNumber):
    scan = filter(lambda x: x != ' ',ws_scan)
    try:
        parsed = AssignStmt.parse(scan,lineNumber)
        parsed.rawline = ''.join(ws_scan).strip()
        return parsed
    except AssemblerException:
        lscan = tuple([ l.lower() for l in scan ])
        kw3g  = kw_multi.kw3.get
        kw2g  = kw_multi.kw2.get
        kw = len(scan) >=3 and kw3g(lscan[0:3]) and sqz(3,[scan]) or \
             len(scan) >=2 and kw2g(lscan[0:2]) and sqz(2,[scan]) or \
             lscan[0]
        if (kw in kwBuiltInTypesTbl.keys() or kw == 'type') and FunctionStmt.kw in lscan:
            kw = FunctionStmt.kw
        if (kw in FuncOrSubStmt.ourQualifiers):
            if FunctionStmt.kw in lscan:
                kw = FunctionStmt.kw
            if SubroutineStmt.kw in lscan:
                kw = SubroutineStmt.kw
        # special case for module procedure statements:
        elif (kw == ProcedureStmt.optKwPrefix) and (lscan[1] == ProcedureStmt.kw) :
            kw = ProcedureStmt.kw

        if not kwtbl.get(kw):
            if '=>' in scan:
                try:
                    parsed = PointerAssignStmt.parse(scan,lineNumber)
                    parsed.rawline = ''.join(ws_scan).strip()
                    return parsed
                except ListAssemblerException,e:
                    raise ParseError(lineNumber,scan,PointerAssignStmt,'l_assembler error:'+e.msg+' remainder:'+str(e.rest))
            elif (x.lower()=='do' for x in scan) and (':' in scan):
                try:
                    parsed = DoStmt.parse(scan,lineNumber)
                    parsed.rawline = ''.join(ws_scan).strip()
                    return parsed
                except ListAssemblerException,e:
                    raise ParseError(lineNumber,scan,DoStmt,'l_assembler error:'+e.msg+' remainder:'+str(e.rest))
            else:
                raise ParseError(lineNumber,scan,None)
        else:
            try:
                parsed = kwtbl.get(kw).parse(ws_scan,lineNumber)
                parsed.rawline = ''.join(ws_scan).strip()
                return parsed
            except Exception:
                try:
                    parsed = kwtbl.get(kw).parse(scan,lineNumber)
                    parsed.rawline = ''.join(ws_scan).strip()
                    return parsed
                except ListAssemblerException,e:
                    raise ParseError(lineNumber,scan,kwtbl.get(kw),'l_assembler error:'+e.msg+' remainder:'+str(e.rest))

#
# alias so that stmts like if, etc can call the above routine
#
_kw_parse = parse

'''
if __name__ == '__main__':
    from _Setup.testit import *
'''

def getVarName(anExpression,lineNumber,lhs=True):
    if isinstance(anExpression,str):
        DebugManager.debug(' it\'s an IDENTIFIER OR CONSTANT')
        return anExpression.lower()
    elif isinstance(anExpression,Unary):
        DebugManager.debug(' it\'s a UNARY EXPRESSION')
        return getVarName(anExpression.exp,lineNumber)
    elif isinstance(anExpression,Ops):
        DebugManager.debug(' it\'s a BINARY EXPRESSION')        
        if lhs:
            return getVarName(anExpression.a1,lineNumber)
        else:
            return getVarName(anExpression.a2,lineNumber)
    elif isinstance(anExpression,App):
        DebugManager.debug(' it\'s an APPLICATION')
        return getVarName(anExpression.head,lineNumber)
    elif isinstance(anExpression,NamedParam):
        DebugManager.debug(' it\'s a NAMED PARAMETER')
        if lhs:
            return getVarName(anExpression.myId,lineNumber)
        else:
            return getVarName(anExpression.myRHS,lineNumber)
    elif isinstance(anExpression,Sel):
       DebugManager.debug(' it\'s a SELECTION EXPRESSION')
       return getVarName(anExpression.head,lineNumber)
    elif isinstance(anExpression,Slice) :
       DebugManager.debug(' it\'s a SLICE EXPRESSION')
       return getVarName(anExpression.arg,lineNumber)
    elif isinstance(anExpression,_DimensionArraySpec):
        DebugManager.debug(' it\'s a DIMENSION ARRAY SPEC EXPRESSION')
        return anExpression.arrayName
    elif isinstance(anExpression,_NoInit):
       DebugManager.debug(' it\'s a NO INIT EXPRESSION')
       return getVarName(anExpression.lhs,lineNumber)
    else:
        raise ParseError('Encountered unrecognized type while attempting to get variable name for '+str(anExpression),lineNumber)
