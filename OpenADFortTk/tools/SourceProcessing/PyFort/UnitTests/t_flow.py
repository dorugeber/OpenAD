'''
Test the free format flow_line routine
fixed format flow_line rather extensively tested in other test suites
'''
from Setup     import *
from unittest  import *
from PyFort.flow      import setInputFormat, setOutputFormat
import PyFort.flow as flow

class T1(TestCase):
    def setUp(self):
        setInputFormat('free')
        setOutputFormat('free')

    def tearDown(self):
        setInputFormat('fixed')
        setOutputFormat('fixed')

    def test1(self):
        'simple uncontinued free format string'

        ae = self.assertEquals
        a_ = self.assert_
        flow_line = flow.flow_line
        
        l1 = preclip('''
  logical subroutine fleeeef435678988(xxxxxxxx,yyyyyyyyyyy,zzzzzzzzzzz,h878787)
''')
        ae(flow_line(l1),l1)

    def test2(self):
        'simple continued free format string'

        ae = self.assertEquals
        a_ = self.assert_
        flow_line = flow.flow_line
        
        l1 = preclip('''
  logical subroutine fleeeef435678988(xxxxxxxx,yyyyyyyyyyy,zzzzzzzzzzz,h878787,k65655463)
''')
        l2 = preclip('''
  logical subroutine fleeeef435678988(xxxxxxxx,yyyyyyyyyyy,zzzzzzzzzzz,h878787,k&
     &65655463)
''')
        ae(flow_line(l1),l2)

s1 = asuite(T1)
suite = asuite(T1)

if __name__ == '__main__':
    sys.exit(runit(suite))
