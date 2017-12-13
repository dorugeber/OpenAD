#!/usr/bin/env python

from Setup     import *
from unittest  import *

from PyFort.fortExp import LoopControl,Sel
from PyFort.fortStmts import *
from PyFort.fortStmts import _F90Len,_F90ExplLen,_Star,_NoInit,_Kind,_Prec,_ExplKind,_AssignInit,_PointerInit,_ImplicitDoConstruct,_DimensionArraySpec
from useparse  import *

class C2(TestCase):
    '''assignment statement'''

    def test1(self):
        'simple assignment statement'

        ae = self.assertEquals
        a_ = self.assert_

        s  = 'foo(5,7) = x.lt.y'
        ae(repr(pps(s)),
           repr(AssignStmt(App('foo',['5','7']),Ops('.lt.','x','y')))
           )

    def test2(self):
        'assignment w kw "if" as var'
        ae = self.assertEquals
        a_ = self.assert_

        s  = 'if(ix,iy) = sin(x)'
        ae(repr(pps(s)),
           repr(AssignStmt(App('if',['ix','iy']),App('sin',['x'])))
           )

    def test3(self):
        "selection(%) operator works on lhs"
        ae = self.assertEquals
        a_ = self.assert_

        s  = 'x%v = 5'
        ae(repr(pps(s)),
           repr(AssignStmt(Sel('x','v'),'5')))

    def test4(self):
        "complicated exp including selection(%) works on lhs"
        ae = self.assertEquals
        a_ = self.assert_

        s  = 'x%v(1,2)%q(xx)%r = 5'
        ae(repr(pps(s)),
           repr(AssignStmt(Sel(App(Sel(App(Sel('x','v'),['1', '2']),'q'),
                                   ['xx']),'r'),'5')))

    def test5(self):
        '''Assignment statement with multiple RHS values (for complex, array, user-defined type, etc.)'''
        theString = 'GAMMAL = (0.0D00,0.0D00)'
        theRepr = AssignStmt('GAMMAL',MultiParenExp(['0.0D00', '0.0D00']))
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

class C3(TestCase):
    def test1(self):
        'create comment block'
        ae = self.assertEquals
        a_ = self.assert_
        c1 = comment_bl('This','is','a','comment')
        c2 = Comments('\n'.join(['c This','c is','c a','c comment'])+'\n')
        ae(c1.viz(),c2.viz())

    def test2(self):
        'test flow to free format on comments in fixed format'
        ae = self.assertEquals
        c1 = Comments('C     comment 1 is free format\nC     comment 2 is long long long. it must be more than 72 characters in\nC     order to cause line break for fixed format comment\n')
        c2 = Comments('!     comment 1 is free format\n!     comment 2 is long long long. it must be more than 72 characters in\n!     order to cause line break for fixed format comment\n')
        flow.setInputFormat('free')
        flow.setOutputFormat('free')
        ae(c1.flow(),c2.rawline)

    def test3(self):
        'test flow to fixed format on comments in free format'
        ae = self.assertEquals
        c1 = Comments('!     comment 1 is free format\n!     comment 2 is long long long. it must be more than 72 characters in order to cause line break for fixed format comment\n')
        c2 = Comments('C     comment 1 is free format\nC     comment 2 is long long long. it must be more than 72 characters in \nC order to cause line break for fixed format comment\n')
        flow.setInputFormat('fixed')
        flow.setOutputFormat('fixed')
        ae(c1.flow(),c2.rawline)

class C4(TestCase):
    def test1(self):
        'assignment statement classes have "_sons" attribute'
        ae = self.assertEquals
        a_ = self.assert_

        v = pps('if(ix,iy) = sin(x)')
        a_(hasattr(v,'_sons'))
        ae(v._sons,['lhs','rhs'])

class C5(TestCase):
    '''Test derived type
    '''
    def test1(self):
        'simple derived type declaration'
        ae = self.assertEquals
        a_ = self.assert_
        s = 'type(foo) :: x(10),y(len(kk),3),z,w'
        ps = pps(s)
        a_(isinstance(ps,fortStmts.DrvdTypeDecl))
        a_(not ps.attrs)
        ae(len(ps.decls),4)

    def test2(self):
        'simple derived type definition'
        ae = self.assertEquals
        a_ = self.assert_
        s = 'type bar'
        ps = pps(s)
        a_(isinstance(ps,fortStmts.DrvdTypeDefn))
        ae(ps.name,'bar')

    def test3(self):
        'derived type declaration with attributes'
        ae = self.assertEquals
        a_ = self.assert_
        s = 'type(foo),dimension(10),target :: x,y,z(30)'
        ps = pps(s)
        a_(isinstance(ps,fortStmts.DrvdTypeDecl),'instance check')
        ae(len(ps.attrs),2,'attr check')
        ae(len(ps.decls),3,'decls check')

    def test4(self):
        '''derived type declaration with empty array bounds'''
        s = 'type(OpenADTy_active) :: X(:)'
        r = DrvdTypeDecl(['OpenADTy_active'],[],[_NoInit(App('X',[':']))])
        self.assertEquals(repr(pps(s)),repr(r))
        self.assertEquals(s,str(r))

class C6(TestCase):
    '''F90 style types
    '''

    def test2(self):
        'double precision with attributes'
        ae = self.assertEquals
        a_ = self.assert_
        s = 'double precision,pointer,intent(inout) :: y'
        ps = pps(s)
        a_(isinstance(ps,fortStmts.DoubleStmt))
        a_(ps.attrs)
        ae(len(ps.attrs),2)
        ae(str(ps.attrs[1]),str(App('intent',['inout'])))

    def test4(self):
        'string value of double stmt w attributes'
        ae = self.assertEquals
        a_ = self.assert_
        vv = DoubleStmt([],[App('intent',['in']),'allocatable'],['x','y',])
        ae(str(vv),'double precision,intent(in),allocatable :: x,y')

class C8(TestCase):
    'Arrays with attributes and asterisk dimension specifications'
    def test4(self):
        '''double precision w attributes, and '*' dimensions
        '''
        ae = self.assertEquals
        a_ = self.assert_

        ss = pps('double precision,dimension(5) :: x,y(*,*,3),z')
        ae(repr(ss),
           repr(DoubleStmt([],[App('dimension',['5'])],
                           [_NoInit('x'),
                            _NoInit(App('y',['*', '*', '3'])),
                            _NoInit('z')])))
    def test5(self):
        '''star ranges (gawd)'''
        ae = self.assertEquals
        a_ = self.assert_

        ss = pps('double precision,dimension(5) :: x,y(*,2*x:*,3),z')
        ae(repr(ss),
           repr(DoubleStmt([],[App('dimension',['5'])],
                           [_NoInit('x'),
                            _NoInit(App('y',['*', Ops(':',Ops('*','2','x'),'*'), '3'])),
                            _NoInit('z')])))

class C9(TestCase):
    'end interface block stuff'
    def test1(self):
        '''end interface block'''
        ae = self.assertEquals
        a_ = self.assert_
        it = pps('end interface')
        a_(isinstance(it,fortStmts.EndInterfaceStmt))
        
    def test2(self):
        '''end other things (module, program, function, subroutine, block data)'''
        ae = self.assertEquals
        a_ = self.assert_
        chk = ['module','program','function','subroutine','block data']
        for l in chk:
            a_(isinstance(pps('end '+l),fortStmts.EndStmt))

