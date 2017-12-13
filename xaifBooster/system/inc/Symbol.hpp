#ifndef _SYMBOL_INCLUDE_
#define _SYMBOL_INCLUDE_
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

#include "xaifBooster/utils/inc/XMLPrintable.hpp"
#include "xaifBooster/utils/inc/ObjectWithId.hpp"
#include "xaifBooster/utils/inc/GenericTraverseInvoke.hpp"
#include "xaifBooster/utils/inc/HashTable.hpp"
#include "xaifBooster/utils/inc/FrontEndDecorations.hpp"

#include "xaifBooster/system/inc/ObjectWithAnnotation.hpp"
#include "xaifBooster/system/inc/SymbolKind.hpp"
#include "xaifBooster/system/inc/SymbolType.hpp"
#include "xaifBooster/system/inc/FrontEndType.hpp"
#include "xaifBooster/system/inc/SymbolShape.hpp"
#include "xaifBooster/system/inc/DimensionBounds.hpp"
#include "xaifBooster/system/inc/SymbolAlgBase.hpp"

namespace xaifBooster { 

  class Symbol : public XMLPrintable,
		 public ObjectWithId,
		 public ObjectWithAnnotation,
		 public GenericTraverseInvoke {
  public:
    
    const SymbolKind::SymbolKind_E& getSymbolKind() const;

    const SymbolType::SymbolType_E& getSymbolType() const;

    const SymbolShape::SymbolShape_E& getSymbolShape() const;

    bool getActiveTypeFlag() const;

    void printXMLHierarchy(std::ostream& os) const;

    void printXMLHierarchyImpl(std::ostream& os) const;

    std::string debug() const ;

    
    /**
     * name for this class as represented in XAIF schema
     */
    static const std::string ourXAIFName;

    /**
     * name for inherited member myId as represented in XAIF schema
     */
    static const std::string our_myId_XAIFName;

    /**
     * name for member myActiveTypeFlag as represented in XAIF schema
     */
    static const std::string our_myActiveTypeFlag_XAIFName;

    /**
     * name for inherited member myTempFlag as represented in XAIF schema
     */
    static const std::string our_myTempFlag_XAIFName;

    /**
     * Destructor.
     * Symbols should only be destroyed through SymbolTable's methods
     * but it is not private because it is used for a hash table template
     */    
    virtual ~Symbol();

    typedef std::list<DimensionBounds*> DimensionBoundsPList;

    void addDimensionBounds(int aLower, 
			    int anUpper);

    const DimensionBoundsPList& getDimensionBoundsPList() const;

    bool hasDimensionBounds()const;

    /**
     * get algorithm
     */
    SymbolAlgBase& getSymbolAlgBase() const;

    virtual void traverseToChildren(const GenericAction::GenericAction_E anAction_c);

    static void addSymbolNamesToPassivate(const std::string& theSymbolNamesSeparatedBySpaces);

    /** 
     * passivate according to the symbol names 
     * passed into addSymbolNamesToPassivate
     * this should happen AFTER all scopes are 
     * initialized.
     */
    void forcedPassivation(); 

    /** 
     *  symbol name without front-end decorations
     */
    std::string plainName() const;

    static void setFrontEndDecorations(const FrontEndDecorations::FrontEndDecorations_E& aStyle);

    static std::string stripFrontEndDecorations(const std::string& aDecoratedName,
						bool isSubroutineName);

    static void setCaseSensitive();

    /** 
     * checks if a given symbol can be matched 
     * to a plain name taking into account 
     * front-end decorations and case sensitivity
     */
    bool samePlainName(const std::string& aPlainName) const; 

    const FrontEndType& getFrontEndType() const;

    void setFrontEndType(const FrontEndType&);

    bool isTemporary() const;

  private:

    friend class SymbolTable;

    /**
     * Constructor.
     * Symbols can only be created through SymbolTable::addSymbol
     * otherwise we deal with references only
     */      
    Symbol(const std::string& aName, 
	   const SymbolKind::SymbolKind_E& aKind,
	   const SymbolType::SymbolType_E& aType,
	   const SymbolShape::SymbolShape_E& aShape,
	   bool anActiveTypeFlag,
	   bool aTempFlag,
	   bool makeAlgorithm=true);

    const SymbolKind::SymbolKind_E myKind;
    const SymbolType::SymbolType_E myType;
    FrontEndType myFrontEndType;
    const SymbolShape::SymbolShape_E myShape;

    /**
     * no def
     */
    Symbol(const Symbol&);

    /**
     * no def
     */
    Symbol& operator=(const Symbol&);

    /**
     * is it an active data type?
     */
    bool myActiveTypeFlag;

    /**
     * is this a temporary variable created by xaifBooster?
     */
    bool myTempFlag;

    /** 
     * optionally empty list of dimension bounds
     * as a tentative rule: we either know all or no dimension bounds
     * we own the items in the list
     */
    DimensionBoundsPList myDimensionBoundsPList;
    
    /**
     * this will be set to point a dynamically instance
     * during construction and deleted during
     * destruction
     */
    SymbolAlgBase* mySymbolAlgBase_p;

    /** 
     * to keep track what has been passivated for which name
     */
    class SymbolPassivation { 
    public:

      SymbolPassivation();

      SymbolPassivation(const std::string& aCommandLineName);

      void passivate(const std::string& anXaifName,
		     Symbol& aSymbol);

      bool hasPassivatedSymbol() const; 

      std::string getCommandLineName()const; 

      Symbol& getSymbol()const; 

    private: 

      /**
       * the plain name, with front-end suffix
       */
      std::string myCommandLineName;

      /**
       * the name with front-end suffix as found in xaif
       */
      std::string myXaifName;

      /** 
       * the symbol once we found a matching name
       */
      Symbol* mySymbolp;

      /**
       * did the xaif specify the symbol to be active?
       * in case we need to move the reference up the scope
       * hierarchy this flag allows resetting the activity 
       * of the symbol we first found
       */
      bool myPassivateFlag;

      /** 
       * avoid duplcate warnings
       */
      bool myWarnedFlag; 
    };
      
    
    /** 
     * list of symbols forced to be passive
     * since we don't have a scope context
     * only the top scope matching symbols are going to 
     * be passivated.
     * This is only a stop gap measure to overcome 
     * certain overestimates in the activity analysis
     * mostly for global module variables.
     * the parser will issue warnings for symbols
     * with lower level scope and errors for 
     * ambiguous scopes.
     */
    static HashTable<SymbolPassivation> ourPassivatedSymbolHashTable;

    /**
     * are symbol names case sensitive?
     */
    static bool ourCaseSensitiveFlag;

    /**
     * front end decoration style
     */
    static FrontEndDecorations::FrontEndDecorations_E ourFrontEndDecorations;

  };
 
} // end of namespace xaifBooster
                                                                     
#endif
