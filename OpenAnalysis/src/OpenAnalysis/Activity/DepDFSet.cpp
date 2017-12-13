/*! \file
  
  \brief Implementation of DepDFSet.

  \author Michelle Strout
  \version $Id: DepDFSet.cpp,v 1.3 2007/10/08 19:19:08 utke Exp $

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#include "DepDFSet.hpp"
#include <Utils/Util.hpp>

namespace OA {
  namespace Activity {

static bool debug = false;

//! default constructor
DepDFSet::DepDFSet() //: mMakeImplicitExplicitMemoized(false)
{
    OA_DEBUG_CTRL_MACRO("DEBUG_DepDFSet:ALL", debug);
    mImplicitRemoves = new DataFlow::LocDFSet;
    mUses = new DataFlow::LocDFSet;
    mDefs = new DataFlow::LocDFSet;
}

//! copy constructor
DepDFSet::DepDFSet(const DepDFSet &other) //: mMakeImplicitExplicitMemoized(false)
{ 
    mImplicitRemoves = other.mImplicitRemoves;
    mUseToDefsMap = other.mUseToDefsMap;
    mDefToUsesMap = other.mDefToUsesMap;
    mDefs = other.mDefs;
    mUses = other.mUses;
}

//! assignment
DepDFSet& DepDFSet::operator=(const DepDFSet &other)
{
    //mMakeImplicitExplicitMemoized = other.mMakeImplicitExplicitMemoized;
    mImplicitRemoves = other.mImplicitRemoves;
    mUseToDefsMap = other.mUseToDefsMap;
    mDefToUsesMap = other.mDefToUsesMap;
    mDefs = other.mDefs;
    mUses = other.mUses;
    return *this;
}

//*****************************************************************
// DataFlowSet Interface Implementation
//*****************************************************************
    
OA_ptr<DataFlow::DataFlowSet> DepDFSet::clone()
{ 
    OA_ptr<DepDFSet> retval;
    retval = new DepDFSet(*this); 
    return retval; 
}

bool DepDFSet::operator==(DataFlow::DataFlowSet &other) const
{ 
    DepDFSet& recastOther = dynamic_cast<DepDFSet&>(other);
    bool retval = true;

    // make sure that each have their implicits made explicit
    // will have to copy our own because makeImplicitExplicit
    // changes the data-structure
    //DepDFSet copyThis = *this;
    //copyThis.makeImplicitExplicit();
    //recastOther.makeImplicitExplicit();

    if (mImplicitRemoves == recastOther.mImplicitRemoves
        && mUseToDefsMap == recastOther.mUseToDefsMap
        && mDefToUsesMap == recastOther.mDefToUsesMap) 
    {
        retval = true;
    } else {
        retval = false;
    }

    /*
    // make sure each dep pair in this is in other
    OA_ptr<DepIterator> depIter = getDepIterator();
    for ( ; depIter->isValid(); (*depIter)++ ) {
        OA_ptr<Location> use = depIter->use();
        OA_ptr<Location> def = depIter->def();

        bool found = false;
        OA_ptr<DepIterator> otherDepIter = recastOther.getDepIterator();
        for ( ; otherDepIter->isValid(); (*otherDepIter)++ ) {
            OA_ptr<Location> otherUse = otherDepIter->use();
            OA_ptr<Location> otherDef = otherDepIter->def();

            if (use==otherUse  && def==otherDef) {
                found = true;
            }
        }

        if (!found) {
            retval = false;
        }
    }
    
    // make sure each dep pair in this is in other
    OA_ptr<DepIterator> otherDepIter = recastOther.getDepIterator();
    for ( ; otherDepIter->isValid(); (*otherDepIter)++ ) {
        OA_ptr<Location> otherUse = otherDepIter->use();
        OA_ptr<Location> otherDef = otherDepIter->def();

        bool found = false;
        OA_ptr<DepIterator> depIter = getDepIterator();
        for ( ; depIter->isValid(); (*depIter)++ ) {
            OA_ptr<Location> use = depIter->use();
            OA_ptr<Location> def = depIter->def();

            if (use==otherUse  && def==otherDef) {
                found = true;
            }
        }

        if (!found) {
            retval = false;
        }
    }

    */
    if (debug) {
        std::cout << "operator= returning " <<  retval << std::endl;
    }
    return retval;

}

