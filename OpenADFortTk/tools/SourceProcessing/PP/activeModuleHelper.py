import itertools
from _Setup import *
from PyFort.fortStmts import Comments, UseAllStmt

##
# the name of the module containing the active type definition
#
__concreteTypeModuleName='OAD_active'

def getActiveModuleUse():
    return UseAllStmt(moduleName=__concreteTypeModuleName,renameList=None)

##
# adds a module or include statement to decls if necessary;
# it should put it in the top level unit if there are subunits to avoid littering the subunits 
#
def addReferenceIfNeeded(unit,decls,aDecl):
    if (unit.uinfo is None):
        return
    if (unit.parent) : # done in parent
        return
    lead=unit.uinfo.lead or ''
    ncdeclsIter=itertools.ifilter(lambda l: not isinstance(l, Comments),unit.decls)
    try : 
        lead=ncdeclsIter.next().lead
    except StopIteration, e:
        ncExecIter=itertools.ifilter(lambda l: not isinstance(l, Comments),unit.execs)
        try:
            lead=ncExecIter.next().lead
        except StopIteration, e: 
            if (unit.ulist):
                pass #  add it for the subunits that may need it
            else:
                return # no non-comment decls,  execs or subunits - don't bother
    aDecl.lead=lead
    decls.append(aDecl)
