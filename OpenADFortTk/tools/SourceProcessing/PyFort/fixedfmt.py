from _Setup import *

from PyUtil.flatten import flatten
from PyUtil.chomp import chomp
from PyFort.kill_bang import *
from PyUtil.assembler import *
from PyFort.fortScan import inputLineLength

def cont_p(l):
    '''given a line l, return true if l is a continuation line'''
    contre = re.compile(r'''\ {5} \S''',re.X)
    return contre.match(l)

def _comment_p(l):
    '''given a line l, return true if l is a comment (or blank) line'''
    blankre = re.compile(r'''\s* $''',re.X)
    return nbl_comment_p(l) or blankre.match(l)

def stmt_p(l):
    '''given a line l, return true if l is a starting statement'''
    return not (_comment_p(l) or cont_p(l))

_comm   = pred(_comment_p)

strt   = pred(stmt_p)
cont   = pred(cont_p)
_a_stmt   = seq(strt,star(seq(star(_comm),cont)))

_comre   = re.compile(r'''[^\d\s]''')
_shoutre = re.compile(r''' \s* !''',re.X)

def nbl_comment_p(l):
    'return true if line is a (nonblank) comment'
    return _comre.match(l) or _shoutre.match(l)

def _fjoin(dta):
    '''take in a pattern match nested seq data structure for a fortran stmt,
    return tuple (rawline,joined_line,internal_comments)

    For fixed format lines:
    a true fortran line dta structure comes in as:
    [initial stmt [continuation_lines]]

    continuation_lines are structures as well:
        [[comments] continuation_line]
    '''
    rawline           = ''.join(flatten(dta))
    internal_comments = []
    (line,c)          = kill_bang_comment(chomp(dta[0]))
    if not c == '':
        internal_comments.append(c)

    for cont in dta[1]:
        if inputLineLength != 0:
            if cont[inputLineLength:] != '':
                print >> sys.stderr, "The following text is being cut because the statement exceeded the specified line length: "+jl[inputLineLength:]
            cont = cont[:inputLineLength]
        internal_comments.extend(cont[0])
        (l,c) = kill_bang_comment(chomp(cont[1])[6:])
        if not c == '':
            internal_comments.append(chomp(c))
        line += l

    return (rawline,line,internal_comments)

class fixedfmt(object):
    'hold helper functions for fixed fmt'

    a_stmt    = staticmethod(_a_stmt)
    comm      = staticmethod(_comm)
    comment_p = staticmethod(_comment_p)
    fjoin     = staticmethod(_fjoin)
