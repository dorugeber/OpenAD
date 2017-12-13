import sys
import os
from optparse import OptionParser

from PyUtil.debugManager import DebugManager
from PyUtil.symtab import Symtab,SymtabError
from PyFort.flow import setOutputLineLength, setOutputFormat
from PyFort.intrinsic import getNonStandard,useNonStandard
from PyFort.fortFile import Ffile
from PyFort.fortScan import setInputLineLength
import PyFort.fortStmts as fs
import PyFort.fortExp as fe
from Canon.canon import UnitCanonicalizer,CanonError
from PP.unitPostProcess import UnitPostProcessor,PostProcessError
from PP.transformActiveVariables import TransformActiveVariables,TransformError
from PP.templateExpansion import TemplateExpansion

formatOpts={'fixed':72,'free':132}
   
############## ADD OPTIONS TO OPT PARSER ##############
def addSourceProcessingOptions(opt):
    opt.add_option('',
                   '--inputFormat',
                   dest='inputFormat',
                   type='choice', choices=formatOpts.keys(),
                   metavar="{ "+ " | ".join(formatOpts.keys())+" }",
                   help="input file format",
                   default=None)
    opt.add_option('--outputFormat',
                   dest='outputFormat',
                   metavar="{ "+ " | ".join(formatOpts.keys())+" }",
                   type='choice', choices=formatOpts.keys(),
                   help="output file format",
                   default=None)
    opt.add_option('','--infoUnitFile',
                   dest='infoUnitFile',
                   help="<infoUnitFile> contains modules which have type information needed for parsing, but should not be processed themselves",
                   default=None)
    opt.add_option('','--inputLineLength',
                   dest='inputLineLength',
                   metavar='INT',
                   type=int,
                   help='sets the max line length of the input file. The default line lengths based on the format are '+' '.join(map(lambda l: l[0]+":"+str(l[1]),formatOpts.items())),
                   default=None)
    opt.add_option('','--outputLineLength',
                   dest='outputLineLength',
                   metavar='INT',
                   type=int,
                   help='sets the max line length of the output file. The default line lengths based on the format are '+' '.join(map(lambda l: l[0]+":"+str(l[1]),formatOpts.items())),
                   default=None)
    opt.add_option('-o',
                   '--output',
                   dest='outputFile',
                   help='redirect output to  file OUTPUT (default output is stdout); If the "--outputFormat" option is not used, the output format is taken from the extension of this filename',
                   metavar='<output_file>',
                   default=None)
    opt.add_option('-v',
                   '--verbose',
                   dest='isVerbose',
                   help='turns on verbose debugging output',
                   action='store_true',
                   default=False)
    opt.add_option('',
                   '--check',
                   dest='check',
                   help='turns on sanity checking which can slow down execution',
                   action='store_true',
                   default=False)
    opt.add_option('--noWarnings',
                   dest='noWarnings',
                   help='suppress warning messages (defaults to False)',
                   action='store_true',
                   default=False)
    opt.add_option('-n',
                   '--noCleanup',
                   dest='noCleanup',
                   help='do not remove the output file if an error was encountered (defaults to False)',
                   action='store_true',
                   default=False)

def addPrePostOptions(opt):
    addSourceProcessingOptions(opt)
    modes={'f':'forward','r':'reverse'}
    modeChoices=modes.keys()
    modeChoicesHelp=""
    for k,v in modes.items():
        modeChoicesHelp+=k+" = "+v+"; "
    opt.add_option('-m','--mode',dest='mode',
                   type='choice', choices=modeChoices,
                   help='set default options for transformation mode with MODE being one of: '+ modeChoicesHelp+ '  reverse mode  implies -H but not -S; specific settings override the mode defaults.',
                   default=None)
    opt.add_option('--pathPrefix',
                   dest='pathPrefix',
                   help='for use with --separateOutput: prepend this prefix to the directory name of the corresponding input file (defaults to an empty string)',
                   default='')
    opt.add_option('',
                   '--pathSuffix',
                   dest='pathSuffix',
                   help='for use with --separateOutput: append this suffix to the directory name of the corresponding input file (defaults to "OAD")',
                   default='OAD')
    opt.add_option('',
                   '--filenameSuffix',
                   dest='filenameSuffix',
                   help='for use with --separateOutput: append this suffix to the name of the corresponding input file (defaults to an empty string)',
                   default='')   
    opt.add_option('--recursionLimit',
                   dest='recursionLimit',
                   metavar='INT',
                   type='int',
                   help='recursion limit for the python interpreter (default: '+str(sys.getrecursionlimit())+'; setting it too high may permit a SEGV in the interpreter)')
    opt.add_option('--timing',
                   dest='timing',
                   help='simple timing of the execution',
                   action='store_true',
                   default=False)
    opt.add_option('--separateOutput',
                   dest='separateOutput',
                   help='split output into files corresponding to input files (defaults to False; conflicts with --output; if --outputFormat is specified the extension will be adjusted to .f for fixed format and .f90 for free formar)',
                   action='store_true',
                   default=False)

