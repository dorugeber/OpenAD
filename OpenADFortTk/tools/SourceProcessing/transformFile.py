#!/usr/bin/env python
'''
Rudimentary transformations on non-transformed files
'''
import os
import sys
from optparse import OptionParser
from PyUtil.options import addTransformFileOptions,TransformFileOptErrors,setTransformFileFlags
from PyUtil.errors import UserError, ScanError, ParseError
from PP.transformActiveVariables import TransformActiveVariables,TransformError
from PyFort.fortUnit import Unit,fortUnitIterator
from PyUtil.debugManager import DebugManager
from PyUtil.symtab import Symtab,SymtabError
from PyFort.fortFile import Ffile
import PyFort.fortStmts as fs
from PP.activeTypeHelper import ActiveTypeHelper

def cleanup(config,ourOutFileHandle):
    import os 
    if  ourOutFileHandle and not ourOutFileHandle.closed : 
        ourOutFileHandle.close()
    if ((not config.noCleanup) and (not config.outputFile is None) and  os.path.exists(config.outputFile)):
        try: 
            os.remove(config.outputFile)
        except:
            print >>sys.stderr,'Cannot remove output file '+config.outputFile
 
def main():
    usage = '%prog <input_file>'
    opt = OptionParser(usage=usage)
    addTransformFileOptions(opt)
    config,args = opt.parse_args()

    TransformFileOptErrors(config,args)
    setTransformFileFlags(config)
    ActiveTypeHelper.setPlaceholderActiveType(TransformActiveVariables._replacement_type)
    inputFileList = args
    if config.outputDir :
        if not os.path.exists(config.outputDir): os.makedirs(config.outputDir)

    currentFile = config.vardefs
    ourOutFileHandle=None
    try:
        TransformActiveVariables.getActiveVars(config.vardefs, config.inputFormat)
        # only one input file
        if len(inputFileList) == 1 :
            currentFile = inputFileList[0]
            ourOutFileHandle = config.outputFile and open(config.outputFile,'w') \
                                 or sys.stdout
            for aUnit in fortUnitIterator(currentFile,config.inputFormat):
                TransformActiveVariables(aUnit).transformUnit().printit(ourOutFileHandle)
            if config.outputFile :
                ourOutFileHandle.close()
        # multiple input files
        else :
            for anInputFile in inputFileList:
                currentFile = anInputFile
                ourOutFileHandle = open(os.path.join(config.outputDir,currentFile),'w')
                for aUnit in fortUnitIterator(currentFile,config.inputFormat):
                    TransformActiveVariables(aUnit).transformUnit().printit(ourOutFileHandle)
                ourOutFileHandle.close()

    except (TransformError,SymtabError,UserError,ScanError,ParseError),e:
        sys.stderr.write(str(e)+'\n')
        cleanup(config,ourOutFileHandle)
        return 1

if __name__ == "__main__":
    sys.exit(main())