class TestImplicitStmt(TestCase):
    '''Implicit statements'''
    def test1(self):
        'implicit character*10'
        s1 = 'implicit character*10 (b,d,f-h,l-n)'
        v = pps(s1)
        a_(isinstance(v,fortStmts.ImplicitStmt))
        ae(len(v.lst),1)
        ae(len(v.lst[0]),2)
        ae(len(v.lst[0][1]),4)
        l = v.lst[0][1]
        ae(str(l[0]).lower(),str(ep('b')).lower())
        ae(str(l[1]).lower(),str(ep('d')).lower())
        ae(str(l[2]).lower(),str(ep('f-h')).lower())
        ae(str(l[3]).lower(),str(ep('l-n')).lower())
        impl_typ = v.lst[0][0]
        proto    = pps('character*10 a')
        ae(impl_typ[0],proto.__class__)
        ae(repr(impl_typ[1]),repr(proto.mod))
        ae(kill_blanks(s1),kill_blanks(str(v)))

    def test2(self):
        'implicit character (no len)'
        s1 = 'implicit character (b,d,f-h,l-n)'
        v = pps(s1)
        a_(isinstance(v,fortStmts.ImplicitStmt))
        ae(len(v.lst),1)
        ae(len(v.lst[0]),2)
        ae(len(v.lst[0][1]),4)
        l = v.lst[0][1]
        ae(str(l[0]).lower(),str(ep('b')).lower())
        ae(str(l[1]).lower(),str(ep('d')).lower())
        ae(str(l[2]).lower(),str(ep('f-h')).lower())
        ae(str(l[3]).lower(),str(ep('l-n')).lower())
        impl_typ = v.lst[0][0]
        proto    = pps('character a')
        ae(impl_typ[0],proto.__class__)
        ae(repr(impl_typ[1]),repr(proto.mod))
        ae(kill_blanks(s1),kill_blanks(str(v)))

    def test3(self):
        'implicit real*4'
        s1 = 'implicit real*4 (a,f,h-k)'
        v = pps(s1)
        a_(isinstance(v,fortStmts.ImplicitStmt))
        ae(len(v.lst),1)
        ae(len(v.lst[0]),2)
        ae(len(v.lst[0][1]),3)
        l = v.lst[0][1]
        ae(str(l[0]).lower(),str(ep('a')).lower())
        ae(str(l[1]).lower(),str(ep('f')).lower())
        ae(str(l[2]).lower(),str(ep('h-k')).lower())
        impl_typ = v.lst[0][0]
        proto    = pps('real * 4 a')
        ae(impl_typ[0],proto.__class__)
        ae(repr(impl_typ[1]),repr(proto.mod))
        ae(kill_blanks(s1),kill_blanks(str(v)))

    def test4(self):
        'implicit integer (a,f)'
        s1 = 'implicit integer (a,f)'
        v = pps(s1)
        a_(isinstance(v,fortStmts.ImplicitStmt))
        ae(len(v.lst),1)
        ae(len(v.lst[0]),2)
        ae(len(v.lst[0][1]),2)
        l = v.lst[0][1]
        ae(str(l[0]).lower(),str(ep('a')).lower())
        ae(str(l[1]).lower(),str(ep('f')).lower())
        impl_typ = v.lst[0][0]
        proto    = pps('integer a')
        ae(impl_typ[0],proto.__class__)
        ae(repr(impl_typ[1]),repr(proto.mod))
        ae(kill_blanks(s1),kill_blanks(str(v)))

    def test6(self):
        'implicit real(RARB) (h,i,j)'
        s1 = 'implicit real(RARB) (h,i,j)'
        v = pps(s1)
        a_(isinstance(v,fortStmts.ImplicitStmt))
        ae(len(v.lst),1)

        ae(len(v.lst[0]),2)
        ae(len(v.lst[0][1]),3)
        l = v.lst[0][1]
        ae(str(l[0]).lower(),str(ep('h')).lower())
        ae(str(l[1]).lower(),str(ep('i')).lower())
        ae(str(l[2]).lower(),str(ep('j')).lower())
        impl_typ = v.lst[0][0]
        proto    = pps('real(RARB) a')
        ae(impl_typ[0],proto.__class__)
        ae(repr(impl_typ[1]),repr(proto.mod))

        ae(kill_blanks(s1),kill_blanks(str(v)))

    def test7(self):
        'implicit real(kind=FOOB) (b-d,f),real(RARB) (h,i,j)'
        s1 = 'implicit real(kind=FOOB) (b-d,f),real(RARB) (h,i,j)'
        v = pps(s1)
        a_(isinstance(v,fortStmts.ImplicitStmt))
        ae(len(v.lst),2)
        ae(len(v.lst[0]),2)
        ae(len(v.lst[0][1]),2)
        l = v.lst[0][1]
        ae(str(l[0]).lower(),str(ep('b-d')).lower())
        ae(str(l[1]).lower(),str(ep('f')).lower())
        impl_typ = v.lst[0][0]
        proto    = pps('real(kind=FOOB) a')
        ae(impl_typ[0],proto.__class__)
        ae(repr(impl_typ[1]),repr(proto.mod))

        ae(len(v.lst[1]),2)
        ae(len(v.lst[1][1]),3)
        l = v.lst[1][1]
        ae(str(l[0]).lower(),str(ep('h')).lower())
        ae(str(l[1]).lower(),str(ep('i')).lower())
        ae(str(l[2]).lower(),str(ep('j')).lower())
        impl_typ = v.lst[1][0]
        proto    = pps('real(RARB) a')
        ae(impl_typ[0],proto.__class__)
        ae(repr(impl_typ[1]),repr(proto.mod))

        ae(kill_blanks(s1),kill_blanks(str(v)))


class TestAssignStmt(TestCase):
    '''test assignment statements'''

    def test0(self):
        '''character array constructor assignment'''
        theString = "andchars = (/'&','&'/)"
        theRepr = AssignStmt('andchars',ArrayConstructor(["'&'", "'&'"]))
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test1(self):
        '''integer array constructor assignment'''
        theString = "a = (/2,3,5,7,11,13,17/)"
        theRepr = AssignStmt('a',ArrayConstructor(['2', '3', '5', '7', '11', '13', '17']))
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test2(self):
        '''array slice array constructor assignment (from SCALE)'''
        theString = "e_mid(1:ngrp) = (/((elnmg(i)+elnmg(i+1))*0.5,i=1,ngrp)/)"
        #theRepr = AssignStmt(App('e_mid',[Ops(':','1','ngrp')]),
        #                     ArrayConstructor([ArrayConstructorImpliedDo(Ops('*',ParenExp(Ops('+',App('elnmg',['i']),
        #                                                                                          App('elnmg',[Ops('+','i','1')]))),'0.5'),
        #                                                                 LoopControl('i','1','ngrp',None))]))
        theRepr = AssignStmt(App('e_mid',[Ops(':','1','ngrp')]),
                             ArrayConstructor([MultiParenExp(
                                 [Ops('*',ParenExp(Ops('+',App('elnmg',['i']),
                                                      App('elnmg',[Ops('+','i','1')]))),'0.5'),
                                 NamedParam('i','1'),'ngrp'])]))
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))


class TestCycleStmt(TestCase):
    '''test cycle statements'''

    def test0(self):
        '''cycle statement without do construct name'''
        theString = 'CYCLE'
        theRepr = CycleStmt(None,'CYCLE')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test1(self):
        '''cycle statement with do construct name'''
        theString = 'cycle l120'
        theRepr = CycleStmt('l120','cycle')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))


