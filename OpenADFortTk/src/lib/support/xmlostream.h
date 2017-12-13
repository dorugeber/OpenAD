// ##########################################################
// # This file is part of OpenADFortTk.                     #
// # The full COPYRIGHT notice can be found in the top      #
// # level directory of the OpenADFortTk source tree.       #
// # For more information visit                             #
// # http://www.mcs.anl.gov/openad                          #
// ##########################################################

#ifndef xmlostream_INCLUDED_h
#define xmlostream_INCLUDED_h

#include <iostream>
#include <string>
#include <list> // STL

// FIXME: add xml escaping routines

// ---------------------------------------------------------
// xml::ostream:
// 
// FIXME: we do not require that this be a full xml document (no state
// for dtd, etc.) since one should be able to create subdocuments that
// can be appended to larger ones.
// ---------------------------------------------------------
namespace xml {

class ostream : public std::ostream {
 public:
  ostream(std::streambuf* sb);
  virtual ~ostream();

  // ---------------------------------------------------------

  class Exception {
  public:
    Exception (const char* msg_) { msg = msg_; }
    virtual ~Exception () { }

    virtual const std::string& GetMessage() const { return msg; }
    virtual void Report(std::ostream& os) const { 
      os << "xml::ostream::Exception: " << msg << std::endl; 
    }    
    virtual void Report() const { Report(std::cerr); }

  private: 
    std::string msg;
  };

  // ---------------------------------------------------------
  
  // BegElem: Output an incomplete start tag [<tag...] for the
  // element 'etag' automatically closing any previous incomplete
  // start tags.  The start tag is incomplete so that an arbitrary
  // number of attributes can be added.  Indentation is incremented,
  // if necessary.
  void BegElem(const char* etag)
    throw (Exception);

  // EndElem: Output the current element's end tag, using either the
  // 'compact' form [<.../>] or the 'normal' form [</tag>], whichever
  // is appropriate.  Indentation is decremented, if necessary.
  void EndElem();


  // Attr: Output the attribute 'attr' with the value 'val'
  // [attr="val"] for the element's start tag, leaving the stream
  // ready to accept another attribute (i.e. the element's start tag
  // is still incomplete).  (FIXME: check for invalid character?)
  template <class T>
    void Attr(const char* attr, const T& val);

  // BegAttr and EndAttr together are equivalent to Attr.  These
  // functions are useful, e.g., when the attribute value needs to be
  // concatenated together from several sub-values.
  
  // BegAttr: Prepare the stream for the attribute value,
  // automatically closing any previous incomplete attributes.
  void BegAttr(const char* attr);

  // EndAttr: End this attribute and prepare the stream for the end of
  // the attribute list or another attribute.
  void EndAttr();
  

  // EndAttrs: Manually complete the attribute list and end the
  // current element's start tag (ending any open attribute).  This is
  // useful, e.g., if comments or newlines need to be inserted.
  void EndAttrs();

  // ---------------------------------------------------------
  
  // BegComment/EndComment: Ouput an arbitrary comment, automatically
  // closing any previous incomplete start tag or attribute.  Comments
  // cannot be nested. No indentation. EndComment() ends the current
  // line.
  void BegComment();
  void EndComment();

  // Comment: Output an indented, one-line comment (including
  // newline).  A previous incomplete start tag is closed.
  void Comment(const char* str);

  // ---------------------------------------------------------

  // Indentation: Both IndentAmnt (whitespace between the left margin
  // and the text) and the IndentStep (amount the indentation changes
  // for the next level) can be set independently.  It is
  // possible to have a negative step, but a negative amount is
  // ignored.
  void Indent();
  void IndentIncr() { indentAmnt += indentStep; }
  void IndentDecr() { indentAmnt -= indentStep; }
  bool IsIndent() { return (indentStep == 0 && indentAmnt == 0); }

  void SetIndentAmnt(int amnt) { indentAmnt = amnt; }
  void SetIndentStep(int step) { indentStep = step; }
  int GetIndentAmnt() { return indentAmnt; }
  int GetIndentStep() { return indentStep; }
  
  // FIXME Add a setIndent(xml::ostream) to coordinate indentation settings

 protected:

 private:
  // Disable
  ostream(const ostream& x);
  ostream& operator=(const ostream& x) { return *this; }

  void EndElemUpdateState();

 private: 
  // Stack of elements.  (Use a list instead a stack so that we can
  // easily examine contents.  The top of the stack will be the
  // *front* of the list. (push_front, pop_front).
  typedef std::list<std::string>           ElemStack;
  typedef std::list<std::string>::iterator ElemStackIt;
  typedef std::list<std::string>::const_iterator ElemStackItC;

  enum State {
    // A list of mutually exclusive states 
    INIT       = 0x00000001, // initial state, nothing has happened
    FINI       = 0x00000002, // a complete, top-level element has been finished

    ELEM_OPENI = 0x00000004, // within element, incomplete start tag
                             //   no incomplete attrs
    ELEM_OPENA = 0x00000008, // within element, incomplete start tag
                             //   incomplete attr
    ELEM_OPEN  = 0x00000010, // within element, complete start tag

    STMASK     = 0xff000000, // reserves 8 bits for state qualifiers

    // A list of state qualifiers (can co-exist with some/all of the above)
    ERR        = 0x10000000, // error
    COMMENT    = 0x20000000  // within a comment
  };

  // Access to mutually exclusive states (do not use with state qualifiers!)
  bool IsState(State st) { return (st & state); }
  void SetState(State st) { state = ((state & STMASK) | st); }

