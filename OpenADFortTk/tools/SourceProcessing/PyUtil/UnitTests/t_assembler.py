from Setup import *

from PyUtil.assembler import *
from unittest import *
from PyUtil.flatten import flatten
from PyUtil.chomp import chomp
from PyUtil.buf_iter import buf_iter

def mkpred(id):
    '''make a predicate assembler for a given id'''
    return pred(lambda (x): x == id)

def mklinepred(id):
    '''make a predicate assembler for an id on a line'''
    return pred(lambda(x): x == (id+'\n'))

def mkl(id):
    'just check 1st letter'
    return pred(lambda x: x[0] == id)

class _T(object):
    def __init__(self,parent=None):
        self.cmnt     =  None
        self.unit     =  None
        self.decls    = []
        self.execs    = []
        self.contains = []
        self.ulist    = []
        self.end      = []
        self.parent   = parent

class _TT(object):
    def __init__(self):
        self.val    = _T()
        self.fval   = None

    def uhead(self,s,dc=None):
        self.uinfo(s[1])
        if s[0]:
            self.ucomm(s[0][0])
        return self.val

    def ucomm(self,s,dc=None):
        self.val.cmnt = s
        return self.val

    def uinfo(self,s,dc=None):
        self.val.unit = s
        return self.val

    def udecl(self,s,dc=None):
        self.val.decls = s
        return self.val

    def uexec(self,s,dc=None):
        u = self.val
        u.execs = s
        return u

    def uend(self,s,dc=None):
        u = self.val
        p = u.parent
        u.end = s
        self.fval = u
        self.val  = _T(p)
        return u

    def ucont(self,s,dc=None):
        u = self.val
        u.contains = s
        self.val = _T(u)
        return u

    def ulist(self,l,dc=None):
        p = self.val.parent
        self.val = p
        self.val.ulist = l
        return self.val

def jjj(dta):
    return ''.join([chomp(x) for x in flatten(dta)])

class strings(TestCase):
    def jj(self,s):
        '''utility to join string remainders'''
        return ''.join(list(s))
    
    def setUp(self):
        self.p1   = buf_iter('ssskkss')
        self.p2   = buf_iter('ccscssscskkskscskskksss')
        self.p3   = buf_iter('ccscckkscscckcckccssk')

        self.ss   = mkpred('s')
        self.k    = mkpred('k')
        self.c    = mkpred('c')

        ss        = self.ss
        k         = self.k
        c         = self.c

        self.cblk = star(c)
        self.stmt = seq(ss,star(seq(star(c),k)))

    def tearDown(self):
        pass

    def testS1(self):
        '''1st stmt from p1'''
        v,rst = self.stmt(self.p1)
        self.assertEqual(v,['s',[]])
        self.assertEqual(self.jj(rst),'sskkss')

    def testP1(self):
        '''complete assembly of p1'''
        stmt = self.stmt
        rst  = self.p1
        for i in (xrange(2)):
            v,rst = stmt(rst)
            self.assertEqual(v,['s',[]])
        v,rst = stmt(rst)
        self.assertEqual(v,['s',[[[],'k'],[[],'k']]])
        for i in (xrange(2)):
            v,rst = stmt(rst)
            self.assertEqual(v,['s',[]])
        self.assertEqual(list(rst),[])

    def testP2(self):
        '''complete assembly of p2'''
        cblk  = self.cblk
        stmt  = self.stmt
        rst   = self.p2
        jj    = self.jj
        ae    = self.assertEqual

        v,rst = cblk(rst)
        ae(v,['c'] * 2)
        v,rst = stmt(rst)
        ae(v,['s', []])
        v,rst = cblk(rst)
        ae(v,['c'])
        v,rst = star(stmt)(rst)
        ae(v,[['s',[]]] * 3)
        v,rst = cblk(rst)
        ae(v,['c'])
        v,rst = stmt(rst)
        ae(v,['s',[[['c']*0,'k']]*2])
        v,rst = stmt(rst)
        ae(v,['s',[[['c']*0,'k']]])
        v,rst = stmt(rst)
        ae(v,['s',[]])
        v,rst = cblk(rst)
        ae(v,['c'])
        v,rst = stmt(rst)
        ae(v,['s',[[[],'k']]])
        v,rst = stmt(rst)
        ae(v,['s',[[[],'k']]*2])
        for i in (xrange(3)):
            v,rst = stmt(rst)
            ae(v,['s',[]])
        ae(list(rst),[])

    def testP3(self):
        '''commented continuation for p3'''
        cblk  = self.cblk
        stmt  = self.stmt
        rst   = self.p3
        jj    = self.jj
        ae    = self.assertEqual

        v,rst = cblk(rst)
        ae(v,['c']*2)
        v,rst = stmt(rst)
        ae(v,['s',[[['c']*2,'k'],[[],'k']]])
        v,rst = seq(any,any)(rst)
        ae(jj(v),'sc')
        v,rst = stmt(rst)
        ae(''.join(flatten(v)),'scckcck')
        v,rst = seq(any,any,any)(rst)
        ae(''.join(v),'ccs')
        v,rst = stmt(rst)
        ae(''.join(flatten(v)),'sk')
        ae(list(rst),[])


