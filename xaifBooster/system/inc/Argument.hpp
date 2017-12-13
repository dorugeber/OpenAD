#ifndef _ARGUMENT_INCLUDE_
#define _ARGUMENT_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include <set>

#include "xaifBooster/system/inc/Variable.hpp"
#include "xaifBooster/system/inc/ExpressionVertex.hpp"
#include "xaifBooster/system/inc/ArgumentAlgBase.hpp"

namespace xaifBooster { 

  /**
   * a class to express a variable argument within 
   * an expression, i.e. think of a variable
   *  inside a LHS of an assignment as 
   * part of the expression graph 
   * vs a RHS. However, we use the RHS representation, 
   * the Variable as a data member
   */
  class Argument : public ExpressionVertex { 

  public: 

    Argument(bool makeAlgorithm=true);
    
    ~Argument(){};

    void printXMLHierarchy(std::ostream& os) const;
    void printXMLHierarchyImpl(std::ostream& os) const;
    void printXMLHierarchyImpl(std::ostream& os, const Expression& theExpression) const {};
    
    std::string debug() const ;

    static const std::string ourXAIFName;
    static const std::string our_myId_XAIFName;

    Variable& getVariable();

    const Variable& getVariable()const;

    virtual std::string equivalenceSignature() const ;

    /** 
     * deep copy
     */
    virtual ExpressionVertex& createCopyOfMyself(bool withAlgorithm=false) const;

    ArgumentAlgBase& getArgumentAlgBase() const;

    void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    /**
     * @return true
     */
    virtual bool isArgument() const;

  private: 

    /**
     * no def
     */
    Argument (const Argument&);

    /**
     * no def
     */
    Argument& operator=(const Argument&);

    /**
     * the actual representation of the 
     * variable 
     */
    Variable myVariable; 

  };  

  typedef std::set<const Argument*> CArgumentPSet;

} 

#endif 
