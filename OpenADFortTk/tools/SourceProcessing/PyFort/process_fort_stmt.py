from _Setup import *
import PyFort.flow as flow
import PyFort.fortStmts as fs
from PyUtil.flatten import flatten
from PyUtil.chomp import chomp

import re
import sys
_label_re = re.compile(r'(\s*)(\d+)')
_lead_re   = re.compile(r'(\s*)')

def process_fort_stmt(stmt_tuple,lineNumber,jlf):
    '''process a stmt tuple = (rawline,joined_line,internal_comments), using
    jlf routine on the jointed_line part
    convert tabs in line lead into spaces, depending on fixed or free format
    using format_linelead(jl)
    process the joined line
    determine the leading spaces, not including fixed flow spacing
    obj = jlf(joined_line)
    obj.rawline = raw
    obj.internal = internal_comments
    obj.label = label
    obj.lead = lead
    '''
    (raw,jl,intl) = stmt_tuple
    jl = format_linelead(jl)
    m = _label_re.match(jl)
    label = m and int(m.group(2))

    # lead shouldn't include fixed flow spacing (first 6 places)
    if (flow.inputFormat=='free'):
        linelead = m and m.end(0) or 0
        lead = linelead * ' '+_lead_re.match(jl[linelead:]).group(1)
        raw = jl[len(lead):].strip()
        init_len = len(lead)
    else:
        lead = _lead_re.match(jl[6:]).group(1)
        raw = jl[6:].lstrip()
        init_len = 6+len(lead)
    obj = jlf(jl[init_len:],lineNumber)
    ## if there were multiple statements in the line, than the obj will be a list
    # process each statement by assigning its lead,label and internal comments
    # rawline shouldn't be assigned, because each obj in the list already has one
    # assigned
    if isinstance(obj,list):
        obj[0].internal = intl
        obj[0].label = label
        for anObj in obj:
            anObj.lead = lead
    else:
        ## assign rawline, internal comments, label, and lead to the statement obj
        obj.rawline = obj.rawline
        obj.internal = intl
        obj.label = label
        obj.lead = lead
    return obj

def process_fort_cmnt(dta,lineNumber,cmnt_list_fn):
    """set the comment's rawline and return the fort statement comment object"""
    obj         = cmnt_list_fn(dta,lineNumber)
    obj.rawline = ''.join(flatten(dta))
    return obj

def format_linelead(jl):
    """convert tabs to spaces in the line's lead. if the input is in free
    format, convert tabs to 8 spaces. if it's in fixed, convert them so that
    the beginning of the line is indented 6 spaces according to the fixed
    format convention """
    if (flow.inputFormat=='free'):
        if jl[0] == '\t':
            jl = ' ' * 8 + jl[1:]
    else:
        i = 0
        while i < 6:
            if jl[i] == '\t':
                jl = ' '*6 + jl[i+1:]
                if i != 0:
                    jl = jl[:i-1] + jl
                break
            i += 1
    return jl
