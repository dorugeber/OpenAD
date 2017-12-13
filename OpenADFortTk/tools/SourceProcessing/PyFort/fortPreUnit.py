'''Use assembly patterns, and vgen to produce a unit
generator from a parsed line generator

patterns are as follows:

unit = seq(zo1(cblk),baseunit)

baseunit = seq(startunit,star(ustmt),zo1(cblk),endunit)

startunit, endunit are predicate patterns based on specific statement types

ustmt = seq(zo1(cblk),base_ustmt)

base_ustmt = disj(pred(! (startunit|endunit|contains)),seq(contains,unitlist))

unitlist = plus(unit) # note recursion here, also empty contains not allowed

So, toplevel unit generator = vgen(unit,stmt_generator)

NOTE: These are actually PRE-units, because no symbol table actions have been taken

TODO:
     take care of no unitstart (=program unit f f77)
     handle cblock at end of unit list ???
'''

from _Setup import *
import PyFort.fortStmts as fs
from PyFort.fortFile import Ffile
from PyFort.fortParse import parse_stmts,parse_cmnt
from PyUtil.assembler import *
from PyUtil.buf_iter import buf_iter

def instance_pred(*class_list):
    '''define a predicate that checks if argument is an instance of 1 of the
    classes listed in class_list
    '''
    def the_pred(x):
        for c in class_list:
            if isinstance(x,c):
                return True
        return False
    return the_pred

def contains0(s):
    'eta expansion to simulate letrec'
    return seq(contains,unitlist)(s)

startunit  = pred(instance_pred(fs.PUstart))
endunit    = pred(instance_pred(fs.PUend))
cblk       = pred(instance_pred(fs.Comments))
contains   = pred(instance_pred(fs.ContainsStmt))

_base_helper = instance_pred(fs.PUstart,fs.PUend,fs.ContainsStmt)

base_ustmt0 = pred(lambda x:not _base_helper(x))
base_ustmt  = disj(base_ustmt0,contains0)
ustmt       = seq(zo1(cblk),base_ustmt)
baseunit    = seq(startunit,star(ustmt),zo1(cblk),endunit)
unit        = disj(seq(zo1(cblk),baseunit),cblk)
unitlist    = plus(unit)

def fortPreUnitIterator(fileName,free):
    if free: inputFormat = 'free'
    else: inputFormat = 'fixed'
    return vgen(unit,buf_iter(Ffile.file(fileName,parse_cmnt,parse_stmts,inputFormat).lines))

if __name__ == '__main__':
    from _Setup.utest import *
    from itertools import *
    from PyUtil.flatten import flatten,eflatten

    fn0 = 'units1.f90'
    i0  = fortPreUnitIterator(fname_t(fn0),True)
    fn1 = 'units2.f90'
    i1  = fortPreUnitIterator(fname_t(fn1),True)
    fn2 = 'units3.f90'
    i2  = fortPreUnitIterator(fname_t(fn2),True)