class TestEndStmt(TestCase):
    '''Test various end statements'''

    def test0(self):
        'endif stmt as "end if"'
        theString = 'end if'
        theRepr = EndifStmt()
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test1(self):
        'endif stmt as "endif"'
        theString = 'endif'
        theRepr = EndifStmt()
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
       # the following check will fail, because we don't store
       # the formatting of "endif" for this type of statement
       #self.assertEquals(theString,str(pps(theString)))

    def test2(self):
        'end do statement without do construct name'''
        theString = 'end do '
        theRepr = EnddoStmt(None,'end do')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test3(self):
        'end do statement with do construct name'''
        theString = 'end do  l120'
        theRepr = EnddoStmt('l120','end do')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))


class TestRealStmt(TestCase):
    'test declaration of real variables'

    def test0(self):
        '''real variable with an intent'''
        theString = 'real,INTENT(IN) :: Y'
        theRepr = RealStmt([],[App('INTENT',['IN'])],[_NoInit('Y')])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test1(self):
        '''real variable with * dimension and inout intent'''
        theString = 'real,DIMENSION(*),INTENT(INOUT) :: Y'
        theRepr = RealStmt([],[App('DIMENSION',['*']), App('INTENT',['INOUT'])],[_NoInit('Y')])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test2(self):
        'simple real stmt'
        s  = 'real  x(10),y,z'
        self.assertEquals(repr(pps(s)),repr(RealStmt([],[],[_NoInit(App('x',['10'])), _NoInit('y'), _NoInit('z')])))
        s  = 'real :: x(:)'
        self.assertEquals(repr(pps(s)),repr(RealStmt([],[],[_NoInit(App('x',[':']))])))
        s  = 'real :: x(1:)'
        self.assertEquals(repr(pps(s)),repr(RealStmt([],[],[_NoInit(App('x',[Ops(':','1','')]))])))

    def test3(self):
        '''real variable with * dimension and inout intent'''
        theString = 'real(DOUBLE),DIMENSION(NGP3,*) :: ELIN'
        theRepr = RealStmt([_Kind('DOUBLE')],[App('DIMENSION',['NGP3', '*'])],[_NoInit('ELIN')])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test4(self):
        'F90 style real statement with kind "3" and variable dimension'
        theString = 'real(3),dimension(aaa) :: x,y'
        theRepr = RealStmt([_Kind('3')],[App('dimension',['aaa'])],[_NoInit('x'), _NoInit('y')])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))
        self.assert_(pps(theString).attrs)
        self.assertEquals(len(pps(theString).decls),2)

    def test5(self):
        'F90 style real statement with intent and allocatable attribute'
        theString = 'real,intent(in),allocatable :: x,y'
        theRepr = RealStmt([],[App('intent',['in']), 'allocatable'],[_NoInit('x'), _NoInit('y')])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test6(self):
        'F77 style real statement with and variable dimension'
        flow.setInputFormat('fixed')
        theString = '      Real*8     array_p( 1-myOLw:sNx+myOLe,1-myOLs:sNy+myOLn,myNz, nSx, nSy )'
        theRepr = RealStmt([_Prec('8')],[],[_NoInit(App('array_p',[Ops(':',Ops('-','1','myOLw'),Ops('+','sNx','myOLe')),
                                                                   Ops(':',Ops('-','1','myOLs'),Ops('+','sNy','myOLn')),
                                                                   'myNz', 'nSx', 'nSy']))])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(len(pps(theString).decls),1)
        flow.setInputFormat('free')

class TestCharacterDecls(TestCase):
    'test character declaration statements'
    def test1(self):
        '''f77 style character decls
        '''
        ae = self.assertEquals
        a_ = self.assert_

        v = pps('character*5  foo,bar,baz(10)')
        a_(isinstance(v,fortStmts.CharacterStmt))
        a_(v.mod)
        ae(v.mod[0].len,'5')
        a_(not v.attrs)
        a_(v.decls)
        vars = [repr(i.lhs) for i in v.decls]
        for vv in ['foo','bar']: a_(repr(vv) in vars)
        a_(repr(App('baz',['10'])) in vars)

    def test2(self):
        '''f77 style character decls, w *(*) len
        '''
        ae = self.assertEquals
        a_ = self.assert_

        v = pps('character*(*)  foo,bar,baz(10)')
        a_(isinstance(v,fortStmts.CharacterStmt))
        a_(isinstance(v.mod[0].len,_Star))
        a_(not v.attrs)
        a_(v.decls)
        vars = [repr(i.lhs) for i in v.decls]
        for vv in ['foo','bar']: a_(repr(vv) in vars)
        a_(repr(App('baz',['10'])) in vars)

    def test3(self):
        '''f90 style decls, w len modifiers & attribs
        '''
        ae = self.assertEquals
        a_ = self.assert_

        ss = 'character(len=AAA+9),dimension(3) :: gack(*,*)*5,floob,foo*2,bar(2)'
        v = pps(ss)
        #ae(repr(v).lower(),
        #   repr(CharacterStmt(
        #    [_F90ExplLen(Ops('+','aaa','9'))],
        #    [App('dimension',['3'])],
        #    [_NoInit(Ops('*',App('gack',['*', '*']),'5')),
        #     _NoInit('floob'),
        #     _NoInit(Ops('*','foo','2')),
        #     _NoInit(App('bar',['2']))])).lower())
        ae(repr(v).lower(),
           repr(CharacterStmt(
            [_F90Len(NamedParam('len',Ops('+','aaa','9')))],
            [App('dimension',['3'])],
            [_NoInit(Ops('*',App('gack',['*', '*']),'5')),
             _NoInit('floob'),
             _NoInit(Ops('*','foo','2')),
             _NoInit(App('bar',['2']))])).lower())

    def test4(self):
        '''character type declaration with asterisk length specification'''
        s = 'character :: temp*16'
        r = CharacterStmt([],[],[_NoInit(Ops('*','temp','16'))])
        self.assertEquals(repr(pps(s)),repr(r))
        self.assertEquals(s,str(r))

    def test5(self):
        '''character type declaration with asterisk length specification and optional attribute (from scalelib/opnfil_I.f90)'''
        theString = 'character(len=*),intent(in),optional :: act'
        theRepr = CharacterStmt([_F90ExplLen('*')],[App('intent',['in']), 'optional'],[_NoInit('act')])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test6(self):
        '''character type declaration with array constructor (from SCALE: scalelib/free_form_C.f90)'''
        theString = "character(len=1),dimension(0:27),parameter :: char_array = (/'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f',' ',',','r','*','$','&','+','-','z','.','o','p'/)"
        #theRepr = CharacterStmt([_F90ExplLen('1')],
        #                        [App('dimension',[Ops(':','0','27')]), 'parameter'],
        #                        [_AssignInit('char_array',
        #                                     ArrayConstructor(["'0'", "'1'", "'2'", "'3'", "'4'", "'5'", "'6'", "'7'", "'8'", "'9'", "'a'", "'b'", "'c'", "'d'", "'e'", "'f'", "' '", "','", "'r'", "'*'", "'$'", "'&'", "'+'", "'-'", "'z'", "'.'", "'o'", "'p'"]))])
        theRepr = CharacterStmt([_F90Len(NamedParam('len','1'))],
                                [App('dimension',[Ops(':','0','27')]), 'parameter'],
                                [_AssignInit('char_array',
                                             ArrayConstructor(["'0'", "'1'", "'2'", "'3'", "'4'", "'5'", "'6'", "'7'", "'8'", "'9'", "'a'", "'b'", "'c'", "'d'", "'e'", "'f'", "' '", "','", "'r'", "'*'", "'$'", "'&'", "'+'", "'-'", "'z'", "'.'", "'o'", "'p'"]))])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test7(self):
        '''character type declaration with array constructor'''
        theString = "character(len=1),dimension(2) :: andchars = (/'&','&'/)"
        #theRepr = CharacterStmt([_F90ExplLen('1')],
        #                        [App('dimension',['2'])],
        #                        [_AssignInit('andchars',ArrayConstructor(["'&'","'&'"]))])
        theRepr = CharacterStmt([_F90Len(NamedParam('len','1'))],
                                [App('dimension',['2'])],
                                [_AssignInit('andchars',ArrayConstructor(["'&'","'&'"]))])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test8(self):
        '''character type declaration with optional attribute'''
        theString = 'character(len=*),intent(in),optional :: act'
        theRepr = CharacterStmt([_F90ExplLen('*')],
                                [App('intent',
                                     ['in']),
                                 'optional'],
                                [_NoInit('act')])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test9(self):
        '''logical declaration with kind specifier'''
        theString = 'logical(kind = kind(.true.)) :: l2'
        theRepr = LogicalStmt([_ExplKind(App('kind',['.true.']))],[],[_NoInit('l2')])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test10(self):
        '''character type declaration with kind and optional and intent attribute'''
        theString = 'character(kind = kind("A"),len=*),intent(in),optional :: name'
        theRepr = CharacterStmt([_ExplKind(App('kind',['"A"'])),_F90ExplLen('*')],
                                [App('intent',
                                     ['in']),
                                 'optional'],
                                [_NoInit('name')])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

