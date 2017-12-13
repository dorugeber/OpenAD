#!/usr/bin/env python

import os
import shutil
import sys

def showGraphs(errDict,errLimDict,withAD,name,n,m,impulse,term):
    import tempfile
    plotFileName=tempfile.mktemp(suffix='.gnu')
    plotFile=open(plotFileName,"w")

    # set output terminal
    if term=="svg":
        outputName = 'numericalComparison-'+name+'.svg'
        plotFile.write('set term svg font \'arial,11\' size 900,690\n')
        plotFile.write('set output "'+outputName+'"\n')
    elif term=="postscript":
        outputName = 'numericalComparison-'+name+'.ps'
        plotFile.write('set term postscript font \'arial,11\'\n')
        plotFile.write('set output "'+outputName+'"\n')
    else:
        plotFile.write('set terminal x11 persist\n')
    # set global options
    plotFile.write('unset key\n')
    plotFile.write('unset xtics\n')
    plotFile.write('unset xlabel\n')
    plotFile.write('unset ylabel\n')
    plotFile.write('set logscale y\n')
    # set layout
    if withAD: 
        plotFile.write('set multiplot layout 2, 3 title \"'+str(name)+' n='+str(n)+',m='+str(m)+'\"\n')
    else:     
        plotFile.write('set multiplot layout 2, 1 title \"'+str(name)+' n='+str(n)+',m='+str(m)+'\"\n')
    datFileNames=[]
    for errName in sorted(errDict.keys()):
	errValList=errDict[errName]
        # write data to temp files
        datOverFileName=tempfile.mktemp()
        datFileNames.append(datOverFileName)
        datOverFile=open(datOverFileName,"w")
        datUnderFileName=tempfile.mktemp()
        datFileNames.append(datUnderFileName)
        datUnderFile=open(datUnderFileName,"w")
	errMin=errLimDict[errName]
        foundOverLimit = False
        for (x,y) in enumerate(errValList):
            if (y>errLimDict[errName] ) :
                foundOverLimit = True
                datOverFile.write(str(x)+' '+str(y)+'\n')
            else :     
                datUnderFile.write(str(x)+' '+str(y)+'\n')
            if (y>0 and y<errMin) :
                errMin=y
        datOverFile.close()
        datUnderFile.close()
        # set title
        titleColor = foundOverLimit and 'red' \
                                     or 'green'
        plotFile.write('set title \"'+errName+'\" tc rgb \"'+titleColor+'\"\n')
        # set ranges
        plotFile.write('set xrange [-'+str(1 + n*m/20)+':'+str(n*m + n*m/20)+']\n')
        if (errMin==errLimDict[errName]) :
            plotFile.write('set yrange ['+str(errMin/10)+':] \n')
        errMax=max(max(errDict[errName]),errLimDict[errName])
        if (errMax==errLimDict[errName]) :
            plotFile.write('set yrange [:'+str(errMax*10)+'] \n')
        # execute plot command
        plotFile.write('plot\\\n')
        if impulse:
            plotFile.write('\"'+datOverFileName+'\" with impulses lc 1 lw 2,\\\n')
            plotFile.write('\"'+datUnderFileName+'\" with impulses lc 2 lw 2,\\\n')
        else:
            plotFile.write('\"'+datOverFileName+'\" with points pt 3 lc 1, \\\n')
            plotFile.write('\"'+datUnderFileName+'\" with points pt 3 lc 2, \\\n')
        plotFile.write(str(errLimDict[errName])+' with lines lt 3 lc 3\n')
        # reset y range
        plotFile.write('set yrange [*:*] \n')
    plotFile.write('unset multiplot\n')
    plotFile.close()

    # run gnuplot
    if (os.system("gnuplot "+plotFileName+" 2>/dev/null")):
        sys.stderr.write('gnuplot failed.  plotFile: '+plotFileName+'\n')
    else:     
        os.remove(plotFileName)
        map(os.remove,datFileNames)

    if term=="svg" or term=="postscript":
        plotDir = 'plotOutput'
        if not (os.path.exists(plotDir)):
            os.mkdir(plotDir)
        if (os.path.exists(os.path.join(plotDir,outputName))):
            os.remove(os.path.join(plotDir,outputName))
        shutil.move(outputName,plotDir)

