import sys,os.path
from string import maketrans

mypath  = sys.path[0]
libpath = os.path.normpath(os.path.join(sys.path[0],'../..'))

if libpath not in sys.path:
    sys.path.insert(0,libpath)

from unittest import TestCase

class _C(TestCase):
    def runTest(self):
        pass

_cc = _C()
ae = _cc.assertEquals
a_ = _cc.assert_

def kill_blanks(s):
    t = maketrans('','')
    return s.translate(t,' ')

def preclip(s):
    'clip leading "\n" on string s'
    if s[0] == '\n':
        return s[1:]
    else:
        return s

def open_t(fname,mode='r',bufsize=-1):
    '''utility to open a file in the Tfiles directory'''
    return open(os.path.join(mypath,'Tfiles',fname),mode,bufsize)

def fname_t(fname):
    '''add the path to Tfiles to fname'''
    return os.path.join(mypath,'Tfiles',fname)

def asuite(*cases):
    'make a suite from a list of cases'
    from unittest import makeSuite
    rv = makeSuite(cases[0])
    for case in cases[1:]:
        rv.addTest(makeSuite(case))

    return rv

def runit(s):
    'standard way to run the test runner'
    from unittest import TextTestRunner
    testResult=TextTestRunner(verbosity=2).run(s)
    if testResult.wasSuccessful():
        return 0
    else:
        return 1
