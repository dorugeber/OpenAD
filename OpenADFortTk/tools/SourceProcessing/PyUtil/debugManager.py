'''
utility for managing debugging verbosity on a global level 
'''

from _Setup import *
import PyUtil.enum as enum

class DebugManager(object):

    _verbose = False

    _quiet = False

    _check = False

    @staticmethod
    def setVerbose(isVerbose):
        DebugManager._verbose = isVerbose

    @staticmethod
    def setQuiet(aBool):
        DebugManager._quiet = aBool

    @staticmethod
    def setCheck(aBool):
        DebugManager._check = aBool

    @staticmethod
    def check():
        return DebugManager._check

    _progress = False
    @staticmethod
    def dumpProgress():
        DebugManager._progress=True
        
    _processedFile = ""

    @staticmethod
    def setProcessedFile(aFileName):
        DebugManager._processedFile = aFileName
        if (DebugManager._progress):
            sys.stdout.write('SourceProcessing: PROGRESS: start with file '+ DebugManager._processedFile+'\n')

    @staticmethod
    def processedFile():
        return DebugManager._processedFile

    WarnType = enum.Enum(['undefined','implicit','hoisting','ifStmtToIfConstr','controlFlow','nesting','noDefinition','activeIO'])
        
    _warnOnlyTypeList=[]
    
    @staticmethod
    def warnOnlyOn(theWarnTypeList):
        for w in theWarnTypeList:
            DebugManager._warnOnlyTypeList.append(eval('DebugManager.WarnType.'+w))
        
    @staticmethod
    def warning(warningMessage,lineNumber=0,warnType=WarnType.undefined):
        if ((not DebugManager._quiet and not DebugManager._warnOnlyTypeList) 
            or 
            (DebugManager._warnOnlyTypeList and warnType in DebugManager._warnOnlyTypeList)): 
            sys.stderr.write('SourceProcessing: WARNING: '+warningMessage+' ('+DebugManager.processedFile())
            if (lineNumber>0):
                sys.stderr.write(':'+str(lineNumber))
            sys.stderr.write(')\n')
            sys.stderr.flush()

    @staticmethod
    def debug(debugMessage,outStream=sys.stdout,newLine=True,lineNumber=None):
        import inspect
        if (DebugManager._verbose):
            newLineStr = newLine and '\n' \
                                  or ''
            lineNumberStr = ''
            if lineNumber :
                lineNumberStr += '[Line '+str(lineNumber)+']: '
            frame = inspect.currentframe().f_back
            func = frame.f_code
            prefix="("
            mi=inspect.getmoduleinfo(func.co_filename)
            if mi:
                prefix+=mi.name+":"
            prefix+=func.co_name+":"
            prefix+=func.co_filename[func.co_filename.rfind('SourceProcessing')+len('SourceProcessing')+1:]+":"
            prefix+=str(frame.f_lineno)+")"
            outStream.write(prefix+lineNumberStr+debugMessage+newLineStr)
            outStream.flush()

