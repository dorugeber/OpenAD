#ifndef _MARKER_INCLUDE_
#define _MARKER_INCLUDE_
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

#include "xaifBooster/system/inc/BasicBlockElement.hpp"
#include "xaifBooster/system/inc/MarkerAlgBase.hpp"

namespace xaifBooster { 

  /**
   * nop statement with an annotation
   */

  class Marker : public BasicBlockElement {
  public:

    Marker(bool makeAlgorithm=true);

    virtual ~Marker();
    
    /** 
     * algorithm access where the Marker may 
     * be const but in difference to the 
     * internal representation (wich is always 
     * const for the algorithms) the algorithm 
     * instances will always be modifiable.
     */
    MarkerAlgBase& getMarkerAlgBase()const;

    /**
     * Debug routine
     */    
    virtual std::string debug() const;

    /**
     * Get annotation if set
     */    
    const std::string& getAnnotation() const;
    
    /**
     * set annotation once
     */
    void setAnnotation(const std::string& anAnnotation);

    virtual void printXMLHierarchy(std::ostream& os) const;

    /**
     * Actual implementation for printing XAIF
     * of this expression
     */
    void printXMLHierarchyImpl(std::ostream& os) const;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c) {};

    /** 
     * name for this class as represented in XAIF schema
     */
    static const std::string ourXAIFName;

    /**
     * name for inherited member myId as represented in XAIF schema
     */
    static const std::string our_myId_XAIFName;

    /**
     * name for member myAnnotation as represented in XAIF schema
     */
    static const std::string our_myAnnotation_XAIFName;

  private:

    /**
     * the annotation
     */
    std::string myAnnotation;
    
    /**
     * prevent annotation from changing once it is set
     */
    bool myAnnotationFlag;

  };  // end  of class Marker 

} // end of namespace xaifBooster
                                                                     
#endif

