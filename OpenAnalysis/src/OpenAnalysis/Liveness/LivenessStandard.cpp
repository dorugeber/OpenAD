// LivenessStandard.cpp


#include "LivenessStandard.hpp"

namespace OA {
  namespace Liveness {

OA_ptr<LocIterator> LivenessStandard::getLivenessIterator(StmtHandle s) 
{ 
    OA_ptr<LocIterator> retval;

    if (mLiveness[s].ptrEqual(0)) {
        OA_ptr<LocSet> emptySet; emptySet = new LocSet;
        retval = new LocSetIterator(emptySet);
    } else {
    retval = new LocSetIterator(mLiveness[s]);
    }
    return retval;
}

OA_ptr<LocIterator> LivenessStandard::getExitLivenessIterator() 
{ 
    OA_ptr<LocIterator> retval;
     if (mExitLiveness.ptrEqual(0)) {
        OA_ptr<LocSet> emptySet; emptySet = new LocSet;
        retval = new LocSetIterator(emptySet);
    }
    else {
    retval = new LocSetIterator(mExitLiveness);
    }
    return retval;
}

void LivenessStandard::output(IRHandlesIRInterface& ir)
{    
    sOutBuild->objStart("LivenessStandard");

    sOutBuild->mapStart("mLiveness", "StmtHandle", "OA_ptr<LocSet>");
    std::map<StmtHandle,OA_ptr<LocSet> >::iterator mapIter;
    for (mapIter = mLiveness.begin(); mapIter != mLiveness.end(); mapIter++) {
        StmtHandle s = mapIter->first;
        OA_ptr<LocSet> rdset = mapIter->second;
        if ( rdset.ptrEqual(0) ) continue;

        sOutBuild->mapEntryStart();
        sOutBuild->mapKeyStart();
        sOutBuild->outputIRHandle(s, ir);
        sOutBuild->mapKeyEnd();
        sOutBuild->mapValueStart();
       
       
        sOutBuild->listStart();
        OA_ptr<LocIterator> setIter;
        setIter = getLivenessIterator(s);
        for ( ; setIter->isValid(); (*setIter)++ ) {
             sOutBuild->listItemStart(); {
             setIter->current()->output(ir);  
            } sOutBuild->listItemEnd();
        }
        sOutBuild->listEnd();  
        sOutBuild->mapValueEnd();
        sOutBuild->mapEntryEnd();
    }
    sOutBuild->mapEnd("mLiveness");
   
/*    sOutBuild->fieldStart("mExitLiveness");
    sOutBuild->listStart();
    OA_ptr<LocIterator> setIter1;
     setIter1 = getExitLivenessIterator();
    for (;setIter1->isValid(); (*setIter1)++)
    {
        sOutBuild->listItemStart();{
          setIter1->current()->output(ir);
        }
        sOutBuild->listItemEnd();
    }
    sOutBuild->listEnd();
    sOutBuild->fieldEnd("mExitLiveness");*/

    sOutBuild->objEnd("LivenessStandard");
}



void LivenessStandard::dump(std::ostream& os, OA_ptr<IRHandlesIRInterface> ir)
{
    std::cout<< "======Liveness===="<< std::endl;
    OA_ptr<LocIterator> locIterPtr;
    std::map<StmtHandle,OA_ptr<LocSet> > ::iterator mapIter;
    for (mapIter = mLiveness.begin(); mapIter != mLiveness.end(); mapIter++) {
        std::cout<< "\tstmt = " << ir->toString(mapIter->first) << std::endl; 
         locIterPtr = getLivenessIterator(mapIter->first);
        for ( ; locIterPtr->isValid(); (*locIterPtr)++ ) {
            std::cout << "\t\t";
            //locIterPtr->current()->output(*ir);
            locIterPtr->current()->dump(std::cout,ir);
            std::cout << std::endl;
        }

    }
}



  } // end of Liveness namespace
} // end of OA namespace