class TestDimensionStmt(TestCase):
    '''Dimension statement'''
    def test1(self):
        'dimension m(4,4), v(1000)'
        self.assertEquals(repr(pps('dimension m(4,4), v(1000)')),
                          repr(DimensionStmt([_DimensionArraySpec('m',['4', '4']),
                                              _DimensionArraySpec('v',['1000'])])))

    def test2(self):
        'dimension helio (-3:3, 4, 3:9)'
        self.assertEquals(repr(pps('dimension helio (-3:3, 4, 3:9)')),
                          repr(DimensionStmt([_DimensionArraySpec('helio',
                                                  [Ops(':',Umi('3'),'3'),
                                                   '4',
                                                   Ops(':','3','9')])])))

    def test3(self):
        'dimension a(hi, hi*3 + lo )'
        self.assertEquals(repr(pps('dimension a(hi, hi*3 + lo )')),
                          repr(DimensionStmt([_DimensionArraySpec('a',
                                                  ['hi',
                                                   Ops('+',Ops('*','hi','3'),'lo')])])))

    def test4(self):
        'dimension c(*),n(igm+ipm)'
        self.assertEquals(repr(pps('dimension c(*),n(igm+ipm)')),
                          repr(DimensionStmt([_DimensionArraySpec('c',['*']),
                                              _DimensionArraySpec('n',
                                                                  [Ops('+','igm','ipm')])])))

class TestDoStmt(TestCase):
    '''Do statements'''
    def test1(self):
        '''simplest example with loop control: "do i = 1,2"'''
        theString = 'do i = 1,2'
        theRepr = DoStmt(None,None,LoopControl('i','1','2',None))
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test2(self):
        '''simplest example with loop control and stride: "do i = 1,2,1"'''
        theString = 'do i = 1,2,1'
        theRepr = DoStmt(None,None,LoopControl('i','1','2','1'))
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test3(self):
        '''do i = floor(x),2'''
        theString = 'do i = floor(x),2'
        theRepr = DoStmt(None,None,LoopControl('i',App('floor',['x']),'2',None))
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test4(self):
        '''do i = floor(x)+10,2,abs(y+2)'''
        theString = 'do i = floor(x)+10,2,abs(y+2)'
        theRepr = DoStmt(None,
                         None,
                         LoopControl('i',
                                      Ops('+',App('floor',['x']),'10'),
                                      '2',
                                      App('abs',[Ops('+','y','2')])))
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test5(self):
        '''do statement with label: "do 40 i = 1,2"'''
        theString = 'do 40 i = 1,2'
        theRepr = DoStmt(None,'40',LoopControl('i','1','2',None))
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test6(self):
        '''do statement with label and stride: "do 40 i = 1,2,1"'''
        theString = 'do 40 i = 1,2,1'
        theRepr = DoStmt(None,'40',LoopControl('i','1','2','1'))
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test7(self):
        '''do statement with do name: "l1000: do ic = 1,icm"'''
        theString = 'l1000: do ic = 1,icm'
        theRepr = DoStmt('l1000',None,LoopControl('ic','1','icm',None))
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test8(self):
        '''do statement without optional loop control'''
        theString = 'do'
        theRepr = DoStmt(None,None,None)
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test9(self):
        '''do statement from ad_inline.f'''
        theString = 'do cp_loop_variable_1 = ubound(x,1),lbound(x,1),-1'
        theRepr = DoStmt(None,None,LoopControl('cp_loop_variable_1',App('ubound',['x', '1']),App('lbound',['x', '1']),Umi('1')))
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))


class TestIfStmt(TestCase):
    '''If statements have three types: ArithIfStmt, IfThenStmt, and IfNonThenStmt'''

    def test0(self):
        'if statement classes have "_sons" attribute'
        s  = 'if (x .eq. 5) goto 23'
        self.assert_(hasattr(pps(s),'_sons'))

    def test1(self):
        'IfNonThenStmt with a relatively complicated conditional'
        theString = 'if (x(11).EQ.y.OR.x(12).lt.(floob*(i**k))) go to 10'
        theRepr = IfNonThenStmt(Ops('.OR.',Ops('.EQ.',App('x',['11']),'y'),Ops('.lt.',App('x',['12']),ParenExp(Ops('*','floob',ParenExp(Ops('**','i','k')))))),SimpleGotoStmt('10'))
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test2(self):
        '''if (non-then) statement from scale, with rewind stmt'''
        theString = 'if (nt4>0) rewind(nt4)'
        theRepr = IfNonThenStmt(Ops('>','nt4','0'),RewindStmt({'unit':'nt4'}))
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test3(self):
        '''if (then) statement '''
        theString = 'if (x(11).EQ.y.OR.x(12).lt.(floob*(i**k))) then'
        theRepr = IfThenStmt(Ops('.OR.',Ops('.EQ.',App('x',['11']),'y'),Ops('.lt.',App('x',['12']),ParenExp(Ops('*','floob',ParenExp(Ops('**','i','k')))))))
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test4(self):
        '''arithmetic if statement '''
        theString = 'if (x(11)+3) 10,20,10'
        theRepr = ArithmIfStmt(Ops('+',App('x',['11']),'3'),('10','20','10'))
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))


class TestWhileStmt(TestCase):
    '''While statements'''
    def test1(self):
        '''do while (1.lt.3)'''
        s = 'do while (1.lt.3)'
        r = WhileStmt(Ops('.lt.','1','3'))
        self.assertEquals(repr(pps(s)),repr(r))
        self.assertEquals(str(pps(s)),str(r))

    def test2(self):
        '''do while (foo(x).and.p.eq.q)'''
        s = 'do while (foo(x).and.p.eq.q)'
        r = WhileStmt(Ops('.and.',
                          App('foo',['x']),
                          Ops('.eq.','p','q')))
        self.assertEquals(repr(pps(s)),repr(r))
        self.assertEquals(str(pps(s)),str(r))

    def test3(self):
        'do while statement from SCALE'
        theString = 'do while ( j /= ma(i02) )'
        theRepr = WhileStmt(Ops('/=','j',App('ma',['i02'])))
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))


