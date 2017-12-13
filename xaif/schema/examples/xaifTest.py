#!/usr/bin/env python

import sys
sys.path.insert(0,"../../graph")

from XSV.driver import runit, runitAndShow
from xaifGraph import XAIFGraph, XAIFVertex
from xaifParser import XAIFParser, XAIFContentHandler


# Test XAIF parsing, graph utilities
if len(sys.argv) < 1:
  print 'Usage: xaifTest.py <xaifFile.xaif>'
  exit

filename = sys.argv[1]

print 'File is', filename

parser = XAIFParser()
parser.validate(filename)
#parser.parse("../schema/examples/uwe_ex_3.xaif")
#parser.displayGraph()
  
