from _Setup import *

import PyFort.fortExp as fe

class ArrayBoundsTab(object):
    '''class to keep track of array bounds for array entry types in type table'''
    def __init__(self):
        self.ids = dict() # string for the key and ArrayBoundsTabEntry for the value
        self.array_counter=1

    def lookupArrayId(self,arrayid):
        '''check for arrayid in the array bounds table'''
        if arrayid in self.ids:
            return self.ids[arrayid]
        else:
            return None

    # only check equivalence for arrays with constant dimensions
    def arrayBoundsEntryEq(self,arrayBoundsEntryPair):
        '''return true if the two array bounds entries in the pair are equivalent'''
        if all(map(lambda l: (not l.constant_dims),arrayBoundsEntryPair)):
            return False
        if arrayBoundsEntryPair[0].rank!=arrayBoundsEntryPair[1].rank:
            return False
        return self.checkDimArrayEq([arrayBoundsEntryPair[0].dimArray,
                                     arrayBoundsEntryPair[1].dimArray])

    def checkDimArrayEq(self,dimArrayPair):
        '''return True if the dimensions in each of the two dimensionEntry lists are equivalent'''
        # pair of dimensionEntry lists; want to have list of dimensionEntry pairs & then compare
        dimEntryPairsArray=[]; i=0
        while i<len(dimArrayPair[0]):
            dimEntryPairsArray.append((dimArrayPair[0][i],dimArrayPair[1][i]))
            i+=1
        if all(map(lambda l: (l[0].lower==l[1].lower and l[0].upper==l[1].upper),dimEntryPairsArray)):
            return True
        return False

    # if array dimensions are constants, look up array bounds in table
    # if not, return None
    def lookupArrayBounds(self,arrayDims):
        ''' get the id for the array bounds entry corresponding to the given dimensions, or return None'''
        rank=len(arrayDims)
        rankEntryMatches = [e for e in self.ids.values() if (e.constant_dims and e.rank==rank)]
        if not rankEntryMatches:
            return None
        # create dimArray to filter rankEntryMatches
        dimArray=[]
        # attempt lookup if array dimensions are constants
        # if not, just return (takes too long)
        for dim in arrayDims:
            if isinstance(dim,fe.Ops):
                if not isinstance(dim.a1,int) or not isinstance(dim.a2,int):
                    return None
                else:
                    dimArrayEntry=ArrayBoundsTabEntry.DimensionEntry(dim.a1,dim.a2)
                    dimArray.append(dimArrayEntry)
            elif not isinstance(dim,int):
                return None
            else:
                dimArrayEntry=ArrayBoundsTabEntry.DimensionEntry(1,dim)
                dimArray.append(dimArrayEntry)
        match = [e for e in rankEntryMatches if self.checkDimArrayEq([e.dimArray,dimArray])]
        if not match:
            return None
        return match[0].arrayid

    # add new array bounds entry if the bounds of arrayType do not already have an entry in the table
    # return arrayid
    def enterNewArrayBounds(self,dimensions):
        '''create an array bounds table entry for the given dimensions'''
        dimArray=[]
        rank=len(dimensions)
        constant_dims=True
        for dim in dimensions:
            if isinstance(dim,fe.Ops):
                dimEntry=ArrayBoundsTabEntry.DimensionEntry(dim.a1,dim.a2)
                if constant_dims and (not isinstance(dim.a1,int) or not isinstance(dim.a2,int)):
                    constant_dims=False
            else:
                dimEntry=ArrayBoundsTabEntry.DimensionEntry(1,dim)
                if constant_dims and not isinstance(dim,int):
                    constant_dims=False
            dimArray.append(dimEntry)
        if constant_dims:
            # check to see if array bounds are already present in table
            rankEntryMatches = [e for e in self.ids.values() if (e.constant_dims and e.rank==rank)]
            if rankEntryMatches:
                match = [e for e in rankEntryMatches if (e.dimArray==dimArray)]
                if match:
                    return match[0].arrayid
        newEntry=ArrayBoundsTabEntry(rank,dimArray,self.array_counter,constant_dims)
        self.ids[self.array_counter]=newEntry
        self.array_counter += 1
        return newEntry.arrayid

    def getArrayBoundsId(self,dimensions):
        '''get the array bounds entry id for the given dimensions, creating a new entry if necessary'''
        array_id=self.lookupArrayBounds(dimensions)
        if array_id==None:
            array_id=self.enterNewArrayBounds(dimensions)
        return array_id

class ArrayBoundsTabEntry(object):
    '''class to manage array bounds table entries'''

    class DimensionEntry(object):
        '''class to manage array dimensions'''
        def __init__(self,lower,upper):
            self.lower=lower
            self.upper=upper

    def __init__(self,rank,dimArray,arrayid,constant_dims):
        self.rank=rank    # number of dimensions in array
        self.dimArray=dimArray  # an array of DimensionEntry specified as (lower,upper);
                                # dimArray has size numDims and each dimension info triple is indexed at its dimension number
        self.arrayid=arrayid    # array bounds info id number in ArrayBoundsTab
        self.constant_dims=constant_dims  # if all dimensions are constants than we will try to look up these array bounds

