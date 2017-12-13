#ifndef _OBJECTWITHID_INCLUDE_
#define _OBJECTWITHID_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <string>
#include "xaifBooster/utils/inc/Debuggable.hpp"

namespace xaifBooster { 

  /**
   * an abstract base class of classes with an id
   * that can be set once and throws an exception 
   * if we try to get an Id that has not been set
   * There is on purpose no default!
   */
  class ObjectWithId : virtual public Debuggable {
  public:

    ObjectWithId();

    virtual ~ObjectWithId() {};

    typedef std::string Id;

    /**
     * Debug routine
     */    
    virtual Id debug() const;

    /**
     * Get id if set
     */    
    const Id& getId() const;
    
    /**
     * set Id once
     */
    void setId(const Id& anId);

    /**
     * set Id once
     */
    void setId(int anId);
    
    /** 
     * when copying vertices from 
     * one graph into another.
     * this should be used with a lot of caution. 
     * E.g. the boost edges rely on internal descriptors 
     * but the XAIF edges rely on this Id
     */
    void overWriteId(const Id& anId);

    /** 
     * when copying vertices from 
     * one graph into another.
     * this should be used with a lot of caution. 
     * E.g. the boost edges rely on internal descriptors 
     * but the XAIF edges rely on this Id
     */
    void overWriteId(unsigned int anId);

  private:

    /**
     * the Id
     */
    Id myId;
    
    /**
     * prevent Id from changing once it is set
     */
    bool myIdFlag;

  };  // end  of class ObjectWithId 

} // end of namespace xaifBooster
                                                                     
#endif

