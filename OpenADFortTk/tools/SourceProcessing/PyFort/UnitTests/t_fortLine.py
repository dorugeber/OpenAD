from Setup import *

from unittest  import *
from PyFort.fortLine  import *
from cStringIO import StringIO
from PyFort.flow      import flow_line
from PyUtil.flatten   import flatten
from PyUtil.chomp     import chomp

class T1(TestCase):

    s1 = '''
      x1(3:5) = 'This is an extremely long string that must be spread over several lines' // 'concatenated with another extreeeeeeeeemly long string'_foobar // 'one more string just to round things out ...'
'''
    s2 = '''
c This is a comment line that may be a little long, but that should be ok
'''
    s3 = '''
      x1(42,39,x**2 + ix * y + iz,44) = sin(cos(atan(foo,bar)))
'''
    (s1,s2,s3) = [x[1:] for x in (s1,s2,s3)]
    
    def setUp(self):
        T1.fl = fortLine()

    def tearDown(self):
        pass

    def test1(self):
        '''flow a long line'''
        fl = T1.fl
        s1 = T1.s1
        correct = '''
      x1(3:5) = 'This is an extremely long string that must be spread ov
     +er several lines' // 'concatenated with another extreeeeeeeeemly l
     +ong string'_foobar // 'one more string just to round things out ..
     +.'
'''
        correct = correct[1:-1]
        self.assertEquals(flow_line(s1),correct)

    def test2(self):
        '''flow a comment gives no change'''
        s2 = T1.s2
        self.assertEquals(flow_line(s2),s2)

    def test3(self):
        '''flow a line shorter than 72 leaves line unchanged'''
        s3 = T1.s3
        self.assertEquals(flow_line(s3),s3)

class fline_t(TestCase):
    right = "       x = foo // 'bar  '' '    // ' some more string with ! in itfinishes the string'"
    p1 = '''
       x = foo // ! concatenation comment for fun

c Internal comment
c
     +'bar  '' ' ! another shout comment
     &   // ' some more string with ! in it
     *finishes the string'
'''
    p1 = p1[1:]
    fl = fortLine()
    p1f = StringIO(p1)
    full = fl.a_line

    (fln1,rst) = full(buf_iter(p1f))

    def test1(self):
        'fline class, rawline,line,and internal_comments attributes'
        internal_c = '! concatenation comment for fun|\n|c Internal comment\n|c\n|! another shout comment'
        ae = self.assertEquals
        aa = self.assert_

        fln1 = fline_t.fln1
        rst  = fline_t.rst

        aa(fln1[0] == fline_t.p1)
        ae(fln1[1],fline_t.right)
        ae(list(rst),[])
        ae('|'.join(fln1[2]),internal_c)

    def test2(self):
        'cline class, rawline attribute, comment_list method'
        aa = self.assert_
        ae = self.assertEquals
        fl = fortLine()
        cblk = fl.a_line

        p1 = '''
   ! This is a comment
C
c Internal comment
c
* ! another !
d    a possible debugging line
'''
        p1   = p1[1:]
        p1f  = buf_iter(StringIO(p1))

        (fln1,rst) = cblk(p1f)

        aa(''.join(flatten(fln1)) == p1)
        ae([chomp(c) for c in fln1],
           ['   ! This is a comment', 'C', 'c Internal comment', 'c', '* ! another !', 'd    a possible debugging line'])

def s1():
    return makeSuite(fline_from_line_t)

def suite():
    
    rv = makeSuite(T1)
    rv.addTest(makeSuite(fline_t))

    return rv

if __name__ == "__main__":
    sys.exit(runit(suite()))
