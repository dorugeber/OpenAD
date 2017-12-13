#ifndef _ARRAYACCESS_INCLUDE_
#define _ARRAYACCESS_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <list>
#include "xaifBooster/system/inc/VariableVertex.hpp"
#include "xaifBooster/system/inc/IndexTriplet.hpp"

namespace xaifBooster { 

  /**
   * class to identify an array access operation, 
   * e.g. [] operator in C++ that is represented 
   * as a graph defined as BaseVariableRerefence
   * used on LHS of assignments and in expressions
   */
  class ArrayAccess : public VariableVertex {
  public:

    ArrayAccess (){};

    ~ArrayAccess(){};

    void printXMLHierarchy(std::ostream& os) const;

    /** 
     * \todo implementation incomplete
     */
    std::string debug() const { return std::string("ArrayAccess");} ;

    /** 
     * \todo implementation incomplete
     */
    virtual std::string equivalenceSignature() const;

    static const std::string ourXAIFName;
    static const std::string our_myId_XAIFName;

    virtual VariableVertex& createCopyOfMyself() const ;

    typedef std::list<IndexTriplet*> IndexTripletListType;

    /**
     * access index list
     */
    const IndexTripletListType& getIndexTripletList() const ;

    /**
     * access index list
     */
    IndexTripletListType& getIndexTripletList();
    
    /** 
     * add a new expression to the list 
     */
    IndexTriplet& addIndexTriplet();

    /**
     * Check whether this array access has \p anExpression anywhere inside the index expressions
     */
    virtual bool hasExpression(const Expression& anExpression) const;

  private:
    
    /**
     * no def
     */
    ArrayAccess(const ArrayAccess&);

    /**
     * no def
     */
    ArrayAccess& operator=(const ArrayAccess&);

    /**
     * Array indeces
     */
    IndexTripletListType myIndexTripletList;

  };
 
} // end of namespace xaifBooster
                                                                     
#endif
