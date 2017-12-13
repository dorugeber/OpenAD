#ifndef _SYMBOLTABLE_INCLUDE_
#define _SYMBOLTABLE_INCLUDE_
// ========== begin copyright notice ===================
// This file is part of
// ---------------
// xaifBooster
// ---------------
// which is distributed under the BSD license.
// The full COPYRIGHT notice can be found in the top
// level directory of the xaifBooster distribution.
// ========== end copyright notice =====================

#include "xaifBooster/utils/inc/HashTable.hpp"
#include "xaifBooster/utils/inc/XMLPrintable.hpp"
#include "xaifBooster/utils/inc/NameCreator.hpp"
#include "xaifBooster/system/inc/Symbol.hpp"

namespace xaifBooster { 
  
  class Variable;

  class SymbolTable : public XMLPrintable {
  public:
    
    SymbolTable();

    ~SymbolTable();

    /**
     * adds the symbol to the table, 
     */
    Symbol&  addSymbol(const std::string& aName, 
		       const SymbolKind::SymbolKind_E& aKind,
		       const SymbolType::SymbolType_E& aType,
		       const SymbolShape::SymbolShape_E& aShape,
		       bool anActiveTypeFlag,
		       bool aTempFlag);

    Symbol&  addUniqueAuxSymbol(const SymbolKind::SymbolKind_E& aKind,
				const SymbolType::SymbolType_E& aType,
				const SymbolShape::SymbolShape_E& aShape,
				bool anActiveTypeFlag);

    Symbol& addUniqueSymbol(const NameCreator& aNameCreator,
                            const SymbolKind::SymbolKind_E& aKind,
                            const SymbolType::SymbolType_E& aType,
                            const SymbolShape::SymbolShape_E& aShape,
                            bool anActiveTypeFlag);

    const Symbol& getSymbol(const std::string& aName) const;

    std::list<Symbol*> getActiveSymbols();

    std::string debug() const ; 

    void printXMLHierarchy(std::ostream& os) const; 

    /**
     * name as specified in XAIF schema
     */
    static const std::string ourXAIFName;

    /** 
     * iterate through all Symbols 
     * to find some requiring forced passivation
     */
    void forcedPassivation();

    /** 
     * this tests Symbol instance identity
     * it does NOT test data equivalence
     */
    bool hasElement(const Symbol& aSymbol) const; 
    
    /** 
     * tries to find a matching symbol disregarding fron-end decorations
     */
    bool hasSymbolWithPlainName(const std::string& aPlainName) const; 

    const Symbol&  getSymbolWithPlainName(const std::string& aPlainName) const; 
    
  private: 
    
    typedef HashTable<Symbol*> HashTableSymbolP;

    HashTableSymbolP myHashTableSymbolP;

  }; // end of class SymbolTable

} // end of namespace 
                                                                     
#endif
