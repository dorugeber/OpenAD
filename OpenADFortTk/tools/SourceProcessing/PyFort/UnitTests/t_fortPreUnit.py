from Setup import *

from unittest import *
from PyFort.fortPreUnit import fortPreUnitIterator

class T1(TestCase):
    def test1(self):
        'simple file w no contains'
        fn = fname_t('units1.f90')
        i0 = fortPreUnitIterator(fn,True)
        ae(len(list(i0)),3)

    def test2(self):
        'file w module w contains'
        fn = fname_t('units2.f90')
        i0 = fortPreUnitIterator(fn,True)
        ae(len(list(i0)),2)

s = asuite(T1)

if __name__ == '__main__':
    sys.exit(runit(s))
