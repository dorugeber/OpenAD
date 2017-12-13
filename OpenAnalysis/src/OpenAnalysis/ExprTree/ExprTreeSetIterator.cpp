#include "ExprTreeSetIterator.hpp"

namespace OA {

    ExprTreeSetIterator::ExprTreeSetIterator(OA_ptr<ExprTreeSet> pSet): mSet(pSet) {
       mIter = mSet->begin();
    }
    
    ExprTreeSetIterator::~ExprTreeSetIterator () {}

    void ExprTreeSetIterator::operator++(){
    if (mIter!=mSet->end()) ++mIter;
    }

    void ExprTreeSetIterator::reset() {
    mIter = mSet->begin();
    }

    //! is the iterator at the end
    bool ExprTreeSetIterator::isValid(){
    return (mIter != mSet->end());
    }

    //! return current node
    OA_ptr<ExprTree> ExprTreeSetIterator::current(){
    if (isValid()) {
        return *mIter;
    } else {
        OA_ptr<ExprTree> retval;
        return retval;
    }
   }


}//end namespace