  // Access to state qualifiers
  bool IsStateError() { return (state & ERR); }
  void SetStateError() { state = (state | ERR); }

  bool IsStateComment() { return (state & COMMENT); }
  void SetStateComment() { state = (state | COMMENT); }
  void ResetStateComment() { state = (state & ~COMMENT); }
  
 private:
  ElemStack elemstack;
  unsigned int state;
  
  int indentAmnt;
  int indentStep;
};

}; /* namespace xml */


//***************************************************************************
// Definitions for xml::ostream member template functions
//***************************************************************************

template<class T> 
void
xml::ostream::Attr(const char* attr, const T& val)
{
  // Sanity check -- rely on BegAttr()

  BegAttr(attr);
  (*this) << val;
  EndAttr();
}

//***************************************************************************
// xml::ostream operators that take no arguments
//***************************************************************************

namespace xml {

typedef ostream& (*omanip)(ostream&);

inline ostream& 
operator<<(std::ostream& os, omanip f)
{ 
  ostream& xos = dynamic_cast<ostream&>(os);
  return f(xos);
}


inline ostream& 
EndElem(ostream& xos)
{ 
  xos.EndElem();
  return xos;
}

inline ostream& 
EndAttr(ostream& xos)
{ 
  xos.EndAttr();
  return xos;
}

inline ostream& 
EndAttrs(ostream& xos)
{ 
  xos.EndAttrs();
  return xos;
}


inline ostream& 
BegComment(ostream& xos)
{ 
  xos.BegComment();
  return xos;
}

inline ostream& 
EndComment(ostream& xos)
{ 
  xos.EndComment();
  return xos;
}


inline ostream& 
Indent(ostream& xos)
{ 
  xos.Indent();
  return xos;
}

inline ostream& 
IndentIncr(ostream& xos)
{ 
  xos.IndentIncr();
  return xos;
}

inline ostream& 
IndentDecr(ostream& xos)
{ 
  xos.IndentDecr();
  return xos;
}

}; /* namespace xml */


//***************************************************************************
// xml::ostream operators that take arguments
//***************************************************************************

namespace xml {

// ---------------------------------------------------------
// BegElem
// ---------------------------------------------------------
struct BegElemInfo_ {
  const char* etag;
};

inline ostream&
operator<<(std::ostream& os, const BegElemInfo_ x) // ok to pass x directly
{
  ostream& xos = dynamic_cast<ostream&>(os); // FIXME
  xos.BegElem(x.etag);
  return xos;
}

inline BegElemInfo_ 
BegElem(const char* etag_)
{
  BegElemInfo_ x;
  x.etag = etag_;
  return x;
}

// ---------------------------------------------------------
// Attr
// ---------------------------------------------------------
template<class T> 
struct AttrInfo_ {
  const char* attr;
  const T* val;
};

template<class T> 
ostream& 
operator<<(std::ostream& os, const AttrInfo_<T>& x)
{
  ostream& xos = dynamic_cast<ostream&>(os);
  xos.Attr(x.attr, *x.val);
  return xos;
}

template<class T> 
AttrInfo_<T>
Attr(const char* attr_, const T& val_)
{
  AttrInfo_<T> x;
  x.attr = attr_;
  x.val = &val_;
  return x;
}

// ---------------------------------------------------------
// BegAttr
// ---------------------------------------------------------
struct BegAttrInfo_ {
  const char* attr;
};

inline ostream&
operator<<(std::ostream& os, const BegAttrInfo_ x) // ok to pass x directly
{
  ostream& xos = dynamic_cast<ostream&>(os); // FIXME
  xos.BegAttr(x.attr);
  return xos;
}

inline BegAttrInfo_ 
BegAttr(const char* attr_)
{
  BegAttrInfo_ x;
  x.attr = attr_;
  return x;
}

// ---------------------------------------------------------
// Comment
// ---------------------------------------------------------
struct CommentInfo_ {
  const char* str;
};

inline ostream&
operator<<(std::ostream& os, const CommentInfo_ x) // ok to pass x directly
{
  ostream& xos = dynamic_cast<ostream&>(os);
  xos.Comment(x.str);
  return xos;
}

inline CommentInfo_ 
Comment(const char* str_)
{
  CommentInfo_ x;
  x.str = str_;
  return x;
}

// ---------------------------------------------------------
// SetIndentAmnt
// ---------------------------------------------------------
struct IndentAmntInfo_ {
  int amnt;
};

inline ostream& 
operator<<(std::ostream& os, const IndentAmntInfo_ x) // ok to pass x directly
{
  ostream& xos = dynamic_cast<ostream&>(os);
  xos.SetIndentAmnt(x.amnt);
  return xos;
}

inline IndentAmntInfo_ 
SetIndentAmnt(int amnt_)
{
  IndentAmntInfo_ x;
  x.amnt = amnt_;
  return x;
}

// ---------------------------------------------------------
// SetIndentStep
// ---------------------------------------------------------
struct IndentStepInfo_ {
  int step;
};

inline ostream& 
operator<<(std::ostream& os, const IndentStepInfo_ x) // ok to pass x directly
{
  ostream& xos = dynamic_cast<ostream&>(os);
  xos.SetIndentStep(x.step);
  return xos;
}

inline IndentStepInfo_ 
SetIndentStep(int step_)
{
  IndentStepInfo_ x;
  x.step = step_;
  return x;
}

}; /* namespace xml */

// FIXME:
inline void Append_Token_String(xml::ostream& xos, const char *string)
{
  xos << string << " ";
}

#endif /* xmlostream_INLUCDED_h */
