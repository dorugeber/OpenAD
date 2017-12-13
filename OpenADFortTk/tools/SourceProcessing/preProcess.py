#!/usr/bin/env python
'''
canonicalization
'''
import sys
import os
import datetime
import tempfile
import shutil
from optparse import OptionParser

from PyUtil.errors import UserError, ScanError, ParseError, LogicError
from PyUtil.assembler import AssemblerException
from PyUtil.l_assembler import AssemblerException as ListAssemblerException
from PyUtil.debugManager import DebugManager
from PyUtil.symtab import Symtab,SymtabError
from PyUtil.options import addCanonOptions,CanonOptErrors,setCanonFlags
from PyUtil.typetab import globalTypeTable               


from PyFort.fortUnit import Unit,fortUnitIterator
from PyFort.fortFile import Ffile
import PyFort.fortStmts as fs
from PyFort.inference import InferenceError
from PyFort.intrinsic import getNonStandard,useNonStandard

from Canon.canon import UnitCanonicalizer,CanonError
from Canon.subroutinizedIntrinsics import makeSubroutinizedIntrinsics,SubroutinizeError,getModuleName
from Canon.function2subroutine import FunToSubError

sys.setrecursionlimit(1500)

ourOutFileNameList=[]
ourOutFileHandle=None

def cleanup(config):
    import os 
    if  ourOutFileHandle and not ourOutFileHandle.closed : 
        ourOutFileHandle.close()
    if (not config.noCleanup):
        for fileName in ourOutFileNameList:
            if os.path.exists(fileName):
                try: 
                    os.remove(fileName)
                except:
                    print >>sys.stderr,'Cannot remove output file '+fileName

def mkOutputDir(config,head):
    outputDirectory = config.pathPrefix+head+config.pathSuffix
    if outputDirectory == '':
        outputDirectory = './'
    if not os.path.exists(outputDirectory): 
        os.makedirs(outputDirectory)
    return outputDirectory

def main():
    global ourOutFileNameList
    global ourOutFileHandle
    usage = '%prog [options] <input_file> [additional input files]'
    opt = OptionParser(usage=usage)
    addCanonOptions(opt)
    config, args = opt.parse_args()

    startTime=None
    if (config.timing):
        startTime=datetime.datetime.utcnow()

    CanonOptErrors(opt,config,args)
    inputFileList = args
    setCanonFlags(config)

    try: 
        if (not (config.outputFile or config.separateOutput)) :
            ourOutFileHandle = sys.stdout
            if (len(inputFileList) > 1): # output the file start pragma for the subroutinized intrinsics
                ourOutFileHandle.write('!$openad xxx file_start ['+getModuleName()+'.f90]\n')
                ourOutFileHandle.flush()
            for aUnit in makeSubroutinizedIntrinsics(False):
                aUnit.printit(ourOutFileHandle)
        currentInputFile = '<none>'
        for anInputFile in inputFileList:
            fileStartTime=None
            if (config.timing):
                fileStartTime=datetime.datetime.utcnow()

            if (config.outputFile and not ourOutFileHandle): 
                ourOutFileNameList.append(config.outputFile)
                ourOutFileHandle = open(config.outputFile,'w')
            if (config.separateOutput):
                if ourOutFileHandle: 
                    ourOutFileHandle.close()
                (head,tail) = os.path.split(anInputFile)
                (base,extension) = os.path.splitext(tail)
                outputDirectory = mkOutputDir(config, head) 
                newOutputFile = os.path.join(outputDirectory,base+config.filenameSuffix+".f90")
                ourOutFileNameList.append(newOutputFile)
                ourOutFileHandle = open(newOutputFile,'w')                                    
            currentInputFile = anInputFile
            if (len(inputFileList) > 1): # output the file start pragma
                ourOutFileHandle.write('!$openad xxx file_start ['+anInputFile+']\n')
                ourOutFileHandle.flush()
            for aUnit in fortUnitIterator(anInputFile,config.inputFormat):
                UnitCanonicalizer(aUnit).canonicalizeUnit().printit(ourOutFileHandle)
                #print aUnit.symtab.debug()
                #print globalTypeTable.debug()
                globalTypeTable.cleanUpUnitTypeEntries()
                #print globalTypeTable.debug()
                globalTypeTable.verifyTypeEntries()    
            if (config.progress):
                msg='SourceProcessing: PROGRESS: done  with file '+anInputFile
                if (config.timing):
                    nTime=datetime.datetime.utcnow()
                    msg+=' took: '+str(nTime-fileStartTime)
                    fileStartTime=nTime
                print msg
        if config.outputFile or config.separateOutput:
            ourOutFileHandle.close()
            if config.separateOutput:
                outputDirectory = mkOutputDir(config, '') 
                newOutputFile = os.path.join(outputDirectory,getModuleName()+'.f90')
            if config.outputFile :
                newOutputFile=tempfile.mktemp()
            ourOutFileNameList.append(newOutputFile)
            ourOutFileHandle=open(newOutputFile,'w')
            if (len(inputFileList) > 1): # output the file start pragma for the subroutinized intrinsics
                ourOutFileHandle.write('!$openad xxx file_start ['+getModuleName()+'.f90]\n')
                ourOutFileHandle.flush()
            for aUnit in makeSubroutinizedIntrinsics(True):
                aUnit.printit(ourOutFileHandle)
            if (config.outputFile):
                oFile=open(config.outputFile)
                ourOutFileHandle.write(oFile.read())
                oFile.close
            ourOutFileHandle.close()
            if (config.outputFile):
                shutil.move(newOutputFile,config.outputFile)
        if (config.timing):
            print 'SourceProcessing: timing: '+str(datetime.datetime.utcnow()-startTime)
    except (CanonError,SymtabError,UserError,ScanError,ParseError,LogicError,InferenceError,AssemblerException,ListAssemblerException,FunToSubError),e:
        sys.stderr.write(str(e)+'\n')
        cleanup(config)
        return 1
    except RuntimeError,e:
        if (len(e.args)>=1 and "maximum recursion depth exceeded" <= e.args[0]):
            print >>sys.stderr,'\nERROR: RuntimeError: python interpreter failed with: ',e.args[0]
            print >>sys.stderr,'\twhich can happen for deeply nested bracketing. Try to set a value larger than the current '+str(sys.getrecursionlimit())+' with --recursionLimit .' 
            cleanup(config)
            return 1
        else:
            raise e
    return 0

if __name__ == "__main__":
    # use the following to generate profile data that can be viewed e.g. with RunSnakeRun
    # by calling:   python /usr/lib/python2.7/site-packages/runsnakerun/runsnake.py  preProcess.profile
    # ----------
    # import cProfile
    # cProfile.runctx( 'main()', globals(), locals(), filename="preProcess.profile" )
    # -- else --
    sys.exit(main())

