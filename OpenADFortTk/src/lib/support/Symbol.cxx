#include "Open64IRInterface/Open64BasicTypes.h"
#include "Open64IRInterface/stab_attr.h"
#include "Open64IRInterface/wn_attr.h"
#include "Diagnostics.h"
#include "Symbol.h"


namespace fortTkSupport { 

  Symbol::Symbol()
    : st(NULL), active(false)
  {
  }

  Symbol::Symbol(const ST* st_, WNId wnid_, bool act_)
  {
    SetST(st_);
    SetPathVorlage(wnid_);
    SetActive(act_);
  }

  Symbol::~Symbol()
  {
  }

  void 
  Symbol::Dump(std::ostream& o) const
  {
    o << "Symbol\n";
  }

  void 
  Symbol::DDump() const
  {
    Dump(std::cerr);
  }

}
