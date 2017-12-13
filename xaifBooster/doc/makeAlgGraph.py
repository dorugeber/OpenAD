#!/usr/bin/env python

import os
import sys
import shutil
import re

class Vertex:
    '''store algNamespace, algorithm name, any algorithm actions,'''

    def __init__(self,algNS,alg):
        self.algActions = ''
        self.algNS = algNS
        self.alg = alg
        #self.namespaceDir = algName

    def printIt(self,out=sys.stdout):
        outStr = '    '+self.algNS+'_'+self.alg
        algActionStr = len(self.algActions) and '('+self.algActions+')' or ''
        outStr += '[label="'+self.alg+algActionStr+'"];\n'
        return outStr


def main():
    from optparse import OptionParser
    opt = OptionParser(usage='%prog [options] ')
    opt.add_option('-v','--verbose',dest='verbose',
                   help="let the pipeline components produce some extra output",
                   action='store_true',default=False)
    (options, args) = opt.parse_args()
    try:
	os.system(". ./makeAlgGraph")

        verticesFile = open("algGraphVertices.txt")
        vertices = []
        for vertexLine in verticesFile.readlines():
            if options.verbose: print "trying line",vertexLine
            (algNS,alg) = vertexLine.rstrip().split("_")
            vertices.append(Vertex(algNS,alg))
        verticesFile.close()

	fout = open("algGraph.dot","w")
	fout.write('Digraph {\n')
	fout.write('  rankdir=RL;\n')
	fout.write('  ordering="out";\n')
	fout.write('  nodesep=0.1;\n')
	fout.write('  size="30,18";\n')
	fout.write('  ratio=fill;\n')
	fout.write('  edge [penwidth=3];\n')
	fout.write('  node [penwidth=3,fontsize=14,style=filled,fillcolor=white];\n')

	# search for an algorithm_action_# implementation
        if options.verbose: print 'searching for algorithm_actions...'
	for vertex in vertices:
            if options.verbose: print 'trying '+vertex.algNS+'::'+vertex.alg+'...   ',
            namespaceDir = vertex.algNS
	    if (vertex.algNS == "Base"):
		continue
	    elif (vertex.algNS == "CFR"):
		namespaceDir = "ControlFlowReversal"
	    elif (vertex.algNS == "BBP"):
		namespaceDir = "BasicBlockPreaccumulation"
	    elif (vertex.algNS == "BBPTape"):
		namespaceDir = "BasicBlockPreaccumulationTape"
	    elif (vertex.algNS == "BBPTapeAdjoint"):
		namespaceDir = "BasicBlockPreaccumulationTapeAdjoint"
	    elif (vertex.algNS == "BBPReverse"):
		namespaceDir = "BasicBlockPreaccumulationReverse"
	    if options.verbose: print 'trying file '+namespaceDir+'/inc/'+vertex.alg+'Alg.hpp'
	    headerFile = open("%s/inc/%sAlg.hpp" % (namespaceDir,vertex.alg))
	    for line in headerFile.readlines():
		if (re.match("\s+virtual void algorithm_action_",line)):
                    vertex.algActions += line.split()[2][17]
	    headerFile.close()

	# Draw clusters
        for i in [["Base","lightblue"],["CFR","plum"],["AddressArithmetic","beige"],["BBP","lightcyan"],\
                  ["BBPReverse","tomato2"],["BBPTapeAdjoint","lightgreen"],["BBPTape","lightgoldenrod"],\
                  ["ConstantFolding","lightgrey"],["Linearization","dimgray"],["TypeChange","palevioletred"],\
                  ["MemOpsTradeoffPreaccumulation","coral"],["TraceDiff","dodgerblue4"],["PushPop","wheat2"]]:
	    (algNamespace,algColor) = i
	    fout.write("\n  subgraph cluster_%s {\n" % algNamespace)
	    fout.write('    URL="http://www.mcs.anl.gov/OpenAD/xaifBoosterDox/namespacexaifBooster'+algNamespace+'.html";\n')
	    fout.write("    fontsize=18;\n")
	    fout.write("    label=\"%s\";\n" % algNamespace)
	    fout.write("    style=filled;\n")
	    fout.write("    fillcolor=%s;\n" % algColor)
	    for vertex in vertices:
                if (vertex.algNS == algNamespace):
                    fout.write(vertex.printIt())
	    fout.write("  }\n") # end this cluster

        # get edges from file and draw them    
        edgesFile = open("algGraphEdges.txt")
        edges = []
        for line in edgesFile.readlines():
            edges.append(line.split("->")) # (source,target)
            fout.write('    '+line)
        edgesFile.close()

	fout.write("\n}\n")
	fout.close()
	os.system("dot -T svg algGraph.dot > algGraph.svg")

    except RuntimeError, errtxt:
	print 'caught exception: ',errtxt
	return -1
    return 0

if __name__ == "__main__":
    sys.exit(main())

