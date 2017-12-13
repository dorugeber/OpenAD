// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################
#ifndef BaseMap_H 
#define BaseMap_H

#include <map>

#include "Diagnostics.h"

namespace fortTkSupport {

// BaseMap: abstract map routines
// Note: FromTy and ToTy should be pointers (or unsigned integers)!
template <class FromTy, class ToTy>
class BaseMap : public std::map<FromTy, ToTy>
{
public:
  BaseMap() { }
  virtual ~BaseMap() { }
  
  // Find: Given x, find y if x --> y.
  virtual ToTy 
  Find(const FromTy x, bool mustFind = false) const
  {
    typename std::map<FromTy, ToTy>::const_iterator it = std::map<FromTy, ToTy>::find(x);
    ToTy y = (it == this->end()) ? 0 /*NULL*/ : (*it).second;
    
    if (mustFind && y == 0 /*NULL*/) {
      FORTTK_MSG(0,"BaseMap: Could not find entry for key '" << x << "' dumping map");
      Dump();
      FORTTK_DIE("BaseMap: Could not find entry for key '" << x << "'");
    }
    
    return y;
  }
  
  // Insert: insert the <x,y> pair in the map and return true; if
  // x is already a member, the operation fails and returns false.
  virtual bool 
  Insert(FromTy x, ToTy y) {
    pair<typename std::map<FromTy, ToTy>::iterator, bool> p = 
      this->insert(make_pair(x, y)); // std::map<FromTy, ToTy>::value_type
    return p.second;
  }
  
  // Dump: Dump that integer values in the map
  virtual void Dump(std::ostream& o = std::cerr) const {
    o << "{ Map (" << this << ")\n";
    for (typename std::map<FromTy, ToTy>::const_iterator it = this->begin(); 
	 it != this->end(); ++it) {
      o << "(" << it->first << " --> " << it->second << ")\n";
    }
    o << "}\n";
    o.flush();
  }
  
  virtual void DDump() const {
    Dump(std::cerr);
  }

protected:
};

} 

#endif 
