'''read Fortran Lines from sources into a Line object'''

from _Setup import *
from PyUtil.flatten import flatten
from PyUtil.assembler import *
from PyUtil.chomp import chomp
from PyFort.fixedfmt import fixedfmt
import PyFort.flow
import re

fmt = 'fix'

_ad_directive_prefix_re    = r'\$OpenAD\s*\$?' # comments that have $OpenAD w opt trailing $

def ident(self):
    '''generic identity function for default mappers'''
    return self

class anyFortLine(object):
    '''generic line class for any fortran line'''
    pass

class fline(anyFortLine):
    '''a non-comment (semantic) class for a fortran line
    This is a trivial joined line processor
    '''
    def __init__(self,line,lineNumber):
        self.line = line
        self.rawline = line
        self.lineNumber = lineNumber

class cline(anyFortLine):
    '''a comment (or blank) line'''
    def __init__(self,dta,lineNumber):
        self.lineNumber = lineNumber
        self.rawline = ''.join(flatten(dta))

    def comment_list(self):
        return self.rawline.splitlines()

def fline_from_asm(fj,dta):
    '''a true fortran line dta structure comes in as:
    [initial stmt [continuation_lines]]

    continuation_lines are structures as well:
    [[comments] continuation_line]
    '''

    sep = fj(dta)
    return fline(sep[0],sep[1],sep[2])

class fortLine(object):
    def __init__(self,fmtobj=fixedfmt):
        self.fmtobj    = fmtobj
        a_stmt         = fmtobj.a_stmt
        comm           = fmtobj.comm
        cblk           = plus(comm)
        stmt           = treat(a_stmt,fmtobj.fjoin)
        self.a_line    = disj(cblk,stmt)
        self.cblk      = cblk
        self.stmt      = stmt

    def fline_from_line(self,line):
        comment_p = self.fmtobj.comment_p
        flow_line = flow.flow_line

        if comment_p(line):
            return cline([line])

        return fline(flow_line(line),line,[])