bool DepDFSet::operator!=(DataFlow::DataFlowSet &other) const
{
    if (*this == other) {
        if (debug) { std::cout << "operator!= will return false" << std::endl; }
        return false;
    } else {
        if (debug) { std::cout << "operator!= will return true" << std::endl; }
        return true;
    }
}

//*****************************************************************
// Methods specific to DepDFSet
//*****************************************************************

/*
void DepDFSet::makeImplicitExplicit()
{
    if (mMakeImplicitExplicitMemoized == true) { 
        return; 
    } else {
        mMakeImplicitExplicitMemoized = true;
    }

    // make all implicit deps that haven't been explicitly
    // removed explicit in the data-structure as well
    LocToLocDFSetMap::const_iterator mapIter;
    for (mapIter=mUseToDefsMap.begin(); mapIter!=mUseToDefsMap.end();
         mapIter++) 
    {
        if (! isImplicitRemoved(mapIter->first) ) {
            insertDep(mapIter->first,mapIter->first);
        }
    }
    for (mapIter=mDefToUsesMap.begin(); mapIter!=mDefToUsesMap.end();
         mapIter++) 
    {
        if (! isImplicitRemoved(mapIter->first) ) {
            insertDep(mapIter->first,mapIter->first);
        }
    }
}
*/

/*!
    This routine generates a new DepDFSet based of the
    current one but with the implicit pairs included.
 */
OA_ptr<DepIterator> DepDFSet::getDepIterator() const
{

    // copy self and then create iterator for it
    OA_ptr<DepDFSet> setCopy; setCopy = new DepDFSet(*this);
    //setCopy->makeImplicitExplicit();
    OA_ptr<DepIterator> retval;
    retval = new DepIterator(setCopy);
    return retval;
}

  
OA_ptr<LocIterator> DepDFSet::getDefsIterator(const OA_ptr<Location> use) const
{
    OA_ptr<DataFlow::LocDFSet> retSet;
    retSet = new DataFlow::LocDFSet();

    // get iterator over uses the given location overlaps with
    OA_ptr<LocIterator> useOverlapIter
        = mUses->getOverlapLocIterator(use);
    for ( ; useOverlapIter->isValid(); (*useOverlapIter)++ ) {
        OA_ptr<Location> loc = useOverlapIter->current();

        // iterator over associated defs
        LocToLocDFSetMap::const_iterator pos = mUseToDefsMap.find(loc);
        if (pos!=mUseToDefsMap.end()) {
            OA_ptr<LocIterator> locIter = pos->second->getLocIterator();
            for ( ; locIter->isValid(); (*locIter)++ ) {
                retSet->insert(locIter->current());
            }
        }
    }

    // loop over sets that the use 
    /*
    LocToLocDFSetMap::const_iterator mapIter;
    for (mapIter=mUseToDefsMap.begin();
         mapIter!=mUseToDefsMap.end(); mapIter++ )
    {
        OA_ptr<Location> depuse = mapIter->first;
        if (depuse->mayOverlap(*use)) {
            OA_ptr<LocIterator> locIter = mapIter->second->getLocIterator();
            for ( ; locIter->isValid(); (*locIter)++ ) {
                retSet->insert(locIter->current());
            }
        }
    }
    */

    // see if reflexive dep for use loc should be included,
    // only reason why it wouldn't is if use
    // is in the ImplicitRemoves
    // FIXME: ok could put hasMustOverlapLoc, but for now going with 
    // conservative assumption which means equivalenced symbols will get
    // short shrifted. insertDep if use==def makes same assumption.
    if (!mImplicitRemoves->hasLoc(use)) {
        retSet->insert(use);
    }

    // construct the iterator
    OA_ptr<DataFlow::LocDFSetIterator> retval;
    retval = new DataFlow::LocDFSetIterator(*retSet);
    return retval;
}
  
