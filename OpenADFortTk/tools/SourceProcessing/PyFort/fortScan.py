'''Various regular expressions for fortran tokens
to use these expressions, be sure to re.compile with re.X
'''

import re
import sys
if sys.version_info[1] == 4:
    from sre import Scanner
else:
    from re import Scanner

from PyFort.tokens import *

def __str_intern_patn(d):
    ''' inside of a string w/o \param d as delimiter;
        pairs of delimiters inside of the string are allowed '''
    patn = r"""(?:%s%s|[^%s])*"""
    return patn % (d,d,d)  # replaces the three %s in patn with d

def closed_q_patn(d):
    ''' closed quotation, i.e. a __str_intern_patn and enclosing delimiters d '''
    return '(?x)' + d + __str_intern_patn(d) + d  

q_re     = closed_q_patn("'")  
qq_re    = closed_q_patn('"')

def ro_q_patn(d):
    ''' right-open quotation with \param d as delimiter, 
        note that this will also match the 
        closing delimiter of closed quotation '''
    # delimiter + string without delimiters + eos/eol
    return '(?x)' + d + __str_intern_patn(d) + '$' 

def preluded_ro_q_patn(d):
    ''' right-open quotation with \param d as delimiter preceded by something;
        this will not match the closing delimiter of a closed quotation '''
    prelude = r'''
    (?x)          # verbose mode                        
    ^(?:          # start of line followed by  characters that are  
        [^%s] |     # not the delimiter (see below first string arg) OR 
        (?: %s)     # closed_q_patn (see below second string arg)
     )*           # 0 or more
     '''
    return '(?x)(' + prelude % (d,closed_q_patn(d)) + ')(' + d + ')(' +  __str_intern_patn(d) + ')$' 
    #return '(?x)' + prelude % (d,closed_q_patn(d)) + d +  __str_intern_patn(d) + '$' 

def lo_q_patn(d):
    ''' left-open quotation with \param d as delimiter, 
        note that this will also match the 
        opening delimiter of closed quotation'''
    # start of line + string without delimiters + delimiter
    return '(?x)^' + __str_intern_patn(d) + d
   

def postluded_lo_q_patn(d,postludeTail=None):
    ''' left-open quotation with \param d as delimiter and something that follows
        this will not match the opening delimiter of a closed quotation'''
    postlude = r'''
    (?:           # characters that are  
        [^%s] |     # not the delimiter (see below first string arg) OR 
        (?: %s)     # closed_q_patn (see below second string arg)
     )*           # 0 or more
     '''
    # start of line + string without delimiters + delimiter
    rPatn='(?x)^' + __str_intern_patn(d) + d + postlude % (d,closed_q_patn(d))
    if postludeTail:
        # if there is a tail we want to group the first part and the tail separately
        # assuming that the tail is given as its own group, i.e. something '(...)'
        # this is the reason why the first part is wrapped in another () pair,
        # otherwise we would not get a group that can be referenced
        rPatn='('+rPatn+')'+postludeTail
    rPatn+='$'    # EOS/EOL
    return rPatn

pointerAssignSymbol_re = r'=>'

id_re    = r'(?i)[_a-zA-Z$][\w$]*'
int_re   = r'(?x)\d+ (?:_ \w+)?'
dcoln_re = r'::'
semi_re  = r';'
conc_re  = r'//'
exp_re   = r'\*\*'
cmpr_re  = r'(?:>|<|=|/)='
symb_re  = r'[-+*/()\[\]=,%:<>]'
dot_re   = r'''(?x)(?i)\.    # fortran "dotted" keywords
            (?:
                eqv    |
                neqv   |
                eq     |
                ne     |
                gt     |
                ge     |
                lt     |
                le     |
                and    |
                or     |
                not    |
                true   |
                false
             )
             \.
'''

floexp_re = r'''(?ix)
              [dDeE]  # exponent marker
              [-+]? # optional sign
              \d+   # exponent
'''

flonum_re = r'''(?ix)
             (?:       # EITHER
               (?:
                  \d+        # some digits
                  (?!%s)     # NOT followed by a dot kw, but ARE
                  (?:        # followed by
                     (?:     #  EITHER
                        \.         # decimal point
                        \d*        # 0 or digits
                        (?:%s)?    # optional exp { subst floexp_re }
                     ) |     #   OR
                     %s            # definite exp { subst floexp_re }
                    )
                  )  |  # OR
                    (?:    # no leading digits
                        \.       # decimal point
                        \d+      # digits after decimal pt
                        (?:%s)?  # optional exp { subst floexp_re }
                    )
               )
'''
flonum_re = flonum_re % ( dot_re,floexp_re,floexp_re,floexp_re )
flonum_re = flonum_re + r'(?:_\w+)?'

white_re  = r'\s+'

def s_ident(self,s):
    'simplest possible scanner function: return matched string'
    return s

def s_split(self,s):
    'split semi-colon separated statements with a newline'
    return '\n'

def s_white(self,s):
    'return one space for whitespace'
    return ' '

inputLineLength = 0
def setInputLineLength(length):
    global inputLineLength
    inputLineLength = length

scan1 = Scanner([
    (pointerAssignSymbol_re, s_ident),
    (id_re,      s_ident),
    (conc_re,    s_ident),
    (dcoln_re,   s_ident),
    (exp_re,     s_ident),
    (cmpr_re,    s_ident),
    (symb_re,    s_ident),
    (dot_re,     s_ident),
    (q_re,       s_ident),
    (qq_re,      s_ident),
    (flonum_re,  s_ident),
    (int_re,     s_ident),
    (semi_re,    s_split),
    (white_re,   s_white),
    ],re.I | re.X)
