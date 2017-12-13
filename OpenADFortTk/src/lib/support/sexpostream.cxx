#include <cstring>  // for strlen

//*************************** User Include Files ****************************

#include "sexpostream.h"

//*************************** Forward Declarations ***************************

//****************************************************************************

/*
  A table showing possible state movements.  The state-state table
  indicates whether it is possible to move from one state (in the left
  margin) to another state.  Given an initial state (in the left
  margin), the state-action table indicates the possible resulting
  states after an action

        States/Qalifiers          Actions
       |--------------------------------------------------------------------
       |          LIST      ATOM| Beg   Beg   End   End   Quot  Beg  End
       |INIT DEF  OPEN ERR  OPEN| Lst   Atom  Atom  Lst         Com  Com
       |-------------------------------------------------------------------- 
  INIT |no   yes  yes  yes  yes | LOPN  DEF   ERR   ERR   DEF  +COM -COM
  DEF  |no   yes  yes  yes  yes | LOPN  DEF   DEF   ERR   DEF  +COM -COM
  LOPN |no   yes  yes  yes  yes | LOPN  LOPN  LOPN  DEF   LOPN +COM -COM
       |                        |                   LOPN    
  ERR  |no   no   no   no   no  |+ERR  +ERR  +ERR  +ERR  +ERR  +ERR +ERR
  AOPN |no   yes  yes  yes  no  |+ERR  +ERR  -AOPN +ERR  +ERR  +ERR +ERR
  COM  |                        |+ERR  +ERR  +ERR  +ERR  +ERR  +ERR -COM


 */

// For a comments to public member functions, see the interface file.

sexp::ostream::ostream(std::streambuf* sb)
  : std::ostream(sb)
{
  curAtomFlags = IOFlags::NONE;
  state = 0;

  SetState(INIT);
  indentAmnt = 0;
  indentStep = 2;
}


sexp::ostream::~ostream()
{
}


// Atom: specialization for 'const char*'
template <>
void 
sexp::ostream::Atom(int xflags, const char* const & val)
{
  using namespace IOFlags;

  // Sanity check -- rely on BegAtom()
  BegAtom(xflags);
  const char* outstr = (val) ? val : "";
  if (IsFlag(xflags, A_DQUOTE)) {
    EscapeString(outstr);
  }
  else {
    (*this) << outstr;
  }
  EndAtom();
}


void
sexp::ostream::BegAtom(int xflags)
{
  using namespace IOFlags;
 
  // Sanity check
  if (!IsStateQClear()) {
    SetStateError();
    throw Exception("BegAtom: Cannot begin atom!");
  }

  // Get and check flags
  int flags = 0;
  if (slistStack.size() != 0) {
    int& f = slistStack.front();
    flags = f;
    SetFlag(f, L_NONEMPTY);
  }
  
  // (note that there can be two sets of atom flags)
  AddSpaceIfNecessary(flags);  
  if (IsFlag(xflags, A_SQUOTE) || IsFlag(flags, A_SQUOTE)) {
    (*this) << "'";
  }
  if (IsFlag(xflags, A_DQUOTE) || IsFlag(flags, A_DQUOTE)) {
    (*this) << '"';
  }
  if (IsFlag(xflags, A_OCT) || IsFlag(flags, A_OCT)) {
    (*this) << std::oct << "0";
  }
  if (IsFlag(xflags, A_HEX) || IsFlag(flags, A_HEX)) {
    (*this) << std::hex << "0x";
  }
  
  curAtomFlags = xflags;
  
  // Set state and qualifier
  if (IsState(INIT)) { SetState(DEFAULT); }
  SetStateQ(ATOM_OPEN); 
  SetAction(BEG_ATOM);
}


void
sexp::ostream::EndAtom()
{
  using namespace IOFlags;

  // Sanity check
  if (!IsStateQ(ATOM_OPEN)) {
    SetStateError();
    throw Exception("EndAtom: No currently open atom!");
  }
  
  // Get and check flags
  int flags = 0;
  if (slistStack.size() != 0) {
    flags = slistStack.front();
  }
  
  // (note that there can be two sets of atom flags)
  if (IsFlag(curAtomFlags, A_DQUOTE) || IsFlag(flags, A_DQUOTE)) {
    (*this) << '"';
  }
  if (IsFlag(curAtomFlags, A_OCT) || IsFlag(flags, A_OCT) ||
      IsFlag(curAtomFlags, A_HEX) || IsFlag(flags, A_HEX)) {
    (*this) << std::dec;
  }
  
  curAtomFlags = IOFlags::NONE;
  ResetStateQ(ATOM_OPEN); // State remains the same, except for qualifier
  SetAction(END_ATOM);
}