OA_ptr<LocIterator> DepDFSet::getUsesIterator(OA_ptr<Location> def) const
{
    OA_ptr<DataFlow::LocDFSet> retSet;
    retSet = new DataFlow::LocDFSet();

    // get iterator over uses the given location overlaps with
    OA_ptr<LocIterator> defOverlapIter
        = mDefs->getOverlapLocIterator(def);
    for ( ; defOverlapIter->isValid(); (*defOverlapIter)++ ) {
        OA_ptr<Location> loc = defOverlapIter->current();
        if (debug) {
            std::cout << "getUsesIterator: loc = ";
            loc->dump(std::cout);
        }

        // iterator over associated deps
        LocToLocDFSetMap::const_iterator pos = mDefToUsesMap.find(loc);
        if (pos!=mDefToUsesMap.end()) {
            OA_ptr<LocIterator> locIter = pos->second->getLocIterator();
            for ( ; locIter->isValid(); (*locIter)++ ) {
                retSet->insert(locIter->current());
            }
        }
    }

    /*
    LocToLocDFSetMap::const_iterator mapIter;
    for (mapIter=mDefToUsesMap.begin();
         mapIter!=mDefToUsesMap.end(); mapIter++ )
    {
        OA_ptr<Location> depdef = mapIter->first;
        if (depdef->mayOverlap(*def)) {
            OA_ptr<LocIterator> locIter = mapIter->second->getLocIterator();
            for ( ; locIter->isValid(); (*locIter)++ ) {
                retSet->insert(locIter->current());
            }
        }
    }
    */

    // see if reflexive dep for def loc should be included,
    // only reason why it wouldn't is if def
    // is in the ImplicitRemoves
    // FIXME: ok could put hasMustOverlapLoc, but for now going with 
    // conservative assumption which means equivalenced symbols will get
    // short shrifted
    if (!mImplicitRemoves->hasLoc(def)) {
        retSet->insert(def);
    }

    OA_ptr<DataFlow::LocDFSetIterator> retval;
    retval = new DataFlow::LocDFSetIterator(*retSet);
    return retval;
}

/*!
   Does a union of all dep pairs and stores the result in this.
*/
DepDFSet& DepDFSet::setUnion(DataFlow::DataFlowSet &other)
{
    DepDFSet& recastOther = dynamic_cast<DepDFSet&>(other);
    if (debug) {
        std::cout << "DepDFSet::setUnion" << std::endl;
    }

    // for each use in other
    LocToLocDFSetMap::const_iterator mapIter;
    for (mapIter=recastOther.mUseToDefsMap.begin();
         mapIter!=recastOther.mUseToDefsMap.end(); mapIter++ )
    {
        OA_ptr<Location> use = mapIter->first;

        // if this use is already in our map then just union it into
        // the associated defs location set
        // if not then will have to make a new LocDFSet and
        // then essentially do the same thing
        if (mUseToDefsMap[use].ptrEqual(0)) {
            mUseToDefsMap[use] = new DataFlow::LocDFSet;
        }
        *(mUseToDefsMap[use]) 
            = mUseToDefsMap[use]->setUnion(*(mapIter->second));
    }
    
    // for each def in other
    for (mapIter=recastOther.mDefToUsesMap.begin();
         mapIter!=recastOther.mDefToUsesMap.end(); mapIter++ )
    {
        OA_ptr<Location> def = mapIter->first;

        if (mDefToUsesMap[def].ptrEqual(0)) {
            mDefToUsesMap[def] = new DataFlow::LocDFSet;
        }
        mDefToUsesMap[def]->setUnion(*(mapIter->second));
    }

    // take on all of other's implicitRemoves that we already have
    mImplicitRemoves->setIntersect(*(recastOther.mImplicitRemoves));

    // union the uses and defs sets
    mUses->setUnion(*(recastOther.mUses));
    mDefs->setUnion(*(recastOther.mDefs));

    return *this;
}


