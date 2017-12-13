from _Setup import *

from PyUtil.flatten import flatten
from PyUtil.chomp import chomp
from PyFort.kill_bang import *
from PyUtil.assembler import *
from PyFort.fortScan import postluded_lo_q_patn, preluded_ro_q_patn, inputLineLength

_cont_re = r'& (([\s]*[!].*) | \s*) $'
_cont_re = re.compile(_cont_re,re.X)

_comment_re = r'\s* !'
_blank_re   = r' \s* $'

_comm_re    = r'(%s | %s)' % (_comment_re,_blank_re)
_comm_re    = re.compile(_comm_re,re.X)

def is_cont(line):
    '''check to see if line is continued (assuming no bang comments)'''
    (l,r) = kill_bang_comment(line)
    return _cont_re.search(l)

def is_comm(line):
    '''check to see if line is a comment line'''
    return _comm_re.match(line)

_delimROpenREs = {
    "'":re.compile(preluded_ro_q_patn("'"),re.X),
    '"':re.compile(preluded_ro_q_patn('"'),re.X)}
_delimLOpenREs = {
    "'":re.compile(postluded_lo_q_patn("'"),re.X),
    '"':re.compile(postluded_lo_q_patn('"'),re.X)}

def hasOpenQuotation(line,delimiter):
    '''check to see if line has an open quatation:
       if delimiter is not None then we expect line to be a continuation 
          of an open quotation and look for a right-open quotation 
       if delimiter is None then we look for left - open quotation'''
    if (delimiter and  _delimLOpenREs[delimiter].match(line)):
        return (True,delimiter)
    else:
        for d,theRe in _delimROpenREs.items():
            if (theRe.match(line)):
                return (True,d)
    return (False,None)
        
_comment_p = is_comm

token_cont_re = r'\s* &'
token_cont_re = re.compile(token_cont_re,re.X)

def kill_cont(l):
    cont_match = _cont_re.search(l)
    if cont_match:
        return l[:cont_match.start()]
    return l

def kill_token_cont(l):
    token_match = token_cont_re.match(l)
    if token_match:
        return l[token_match.end():]
    return ' '+l.lstrip()

sq = "'"
dq = '"'
eolc = '!'
quote_re = re.compile(r'(%s)'%sq+'|'+r'(%s)'%dq+'|'+r'(%s)'%eolc,re.X)
sq_re = re.compile(r'(%s)'%sq,re.X)
dq_re = re.compile(r'(%s)'%dq,re.X)
sqCount=0
dqCount=0
def _process_conts(cl):
    global sqCount
    global dqCount
    index=0
    eol_comm=None
    while index<len(cl):
        # if no open quotes
        if (sqCount==0 and dqCount==0):
            match = re.search(quote_re,cl[index:])
            if match:
                cur_index=match.start()+index
                if (cl[cur_index]==sq):
                    sqCount+=1
                elif (cl[cur_index]==dq):
                    dqCount+=1
                elif (cl[cur_index]==eolc):
                    # eol comment
                    (new_cl,eol_comm) = kill_bang_comment(cl[index:])
                    cl=cl[:index]+new_cl
                index=cur_index+1
            else:
                (new_cl,eol_comm) = kill_bang_comment(cl[index:])                    
                cl=cl[:index]+new_cl
                break;
        # if open single quote
        if (sqCount==1):
            match = re.search(sq_re,cl[index:])
            if match:
                # quote closed; update index to continue searching line
                index = match.start()+index+1
                sqCount-=1
            else:
                # quote left open; continue to next line
                (new_cl,eol_comm) = kill_bang_comment_lo_q(cl[index:],sq)
                cl=cl[:index]+new_cl
                break;
        # if open double quote
        elif (dqCount==1):
            match = re.search(dq_re,cl[index:])
            if match:
                # quote closed; update index to continue searching line
                index = match.start()+index+1
                dqCount-=1
            else:
                # quote left open; continue to next line
                (new_cl,eol_comm) = kill_bang_comment_lo_q(cl[index:],dq)
                cl=cl[:index]+new_cl
                break;
    return (cl,eol_comm)

def _fjoin(asm):
    '''assemble a logical line from the assembled
    lines come in from the assembler as

    [[continuation]*,primary line]

    continuations are also structured as
    [continued_line, [comments]*]
    '''

    rawline = ''.join(flatten(asm))
    internal_comments = []
    (conts,prim) = asm
    current_line = []
    initial_line = True
    for (cl,comments) in conts:
        if inputLineLength != 0:
            if cl[inputLineLength:] != '':
                print >> sys.stderr, "The following text is being cut because the statement exceeded the specified line length: "+jl[inputLineLength:]
            cl = cl[:inputLineLength]
        index=0
        eol_comm=None
        (cl,eol_comm)=_process_conts(cl)
        if eol_comm:
            internal_comments.append(eol_comm)
        internal_comments.extend(comments)
        if initial_line:
            cl = kill_cont(cl)
            initial_line = False
        else:
            cl = kill_cont(kill_token_cont(cl))
        current_line.append(cl)
    # dealing with prim
    if not initial_line:
        prim = kill_token_cont(prim)
    prim=chomp(prim)
    (prim,eol_comm)=_process_conts(prim)
    if (dqCount!=0 or sqCount!=0):
        raise Exception('open quotation bug for '+str(asm))
    if eol_comm:
        internal_comments.append(eol_comm)
    current_line.append(prim)

    joined    = ''.join(current_line)
    return (rawline,joined,internal_comments)

cont = pred(is_cont)
_comm = pred(is_comm)
prim = pred(lambda l: not (is_comm(l) or is_cont(l)))

_a_stmt = seq(star(seq(cont,star(_comm))),prim)

class freefmt(object):
    'hold helper functions for free fmt'

    a_stmt    = staticmethod(_a_stmt)
    comm      = staticmethod(_comm)
    comment_p = staticmethod(_comment_p)
    fjoin     = staticmethod(_fjoin)
