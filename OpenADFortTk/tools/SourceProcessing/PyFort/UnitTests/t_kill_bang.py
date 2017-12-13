from Setup import *
from PyFort.kill_bang import *
from unittest import *

class C1(TestCase):
    l1 = preclip(r'''
    str1 = 'This is a string'
''')
    l2 = preclip(r'''
    str1 = 'This is a string & !it has something in it'
''')
    l3 = preclip(r'''
          str1 = 'This is a string with & ! stuff &
''')
    l4 = preclip(r'''
       str1 = 'This is a string with & ! stuff &' &
''')
    l5 = preclip(r'''
       str1 = 'This is a string with & ! stuff &' & ! this is a comment
''')
    globals().update(locals())

    def test1(self):
        'no bang comments when embedded in strings'

        ae = self.assertEquals
        a_ = self.assert_
        for l in (l1,l2,l3,l4):
            (s,r) = kill_bang_comment(l)
            ae(s,l)
            ae(r,'')

    def test2(self):
        'bang comment after quoted string that contains & !'
        ae = self.assertEquals
        a_ = self.assert_
        (s,r) = kill_bang_comment(l5)
        ae(s,l5[:52])
        ae(r,'! this is a comment')

class C2(TestCase):
    s1 = "       x(ii,jj) = 'foo '' ' // 'bar' ! eol comment"
    s2 = "       x(ii,jj) = 'foo '' ' // 'bar'"
    s3 = "       x(ii,jj) = 'foo '' ' // 'bar ! eol comment"

    def test1(self):
        'kill bang comments from end of line'
        s1 = C2.s1
        s2 = C2.s2
        s3 = C2.s3
        ae = self.assertEquals

        (l,c) = kill_bang_comment(s1)
        ae(l,"       x(ii,jj) = 'foo '' ' // 'bar' ")
        ae(c,"! eol comment")

        (l,c) = kill_bang_comment(s2)
        ae(l,s2)
        ae(c,'')

        (l,c) = kill_bang_comment(s3)
        ae(l,s3)
        ae(c,'')

suite = asuite(C1,C2)

if __name__ == '__main__':
    sys.exit(runit(suite))
    