//! helper function that determines if the given location 
//! has been killed due to a must def
bool DepDFSet::isImplicitRemoved(const OA_ptr<Location> loc) const
{
    return mImplicitRemoves->hasLoc(loc);
}

/*!
   Composes dep pairs in this with other
   DepDFSet and stores result in this.

Examples: let <b,b> has been removed from set2 with removeDep
this        other        result
set1        set2
<use1,def1> <use2,def2>
<a,b>       <b,a>       <a,a> explicitly represented but marked as implicitly blocked
<a,b>                   shouldn't have <a,b> unless <b,b> is explicit in set 2   
Unions mImplicitRemoves for this and other

Assumes that getDefsIterator and getUsesIterator handles the mImplicitRemoves
correctly.
In other words those iterators will return reflexive uses and defs
unless the given parameters overlap with something in the implicitRemoves list.

*/
DepDFSet& DepDFSet::compose(DataFlow::DataFlowSet &other)
{
    DepDFSet& recastOther = dynamic_cast<DepDFSet&>(other);
    if (debug) {
        std::cout << "DepDFSet::compose" << std::endl;
        std::cout << "\t*this = ";
        dump(std::cout);
        std::cout << "\tother = ";
        recastOther.dump(std::cout);
    }
    
    OA_ptr<DepDFSet> tempDFSet;
    tempDFSet = new DepDFSet;
    
    // have to do these before do insertDeps because could be inserting
    // a dep that will take things out of the mImplicitRemoves set
    tempDFSet->mImplicitRemoves->setUnion(*(mImplicitRemoves)); 
    tempDFSet->mImplicitRemoves->setUnion(*(recastOther.mImplicitRemoves)); 

    // iterate over deps in this keying off the def
    // see if the def overlaps with anything in recastOther
    LocToLocDFSetMap::const_iterator mapIter;
    for (mapIter=mDefToUsesMap.begin();
         mapIter!=mDefToUsesMap.end(); mapIter++ )
    {
        OA_ptr<Location> def1 = mapIter->first;
        if (debug) {
            std::cout << "def1 = "; def1->dump(std::cout); 
            std::cout << std::endl;
        }
        OA_ptr<DataFlow::LocDFSet> use1Set = mapIter->second;
        if (debug) {
            std::cout << "use1Set = "; use1Set->dump(std::cout);
        }

        // get all the defs in use2,def2 where def1 overlaps with use2
        OA_ptr<LocIterator> def2Iter = recastOther.getDefsIterator(def1);

        // loop over all use1,def1 pairs
        OA_ptr<LocIterator> use1Iter = use1Set->getLocIterator();
        for ( ; use1Iter->isValid(); (*use1Iter)++ ) {
            OA_ptr<Location> use1 = use1Iter->current();

            // get all the defs in use2, def2 where def1 overlaps with use2
            for (def2Iter->reset(); def2Iter->isValid(); (*def2Iter)++ ) {
                tempDFSet->insertDep(use1, def2Iter->current());
                if (debug) {
                    std::cout << "inserting <use1,def2Iter->current> = "; 
                    use1->dump(std::cout); 
                    std::cout << " , ";
                    def2Iter->current()->dump(std::cout); 
                    std::cout << std::endl;
                }
            }

            /*
            // as long as d,d has not been removed from recastOther,
            // where d overlaps def1, we should add use1,def1 to result
            // if it was removed but is explicitly in recastOther then
            // use1,def1 was correctly inserted into the result in above code
            // eg. x = x + y, x,x will be explictly in recastOther and
            // be an implicit that is removed due to the mustdef of x
            if (! recastOther.isImplicitRemoved(def1) ) {
                tempDFSet->insertDep(use1,def1);
                if (debug) {
                    std::cout << "inserting <use1,def1> = "; 
                    use1->dump(std::cout); 
                    std::cout << " , ";
                    def1->dump(std::cout); 
                    std::cout << std::endl;
                }
            }
            */
        }
    }

    // iterate over deps in recastOther keying off the use
    // see if the use overlaps with anything in this
    // even if it doesn't insert dep pairs for use,def 
    // as long as use,use hasn't been removed from this
    for (mapIter=recastOther.mUseToDefsMap.begin();
         mapIter!=recastOther.mUseToDefsMap.end(); mapIter++ )
    {
        OA_ptr<Location> use2 = mapIter->first;
        if (debug) {
            std::cout << "use2 = "; use2->dump(std::cout); 
            std::cout << std::endl;
        }
        OA_ptr<DataFlow::LocDFSet> def2Set = mapIter->second;
        assert(!def2Set.ptrEqual(0));
        if (debug) {
            std::cout << "def2Set = ";
            def2Set->dump(std::cout);
        }

        OA_ptr<LocIterator> use1Iter = getUsesIterator(use2);

        // loop over all use2,def2 pairs
        OA_ptr<LocIterator> def2Iter = def2Set->getLocIterator();
        for ( ; def2Iter->isValid(); (*def2Iter)++ ) {
            OA_ptr<Location> def2 = def2Iter->current();

            // get all the uses in use1,def1 where def1 overlaps with use2
            for (use1Iter->reset(); use1Iter->isValid(); (*use1Iter)++ ) {
                tempDFSet->insertDep(use1Iter->current(), def2);
                if (debug) {
                    std::cout << "inserting <use1Iter->current,def2> = "; 
                    use1Iter->current()->dump(std::cout); 
                    std::cout << " , ";
                    def2->dump(std::cout); 
                    std::cout << std::endl;
                }
            }

            /*
            // as long as u,u has not been removed from this,
            // where u overlaps use2, we should add use2,def2 to result
            if (! isImplicitRemoved(use2) ) {
                tempDFSet->insertDep(use2,def2);
                if (debug) {
                    std::cout << "inserting <use2,def2> = "; 
                    use2->dump(std::cout); 
                    std::cout << " , ";
                    def2->dump(std::cout); 
                    std::cout << std::endl;
                }
            }
            */
        }
    }

    (*this) = *tempDFSet;
    if (debug) {
        std::cout << "\t*this = ";
        dump(std::cout);
    }
    

    return (*this);
}