class TestCallStmt(TestCase):
    '''Subroutine call statements'''
    def test1(self):
        '''Subroutine call with 1 plain and 1 named parameter argument'''
        s = 'call foo(1,b=bar(x))'
        r = CallStmt('foo',['1',
                            NamedParam('b',App('bar',['x']))])
        self.assertEquals(repr(pps(s)),repr(r))
        self.assertEquals(s,str(r))

    def test2(self):
        '''Subroutine call with both named parameter arguments'''
        s = 'call foo(a=1,b=bar(x))'
        r = CallStmt('foo',[NamedParam('a','1'),
                            NamedParam('b',App('bar',['x']))])
        self.assertEquals(repr(pps(s)),repr(r))
        self.assertEquals(s,str(r))


class TestFunctionStmt(TestCase):
    '''Function statements'''
    def test0(self):
        '''function statement with no arguments'''
        s = 'function foo()'
        r = FunctionStmt(None,'foo',[],None)
        self.assertEquals(repr(pps(s)),repr(r))
        self.assertEquals(s,str(r))

    def test1(self):
        '''function statement with no type'''
        s = 'function foo(x)'
        r = FunctionStmt(None,'foo',['x'],None)
        self.assertEquals(repr(pps(s)),repr(r))
        self.assertEquals(s,str(r))

    def test2(self):
        '''function statement with type real (no modifier)'''
        s = 'real function foo(x)'
        r = FunctionStmt((RealStmt,[]),'foo',['x'],None)
        self.assertEquals(repr(pps(s)),repr(r))
        self.assertEquals(s,str(r))

    def test3(self):
        '''function statement with type real (with modifier)'''
        s = 'real(kind = 16) function foo(x)'
        r = FunctionStmt((RealStmt,[_ExplKind('16')]),'foo',['x'],None)
        self.assertEquals(repr(pps(s)),repr(r))
        self.assertEquals(s,str(r))

    def test4(self):
        '''function statement with result'''
        s = 'function foo(x) result(y)'
        r = FunctionStmt(None,'foo',['x'],'y')
        self.assertEquals(repr(pps(s)),repr(r))
        self.assertEquals(s,str(r))
        
    def test5(self):
        '''function statement with type real (with modifier) and result specifier'''
        s = 'real(kind = 16) function foo(x) result(y)'
        r = FunctionStmt((RealStmt,[_ExplKind('16')]),'foo',['x'],'y')
        self.assertEquals(repr(pps(s)),repr(r))
        self.assertEquals(s,str(r))

    def test6(self):
        '''recursive function statement with type real (with modifier) and result specifier'''
        s = 'recursive real(kind = 16) function foo(x) result(y)'
        r = FunctionStmt((RealStmt,[_ExplKind('16')]),'foo',['x'],'y',['recursive'])
        self.assertEquals(repr(pps(s)),repr(r))
        self.assertEquals(s,str(r))

class TestSelectCaseStmt(TestCase):
    '''select case statements'''
    def test0(self):
        '''select case statement with space'''
        s = 'select case (i)'
        r = SelectCaseStmt('i')
        self.assertEquals(repr(pps(s)),repr(r))
        self.assertEquals(s,str(r))

    def test1(self):
        '''select case statement without space'''
        s = 'selectcase (i)'
        r = SelectCaseStmt('i')
        self.assertEquals(repr(pps(s)),repr(r))
#       self.assertEquals(s,str(r))

class TestCaseStmts(TestCase):
    '''case statements'''
    def test0(self):
        '''case default statement'''
        s = 'case default'
        r = CaseDefaultStmt()
        self.assertEquals(repr(pps(s)),repr(r))
        self.assertEquals(s,str(r))

    def test1(self):
        '''case with single range'''
        s = 'case (1:3)'
        r = CaseRangeListStmt([Ops(':','1','3')])
        self.assertEquals(repr(pps(s)),repr(r))
        self.assertEquals(s,str(r))

    def test2(self):
        '''case with multiple ranges'''
        s = 'case (1:3,8:9)'
        r = CaseRangeListStmt([Ops(':','1','3'), Ops(':','8','9')])
        self.assertEquals(repr(pps(s)),repr(r))
        self.assertEquals(s,str(r))

    def test3(self):
        'case statement with only lower bound'
        theString = 'case (1:)'
        theRepr = CaseRangeListStmt([Ops(':','1','')])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test4(self):
        'case statement with only upper bound'
        theString = 'case (:39)'
        theRepr = CaseRangeListStmt([Ops(':','','39')])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test5(self):
        'case statement with nontrivial expression as upper bound'
        theString = 'case (:3+9)'
        theRepr = CaseRangeListStmt([Ops(':','',Ops('+','3','9'))])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))


class TestSelectTypeStmt(TestCase):
    '''select case statements'''
    def test0(self):
        '''select type statement with space'''
        s = 'select type (foo)'
        r = SelectTypeStmt('foo')
        self.assertEquals(repr(pps(s)),repr(r))
        self.assertEquals(s,str(r))

    def test1(self):
        '''select type statement without space'''
        s = 'selecttype (foo)'
        r = SelectTypeStmt('foo')
        self.assertEquals(repr(pps(s)),repr(r))

class TestTypeIsStmt(TestCase):
    '''type is statements'''
    def test0(self):
        '''type is statement'''
        s = 'type is (foo)'
        r = TypeIsStmt('foo')
        self.assertEquals(repr(pps(s)),repr(r))
        self.assertEquals(s,str(r))

class TestClassIsStmt(TestCase):
    '''class is statements'''
    def test0(self):
        '''class is statement'''
        s = 'class is (foo)'
        r = ClassIsStmt('foo')
        self.assertEquals(repr(pps(s)),repr(r))
        self.assertEquals(s,str(r))

class TestInterfaces(TestCase):
    '''stuff to do with interfaces'''

    def test1(self):
        'empty interface definition'
        ae = self.assertEquals
        a_ = self.assert_
        s = 'interface'
        ps = pps(s)
        a_(isinstance(ps,InterfaceStmt),'instance check')
        ae(str(ps),s)

    def test2(self):
        'nonempty interface definition'
        ae = self.assertEquals
        a_ = self.assert_
        s = 'interface foo'
        ps = pps(s)
        a_(isinstance(ps,InterfaceStmt),'instance check')
        ae(str(ps),s)

class TestUseStmts(TestCase):
    '''various forms of use statements, including use only and renames'''

    def test1(self):
        'plain use statement'
        theString = 'uSe s_lib'
        theRepr = UseAllStmt('s_lib',None,None,'uSe')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test2(self):
        'use statement with renames'
        theString = 'USE S_LIB, pressure => x_pres,altiude => x_alt'
        theRepr = UseAllStmt('S_LIB',
                             [_PointerInit('pressure','x_pres'), _PointerInit('altiude','x_alt')],
                             None,
                             'USE')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test3(self):
        'use statement with only list'
        theString = 'use data_C, only: xn,xj,nthr,eb_grp,art'
        theRepr = UseOnlyStmt('data_C',['xn', 'xj', 'nthr', 'eb_grp', 'art'],
                              None,
                              'use')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test4(self):
        'use statement with only list including renames'
        theString = 'use months, only: january => jan,may,june => jun'
        theRepr = UseOnlyStmt('months',
                              [_PointerInit('january','jan'), 'may', _PointerInit('june','jun')],
                              None,
                              'use')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

