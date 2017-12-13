from PyUtil.debugManager import DebugManager

from PyFort.fortUnit import Unit
from PyFort.fortExp import App,Ops,Zslice,LoopControl
import PyFort.fortStmts as fs
import PyFort.intrinsic as intrinsic
import PyFort.flow as flow
from PyUtil.symtab import Symtab 

class SubroutinizeError(Exception):
    '''exception for errors that occur during the subroutinization of intrinsics'''
    def __init__(self,msg):
        self.msg  = msg

_subroutinizable={'max':2,'maxval':1,'min':2,'minval':1}
_rankCombinations=[[([a]) for a in xrange(0,8)],[([a,b]) for a in xrange(0,8) for b in xrange(0,8)]]

def shouldSubroutinize(aFunction):
    DebugManager.debug('subroutinizedIntrinsics.shouldSubroutinize called on "'+str(aFunction)+'"')
    return intrinsic.getGenericName(aFunction.head) in _subroutinizable.keys()

_requiredSubroutinizedIntrinsics=[]

def markRequired(key,typeClass,aRanks):
    if ((key,typeClass,aRanks) not in _requiredSubroutinizedIntrinsics):
        #print 'marking'+str(key)+str(typeClass)+str(aRanks)
        _requiredSubroutinizedIntrinsics.append((key,typeClass,aRanks))

call_prefix  = 'oad_s_'
typeList=[fs.RealStmt,fs.DoubleStmt,fs.IntegerStmt]

def getModuleName():
    return 'OAD_intrinsics'

def makeName(intrName):
    return call_prefix + intrName     

def __makeNameImpl(intrName,typeClass,aRanks):
    nonNull=lambda x: x!=0
    rankSuffix=''
    if (any(map(nonNull,aRanks))):
        rankSuffix='_'+''.join(str(r) for r in aRanks)
    return makeName(intrName)+'_'+typeClass.kw[0]+rankSuffix

def __skipThisType(onlyRequired,key,typeClass,aRanks):
    return ((onlyRequired
             and
             (((key,typeClass,aRanks) not in _requiredSubroutinizedIntrinsics)
              or 
              (Symtab.getRealTypeDefault()[0]==fs.DoubleStmt
               and
               typeClass==fs.RealStmt
               and
               (key, fs.DoubleStmt,aRanks) in _requiredSubroutinizedIntrinsics)))
            or
            (not onlyRequired
             and
             Symtab.getRealTypeDefault()[0]==fs.DoubleStmt
             and
             typeClass==fs.RealStmt))
   

def makeSubroutinizedIntrinsics(onlyRequired):
    ''' this is incomplete '''
    keyList=_subroutinizable.keys()
    if onlyRequired:
        uniq=set([k for k,t,aRanks in _requiredSubroutinizedIntrinsics])
        keyList=[k for k in uniq] 
    newUnit=Unit()
    newUnit.uinfo=fs.ModuleStmt(getModuleName())
    empty=True
    for k in keyList:
        nameList=[]
        newUnit.decls.append(fs.InterfaceStmt(call_prefix+k,lead='  '))
        for t in typeList:
            for aRanks in _rankCombinations[_subroutinizable[k]-1]: 
                if (__skipThisType(onlyRequired,k,t,aRanks)) :
                    #print 'skipping'+str(k)+str(t)+str(aRanks)
                    continue
                nameList.append(__makeNameImpl(k,t,aRanks))
                empty=False
        newUnit.decls.append(fs.ProcedureStmt('module',nameList,lead='    '))
        newUnit.decls.append(fs.EndInterfaceStmt(lead='  '))
    if not empty: 
        newUnit.decls.append(fs.ContainsStmt(lead='  '))
    for k in keyList:
        for t in typeList:
            for aRanks in _rankCombinations[_subroutinizable[k]-1]: 
                if (__skipThisType(onlyRequired,k,t,aRanks)) : 
                    continue
                newSUnit=Unit()
                if (k in ['max','min']) : 
                    makeSubroutinizedMaxOrMin(newSUnit,k,t,'    ',aRanks)
                else:
                    makeSubroutinizedMaxvalOrMinval(newSUnit,k,t,'    ',aRanks)
                newUnit.ulist.append(newSUnit)            
    newUnit.end.append(fs.EndModuleStmt())
    return [newUnit]

