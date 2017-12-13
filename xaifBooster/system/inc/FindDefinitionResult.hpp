#ifndef _FINDDEFINITIONRESULT_INCLUDE_
#define _FINDDEFINITIONRESULT_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================


namespace xaifBooster { 

  class BasicBlockElement;


  class Assignment;

  /** 
   * class to return a definition statement 
   * if one was found
   */
  class FindDefinitionResult { 

  public:
    
    FindDefinitionResult(const BasicBlockElement* aDefinitionStatement_p);

    ~FindDefinitionResult() {};

    bool foundIt();

    /** 
     * what defines a value may not 
     * be an assignment but some other 
     * kind of statement like a subroutine call etc.
     */ 
    bool foundAssignment();
    
    const Assignment& getAssignment();

  private:

    /**
     * no def
     */
    FindDefinitionResult();

    /**
     * no def
     */
    FindDefinitionResult& operator=(const FindDefinitionResult&);

    /**
     * if this is null we haven't found anything
     */
    const BasicBlockElement* myDefinitionStatement_p;

  }; // end of class FindDefinitionResult

} // end of namespace xaifBooster
                                                                     
#endif
