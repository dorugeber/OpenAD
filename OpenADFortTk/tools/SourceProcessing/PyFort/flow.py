'''
Module to control the flow_line method
'''
from PyFort.fixedfmt     import fixedfmt
from PyFort.freefmt      import freefmt
from PyUtil.chomp import chomp

_fixedFormatStart=' ' * 6
_freeFormatStart=''
_fixed_line_len = 72
_fixed_comment = 'C '

def _fixed_flow_line(l,cont='+'):
    '''given a long line, write it out as a series of continued lines'''
    comment_p = fixedfmt.comment_p
    fl1 = outputLineLength
    fl2 = fl1 - 6
    l1 = chomp(l)
    if comment_p(l) or (len(l1) <= fl1):
        return l
    
    rv = l1[0:fl1] + '\n'
    rem = l1[fl1:]
    while len(rem) > fl2:
        tmp  = rem[0:fl2]
        rv  += ' ' * 5 + cont + tmp + '\n'
        rem  = rem[fl2:]
    if len(rem) > 0:
        rv  += ' ' * 5 + cont + rem
    return rv

_free_line_len = 80
_free_comment = '! '

def _free_flow_line(l,cont='&'):
    'given a long line l, write it out as a series of continued lines'
    comment_p = freefmt.comment_p
    fll = outputLineLength
    fl2 = fll - 6
    l1 = chomp(l)
    if comment_p(l) or (len(l1) <= fll):
        return l

    rv = l1[0:fll] + cont + '\n'
    rem = l1[fll:]
    while len(rem) > fl2:
        tmp  = rem[0:fl2]
        rv  += ' ' * 5  + cont + tmp + cont + '\n'
        rem  = rem[fl2:]
    if len(rem) > 0:
        rv  += ' ' * 5  + cont + rem + '\n'
    return rv

def flow_comment(l):
    '''given a long comment, write it out as a series of continued lines'''
    l1 = chomp(l)
    fl1 = outputLineLength
    fl2 = outputLineLength-2
    rv = l1[0:fl1] + '\n'
    rem = l1[fl1:]
    while len(rem) > fl2:
        tmp = rem[0:fl2]
        rv += commentInit+tmp+'\n'
        rem = rem[fl2:]
    if len(rem) > 0:
        rv += commentInit+ rem+'\n'
    return rv

# the default is fixed format:
flow_line = _fixed_flow_line
formatStart = _fixedFormatStart
outputLineLength = _fixed_line_len
outputLineLengthSet = False
commentInit = _fixed_comment

inputFormat = 'fixed'
outputFormat = 'free'
outputFormatSet = False

def setInputFormat(input_format='fixed'):
    '''sets input and output formats. When the output format is not specified, it defaults to be the same as the input format. When the input format is not specified, it defaults to fixed format.'''
    global inputFormat, outputFormatSet
    inputFormat = input_format

# default outputFormat is free
def setOutputFormat(output_format):
    '''sets the output format'''
    global outputFormat, outputFormatSet
    outputFormat = output_format
    _setFormatDefaults(output_format=='free')

def _setFormatDefaults(switch=True):
    '''sets the defaults for the given output format'''
    global flow_line
    flow_line = (switch and _free_flow_line) or _fixed_flow_line
    global formatStart
    formatStart = (switch and _freeFormatStart) or _fixedFormatStart
    global commentInit
    commentInit = (switch and _free_comment) or _fixed_comment
    global outputLineLengthSet
    # if outputLineLength was previously explicitly set, do nothing
    if not outputLineLengthSet:
        global outputLineLength
        outputLineLength = (switch and _free_line_len) or _fixed_line_len

def setOutputLineLength(length):
    global outputLineLength
    global outputLineLengthSet
    outputLineLength = length
    outputLineLengthSet = True

