from Setup import *

from PyUtil.l_assembler import *
from unittest import *
from PyUtil.flatten import flatten
from PyUtil.chomp import chomp

def mkpred(id):
    '''make a predicate assembler for a given id'''
    return pred(lambda (x): x == id)

def mklinepred(id):
    '''make a predicate assembler for an id on a line'''
    return pred(lambda(x): x == (id+'\n'))

def jjj(dta):
    return ''.join([chomp(x) for x in flatten(dta)])

class strings(TestCase):
    def jj(self,s):
        '''utility to join string remainders'''
        return ''.join(list(s))
    
    def setUp(self):
        self.p1   = list('ssskkss')
        self.p2   = list('ccscssscskkskscskskksss')
        self.p3   = list('ccscckkscscckcckccssk')

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

        self.t1 = ['string','string','string','no','string','string']
        self.t2 = ['no','string','no','no']
        self.t3 = ['string','no','string','no','string','string']

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
        self.p1 = list('ccscckkscscckcckccssk')
        self.ee = []
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
    
        self.f    = file(fname_t('f0.f')).readlines()
        self.stmt = stmt
        self.cblk = cblk

    def tearDown(self):
        pass

    def testF1(self):
        '''read line assemblies from file'''
        ae   = self.assertEqual
        rst  = self.f
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
        self.f    = file(fname_t('f0.f')).readlines()

    def tearDown(self):
        pass

    def test1(self):
        'vgen gets all assemblies from a file'
        ae    = self.assertEqual
        aiter = vgen(self.asem,self.f)

        asems = list(aiter)

        ae(asems,['skk','s','s','ccc'])

def s1():
    return makeSuite(simple)

def suite():
    s1 = makeSuite(simple)
    s2 = makeSuite(strings)
    s3 = makeSuite(misc1)

    rv = s1
    rv.addTest(s2)
    rv.addTest(s3)
    rv.addTest(makeSuite(filetst))
    rv.addTest(makeSuite(generatorTest))

    return rv

if __name__ == '__main__':
    sys.exit(runit(suite()))

