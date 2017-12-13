#ifndef _INDEXTRIPLET_INCLUDE_
#define _INDEXTRIPLET_INCLUDE_
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
#include "xaifBooster/system/inc/Expression.hpp"

namespace xaifBooster { 

  /**
   * contains array dereference allowing 
   * for slicing and non-unit stride
   */
  class IndexTriplet {
  public:

    IndexTriplet ();

    virtual ~IndexTriplet();

    void printXMLHierarchy(std::ostream& os) const;

    void printXMLHierarchyIndexTripletElement(std::ostream& os,
					     const Expression& theIndex,
					     const std::string& anXaifName) const;

    std::string debug() const;

    /** 
     * \todo implementation incomplete
     */
    virtual std::string equivalenceSignature() const;

    static const std::string ourXAIFName;
    static const std::string our_myIndex_XAIFName;
    static const std::string our_myBound_XAIFName;
    static const std::string our_myStride_XAIFName;

    void copyMyselfInto(IndexTriplet& anotherIndexTriplet) const ;

    enum IndexTriplet_E{
      IT_INDEX,
      IT_BOUND,
      IT_STRIDE};
      
    /** 
     * add an expression identified by discriminator
     */
    Expression& addExpression(const IndexTriplet_E& discriminator);

    /** 
     * get an expression identified by discriminator
     */
    const Expression& getExpression(const IndexTriplet_E& discriminator) const;
    Expression& getExpression(const IndexTriplet_E& discriminator);

    /** 
     * test if an expression identified by discriminator exists
     */
    bool hasExpression(const IndexTriplet_E& discriminator) const;

    typedef std::pair<IndexTriplet_E,Expression*> IndexPair;

    typedef std::list<IndexPair> IndexPairList;

    const IndexPairList& getIndexPairList() const;

    IndexPairList& getIndexPairList();

    /** 
     * if the extent of the slice is 1 
     * this returns true
     */
    bool isScalarDeref() const; 

  private:
    
    /**
     * no def
     */
    IndexTriplet(const IndexTriplet&);

    /**
     * no def
     */
    IndexTriplet& operator=(const IndexTriplet&);

    /** 
     * the list containing the index,bound,stride (all optional)
     */ 
    IndexPairList myIndexPairList;

  };
 
} // end of namespace xaifBooster
                                                                     
#endif
