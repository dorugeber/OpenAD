'''
create scanner objects, like the old sre undocumented
class
'''
import re

re1 = r'a+'
re2 = r'b+'
re3 = r'c+'

ss = '|'.join(['('+ s + ')' for s in (re1,re2,re3)])

t1 = 'aaaa bbbbb dddd cccc'



