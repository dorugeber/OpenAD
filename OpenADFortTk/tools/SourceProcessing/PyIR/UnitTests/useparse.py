from _Setup import *
from PyFort.fortScan import scan1
from PyFort.fortExp import Exp
from PyFort.fortStmts import parse

def scan(s):
    (v,rst) = scan1.scan(s)
    if rst:
        raise Exception("incomplete scan '%s', rst = '%s'" % (s,rst))
    return v

def ep(s):
    (v,rst) = Exp(filter(lambda x: x != ' ',scan(s)))
    if rst:
        raise Exception("incomplete Exp parse: '%s', rst = %s" % (s,rst))
    return v

def pps(s):

    return parse(scan(s),lineNumber=0)
#
#    (t,rst) = parse(scan(s))
#    if not rst:
#        raise("parse failure: '%s', leaves rst = %s" % (s,rst))
#    return t
