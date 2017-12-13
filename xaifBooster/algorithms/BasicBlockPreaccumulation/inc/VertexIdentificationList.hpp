#ifndef _VERTEXIDENTIFICATIONLIST_INCLUDE_
#define _VERTEXIDENTIFICATIONLIST_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/Debuggable.hpp"
#include "xaifBooster/system/inc/Variable.hpp"
#include "xaifBooster/system/inc/AliasMap.hpp"
#include "xaifBooster/system/inc/DuUdMap.hpp"

using namespace xaifBooster;

namespace xaifBoosterBasicBlockPreaccumulation { 

  /**
   * this is the list against which we will check 
   * for identities with other Variable and Argument instances
   */
  class VertexIdentificationList : public Debuggable {

  public:

    VertexIdentificationList();

    virtual ~VertexIdentificationList();

    virtual std::string debug() const;

    /** 
     * the result of an identification attempt
     */
    enum IdentificationResult_E{ NOT_IDENTIFIED,
				 AMBIGUOUSLY_IDENTIFIED,
				 UNIQUELY_IDENTIFIED };

    // JU: I don't believe ListItem should have to be 
    // public but SUN CC 5.6 insists. Moved protected down
    //  protected:

    /** 
     * an entry in the list 
     */
    class ListItem : public Debuggable { 
      
    public: 

      ListItem(const AliasMapKey& anAliasMapKey,
	       const StatementIdSetMapKey& aDuUdMapKey);

      virtual ~ListItem(){};

      const AliasMapKey& getAliasMapKey()const;

      const StatementIdSetMapKey& getDuUdMapKey()const;
      
      virtual std::string debug() const;
      
    private: 

      /**
       * no def
       */
      ListItem();

      /**
       * no def
       */
      ListItem(const ListItem&);

      /**
       * no def
       */
      ListItem& operator=(const ListItem&);

      const AliasMapKey& myAliasMapKey;
      const StatementIdSetMapKey& myDuUdMapKey;
      
    };
   
   protected: 
 
    /** 
     * retrieves the alias keys found in myList
     */
    void getAliasMapKeyPList(AliasMap::AliasMapKeyPList& anAliasMapKeyPList) const;

    /**
     * defining this here requires us
     * to do dynamic casts unless we
     * want to start deriving our own list classes...
     */
    typedef std::list<ListItem*> ListItemPList;

    /**
     * this list owns all the items in it
     */
    ListItemPList myList;

    /** 
     * lock down list behavior to be based on 
     * DuUdMap
     * once it is locked down we cannot change back
     */
    void baseOnDuUdMap()const; 

    bool isDuUdMapBased()const;

  private:

    /** 
     * track if this list is DuUdMap based or not
     * behavior changes based on this flag
     */
    mutable bool myBasedOnDuUdMapFlag;

  }; // end of class VertexIdentificationList  
   
} // end namespace 
                                                                  
#endif
