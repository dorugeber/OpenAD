import sys,os

__mypath = sys.path[0]

from copy import copy as ocp
from copy import deepcopy as docp

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
    return os.path.join(__mypath,'UnitTests','Tfiles',fname)
