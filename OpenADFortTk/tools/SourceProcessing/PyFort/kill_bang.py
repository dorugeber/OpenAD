'''
Routine to kill bang(= !) comments from the end of the line
'''
from PyFort.fortScan import q_re,qq_re, postluded_lo_q_patn
import re

__eol_bang_re = r'''^((?: [^'"!] | (?: %s ) | (?: %s) )*) (! .*)$'''
__eol_bang_re = __eol_bang_re % (q_re,qq_re)
__eol_bang_re = re.compile(__eol_bang_re,re.X)

def kill_bang_comment(l):
    '''kill bang comments from end of line'''
    mm = __eol_bang_re.match(l)
    if not mm:
        return(l,'')
    else:
        return(mm.group(1),mm.group(2))

__eol_bang_loREs = {
    "'":re.compile(postluded_lo_q_patn("'","(! .*)"),re.X),
    '"':re.compile(postluded_lo_q_patn('"','(! .*)'),re.X)}


def kill_bang_comment_lo_q(l,d):
    '''kill bang comments from end of line that has a left-open quotation with delimiter d'''
    mm = __eol_bang_loREs[d].match(l)
    if not mm:
        return(l,'')
    else:
        return(mm.group(1),mm.group(2))