def addCanonOptions(opt):
    addPrePostOptions(opt)
    opt.add_option('-I','',
                   metavar='PATH',
                   dest='includePaths',
                   type='string',
                   help='directory to be added to the search path for Fortran INCLUDE directives; (default is the current directory)',
                   action='append',
                   default=[])
    opt.add_option('--warn',
                   dest='warn',
                   type='choice',
                   choices=DebugManager.WarnType.getNames()[1:],
                   help='issue warning messages only for the specified type which is one of ( '+' | '.join(name for name in DebugManager.WarnType.getNames()[1:])+' ); conflicts with --noWarnings',
                   action='append',
                   default=[])
    opt.add_option('',
                   '--keepGoing',
                   dest='keepGoing',
                   help="try to continue despite error messages; this is intended only to find trouble spots for the canonicalization, if problems occur the output may contain invalid code (defaults to False)",
                   action='store_true',
                   default=False)
    opt.add_option('',
                   '--removeFunction',
                   dest='removeFunction',
                   help="remove original function definition when it is transformed to a subroutine definitions",
                   action='store_true',
                   default=False)
    opt.add_option('',
                   '--r8',
                   dest='r8',
                   help="set default size of REAL to 8 bytes",
                   action='store_true',
                   default=False)
    opt.add_option('-H',
                   '--hoistNonStringConstants',
                   dest='hoistConstantsFlag',
                   help='enable the hoisting of non-string constant arguments to subroutine calls (defaults to False)',
                   action='store_true',
                   default=False)
    opt.add_option('-S',
                   '--hoistStringConstants',
                   dest='hoistStringsFlag',
                   help='enable the hoisting of string constant arguments to subroutine calls (defaults to False)',
                   action='store_true',
                   default=False)
    opt.add_option('','--nonStandard',dest='nonStandard',
                   type='choice', choices=getNonStandard(),
                   help='allow non-standard intrinsics: ( '+' | '.join(getNonStandard())+' ) ; can be specified multiple times  (defaults to None).',
                   action='append',
                   default=[])
    opt.add_option('--subroutinizeIntegerFunctions',
                   dest='subroutinizeIntegerFunctions',
                   help='should integer function calls be subroutinized (defaults to False)',
                   action='store_true',
                   default=False)
    opt.add_option('--progress',
                   dest='progress',
                   help='issue progress message to stderr per opened input file (default is False)',
                   action='store_true',
                   default=False)
    opt.add_option('--overloading',
                   dest='overloading',
                   help='prepare to handle the type conversion in a manner suitable for overloading, e.g. with Rapsodia generated libraries; this skips the canonicalization that turns non-inlinable intrinsics into subroutines; not usable for source transformation with OpenAD; defaults to false',
                   action='store_true',
                   default=False)


def addPostProcessorOptions(opt):
    addPrePostOptions(opt)
    opt.add_option('-d', '--deriv', dest='deriv',
                   help='appends %d to deriv types instead of removing __deriv__',
                   action='store_true',
                   default=False)
    opt.add_option('-i',
                   '--inline',
                   dest='inline',
                   help='file with definitions for inlinable routines (defaults to none for forward mode and ad_inline.f for reverse mode)',
                   default=None) # cannot set default here because of defaults only for reverse mode
    opt.add_option('-p',
                   '--progress',
                   dest='progress',
                   help='progress message to stdout per processed unit',
                   action='store_true',
                   default=False)
    opt.add_option('-t',
                   '--template',
                   dest='template',
                   help='file with subroutine template for reverse mode post processing (defaults to ad_template.f) for subroutines that do not have a template file specified via the template pragma; requires reverse mode ( -m r )',
                   default=None) # cannot set default here because of required reverse mode
    opt.add_option('--abstractType',
                   dest='abstractType',
                   help='change the abstract active type name to be replaced  (see also --concreteType ) to ABSTRACTTYPE; defaults to \'oadactive\')',
                   default='oadactive')
    opt.add_option('--activeVariablesFile',
                   dest='activeVariablesFile',
                   help='write all active variable declarations into file ACTIVEVARIABLESFILEFILE.',
                   default=None) 
    opt.add_option('--concreteType',
                   dest='concreteType',
                   help='replace abstract active string (see also --abstractType ) with concrete active type CONCRETETYPE; defaults to \'active\'',
                   default='active')
    opt.add_option('--overloading',
                   dest='overloading',
                   help='handle the type conversion in a manner suitable for overloading, e.g. with Rapsodia generated libraries, by not making explicit and value references in active variables with the exception of I/O statements and assignments with a passive righ-hand side; this suppresses the injection of "use oad_active", see also the --extraReference option; not usable for source transformation with OpenAD; defaults to false',
                   action='store_true',
                   default=False)
    opt.add_option('--extraReference',
                   dest='extraReference',
                   help='if specified inject this string as an additional line after the standard "use w2f__types" statement',
                   default=None)
    opt.add_option('--noInline',
                   dest='noInline',
                   help='no inlining; overrides the defaults of the reverse mode settings; (defaults to False)',
                   action='store_true',
                   default=False)
    opt.add_option('--explicitInit',
                   dest='explicitInit',
                   help='create subroutines for the explicit initialization of active variables in common blocks and modules',
                   action='store_true',
                   default=False)
    opt.add_option('--width',
                   dest='width',
                   help='write one compile unit per output file with WIDTH digits prepended to the extension of <input_file>, e.g. for -n 2 and three compile units in an input file named \'a.f\' we create \'a.00.f\', a.01.f\', \'a.02.f\'; also creates a file named \'postProcess.make\' for reference within a makefile; cannot be specified together with -o')
    opt.add_option('--whitespace',
                   dest='whitespace',
                   help='inserts whitespaces between tokens',
                   action='store_true',
                   default=False)

