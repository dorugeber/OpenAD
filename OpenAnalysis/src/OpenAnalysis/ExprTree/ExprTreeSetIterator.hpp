#ifndef ExprTreeSetIterator_H
#define ExprTreeSetIterator_H

#include <OpenAnalysis/ExprTree/ExprTree.hpp>

namespace OA {

class ExprTreeSetIterator {
public:

    ExprTreeSetIterator(OA_ptr<ExprTreeSet> pSet);
   
    ~ExprTreeSetIterator();

    void operator++();

    void reset();

    bool isValid();

    OA_ptr<ExprTree> current();



private:
    OA_ptr<ExprTreeSet> mSet;
    ExprTreeSet::const_iterator mIter;
};
} //end namespace OA

#endif