class TestPointerAssignStmt(TestCase):
    '''pointer assignment statements'''

    def test0(self):
        '''simple pointer assignment'''
        theString = 'a => b'
        theRepr = PointerAssignStmt('a','b')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test1(self):
        '''more complicated pointer assignment'''
        theString = 'xjtn => xj(:,:,iftg:)'
        theRepr = PointerAssignStmt('xjtn',App('xj',[Zslice(), Zslice(), Lslice('iftg')]))
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

class TestSubroutineStmt(TestCase):
    '''subroutine statements'''

    def test0(self):
        'subroutine stmt property predicates: is both decl and ustart'
        s = pps('subroutine foo')
        self.assert_(s.is_decl() and s.is_ustart())

    def test1(self):
        'simple subroutine stmt'
        theString  = 'subroutine foo(x,y,z)'
        theRepr = SubroutineStmt('foo',['x', 'y', 'z'])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test2(self):
        'recursive subroutine stmt'
        theString  = 'recursive subroutine foo(x,y,z)'
        theRepr = SubroutineStmt('foo',['x', 'y', 'z'],['recursive'])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))


class TestWhereStmt(TestCase):
    '''where statements'''

    def test0(self):
        '''simple where statement'''
        theString = 'where (1.0<=0.0) a(1:2) = b(1:2)'
        theRepr = WhereStmt(Ops('<=','1.0','0.0'),
                            AssignStmt(App('a',[Ops(':','1','2')]),
                                       App('b',[Ops(':','1','2')])))
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test1(self):
        '''complicated where statement from SCALE'''
        theString = 'where (wd<=0.0) btn(:mm,ig) = (xntn(1,ig)/xnto(1,ig))*bto(:mm,ig)'
        theRepr = WhereStmt(Ops('<=','wd','0.0'),
                            AssignStmt(App('btn',[Rslice('mm'), 'ig']),
                            Ops('*',
                                ParenExp(Ops('/',
                                         App('xntn',['1', 'ig']),
                                         App('xnto',['1', 'ig']))),
                                App('bto',[Rslice('mm'), 'ig']))))
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

class TestIntegerStmt(TestCase):
    '''tests declarations of integer variables'''

    def test0(self):
        '''integer array decl'''
        theString = 'integer :: NA(6)'
        theRepr = IntegerStmt([],[],[_NoInit(App('NA',['6']))])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test1(self):
        '''integer decl with dimension attr'''
        theString = 'integer,DIMENSION(6) :: NA'
        theRepr = IntegerStmt([],
                              [App('DIMENSION',['6'])],
                              [_NoInit('NA')])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test2(self):
        theString = 'integer,dimension(6) :: NA = [ 19,20,21,22,23,24 ]'
        theOutString = 'integer,dimension(6) :: NA = (/19,20,21,22,23,24/)'
        theRepr = IntegerStmt([],[App('dimension',['6'])],
                              [_AssignInit('NA',ArrayConstructor(['19','20','21','22','23','24']))])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theOutString,str(theRepr))
        self.assertEquals(theOutString,pps(theString).get_rawline())

class TestAllocateStmt(TestCase):
    '''allocate istatements'''

    def test0(self):
        'simple allocate stmt'
        theString = 'allocate(a(2))'
        theRepr = AllocateStmt([App('a',['2'])],None)
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test1(self):
        'complicated allocate stmt from SCALE'
        theString = 'allocate(aa(ip),at(im,30),ca(im),ch(im,isct),ct(im),cs(im),da(im,mm),db(im,mm),dc(im),ds(ip,mm),qg(igp),rav(im),sa(im,jt),sat(im,jt),sr(im),st(im),v(im),xna(im,jt),xnd(ip,mm),xne(im),xni(im,jt+1),xnn(im),xnr(im),abar(izm),rinnr(izm),rbar(izm),zon_vol(izm))'
        theRepr = AllocateStmt([App('aa',['ip']), App('at',['im', '30']), App('ca',['im']), App('ch',['im', 'isct']), App('ct',['im']), App('cs',['im']), App('da',['im', 'mm']), App('db',['im', 'mm']), App('dc',['im']), App('ds',['ip', 'mm']), App('qg',['igp']), App('rav',['im']), App('sa',['im', 'jt']), App('sat',['im', 'jt']), App('sr',['im']), App('st',['im']), App('v',['im']), App('xna',['im', 'jt']), App('xnd',['ip', 'mm']), App('xne',['im']), App('xni',['im', Ops('+','jt','1')]), App('xnn',['im']), App('xnr',['im']), App('abar',['izm']), App('rinnr',['izm']), App('rbar',['izm']), App('zon_vol',['izm'])],None)
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test2(self):
        'simpler allocate statement from SCALE'
        theString = 'allocate(il(2,izm))'
        theRepr = AllocateStmt([App('il',['2', 'izm'])],None)
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test3(self):
        'allocate statement with stat (from SCALE)'
        theString = 'allocate(hlm(lmm,mt),stat=astat)'
        theRepr = AllocateStmt([App('hlm',['lmm', 'mt'])],'astat')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

class TestDeallocateStmt(TestCase):
    '''deallocate statements'''

    def test2(self):
        'simple deallocate stmt'
        theString = 'deallocate(a)'
        theRepr = DeallocateStmt(['a'])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

class TestIOtmt(TestCase):
    ''' I/O statements '''
    def test0(self):
        '''write statement with defaults in io_ctrl_spec_list'''
        theString = 'write(*,*) x+1,y,i'
        theRepr = WriteStmt(['*','*'],[Ops('+','x','1'),'y','i'])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test1(self):
        '''formatted write statement'''
        theString = 'write(2,"(A,I3,A,I3,A,EN26.16E3)") "F(",i,",",k,")=",res_adj(i,k)'
        theRepr = WriteStmt(['2','"(A,I3,A,I3,A,EN26.16E3)"'],['"F("','i','","',"k",'")="',App('res_adj',['i','k'])])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test2(self):
        '''write statement with extra arguments'''
        theString = 'write(*,*,ADVANCE="NO") x+1,y,i'
        theRepr = WriteStmt(['*','*',NamedParam('ADVANCE','"NO"')],[Ops('+','x','1'),'y','i'])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test3(self):
        '''read statement with defaults in io_ctrl_spec_list'''
        theString = 'read(*,*) x,y,i'
        theRepr = ReadStmt(['*','*'],['x','y','i'])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test4(self):
        '''formatted read statement'''
        theString = 'read(2,"(A,I3,A,I3,A,EN26.16E3)") "F(",i,",",k,")=",res_adj(i,k)'
        theRepr = ReadStmt(['2','"(A,I3,A,I3,A,EN26.16E3)"'],['"F("','i','","',"k",'")="',App('res_adj',['i','k'])])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test5(self):
        '''read statement with extra arguments'''
        theString = 'read(*,*,REC=1) x,y,i'
        theRepr = ReadStmt(['*','*',NamedParam('REC','1')],['x','y','i'])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test6(self):
        '''read statement with simpler syntax'''
        theString = 'read *,x,y,i'
        theRepr = SimpleReadStmt(['*'],['x','y','i'])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test7(self):
        '''print statement'''
        theString = 'print *,x+1,y,i'
        theRepr = PrintStmt(['*'],[Ops('+','x','1'),'y','i'])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test8(self):
        '''write statement with more extra arguments'''
        theString = 'write(UNIT=pi_double,FMT=*) pi'
        theRepr = WriteStmt([NamedParam('UNIT','pi_double'),NamedParam('FMT','*')],['pi'])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test9(self):
        '''write statement with problematic backslash in string constant (from SCALE: scalelib/html_M.f90)'''
        theString = "write(unit,'(a)') ' <APPLET \'"
        theRepr = WriteStmt(['unit',"'(a)'"],["' <APPLET '"])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test10(self):
        '''print statement with format only'''
        theString = "print 1542"
        theRepr = PrintStmt(['1542'],[])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

