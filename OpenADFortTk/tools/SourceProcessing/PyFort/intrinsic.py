import PyFort.fortStmts as fs

'''
Module to list the intrinsic functions, and
a predicate to test for a function being an intrinsic
'''
__intrinsics = (
    'abs',
    'achar',
    'acos',
    'adjustl',
    'adjustr',
    'aimag',
    'all',
    'allocated',
    'alog',
    'amax0',
    'amax1',
    'amin0',
    'amin1',
    'any',
    'asin',
    'associated',
    'atan',
    'atan2',
    'close',
    'cmplx',
    'cos',
    'cosh',
    'dabs',
    'dacos',
    'dble',
    'dcos',
    'dexp',
    'dfloat',
    'dlog',
    'dmax1',
    'dmin1',
    'dmod',
    'dot_product',
    'dsign',
    'dsin',
    'dsqrt',
    'exp',
    'float',
    'iabs',
    'iachar',
    'iand',
    'ichar',
    'idint',
    'index',
    'int',
    'ior',
    'kind',
    'lbound',
    'len',
    'len_trim',
    'lge',
    'lgt',
    'lle',
    'llt',
    'log',
    'log10',
    'max',
    'max0',
    'maxloc',
    'maxval',
    'min',
    'min0',
    'minloc',
    'minval',
    'mod',
    'nint',
    'null',
    'open',
    'present',
    'read',
    'real',
    'repeat',
    'reshape',
    'scan',
    'shape',
    'sign',
    'sin',
    'sinh',
    'size',
    'sqrt',
    'sum',
    'tan',
    'tanh',
    'transfer',
    'transpose',
    'trim',
    'ubound',
    )

__nonStandard = (
                 'etime',
		 'free',
                 'getuid',
                 'getpid',
                 'hostnam',
                 'hostnm',
                 'loc',
                 'malloc',
                 'sleep',
                 'time'   
                 )
__builtInTypes = [
    (fs.CharacterStmt,4),
    (fs.LogicalStmt,4),
    (fs.IntegerStmt,1),
    (fs.IntegerStmt,2),
    (fs.IntegerStmt,4),
    (fs.IntegerStmt,8),
    (fs.RealStmt,4),
    (fs.DoubleStmt,8),
    (fs.ComplexStmt,8),
    (fs.DoubleCplexStmt,16),
    (fs.ExternalStmt,0) #special case external subroutine
]

def getNonStandard():
    return __nonStandard

__useNonStandard=[]
def useNonStandard(aList):
    global __useNonStandard
    if aList: 
        __useNonStandard=aList

def isUsedNonStandard(op):
    global __useNonStandard
    return (op.lower() in __useNonStandard)

def getBuiltInTypes():
    return __builtInTypes

def is_intrinsic(op):
    return (op.lower() in __intrinsics) or (op.lower() in __useNonStandard)

__inquiry = (
    'any',
    'associated',
    'allocated',
    'kind',
    'lbound',
    'present',
    'shape',
    'size',
    'ubound'
    )

def is_inquiry(op):
    return op.lower() in __inquiry

def getGenericName(op):
    ''' convert F77 archaic specific names to generic names '''
    if not is_intrinsic(op):
        raise Exception("argument "+op+" is not an intrinsic")
    archaicName=op.lower()
    if (archaicName[0] == 'a' and archaicName[-1] in ('0','1') and archaicName[1:-1] in ('max','min') ):
        return archaicName[1:-1]
    elif (archaicName[0] == 'd' and archaicName[-1] == '1' and archaicName[1:-1] in ('max','min') ):
        return archaicName[1:-1]
    elif (archaicName[-1] in ('0','1') and archaicName[:-1] in ('max','min') ):
        return archaicName[:-1]
    elif (archaicName[0] == 'd' and archaicName[1:] in ('abs','acos','cos','cosh','log','mod','sign','sin','sqrt') ):
        return archaicName[1:]
    elif (archaicName[0] == 'i' and archaicName[1:] in ('abs') ):
        return archaicName[1:]
    elif (archaicName[0] == 'a' and archaicName[1:] in ('log') ):
        return archaicName[1:]
    elif (archaicName == 'idint') :
        return 'int'
    elif (archaicName == 'dfloat') :
        return 'dble'
    else :
        return archaicName
    
def isPolymorphic(op):
    '''  true for intrinscis with generic names '''
    if not is_intrinsic(op):
        raise Exception("argument "+op+" is not an intrinsic")
    return op.lower() in ('max','min','sign')