/*!
   A use or def must be in both for them to be in a result.
   In the sets, an intersection is done as well.

FIXME: this isn't needed right now so not implemented
*/
/*
DepDFSet& DepDFSet::setIntersect(DataFlowSet &other)
{
    assert(0);
}
*/

//*****************************************************************
// Output
//*****************************************************************
void DepDFSet::dump(std::ostream &os) 
{
  /*  
    os << "\tmUseToDefsMap.size() = " << mUseToDefsMap.size() << std::endl;
    os << "\tmDefToUsesMap.size() = " << mDefToUsesMap.size() << std::endl;
   */ 
   
    os << "\tuses" << std::endl;
    // iterate over all uses
    std::map<OA_ptr<Location>,OA_ptr<DataFlow::LocDFSet> >::const_iterator 
        mapIter;
    for (mapIter=mUseToDefsMap.begin();
         mapIter!=mUseToDefsMap.end(); mapIter++ )
    {
        OA_ptr<Location> use = mapIter->first;
        OA_ptr<DataFlow::LocDFSet> locset = mapIter->second;

        os << "\t\t";
        use->dump(os);
        os << " ==> ";
        locset->dump(os);
        os << std::endl;
    }
    
    os << "\tdefs" << std::endl;
    // iterate over all defs
    for (mapIter=mDefToUsesMap.begin();
         mapIter!=mDefToUsesMap.end(); mapIter++ )
    {
        OA_ptr<Location> def = mapIter->first;
        OA_ptr<DataFlow::LocDFSet> locset = mapIter->second;

        os << "\t\t";
        def->dump(os);
        os << " ==> ";
        locset->dump(os);
        os << std::endl;
    }
   
    os << "\tmImplicitRemoves = ";
    mImplicitRemoves->dump(os);
  
    os << "\tmUses = ";
    mUses->dump(os);
  
    os << "\tmDefs = ";
    mDefs->dump(os);

}