def addTransformFileOptions(opt):
    addSourceProcessingOptions(opt)
    vardefsDefault='activeVariableDefinitions.f'
    opt.add_option('-d',
                   '--vardefs',
                   dest='vardefs',
                   help='file with definitions for active variables (default '+vardefsDefault+' )',
                   default=vardefsDefault)
    opt.add_option('--outputDir',
                   dest='outputDir',
                   help='for use with >1 input file (and not with --output): output each file in this directory, keeping the same file name (defaults to the local directory)',
                   default='')
    opt.add_option('--concreteType',
                   dest='concreteType',
                   help='replace abstract active string (see also --abstractType ) with concrete active type CONCRETETYPE; defaults to \'active\'',
                   default='active')

############## CHECK OPTION ERRORS ##############    
def SourceProcessingOptErrors(config,args):
    # set free/fixed format
    if (config.inputFormat<>'fixed') and \
           (config.inputFormat<>'free') and \
           (config.inputFormat is not None):
        opt.error("inputFormat option must be specified with either 'fixed' or 'free' as an argument")
    if config.inputLineLength:
        if config.inputLineLength < formatOpts['fixed'] :
            opt.error("inputLineLength option must be specified with a value >="+str(formatOpts['fixed']))
    if config.outputLineLength:
        if config.outputLineLength < formatOpts['fixed'] :
            opt.error("outputLineLength option must be specified with a value >="+str(formatOpts['fixed']))    

def PrePostOptErrors(config,args):
    SourceProcessingOptErrors(config,args)

def CanonOptErrors(opt,config,args):
    PrePostOptErrors(config,args)
    # check input/output options
    if len(args) == 0:
        opt.error('expected at least one argument <input_file> ;' \
                 +' the following options were given: '+str(config))
    if config.outputFile and config.separateOutput: 
        opt.error("options --outputFile and --separateOutput are mutually exclusive")
    if (config.noWarnings and config.warn):
        opt.error("Option --noWarnings conflicts with option --warn="+config.warn[0])

def PostProcessorOptErrors(opt,config,args):
    PrePostOptErrors(config,args)
    # check input/output options
    if len(args) != 1:
        opt.error("expect exactly one argument <input_file>, given are "+str(len(args))+" which are:"+str(args)+" and the following options: "+str(config))
    if ((config.width and config.outputFile) or
        (config.width and config.separateOutput) or
        (config.outputFile and config.separateOutput)):
        opt.error("cannot specify more than one of --width, --separateOutput, and -o")
    if (config.mode != 'r'):
        if (config.template):
            opt.error("option -t requires reverse mode ( -m r )")
    if (config.inline):
        if (config.noInline):
            opt.error("option --noInline conflicts with option -i")

def TransformFileOptErrors(config,args):
    SourceProcessingOptErrors(config,args)
    # check input/output options
    if len(args) == 0:
        opt.error('expected at least one argument <input_file> ;' \
                 +' the following options were given: '+str(config))
    if config.outputFile and len(args) > 1 :
        opt.error('No output file can be specified when more than one input file is given.' \
                 +' the following options were given: '+str(config))


############## SET FLAGS ##############