def makeSubroutinizedMaxOrMin(newUnit,aKey,aTypeClass,indent,aRanks):
    newUnit.uinfo=fs.SubroutineStmt(__makeNameImpl(aKey,aTypeClass,aRanks),
                                    ["a0","a1","r"],
                                    lead=indent)
    maxRank=0
    argArrIndx=0
    for arg in xrange(0,2):
        mods=[App('intent',['in'])]
        dArgs=[Zslice() for  i in xrange(1,aRanks[arg]+1)]
        if aRanks[arg]>0 :
            mods.append(App('dimension',dArgs))
            argArrIdx=arg
        newUnit.decls.append(aTypeClass(None,
                                        mods,
                                        ['a'+str(arg)],
                                        lead=indent+'  '))
        maxRank=max(maxRank,aRanks[arg])
    mods=[App('intent',['out'])]
    dArgs=[Zslice() for  i in xrange(1,maxRank+1)]
    if maxRank>0 :
        mods.append(App('dimension',dArgs))
    newUnit.decls.append(aTypeClass(None,
                                    mods,
                                    ['r'],
                                    lead=indent+'  '))
    if maxRank>0:
        newUnit.decls.append(fs.IntegerStmt(None,[],['i'+str(i) for i in xrange(1,maxRank+1)],lead=indent+'  '))
        newUnit.decls.append(fs.IntegerStmt(None,[],['l'+str(i) for i in xrange(1,maxRank+1)],lead=indent+'  '))
        newUnit.decls.append(fs.IntegerStmt(None,[],['u'+str(i) for i in xrange(1,maxRank+1)],lead=indent+'  '))
        for i in xrange(1,maxRank+1):
            newUnit.execs.append(fs.AssignStmt('l'+str(i),App('lbound',['a'+str(argArrIdx),str(i)]),lead=indent+'  '))
            newUnit.execs.append(fs.AssignStmt('u'+str(i),App('ubound',['a'+str(argArrIdx),str(i)]),lead=indent+'  '))
        for i in xrange(1,maxRank+1):
            newUnit.execs.append(fs.DoStmt(None,None,
                                           LoopControl('i'+str(i),
                                                       'l'+str(i),
                                                       'u'+str(i),
                                                       None),
                                           lead=indent+'  '))
        testExpr=None
        ops=[]
        for arg in xrange(0,2):
            if aRanks[arg]:
                ops.append(App('a'+str(arg),['i'+str(i) for i in xrange(1,aRanks[arg]+1)]))
            else:
                ops.append('a'+str(arg))
        if (aKey=='max'):
            testExpr=Ops('>',ops[0],ops[1])
        else:
            testExpr=Ops('<',ops[0],ops[1])
        rOp=App('r',['i'+str(i) for i in xrange(1,maxRank+1)])
        newUnit.execs.append(fs.IfThenStmt(testExpr,lead=indent+'  '))
        newUnit.execs.append(fs.AssignStmt(rOp,ops[0],lead=indent+'    '))
        newUnit.execs.append(fs.ElseStmt(lead=indent+'  '))
        newUnit.execs.append(fs.AssignStmt(rOp,ops[1],lead=indent+'    '))
        newUnit.execs.append(fs.EndifStmt(lead=indent+'  '))
        for i in xrange(1,maxRank+1):
            newUnit.execs.append(fs.EnddoStmt(None,lead=indent+'  '))
    else:
        testExpr = (aKey=='max') and Ops('>','a0','a1') or Ops('<','a0','a1')
        newUnit.execs.append(fs.IfThenStmt(testExpr,lead=indent+'  '))
        newUnit.execs.append(fs.AssignStmt('r','a0',lead=indent+'    '))
        newUnit.execs.append(fs.ElseStmt(lead=indent+'  '))
        newUnit.execs.append(fs.AssignStmt('r','a1',lead=indent+'    '))
        newUnit.execs.append(fs.EndifStmt(lead=indent+'  '))
    newUnit.end.append(fs.EndSubroutineStmt(lead=indent))

def makeSubroutinizedMaxvalOrMinval(newUnit,aKey,aTypeClass,indent,aRanks):
    newUnit.uinfo = fs.SubroutineStmt(__makeNameImpl(aKey,aTypeClass,aRanks),
                                      ['a','r'],
                                      lead=indent)
    mods=[App('intent',['in'])]
    dArgs=[Zslice() for  i in xrange(1,aRanks[0]+1)]
    if aRanks[0]>0 :
        mods.append(App('dimension',dArgs))
    newUnit.decls.append(aTypeClass(None,
                                    mods,
                                    ['a'],
                                    lead=indent+'  '))
    newUnit.decls.append(aTypeClass(None,
                                    [App('intent',['out'])],
                                    ['r'],
                                    lead=indent+'  '))
    newUnit.decls.append(fs.IntegerStmt(None,
                                        None,
                                        [fs._NoInit(App('i',aRanks))],
                                        lead=indent+'  '))
    locVersion = (aKey=='maxval') and 'maxloc' \
                                   or 'minloc'
    newUnit.execs.append(fs.AssignStmt('i',
                                       App(locVersion,['a']),
                                       lead=indent+'    '))
    newUnit.execs.append(fs.AssignStmt('r',
                                       App('a',[(App('i',[r])) for r in xrange(1,aRanks[0]+1)]),
                                       lead=indent+'    '))
    newUnit.end.append(fs.EndSubroutineStmt(lead=indent))