void DepDFSet::dump(std::ostream &os) const
{
   
/*
    os << "\tmUseToDefsMap.size() = " << mUseToDefsMap.size() << std::endl;
    os << "\tmDefToUsesMap.size() = " << mDefToUsesMap.size() << std::endl;
*/

    os << "\tuses" << std::endl;
    // iterate over all uses
    std::map<OA_ptr<Location>,OA_ptr<DataFlow::LocDFSet> >::const_iterator 
        mapIter;
    for (mapIter=mUseToDefsMap.begin();
         mapIter!=mUseToDefsMap.end(); mapIter++ )
    {
        OA_ptr<Location> use = mapIter->first;
        OA_ptr<DataFlow::LocDFSet> locset = mapIter->second;

        os << "\t\t";
        use->dump(os);
        os << " ==> ";
        locset->dump(os);
        os << std::endl;
    }
    
    os << "\tdefs" << std::endl;
    // iterate over all defs
    for (mapIter=mDefToUsesMap.begin();
         mapIter!=mDefToUsesMap.end(); mapIter++ )
    {
        OA_ptr<Location> def = mapIter->first;
        OA_ptr<DataFlow::LocDFSet> locset = mapIter->second;

        os << "\t\t";
        def->dump(os);
        os << " ==> ";
        locset->dump(os);
        os << std::endl;
    }
   
    os << "\tmImplicitRemoves = ";
    mImplicitRemoves->dump(os);
  
    os << "\tmUses = ";
    mUses->dump(os);
  
    os << "\tmDefs = ";
    mDefs->dump(os);

 }


void DepDFSet::dump(std::ostream &os, OA_ptr<IRHandlesIRInterface> ir) 
{
/*
    os << "\tmUseToDefsMap.size() = " << mUseToDefsMap.size() << std::endl;
    os << "\tmDefToUsesMap.size() = " << mDefToUsesMap.size() << std::endl;
*/


    os << "\tuses" << std::endl;
    // iterate over all uses
    std::map<OA_ptr<Location>,OA_ptr<DataFlow::LocDFSet> >::const_iterator mapIter;
    for (mapIter=mUseToDefsMap.begin();
         mapIter!=mUseToDefsMap.end(); mapIter++ )
    {
        OA_ptr<Location> use = mapIter->first;
        OA_ptr<DataFlow::LocDFSet> locset = mapIter->second;

        os << "\t\t";
        use->dump(os,ir);
        os << " ==> ";
        locset->dump(os,ir);
        os << std::endl;
    }
    
    os << "\tdefs" << std::endl;
    // iterate over all defs
    for (mapIter=mDefToUsesMap.begin();
         mapIter!=mDefToUsesMap.end(); mapIter++ )
    {
        OA_ptr<Location> def = mapIter->first;
        OA_ptr<DataFlow::LocDFSet> locset = mapIter->second;

        os << "\t\t";
        def->dump(os,ir);
        os << " ==> ";
        locset->dump(os,ir);
        os << std::endl;
    }
    
    // dump implicitRemoves
    os << "\tmImplicitRemoves = ";
    mImplicitRemoves->dump(os,ir); 
  
    os << "\tmUses = ";
    mUses->dump(os,ir);
  
    os << "\tmDefs = ";
    mDefs->dump(os,ir);

}

