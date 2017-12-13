#ifndef _NONINLINABLEINTRINSICSCATALOGUE_INCLUDE_
#define _NONINLINABLEINTRINSICSCATALOGUE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/system/inc/NonInlinableIntrinsicsCatalogueItem.hpp"
#include "xaifBooster/utils/inc/HashTable.hpp"

namespace xaifBooster { 
  
  /**
   * the catalogue of all noninlinable intrinsics
   */
  class NonInlinableIntrinsicsCatalogue { 
    
  public: 
    
    NonInlinableIntrinsicsCatalogue();

    /**
     * deletes the pointers in 
     * myHashTable
     * \todo JU needs to implement the deletion
     */
    ~NonInlinableIntrinsicsCatalogue();
    
    /**
     * the key should be a unique name, 
     * e.g. think of mangled names in C++
     * to resolve ambiguities in the signature
     */
    NonInlinableIntrinsicsCatalogueItem& addCatalogueItem(const std::string& theKey,
							  bool aNonSmoothFlag);
    
    /** 
     * look it up by the name, 
     * will throw an exception if not found
     */
    const NonInlinableIntrinsicsCatalogueItem& getElement(const std::string& theKey) const;

    /**
     * name as specified in XAIF schema
     */
    static const std::string ourXAIFName;

    typedef HashTable<NonInlinableIntrinsicsCatalogueItem*> HashTableType;

  private: 

    /**
     * no def
     */
    NonInlinableIntrinsicsCatalogue(const NonInlinableIntrinsicsCatalogue&);

    /**
     * no def
     */
    NonInlinableIntrinsicsCatalogue& operator=(const NonInlinableIntrinsicsCatalogue&);    


    /**
     * the actual container
     * this class instance owns the pointers,
     * the are allocated in addCatalogueItem
     * and deleted in the class' dtor
     */
    HashTableType myHashTable;
    
  }; 
  
} 

#endif