def compareFiles (fileDict,withAD,doBatch,graphs,name,impulse,makeSVG,verbose):
    paramsFile=open("params.conf","r")
    n=int(paramsFile.readline())
    m=int(paramsFile.readline())
    # the following readline gets h, which isn't used here.
    # we don't even bother with it, because it might be
    # in a fortran-specific format for specifying constants
    paramsFile.readline()
    relErrorMax=float(paramsFile.readline())
    if (relErrorMax!=relErrorMax):
        raise RuntimeError('max rel error in params is not a NaN')
    paramsFile.close()
    numbers={}
    # order is given in usage
    for fileKey, fileName in fileDict.items() :
        numbers[fileKey]=[]
        numberFile=open(fileName,"r")
        for theLine in numberFile:
            if (theLine[0] == "F"):
                fElementString,fValueString = theLine.split('=')
                fVal=float(fValueString)
                numbers[fileKey].append(fVal)
                if (fVal!=fVal):
                    raise RuntimeError('read a floating point number from '+fileName+' that is a NaN')
    errDict={}
    if (withAD): 
        for key in ['absDiscrADDD','relDiscrADDD','absDiscrCvR_AD','relDiscrCvR_AD'] :
            errDict[key]=[]
    for key in ['absDiscrCvR_DD','relDiscrCvR_DD'] :
      errDict[key]=[]
    for i in range(0,n*m):
        if (withAD): 
            # current AD vs DD
            errDict['absDiscrADDD'].append(abs(numbers['curDD'][i]-numbers['curAD'][i]))
            if (numbers['curAD'][i]!=0) :
                errDict['relDiscrADDD'].append(abs((numbers['curDD'][i]-numbers['curAD'][i])/numbers['curAD'][i]))
            else:
                errDict['relDiscrADDD'].append(errDict['absDiscrADDD'][-1])
            # AD current vs. ref
            errDict['absDiscrCvR_AD'].append(abs(numbers['curAD'][i]-numbers['refAD'][i]))
            if (numbers['refAD'][i]!=0) :
                errDict['relDiscrCvR_AD'].append(abs((numbers['curAD'][i]-numbers['refAD'][i])/numbers['refAD'][i]))
            else:
                errDict['relDiscrCvR_AD'].append(errDict['absDiscrCvR_AD'][-1])
        # DD current vs. ref
        errDict['absDiscrCvR_DD'].append(abs(numbers['curDD'][i]-numbers['refDD'][i]))
        if (numbers['refDD'][i]!=0) :
            errDict['relDiscrCvR_DD'].append(abs((numbers['curDD'][i]-numbers['refDD'][i])/numbers['refDD'][i]))
        else:
            errDict['relDiscrCvR_DD'].append(errDict['absDiscrCvR_DD'][-1])
    errLimDict={}
    compEps=1.E-16
    errLimDict['absDiscrCvR_DD']=compEps
    errLimDict['relDiscrCvR_DD']=compEps
    maxAbsDiscrCurRef=max(errDict['absDiscrCvR_DD'])
    maxRelDiscrCurRef=max(errDict['relDiscrCvR_DD'])
    returnValue=0
    if (withAD): 
        maxRelErrADDD=max(errDict['relDiscrADDD'])
        maxAbsErrADDD=max(errDict['absDiscrADDD'])
        errLimDict['relDiscrADDD']=relErrorMax
        errLimDict['absDiscrADDD']=relErrorMax
        if (maxRelErrADDD>relErrorMax and maxAbsErrADDD>relErrorMax):
            returnValue=-1
            if not doBatch : 
                sys.stderr.write(" max discrepancy current AD vs. DD abs: %s rel %s\n" % (maxAbsErrADDD,maxAbsErrADDD))
        maxAbsDiscrCurRef=max([max(errDict['absDiscrCvR_AD']),maxAbsDiscrCurRef])
        maxRelDiscrCurRef=max([max(errDict['relDiscrCvR_AD']),maxRelDiscrCurRef])
        errLimDict['absDiscrCvR_AD']=compEps
        errLimDict['relDiscrCvR_AD']=compEps
    if (maxRelDiscrCurRef>compEps and maxAbsDiscrCurRef>compEps):
        returnValue=-1
        if not doBatch : 
            sys.stderr.write(" max discrepancy cur. vs. ref. abs: %s rel %s\n" % (maxRelDiscrCurRef,maxAbsDiscrCurRef))
    if (returnValue and verbose) :
        for errKey, errList in errDict.items():
           if (max(errDict[errKey]) > errLimDict[errKey]) :
             sys.stderr.write(" "+errKey+" > "+str(errLimDict[errKey])+" for:\n")     
             for index,val in enumerate(errDict[errKey]) :
                if (val > errLimDict[errKey]) :
                  sys.stderr.write("  F["+str(index/m+1)+"]["+str(index%m)+"]: %r\n" % (val))
    if (returnValue and graphs) :
        showGraphs(errDict,errLimDict,withAD,name,n,m,impulse,makeSVG)
    return returnValue
          
def main():
    from optparse import OptionParser
    terms=['X11','postscript','svg']
    termOpts='[ '
    for i in terms :
        termOpts+=i
        if  i != terms[-1]:
            termOpts+=" | "
    termOpts+=" ]"        
    opt = OptionParser(usage='%prog [options] <curDD_file> <refDD_file [ <curAD_file> <refAD_file ]')
    opt.add_option('-b','--batchMode',dest='batchMode',
                   help="run in batchMode suppressing output",
                   action='store_true',default=False)
    opt.add_option('-g','--graphs',dest='graphs',
                   help="if limits are exceeded display the discrepancies with gnuplot",
                   action='store_true',default=False)
    opt.add_option('-n','--name',dest='name', type="string",
                   help="the name of the test",
                   action='store',default="")
    opt.add_option('-i','--impulse',dest='impulse',
                   help="draw errors with impulses (bars)",
                   action='store_true',default=False)
    opt.add_option('-t','--term',dest='term',
                   type='choice', choices=terms,
                   help="gnuplot terminal selection (defaults to X11), one from the following list: " +termOpts,
                   default='X11')
    opt.add_option('-v','--verbose',dest='verbose',
                   help="if limits are exceeded show verbose output",
                   action='store_true',default=False)
    (options, args) = opt.parse_args()
    returnValue=0
    keyList=["curDD","refDD"]
    withAD=False
    try:
        if len(args)!=2  and len(args)!=4:
            opt.error("expect 2 or 4 input file arguments, found " + str(len(args))+" : " + str(args))
        if len(args)==4 :
            keyList.extend(["curAD","refAD"])
            withAD=True
        if options.batchMode and (options.graphs or options.verbose) :
            opt.error("options for batchMode and graphs/verbose are mutually exclusive")
        fileDict={}    
        for i in range(0,len(keyList)):
            fileDict[keyList[i]]=args[i]
        returnValue=compareFiles(fileDict,withAD,options.batchMode,options.graphs,options.name,options.impulse,options.term,options.verbose)
    except RuntimeError, e:
        print 'caught exception: ',e
        return 1
    return returnValue

if __name__ == "__main__":
    sys.exit(main())