void DepDFSet::output(OA::IRHandlesIRInterface& ir) {
    
    // iterate over all uses
    sOutBuild->mapStart("mUseToDefsMap","UseLoc","DefLocSet");
    std::map<OA_ptr<Location>,OA_ptr<DataFlow::LocDFSet> >::const_iterator mapIter;
    for (mapIter=mUseToDefsMap.begin();
         mapIter!=mUseToDefsMap.end(); mapIter++ )
    {
        OA_ptr<Location> use = mapIter->first;
        OA_ptr<DataFlow::LocDFSet> locset = mapIter->second;
        sOutBuild->mapEntryStart();
          sOutBuild->mapKeyStart();
            use->output(ir);
          sOutBuild->mapKeyEnd();
          sOutBuild->mapValueStart();
            locset->output(ir);
          sOutBuild->mapValueEnd();
        sOutBuild->mapEntryEnd();
    }
    sOutBuild->mapEnd("mUseToDefsMap");
    
    // iterate over all defs
    sOutBuild->mapStart("mDefToUsesMap","DefLoc","UseLocSet");
    for (mapIter=mDefToUsesMap.begin();
         mapIter!=mDefToUsesMap.end(); mapIter++ )
    {
        OA_ptr<Location> def = mapIter->first;
        OA_ptr<DataFlow::LocDFSet> locset = mapIter->second;
        sOutBuild->mapEntryStart();
          sOutBuild->mapKeyStart();
            def->output(ir);
          sOutBuild->mapKeyEnd();
          sOutBuild->mapValueStart();
            locset->output(ir);
          sOutBuild->mapValueEnd();
        sOutBuild->mapEntryEnd();
    }
    sOutBuild->mapEnd("mDefToUsesMap");

    
    // dump implicitRemoves
    std::ostringstream label1;
    label1 << indt << "ImplicitRemoves:";
    sOutBuild->outputString( label1.str() );
    mImplicitRemoves->output(ir); 

    std::ostringstream label2;
    label2 << indt << "Uses:";
    sOutBuild->outputString( label2.str() );
    mUses->output(ir); 

    std::ostringstream label3;
    label3 << indt << "Defs:";
    sOutBuild->outputString( label3.str() );
    mDefs->output(ir); 
    
}


//*****************************************************************
// Construction
//*****************************************************************
  
/*! 
    Inserts a dep pair <use,def> into the set of Deps.

    The reflexive pairs (eg. <a,a>) are implicitly assumed
    in the datastructure unless they have been explicitly
    removed with removeImplicitDep.  If they have,
    calling this routine will take them out of removeImplicitDep.
    This comes about when a statement
    such as (a = a + b) occurs.  a still depends on itself
    because it is on the rhs and lhs.

*/
void DepDFSet::insertDep(OA_ptr<Location> use, OA_ptr<Location> def)
{
    if (use==def) {
        mImplicitRemoves->remove(use);

    } else {
        if (mUseToDefsMap[use].ptrEqual(0)) {
            mUseToDefsMap[use] = new DataFlow::LocDFSet;
        }
        if (mDefToUsesMap[def].ptrEqual(0)) {
            mDefToUsesMap[def] = new DataFlow::LocDFSet;
        }

        mUseToDefsMap[use]->insert(def);
        mDefToUsesMap[def]->insert(use);

        mUses->insert(use);
        mDefs->insert(def);
    }
}

/*!
   Must allow removeImplicitDep and insertDep to be commutative on
   DepDFSet.  Made possible by taking things out of implicitRemoves
   if it gets added explicitly.
*/
void DepDFSet::removeImplicitDep(OA_ptr<Location> use, OA_ptr<Location> def)
{
//    mMakeImplicitExplicitMemoized = false;
    // to be conservative only insert locations when the use and def must
    // overlap each other, sublocs that only have partial accuracy will
    // only may loc
    if (use->mustOverlap(*def)) {
        mImplicitRemoves->insert(use);
    }
}

  } // end of Activity namespace
} // end of OA namespace