class TestGotoStmt(TestCase):
    '''goto statements'''

    def test0(self):
        '''goto without space'''
        theString = 'GoTo 100' 
        theRepr = SimpleGotoStmt('100')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals('go to 100',str(theRepr)) # always printed lowercase with space

    def test1(self):
        '''goto with space'''
        theString = 'Go to 100'
        theRepr = SimpleGotoStmt('100')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals('go to 100',str(theRepr)) # always printed lowercase with space

    def test2(self):
        '''computed goto without space and optional comma'''
        theString = 'goto (10,20,30), i+3'
        theRepr = ComputedGotoStmt(['10','20','30'],Ops('+','i','3'))
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals('go to (10,20,30) i+3',str(theRepr)) # always printed lowercase with space and without optional comma

class TestDeletedStmts(TestCase):
    ''' test deleted statements we parse so we can identify them in the code without parser errors '''

    def test1(self):
        '''ASSIGN statment (deleted from standard)'''
        theString = 'assign 100 to icall'
        theRepr = DeletedAssignStmt('100','icall')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr)) 

    def test2(self):
        '''assigned GOTO statment (deleted from standard)'''
        theString = 'go to icall (10,20)'
        theRepr = AssignedGotoStmt('icall',['10','20'])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr)) 

    def test3(self):
        '''assigned GOTO statment without label list (deleted from standard)'''
        theString = 'go to icall'
        theRepr = AssignedGotoStmt('icall',[])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr)) 

    def test2(self):
        '''assigned GOTO statment with optional comma (deleted from standard)'''
        theString = 'go to icall , (10,20)'
        theRepr = AssignedGotoStmt('icall',['10','20'])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString[:theString.index(',')-1]+theString[theString.index(',')+1:],str(theRepr)) # printed without the comma

class TestDataStmt(TestCase):
    '''data statements'''

    def test0(self):
        '''simple scalar data statement'''
        theString = 'DATA TTP2 /3.42D+02/'
        theRepr = DataStmt([(['TTP2'],['3.42D+02'])],'DATA')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test1(self):
        '''simple array data statement'''
        theString = 'data simp /0.333333333D0,1.333333333D0,0.666666667D0/'
        theRepr = DataStmt([(['simp'],['0.333333333D0', '1.333333333D0', '0.666666667D0'])],'data')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test2(self):
        '''data statement with implied do construct'''
        theString = 'DATA (A1(tmp0), tmp0 = 1,5,1) /3.4D-01,5.9D-01,1.0D00,1.5D00,1.3D00/'
        theRepr = DataStmt([([_ImplicitDoConstruct(App('A1',['tmp0']),LoopControl('tmp0','1','5','1'))],['3.4D-01', '5.9D-01', '1.0D00', '1.5D00', '1.3D00'])],'DATA')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test3(self):
        '''data statement with negative constant in the value list'''
        theString = 'DATA COEF /-1.D0/'
        theRepr = DataStmt([(['COEF'],[Umi('1.D0')])],'DATA')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test4(self):
        '''long data statement from centrm with continuations removed'''
        theString = 'DATA COEF /1.D0,1.D0,-1.D0,3.D0,-3.D0,5.D0,3.D0,-30.D0,35.D0,15.D0/'
        theRepr = DataStmt([(['COEF'],['1.D0', 
                                       '1.D0', 
                                       Umi('1.D0'), 
                                       '3.D0', 
                                       Umi('3.D0'), 
                                       '5.D0', 
                                       '3.D0', 
                                       Umi('30.D0'), 
                                       '35.D0', 
                                       '15.D0'])],'DATA')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test5(self):
        '''data statement with double implied do and repeat factor'''
        theString = 'DATA ((x(i,j), i = 1,2), j = 1,3) /6*2/'
        theRepr = DataStmt([([_ImplicitDoConstruct(_ImplicitDoConstruct(App('x',['i', 'j']),
                                                                        LoopControl('i','1','2',None)),
                                                                        LoopControl('j','1','3',None))],
                           [Ops('*','6','2')])],
                           'DATA')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test6(self):
        '''data statement with "NULL()" in the value list'''
        theString = 'DATA START /NULL()/'
        theRepr = DataStmt([(['START'],[App('NULL',[])])],'DATA')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test7(self):
        '''data statement with multiple objectList-valueList pairs'''
        theString = 'DATA NAME /"JOHN DOE"/,METERS /10*0/'
        theRepr = DataStmt([(['NAME'],['"JOHN DOE"']),(['METERS'],[Ops('*','10','0')])],'DATA')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test8(self):
        '''data statement with an expression inside the implied do end'''
        theString = 'DATA ((SKEW(K,J), K = 1,J-1), J = 1,100) /4950*1.0/'
        theRepr = DataStmt([([_ImplicitDoConstruct(_ImplicitDoConstruct(App('SKEW',['K', 'J']),LoopControl('K','1',Ops('-','J','1'),None)),LoopControl('J','1','100',None))],[Ops('*','4950','1.0')])],'DATA')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test9(self):
        '''data statement with implied do without optional stride'''
        theString = 'DATA (A1(tmp0), tmp0 = 1,3) /3.4D-01,5.9D-01,1.0D00/'
        theRepr = DataStmt([([_ImplicitDoConstruct(App('A1',['tmp0']),LoopControl('tmp0','1','3',None))],['3.4D-01', '5.9D-01', '1.0D00'])],'DATA')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(theString,str(theRepr))

    def test10(self):
        '''data statement with multiple objects'''
        theString = 'data aoxy,flet3,flet4 /1.5994d+1,0.05,0.1/'
        theRepr = DataStmt([(['aoxy', 'flet3', 'flet4'],['1.5994d+1', '0.05', '0.1'])],'data')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test11(self):
        '''data statement initializing array element'''
        theString = 'data array(1) /1.5/'
        theRepr = DataStmt([([App('array',['1'])],['1.5'])])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test12(self):
        '''data statement with multiple non-comma-separated objectList-valueList pairs--KNOWN TO FAIL. see https://trac.mcs.anl.gov/projects/openAD/ticket/245'''
        theString = 'DATA NAME /"JOHN DOE"/ METERS /10*0/'
        theRepr = DataStmt([(['NAME'],['"JOHN DOE"']),(['METERS'],[Ops('*','10','0')])],'DATA')
        reprStr = 'DATA NAME /"JOHN DOE"/METERS /10*0/'
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(reprStr,str(pps(theString)))

    def test13(self):
        '''data statement with selector in data-object-list'''
        theString = 'data you % age, you % name / 35, "Fred Brown" /'
        theRepr = DataStmt([([Sel('you','age'),Sel('you','name')],[35,'"Fred Brown"'])],'data')
        reprStr = 'data you%age,you%name /35,"Fred Brown"/'
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(reprStr,str(theRepr))