void
sexp::ostream::BegList(int xflags)
  throw (sexp::ostream::Exception)
{
  using namespace IOFlags;

  // Sanity check 
  if (!IsStateQClear()) {
    SetStateError();
    throw Exception("BegList: Cannot begin list!");
  }

  // Get and check flags
  int flags = 0;
  if (slistStack.size() != 0) {
    int& f = slistStack.front();
    flags = f;
    SetFlag(f, L_NONEMPTY);
  }
  
  // [FIXME: more checks]
  AddSpaceIfNecessary(flags);
  
  (*this) << '(';
  IndentIncr();
  
  slistStack.push_front(xflags);
  SetState(LIST_OPEN);
  SetAction(BEG_LIST);
}


void
sexp::ostream::EndList()
{
  using namespace IOFlags;
    
  // Sanity check
  if (!IsState(LIST_OPEN)) {
    SetStateError();
    throw Exception("EndList: Cannot end list!");
  } 

  int& flags = slistStack.front();
  
  IndentDecr();
  (*this) << ")";
  
  // Check flags...
  
  // Determine the appropriate state after an element has been closed
  slistStack.pop_front();
  if (slistStack.size() == 0) { 
    SetState(DEFAULT);
  } 
  else {
    SetState(LIST_OPEN);
  }
  SetAction(END_LIST);
}


void
sexp::ostream::Quote()
{
  // Sanity check 
  if (!IsStateQClear()) {
    SetStateError();
    throw Exception("Quote: Cannot quote!");
  }
  
  // Get and check flags
  int flags = 0;
  if (slistStack.size() != 0) {
    flags = slistStack.front();
  }
  
  AddSpaceIfNecessary(flags);

  (*this) << "'";

  // Set state and qualifier
  if (IsState(INIT)) { SetState(DEFAULT); }
  SetAction(QUOTE);
}


//****************************************************************************

void
sexp::ostream::BegComment()
{
  // Sanity check
  if (IsStateQ(COMMENT)) {
    SetStateError();
    throw Exception("BegComment: Already within a comment!");
  }
  
  (*this) << ";; ";

  SetStateQ(COMMENT);
  SetAction(BEG_COMMENT);
}


void
sexp::ostream::EndComment()
{
  // Sanity check
  if (!IsStateQ(COMMENT)) {
    SetStateError();
    throw Exception("EndComment: Not within a comment!");
  }
  
  SetAction(END_COMMENT); // N.B.: this should come before EndLine()
  EndLine();
  ResetStateQ(COMMENT);
}


void
sexp::ostream::Comment(const char* str)
{
  // Sanity check -- rely on BegComment()
  
  BegComment();
  (*this) << str;
  EndComment();
}


//****************************************************************************

void
sexp::ostream::EscapeString(const char* str)
{
  for (int i = 0; i < strlen(str); ++i) {
    char c = str[i];
    if (c == '\\' || c == '"') { // what about: \n
      (*this) << '\\';
    }
    (*this) << c;    
  }
}


//****************************************************************************

void
sexp::ostream::EndLine()
{
  (*this) << "\n";
  SetAction(END_LINE); // N.B.: this should come before Indent()
  Indent();
}


void
sexp::ostream::Indent()
{
  for (int i = 0; i < indentAmnt; ++i) {
    (*this) << ' ';
  }
  SetAction(INDENT);
}

//****************************************************************************

void
sexp::ostream::AddSpaceIfNecessary(int flags)
{
  using namespace IOFlags;
  
  // short-circuit if we just quoted, ended a line, or indented
  if (WasAction(QUOTE) || WasAction(END_LINE) || WasAction(INDENT)) {
    return;
  }
  
  if (IsFlag(flags, L_NONEMPTY)) {
    (*this) << ' ';
  }
}
