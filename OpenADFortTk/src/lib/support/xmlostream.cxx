// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################
#include "xmlostream.h"

/*
  A table showing possible state movements.  The state-state table
  indicates whether it is possible to move from one state (in the left
  margin) to another state.  Given an initial state (in the left
  margin), the state-action table indicates the possible resulting
  states after an action

        States                         Actions
       |--------------------------------------------------------------------
       |          .....ELEM_....     | Beg  Beg  End  End   End  Beg  End
       |INIT FINI OP_I OP_A OPEN ERR | Elm  Attr Attr Attrs Elem Com  Com
       |-------------------------------------------------------------------- 
  INIT |no   no   yes  no   no   yes | OP_I ERR  ERR  ERR   ERR  COM  ERR
  FINI |no   no   yes  no   no   yes | OP_I ERR  ERR  ERR   ERR  COM  ERR
  ERR  |no   no   no   no   no   no  | .    .    .    .     .    .    .  
  OP_I |no   yes  err  yes  yes  yes | OP_I OP_A OP_I OPEN  FINI COM  ERR
       |                             |                      OPEN
  OP_A |no   yes  yes  yes  yes  yes | OP_I OP_A OP_I OPEN  FINI COM  ERR
       |                             |                      OPEN
  OPEN |no   yes  yes  no   yes  yes | OP_I ERR  ERR  OPEN  FINI COM  ERR
       |                             |                      OPEN
  COM  |                             | ERR  ERR  ERR  ERR   ERR  ERR  <>

 */

// For a comments to public member functions, see the interface file.

xml::ostream::ostream(std::streambuf* sb)
  : std::ostream(sb)
{
  state = 0;
  
  SetState(INIT);
  indentAmnt = 0;
  indentStep = 2;
}

xml::ostream::~ostream()
{
}

void
xml::ostream::BegElem(const char* etag)
  throw (xml::ostream::Exception)
{
  // Sanity check 
  if (IsStateComment()) {
    SetStateError();
    throw Exception("BegElem: Within a comment!");
  }
  
  if (IsState(ELEM_OPENA) || IsState(ELEM_OPENI)) {
    EndAttrs(); // state is updated
  }
  Indent();
  (*this) << '<' << etag;
  IndentIncr();

  elemstack.push_front(std::string(etag));
  SetState(ELEM_OPENI);
}

void
xml::ostream::EndElem()
{
  // Sanity check
  if ( !(IsState(ELEM_OPENA) || IsState(ELEM_OPENI) || IsState(ELEM_OPEN)) ) {
    SetStateError();
    throw Exception("EndElem: No currently open elements to end!");
  } 
  if (IsStateComment()) {
    SetStateError();
    throw Exception("EndElem: Within a comment!");
  }
  
  IndentDecr();
  if (IsState(ELEM_OPENA)) {
    EndAttr(); // state is updated 
  }

  if (IsState(ELEM_OPENI)) {
    (*this) << "/>\n";
  } 
  else { // ELEM_OPEN
    std::string& etag = elemstack.front();
    Indent();
    (*this) << "</" << etag << ">\n";
  }
  
  // Determine the appropriate state after an element has been closed
  elemstack.pop_front();
  if (elemstack.size() == 0) { 
    SetState(FINI);
  } 
  else {
    SetState(ELEM_OPEN);
  }
}


void
xml::ostream::BegAttr(const char* attr)
{
  // Sanity check
  if ( !(IsState(ELEM_OPENA) || IsState(ELEM_OPENI)) ) {
    SetStateError();
    throw Exception("BegAttr: No currently open element start tag!");
  }
  if (IsStateComment()) {
    SetStateError();
    throw Exception("BegAttr: Within a comment!");
  }

  if (IsState(ELEM_OPENA)) {
    EndAttr();
  }
  
  (*this) << ' ' << attr << "=\"";
  SetState(ELEM_OPENA);
}

void
xml::ostream::EndAttr()
{
  // Sanity check
  if (!(IsState(ELEM_OPENA))) {
    SetStateError();
    throw Exception("EndAttr: No currently open attribute!");
  }
  if (IsStateComment()) {
    SetStateError();
    throw Exception("EndAttr: Within a comment!");
  }
  
  (*this) << '"';
  SetState(ELEM_OPENI);
}


void
xml::ostream::EndAttrs()
{
  // Sanity check
  if ( !(IsState(ELEM_OPENA) || IsState(ELEM_OPENI)) ) {
    SetStateError();
    throw Exception("EndAttrs: No currently open element start tag!");
  }
  if (IsStateComment()) {
    SetStateError();
    throw Exception("EndAttrs: Within a comment!");
  }
  
  if (IsState(ELEM_OPENA)) {
    EndAttr();
  }
  (*this) << ">\n"; 
  SetState(ELEM_OPEN);
}

//****************************************************************************

void
xml::ostream::BegComment()
{
  // Sanity check
  if (IsStateComment()) {
    SetStateError();
    throw Exception("BegComment: Already within a comment!");    
  }
  
  if (IsState(ELEM_OPENA) || IsState(ELEM_OPENI)) {
    EndAttrs(); // state is updated
  }
  Indent();
  (*this) << "<!-- ";

  SetStateComment();
}

void
xml::ostream::EndComment()
{
  // Sanity check
  if (!IsStateComment()) {
    SetStateError();
    throw Exception("EndComment: Not within a comment!");
  }
  
  (*this) << " -->\n";

  ResetStateComment();
}

void
xml::ostream::Comment(const char* str)
{
  // Sanity check -- rely on BegComment()
  
  BegComment();
  (*this) << str;
  EndComment();
}

//****************************************************************************

void
xml::ostream::Indent()
{
  for (int i = 0; i < indentAmnt; ++i) {
    (*this) << ' ';
  }
}