class TestProcedureStmt(TestCase):
    '''procedure statements'''

    def test0(self):
        '''simplest procedure statement'''
        theString = 'procedure x'
        theRepr = ProcedureStmt(False,['x'])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test1(self):
        '''procedure statement with multiple procedure items'''
        theString = 'procedure x,y'
        theRepr = ProcedureStmt(False,['x', 'y'])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test2(self):
        '''simplest module procedure statement'''
        theString = 'module procedure x'
        theRepr = ProcedureStmt(True,['x'])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test3(self):
        '''module procedure statement with multiple procedure items'''
        theString = 'module procedure x,y'
        theRepr = ProcedureStmt(True,['x', 'y'])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test4(self):
        '''module procedure statement with multiple procedure items'''
        theString = 'module procedure real_erf,double_erf'
        theRepr = ProcedureStmt(True,['real_erf', 'double_erf'])
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))


class TestExitStmt(TestCase):
    '''exit statements'''

    def test0(self):
        '''plain exit statement'''
        theString = "exit"
        theRepr = ExitStmt(None)
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test1(self):
        '''plain exit statement (all caps)'''
        theString = "EXIT"
        theRepr = ExitStmt(None,stmt_name='EXIT')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test2(self):
        '''exit statement with optional do construct name'''
        theString = "exit l1000"
        theRepr = ExitStmt('l1000')
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

class TestOpenStmt(TestCase):
    '''open statements'''

    def test0(self):
        '''open statement with parenthesis'''
        theString = "open(linpxs)"
        theRepr = OpenStmt({'unit':'linpxs'})
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test1(self):
        '''open statement without parenthesis'''
        theString = "open linpxs"
        compString= "open(linpxs)" # always unparsed with parenthesis
        theRepr = OpenStmt({'unit':'linpxs'})
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(compString,str(pps(theString)))

    def test2(self):
        '''open statement with explicit unit/err parameter'''
        theString = "open(err=1,unit=linpxs)"
        compString= "open(unit=linpxs,err=1)" # always unparsed with ordered parameters
        theRepr = OpenStmt({'err':NamedParam('err','1'),'unit':NamedParam('unit','linpxs')})
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(compString,str(pps(theString)))

    def test3(self):
        '''open statement with positional and named parameters mixed, mixed case for parameters'''
        theString = "open(linpxs, ioStat=2,err=1)"
        compString= "open(linpxs,err=1,iostat=2)" # always unparsed with ordered parameters
        theRepr = OpenStmt({'unit':'linpxs','err':NamedParam('err','1'),'iostat':NamedParam('iostat','2')})
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(compString,str(pps(theString)))

    def test4(self):
        '''open statement with positional and named parameters mixed, mixed case for parameters given as expressions'''
        theString = "open(myType%linpxs, ioStat=2,err=1)"
        compString= "open(myType%linpxs,err=1,iostat=2)" # always unparsed with ordered parameters
        theRepr = OpenStmt({'unit':Sel('myType','linpxs'),'err':NamedParam('err','1'),'iostat':NamedParam('iostat','2')})
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(compString,str(pps(theString)))

class TestRewindStmt(TestCase):
    '''rewind statements'''

    def test0(self):
        '''rewind statement with parenthesis'''
        theString = "rewind(linpxs)"
        theRepr = RewindStmt({'unit':'linpxs'})
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test1(self):
        '''rewind statement without parenthesis'''
        theString = "rewind linpxs"
        compString= "rewind(linpxs)" # always unparsed with parenthesis
        theRepr = RewindStmt({'unit':'linpxs'})
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(compString,str(pps(theString)))

    def test2(self):
        '''rewind statement with explicit unit/err parameter'''
        theString = "rewind(err=1,unit=linpxs)"
        compString= "rewind(unit=linpxs,err=1)" # always unparsed with ordered parameters
        theRepr = RewindStmt({'err':NamedParam('err','1'),'unit':NamedParam('unit','linpxs')})
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(compString,str(pps(theString)))

    def test3(self):
        '''rewind statement with positional and named parameters mixed, mixed case for parameters'''
        theString = "rewind(linpxs, ioStat=2,err=1)"
        compString= "rewind(linpxs,err=1,iostat=2)" # always unparsed with ordered parameters
        theRepr = RewindStmt({'unit':'linpxs','err':NamedParam('err','1'),'iostat':NamedParam('iostat','2')})
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(compString,str(pps(theString)))

    def test4(self):
        '''rewind statement without parenthesis and hardwired unit number'''
        theString = "rewind 13"
        compString= "rewind(13)" # always unparsed with parenthesis
        theRepr = RewindStmt({'unit':'13'})
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(compString,str(pps(theString)))

class TestCloseStmt(TestCase):
    '''close statements'''

    def test0(self):
        '''close statement with parenthesis'''
        theString = "close(linpxs)"
        theRepr = CloseStmt({'unit':'linpxs'})
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

    def test1(self):
        '''close statement without parenthesis'''
        theString = "close linpxs"
        compString= "close(linpxs)" # always unparsed with parenthesis
        theRepr = CloseStmt({'unit':'linpxs'})
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(compString,str(pps(theString)))

    def test2(self):
        '''close statement with explicit unit/err parameter'''
        theString = "close(err=1,unit=linpxs)"
        compString= "close(unit=linpxs,err=1)" # always unparsed with ordered parameters
        theRepr = CloseStmt({'err':NamedParam('err','1'),'unit':NamedParam('unit','linpxs')})
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(compString,str(pps(theString)))

    def test3(self):
        '''close statement with positional and named parameters mixed, mixed case for parameters'''
        theString = "close(linpxs, err=1, ioStat=2)"
        compString= "close(linpxs,iostat=2,err=1)" # always unparsed with ordered parameters
        theRepr = CloseStmt({'unit':'linpxs','err':NamedParam('err','1'),'iostat':NamedParam('iostat','2')})
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(compString,str(pps(theString)))

    def test4(self):
        '''close statement with positional and named parameters mixed, mixed case for parameters given as expressions'''
        theString = "close(myType%linpxs,iostat=2,err=1)"
        theRepr = CloseStmt({'unit':Sel('myType','linpxs'),'err':NamedParam('err','1'),'iostat':NamedParam('iostat','2')})
        self.assertEquals(repr(pps(theString)),repr(theRepr))
        self.assertEquals(str(pps(theString)),str(theRepr))
        self.assertEquals(theString,str(pps(theString)))

suite = asuite(C2,C3,C4,C5,C6,C8,C9,
               TestAssignStmt,
               TestCycleStmt,
               TestEndStmt,
               TestRealStmt,
               TestCharacterDecls,
               TestImplicitStmt,
               TestDimensionStmt,
               TestDoStmt,
               TestIfStmt,
               TestWhileStmt,
               TestCallStmt,
               TestFunctionStmt,
               TestSelectCaseStmt,
               TestCaseStmts,
               TestSelectTypeStmt,
               TestTypeIsStmt,
               TestClassIsStmt,
               TestUseStmts,
               TestPointerAssignStmt,
               TestSubroutineStmt,
               TestWhereStmt,
               TestIntegerStmt,
               TestAllocateStmt,
               TestDeallocateStmt,
               TestIOtmt,
               TestGotoStmt,
               TestDataStmt,
               TestProcedureStmt,
               TestExitStmt,
               TestOpenStmt,
               TestRewindStmt,
               TestCloseStmt,
               TestDeletedStmts)

if __name__ == '__main__':
    sys.exit(runit(suite))
