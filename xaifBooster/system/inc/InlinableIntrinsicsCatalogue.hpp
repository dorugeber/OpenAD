#ifndef _INLINABLEINTRINSICSCATALOGUE_INCLUDE_
#define _INLINABLEINTRINSICSCATALOGUE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/InlinableIntrinsicsCatalogueItem.hpp"
#include "xaifBooster/utils/inc/HashTable.hpp"

namespace xaifBooster { 
  
  /**
   * the catalogue of all the inlinable intrinsics of 
   * a given programming language.
   */
  class InlinableIntrinsicsCatalogue { 
    
  public: 
    
    InlinableIntrinsicsCatalogue();

    /**
     * deletes the pointers in 
     * myHashTable
     * \todo JU needs to implement the deletion
     */
    ~InlinableIntrinsicsCatalogue();
    
    /**
     * the key should be a unique name, 
     * e.g. think of mangled names in C++
     * to resolve ambiguities in the signature
     */
    InlinableIntrinsicsCatalogueItem& addCatalogueItem(const std::string& theKey,
						       const std::string& theNumberOfArguments,
						       bool aNonSmoothFlag,
						       ShapeChange::ShapeChange_E aShapeChange);
    
    /** 
     * look it up by the name, 
     * will throw an exception if not found
     */
    const InlinableIntrinsicsCatalogueItem& getElement(const std::string& theKey) const;

    /**
     * name as specified in XAIF schema
     */
    static const std::string ourXAIFName;

    typedef HashTable<InlinableIntrinsicsCatalogueItem*> HashTableType;

  private: 

    /**
     * no def
     */
    InlinableIntrinsicsCatalogue(const InlinableIntrinsicsCatalogue&);

    /**
     * no def
     */
    InlinableIntrinsicsCatalogue& operator=(const InlinableIntrinsicsCatalogue&);    


    /**
     * the actual container
     * this class instance owns the pointers,
     * the are allocated in addCatalogueItem
     * and deleted in the class' dtor
     */
    HashTableType myHashTable;
    
  }; // end of class InlinableIntrinsicsCatalogue
  
} 

#endif