class simple(TestCase):
    def setUp(self):
        strng = mkpred('string')
        no    = mkpred('no')

        self.strng = strng
        self.no    = no

        self.t1 = buf_iter(['string','string','string','no','string','string'])
        self.t2 = buf_iter(['no','string','no','no'])
        self.t3 = buf_iter(['string','no','string','no','string','string'])

        self.seq1 = seq(no,strng)
        self.seq2 = seq(star(self.strng),no,strng)
        self.seq3 = seq(star(strng),no,star(strng))
        self.asm3 = star(seq(strng,no))

    def tearDown(self):
        pass

    def testP1(self):
        '''check simple predicate match'''
        v,rst = self.strng(self.t1)
        self.assertEqual(v,'string')
        self.assertEqual(list(rst),['string','string','no','string','string'])

    def testSeq1(self):
        '''test simple sequence'''
        v,rst = self.seq1(self.t2)
        self.assertEqual(v,['no','string'])
        self.assertEqual(list(rst),['no','no'])

    def testSeqWstar(self):
        '''test seq w embedded star operator'''
        v,rst = self.seq2(self.t1)
        self.assertEqual(v,[['string','string','string'],'no','string'])
        self.assertEqual(list(rst),['string'])

    def testStarGetsAll(self):
        '''pattern gets entire string'''
        v,rst = self.seq3(self.t1)
        self.assertEqual(v,[['string','string','string'],'no',
            ['string','string']])
        self.assertEqual(list(rst),[])

    def testBacktrackSeq(self):
        '''subseq of pattern needs backtracking'''
        v,rst = self.asm3(self.t3)
        self.assertEqual(v,[['string','no'],['string','no']])
        self.assertEqual(list(rst),['string','string'])

class misc1(TestCase):
    def setUp(self):
        self.p1 = buf_iter('ccscckkscscckcckccssk')
        self.ee = buf_iter([])
        (s,k,c) = [ mkpred(l) for l in 'skc' ]

        self.stmt = seq(s,star(seq(star(c),k)))
        self.cblk = plus(c)
        self.all  = star(disj(self.cblk,self.stmt))

    def tearDown(self):
        pass

    def testSplit(self):
        '''plus,disj,consume all string'''

        all = self.all
        ae  = self.assertEqual
        p1  = self.p1

        (v,rst) = all(p1)
        vl      = [ ''.join(flatten(l)) for l in v ]
        ae(vl,[ 'cc','scckk','s','c','scckcck','cc','s','sk' ])
        ae(list(rst),[])

    def testEmpty(self):
        '''empty source'''
        all  = self.all
        ae   = self.assertEqual
        ee   = self.ee

        (v,rst) = all(ee)
        ae(v,[])
        ae(list(rst),[])

class filetst(TestCase):
    (s,k,c) = [ mklinepred(l) for l in 'skc' ]
    stmt    = seq(s,star(seq(star(c),k)))
    cblk    = plus(c)

    def setUp(self):
        import os.path
        (s,k,c) = [ mklinepred(l) for l in 'skc' ]
        stmt    = seq(s,star(seq(star(c),k)))
        cblk    = plus(c)
    
        self.f    = open_t('f0.f')
        self.stmt = stmt
        self.cblk = cblk

    def tearDown(self):
        self.f.close()

    def testF1(self):
        '''read line assemblies from file'''
        ae   = self.assertEqual
        rst  = buf_iter(self.f)
        stmt = self.stmt
        cblk = self.cblk

        (v,rst) = stmt(rst)
        ae(jjj(v),'skk')
        (v,rst) = stmt(rst)
        ae(jjj(v),'s')
        (v,rst) = stmt(rst)
        ae(jjj(v),'s')
        (v,rst) = cblk(rst)
        ae(jjj(v),'ccc')
        ae(list(rst),[])

class generatorTest(TestCase):

    def setUp(self):
        import os.path
        (s,k,c)   = [ mklinepred(l) for l in 'skc' ]
        stmt      = treat(seq(s,star(seq(star(c),k))),jjj)
        cblk      = treat(plus(c),jjj)

        self.asem = disj(cblk,stmt)
        self.f    = file(fname_t('f0.f'))

    def tearDown(self):
        self.f.close()

    def test1(self):
        'vgen gets all assemblies from a file'
        ae    = self.assertEqual
        aiter = vgen(self.asem,buf_iter(self.f))

        asems = list(aiter)

        ae(asems,['skk','s','s','ccc'])

