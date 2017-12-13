// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################
#ifndef XlationCtxt_INCLUDED
#define XlationCtxt_INCLUDED


#include <iostream>

#include <inttypes.h>

// CtxtFlags: represents the current context
class CtxtFlags {
public: 
  CtxtFlags();
  virtual ~CtxtFlags();

  // -------------------------------------------------------
  // Flags for context
  // -------------------------------------------------------

  // Tests to see if *all* of the specified flags are set
  bool AreFlags(uint32_t f) const { return ((flags & f) == f); }
  
  // Tests to see if *any* of the specified flags are set
  bool IsAnyFlag(uint32_t f) const { return (flags & f); }

  void SetFlags(uint32_t f)       { flags = flags | f; }
  void ResetFlags(uint32_t f)     { flags = flags & ~f; }
  
  virtual void Dump(std::ostream& o = std::cerr) const;
  virtual void DDump() const;
  
private:
  uint32_t flags;
  
};

#endif /* XlationCtxt_INCLUDED */