def setSourceProcessingFlags(config):
    # set outputFormat explicitly if format or output file are supplied by user. 
    # otherwise, outputFormat is set to inputFormat during parsing
    if config.outputFormat == None:
        if config.outputFile:
            ext = os.path.splitext(config.outputFile)[1]
            config.outputFormat = Ffile.get_format(ext)
            setOutputFormat(config.outputFormat)
    elif (config.outputFormat<>'fixed') and (config.outputFormat<>'free'):
        opt.error("outputFormat option must be specified with either 'fixed' or 'free' as an argument")
    else:
        setOutputFormat(config.outputFormat)    
    if config.outputLineLength:
        setOutputLineLength(config.outputLineLength)
    if config.inputLineLength:
        setInputLineLength(config.inputLineLength)
    # set symtab type defaults
    Symtab.setTypeDefaults((fs.RealStmt,[]),(fs.IntegerStmt,[]))
    # parse info units
    if config.infoUnitFile:
        from PyFort.fortUnit import fortUnitIterator
        for aUnit in fortUnitIterator(config.infoUnitFile,config.inputFormat):
            # need to parse this so the type information is available, but do not modify or print units
            pass
    # set verbosity
    DebugManager.setVerbose(config.isVerbose)
    DebugManager.setQuiet(config.noWarnings)
    DebugManager.setCheck(config.check)

def setPrePostFlags(config):
    setSourceProcessingFlags(config)
    if (config.recursionLimit):
        sys.setrecusionlimit(config.recursionLimit);

def setCanonFlags(config):
    setPrePostFlags(config)
    # configure forward/reverse mode
    if config.mode:
        if config.mode[0] == 'f':
            UnitCanonicalizer.setHoistConstantsFlag(False)
            UnitCanonicalizer.setHoistStringsFlag(False)
        elif config.mode[0] == 'r':
            UnitCanonicalizer.setHoistConstantsFlag(True)
            UnitCanonicalizer.setHoistStringsFlag(False)
    # set symtab type defaults
    if config.r8:
        Symtab.setTypeDefaults((fs.DoubleStmt,[]),(fs.IntegerStmt,[]))
    if config.removeFunction:
        UnitCanonicalizer.setKeepFunctionDef(False)
    if config.hoistConstantsFlag:
        UnitCanonicalizer.setHoistConstantsFlag(config.hoistConstantsFlag)
    if config.hoistStringsFlag:
        UnitCanonicalizer.setHoistStringsFlag(config.hoistStringsFlag)
    if config.subroutinizeIntegerFunctions:
        UnitCanonicalizer.setSubroutinizeIntegerFunctions(True)
    if config.keepGoing:
        CanonError.keepGoing()
    if config.warn:
        DebugManager.warnOnlyOn(config.warn)
    if config.progress:
        DebugManager.dumpProgress()    
    if config.includePaths:
        Ffile.setIncludeSearchPath(config.includePaths)
    if config.nonStandard:
        useNonStandard(config.nonStandard)
    if (config.overloading): 
        UnitCanonicalizer.setOverloadingMode()

def setTransformFileFlags(config):
    setSourceProcessingFlags(config)
    # set replacement type 
    TransformActiveVariables.setReplacementType(config.concreteType)

def setPostProcessFlags(config,args):
    setPrePostFlags(config)
    if (config.recursionLimit):
        sys.setrecusionlimit(config.recursionLimit);
    # set __deriv__ output format(__deriv__(v) -> "(v)%d" if -d option or "v" by default)
    UnitPostProcessor.setDerivType(config.deriv)
    if (config.activeVariablesFile):
        UnitPostProcessor.setActiveVariablesFile(config.activeVariablesFile)
        if (os.path.exists(config.activeVariablesFile)):
            os.remove(config.activeVariablesFile)
    if (config.explicitInit):
        UnitPostProcessor.setExplicitInit()
    # configure forward/reverse mode (including inline file for reverse mode)
    if config.mode == 'f':
        UnitPostProcessor.setMode('forward')
    if config.mode == 'r':
        UnitPostProcessor.setMode('reverse')
        if (config.inline):
            UnitPostProcessor.setInlineFile(config.inline)
        if (config.noInline):
            UnitPostProcessor.setInlineFile(None)
        UnitPostProcessor.processInlineFile()
        templateFile = config.template or 'ad_template.f'
        TemplateExpansion.setTemplateFile(templateFile)
    else: 
        if (config.inline):
            UnitPostProcessor.setInlineFile(config.inline)
            UnitPostProcessor.processInlineFile()
        else : 
            UnitPostProcessor.setInlineFile(None)
    # set whitespace
    fe.setWhitespace(config.whitespace)
    # set replacement type 
    UnitPostProcessor.setReplacementType(config.concreteType)
    # set abstract type 
    UnitPostProcessor.setAbstractType(config.abstractType)
    if (config.overloading):
        UnitPostProcessor.setOverloadingMode()
    if (config.extraReference):
        UnitPostProcessor.setExtraReference(config.extraReference)
    
