from Setup     import *

from unittest  import *
from PyFort.fortFile  import *
from itertools import *
from PyUtil.chomp     import chomp

import PyFort.fortLine as fl

# internal comments should be preserved it the "internal" array, but NOT in the rawline
def compareFiles(assertFunc,originalFileName,RefFileName,format):
    try:
        (fd,testFileName) = tempfile.mkstemp()
        testFile  = open(testFileName,'w')
        Ffile.file(originalFileName).printit(testFile,format=='free')
        testFile.close()
        testFile = open(testFileName,'r')
        testFileLines = testFile.readlines()
        refFile = open(fname_t(RefFileName),'r')
        refFileLines = refFile.readlines()
        assertFunc(len(testFileLines),len(refFileLines),'transformation result ('+testFileName+') and reference file ('+RefFileName+') have disparate line counts')
        for testLine,refLine in zip(testFileLines,refFileLines):
            try : 
                assertFunc(testLine,refLine)
            except AssertionError, e:
                print >> sys.stderr, '\ncomparing produced output('+testFileName+') and reference file ('+RefFileName+') fails with:\n'+str(e)
                raise e
        refFile.close()
        testFile.close()
        os.remove(testFileName)
    except UserError,e:
        print >>sys.stderr,"Error: ",e.msg
        return 1

class C1(TestCase):
    def test1(self):
        'file gets lines'
        ff = Ffile.file(fname_t('f2.f'))
        a_(ff,'fortFile object is screwed')
        for l in ff.lines: print l

class fixedfile(TestCase):
    def setUp(self):
        self.fname = fname_t('f1.f')
        self.ff    = open_t('f1.f')

    def tearDown(self):
        self.ff.close()

    def test1(self):
        '''Ffile object removes leads, internal comments, and line continuations from the line (fixed format test)'''
        ff = self.ff
        out = open_t('f1.out.f')

        self.assertEquals(''.join([l.line+'\n' for l in Ffile.file(self.fname).lines]),
                          ''.join(out.readlines()))
        out.close()

    def test2(self):
        '''Test that Ffile object preserves the correct leads (fixed format test)'''
        compareFiles(self.assertEquals,self.fname,'f1.formatted.f','fixed')

    def test3(self):
        '''Test that embedded comments are removed from rawline (fixed format test)'''
        compareFiles(self.assertEquals,self.fname,'f1.comments.f','fixed')

class freefile(TestCase):
    def setUp(self):
        self.fname = fname_t('f1.f90')
        self.ff    = open_t('f1.f90')

    def tearDown(self):
        self.ff.close()

    def test1(self):
        '''Ffile object removes leads, internal comments, and line continuations from the line (free format test)'''
        ff = self.ff
        out = open_t('f1.out.f90')

        self.assertEquals(''.join([l.line+'\n' for l in Ffile.file(self.fname).lines]),
                          ''.join(out.readlines()))
        out.close()

    def test2(self):
        '''Test that Ffile object preserves the correct leads (free format test)'''
        compareFiles(self.assertEquals,self.fname,'f1.formatted.f90','free')

    def test3(self):
        '''Test that embedded comments are removed from rawline (free format test)'''
        compareFiles(self.assertEquals,self.fname,'f1.comments.f90','free')

class fileops(TestCase):
    def setUp(self):

        fname     = fname_t('f1.f')
        f         = file(fname_t('f1.formatted.f'))
        self.ff   = Ffile.file(fname)
        self.fstr = f.read()
        f.close()

    def tearDown(self):
        pass

    def test1(self):
        '''write Ffile to a file'''
        import os

        fname = '__tmp.f'

        self.ff.write(fname)
        f = file(fname)
        
        self.assertEquals(self.fstr,f.read())

        f.close()
        os.remove(fname)

class heretst(TestCase):
    p1 = '''
      subroutine foo(x)
      x = x + 
c
c embedded continuation lines
c
     &5 + 2 * x
c
c more embedded lines
c
     & + si
     &n(x+2.0)
      x = 5.0
      x
     & = 
     & 13.2
      end
'''
    p1    = preclip(p1)
    ff    = Ffile.here(p1)
    fname = fname_t('f1.out.f')
    f     = file(fname)

    def test1(self):
        '''"here" docs'''
        ff = heretst.ff
        f  = heretst.f
        ae = self.assertEquals
        for (here,ffi) in izip(f,(l.line+'\n'
                                  for l in ff.lines)):
            ae(here,ffi)
            
class maptest(TestCase):

    fname = fname_t('f2.f')

    def test1(self):
        '''map operation, join short continuation lines'''

        ae   = self.assertEquals
        ok   = open_t('f2.f.map_ok.1').read()
        ff   = Ffile.file(maptest.fname)
        lex1 = ((fl.cline,lambda l:[ chomp(l.rawline) ]),
                (fl.fline,lambda l:[ l.lead + l.line ]))
        res  = ''.join(l+'\n' for l in ff.map(lex1))
        ae(res,ok)

    def test2(self):
        '''map operation, filter comments'''

        ae = self.assertEquals
        ok = open_t('f2.f.map_ok.2').read()
        ff   = Ffile.file(maptest.fname)
        lex1 = ((fl.cline,lambda l:[]),
                (fl.fline,lambda l:[ l.line ]))
        res  = ''.join(l+'\n' for l in ff.map(lex1))
        ae(res,ok)

s  = asuite(C1)
s1 = asuite(maptest)

suite = asuite(fixedfile,freefile,fileops,heretst,maptest)


if __name__ == '__main__':
    sys.exit(runit(suite))