def install_pat(cur,lst):
    (c,u,d,x,n,e) = lst

    def cmst(p): return treat(seq(zo1(c),p),flatten)

    def _ul(s):
        'eta expansion for recursive patterns'
        return uu(s)

    uh    = treat(seq(zo1(c),u),cur.uhead)
    udcl  = treat(star(cmst(d)),flatten,cur.udecl)
    uexc  = treat(star(cmst(x)),flatten,cur.uexec)
    cnth  = treat(cmst(n),cur.ucont)
    cntl  = treat(star(_ul),cur.ulist)
    uctn  = zo1(seq(cnth,cntl))
    uend  = treat(cmst(e),cur.uend)

    uu    = treat(seq(uh,udcl,uexc,uctn,uend),lambda s:cur.fval)
    return uu

class C4(TestCase):
    'check side-effecting incremental value building from pattern'

    def setUp(self):
        ptns = [ mkl(l) for l in 'cudxne' ]
        self.uu = install_pat(_TT(),ptns)

    def test1(self):
        'incremental side effect build f simple unit'
        uu = self.uu
        ts = 'c1 u1 d1 x1 e1'
        ii = buf_iter((i for i in ts.split()))
        (v,r) = uu(ii)
        a_(isinstance(v,_T))
        a_(not list(r))
        ae(v.cmnt,'c1')
        ae(v.unit,'u1')
        ae(v.decls,['d1'])
        ae(v.execs,['x1'])
        ae(v.contains,[])
        ae(v.ulist,[])
        ae(v.end,['e1'])

    def test2(self):
        'incremental side effect build vgen f 2 simple units'
        uu = self.uu
        ts = 'ua c1 d1 c3 x1 e1 cb ub cdb1 db1 cxb1 xb1 xb2 eb'
        ii = buf_iter((i for i in ts.split()))
        units = list(vgen(uu,ii))
        ae(len(units),2)
        (u1,u2) = units
        ae(u1.cmnt,None)
        ae(u2.execs,['cxb1','xb1','xb2'])

    def test3(self):
        'incremental side effect build w recursive pattern'
        uu = self.uu
        ts = 'ua cd1 d1 cx1 x1 cn1 n cnu1 un1 cn1d dn1 cnx1 xn1 cne en e1'
        ii = buf_iter((i for i in ts.split()))
        (u,r) = uu(ii)
        ae(list(r),[])
        ae(u.cmnt,None)
        ae(u.decls,['cd1','d1'])
        ae(u.contains,['cn1','n'])
        ae(len(u.ulist),1)
        ae(u.end,['e1'])

        u = u.ulist[0]
        a_(isinstance(u,_T))
        ae(u.cmnt,'cnu1')
        ae(u.unit,'un1')
        ae(u.execs,['cnx1','xn1'])

    def test4(self):
        'incremental side effect build 1 unit w 2 subunits'
        uu = self.uu
        ts = '''ua cd1 d1 cx1 x1 cn1 n
                   cnu1 un1 cn1d dn1 cnx1 xn1 cne en
                   unb db1 db2 xb1 xb2 xb3 eb
                 e1'''
        ii = buf_iter((i for i in ts.split()))
        (u,r) = uu(ii)
        ae(list(r),[])
        ae(u.cmnt,None)
        ae(u.decls,['cd1','d1'])
        ae(u.contains,['cn1','n'])
        ae(len(u.ulist),2)
        ae(u.end,['e1'])

        (u,u2) = u.ulist
        a_(isinstance(u,_T))
        ae(u.cmnt,'cnu1')
        ae(u.unit,'un1')
        ae(u.execs,['cnx1','xn1'])
        u = u2
        a_(isinstance(u,_T))
        ae(u.cmnt,None)
        ae(u.unit,'unb')
        ae(u.execs,['xb1','xb2','xb3'])
        ae(u.end,['eb'])

    def test5(self):
        'incremental side effect build 1 unit w subunits having subunits'
        uu = self.uu
        ts = '''umod
                   dm1 dm2 nmod
                     ua cd1 d1 cx1 x1 cn1 n
                      cnu1 un1 cn1d dn1 cnx1 xn1 cne eun1
                      unb db1 db2 xb1 xb2 xb3 eb
                     ea
                 emod'''

        ii = buf_iter((i for i in ts.split()))
        (u,r) = uu(ii)
        ae(list(r),[])
        ae(u.cmnt,None)
        ae(u.decls,['dm1','dm2'])
        ae(u.contains,['nmod'])
        ae(len(u.ulist),1)
        ae(u.end,['emod'])

        u = u.ulist[0]
        a_(isinstance(u,_T))
        ae(u.cmnt,None)
        ae(u.unit,'ua')
        ae(u.execs,['cx1','x1'])
        ae(len(u.ulist),2)
        ae(u.end,['ea'])

        (u,u2) = u.ulist
        a_(isinstance(u,_T))
        ae(u.cmnt,'cnu1')
        ae(u.unit,'un1')
        ae(u.execs,['cnx1','xn1'])
        u = u2
        a_(isinstance(u,_T))
        ae(u.cmnt,None)
        ae(u.unit,'unb')
        ae(u.execs,['xb1','xb2','xb3'])
        ae(u.end,['eb'])

suite = asuite(simple,strings,misc1,filetst,generatorTest,C4)
_s    = asuite(C4)

if __name__ == '__main__':
    sys.exit(runit(suite))
