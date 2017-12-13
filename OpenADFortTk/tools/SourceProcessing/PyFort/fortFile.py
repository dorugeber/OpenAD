'''
Duplicate (most of) the functionality of perl Ffile.pm

In other words, read in a Fortran file, assembling the
line objects.
Provide mappers, rewriters, string conversion facilities
'''
from _Setup import *
from cStringIO import StringIO
from PyFort.fortLine  import fortLine,cline,fline
from PyUtil.assembler import vgen,treat,disj
from PyUtil.buf_iter  import buf_iter
from PyUtil.errors import UserError
from PyFort.freefmt       import freefmt
from PyFort.fixedfmt      import fixedfmt
from PyFort.process_fort_stmt import process_fort_stmt,process_fort_cmnt
import PyFort.flow as flow
import os
import re
import tempfile

def _ident(s):
    return [s]

class Ffile(object):
    def __init__(self,fobj,free=False,c_action=cline,s_action=fline,isTemp=False):
        fmt      = (free and freefmt) or fixedfmt
        fl       = fortLine(fmt)
        s      = lambda x:process_fort_stmt(x,self.rawBufIter.myCounter,s_action)
        c      = lambda x:process_fort_cmnt(x,self.rawBufIter.myCounter,c_action)
        cblk   = treat(fl.cblk,c)
        # produces multiple statements, if multiple statements were on
        # the same line in the original file
        stmt   = treat(fl.stmt,s)
        a_line = disj(cblk,stmt)

        self.rawBufIter = buf_iter(fobj)
        self.lines = vgen(a_line,self.rawBufIter,True)
        self.fobj  = fobj
        self.isTemp = isTemp

    @staticmethod
    def get_format(ext):
        if ext in ['.f90','.f95','.f03']:
            return 'free'
        return 'fixed'

    @staticmethod
    def get_extension(format):
        if (format=='free'):
            return '.f90'
        return '.f'

    __includeSearchPath=[]
    
    @staticmethod
    def setIncludeSearchPath(directoryList):
        Ffile.__includeSearchPath.extend(directoryList)
    
    __includePatn=re.compile('^\s*INCLUDE\s+[\'\"]([\w\.]+)[\'\"].*',re.I)

    @staticmethod
    def inject_include(fileName,includingFileName):
        ''' handle fortran include lines by explicitly injecting the file contents '''
        def searchPath(fileName):
            for p in Ffile.__includeSearchPath:
                if (os.path.isfile(os.path.join(p,fileName))):
                    return os.path.join(p,fileName)
            return fileName
        
        try:
            fileHandle=open(searchPath(fileName),'r')
        except IOError:
            msg="Error cannot open file named: "+fileName
            if includingFileName:
                msg+=" included by "+includingFileName
            raise UserError(msg)
        tempFileHandle=None
        tempFileName=None
        secondInputHandle=None
        linesAhead=0
        for line in fileHandle.readlines():
            matchResult=re.match(Ffile.__includePatn,line)
            if matchResult:
                if not tempFileHandle: 
                    tempFileName=tempfile.mktemp()
                    tempFileHandle=open(tempFileName,'w')
                    secondInputHandle=open(searchPath(fileName),'r')
                    while (linesAhead>0):
                        tempFileHandle.write(secondInputHandle.readline())
                        linesAhead-=1
                    secondInputHandle.close()                
                nestedIncludeName=matchResult.group(1)
                try: 
                    (nestedIncludeHandle,isTemp)=Ffile.inject_include(nestedIncludeName,fileName)
                    tempFileHandle.writelines(nestedIncludeHandle.readlines())
                    nestedIncludeHandle.close()
                    if (isTemp):
                        os.remove(nestedIncludeHandle.name)
                except UserError, e:
                    if includingFileName:
                        e.msg+=" included by "+includingFileName
                    raise e
            else : 
                if tempFileHandle: 
                    tempFileHandle.write(line)
                else:
                    linesAhead+=1
        if tempFileHandle:
            tempFileHandle.close()
            tempFileHandle=open(tempFileHandle.name,'r')
            fileHandle.close()
            return (tempFileHandle,True)
        fileHandle.seek(0)
        return (fileHandle,False)
            
        
    @staticmethod
    def file(name,c_action=cline,s_action=fline,inputFormat=None):
        import PyUtil.debugManager
        try:
            (f,isTemp)=Ffile.inject_include(name,None)
        except IOError:
          msg="Error cannot open file named: "+name
          raise UserError(msg)
        PyUtil.debugManager.DebugManager.setProcessedFile(name)
        #set formatting based on name extension
        ext = os.path.splitext(name)[1]
        if not inputFormat:
            inputFormat = Ffile.get_format(ext)
        flow.setInputFormat(inputFormat)
        return Ffile(f,inputFormat=='free',c_action,s_action,isTemp=isTemp)

    @staticmethod
    def here(str,free=False,c_action=cline,s_action=fline):
        return Ffile(StringIO(str),free,c_action,s_action)

    def printit(self,out=sys.stdout,free=False):
        fmtLead=''
        if not free:
            fmtLead='      '
        for l in self.lines:
            line = fmtLead+l.lead+l.rawline
            print >> out,line
    
    def write(self,fname):
        ff = open(fname,'w')

        self.printit(ff)

        ff.close()

    def map(self,lexi,*args,**kwargs):
        for (cls,meth) in lexi:
            cls.map = meth
        for l in self.lines:
            for ll in l.map(*args,**kwargs):
                yield ll
        for (cls,meth) in lexi:
            cls.map = _ident

    def __del__(self):
        if self.isTemp:
            self.fobj.close()
            os.remove(self.fobj.name)
