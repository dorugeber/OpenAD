import sys,os

def preclip(s):
    'clip leading "\n" on string s'
    if s[0] == '\n':
        return s[1:]
    else:
        return s

def open_t(fname,mode='r',bufsize=-1):
    '''utility to open a file in the Tfiles directory'''
    return open(fname_t(fname),mode,bufsize)

def fname_t(fname):
    '''add the path to Tfiles to fname'''
    return os.path.join('UnitTests','Tfiles',fname)

from fortScan import scan1
from fortStmts import parse

def scan(s):
    (v,rst) = scan1.scan(s)
    if rst:
        raise "incomplete scan '%s', rst = '%s'" % (s,rst)
    return v

def pps(s):
    return parse(scan(s))
