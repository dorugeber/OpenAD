/*! \file
  
  \brief Declarations for the Alias Tag class.

  \authors Michelle Strout

  Copyright (c) 2002-2005, Rice University <br>
  Copyright (c) 2004-2005, University of Chicago <br>
  Copyright (c) 2006, Contributors <br>
  All rights reserved. <br>
  See ../../../Copyright.txt for details. <br>
*/

#ifndef AliasTag_H
#define AliasTag_H

namespace OA {
  namespace AliasTag {

/*!
 * The AliasTag representation contains an integer representing a set of 
 * locations and a boolean indicating whether the locations being accessed
 * are being definitely/must accessed (TRUE) or possibly/may accessed (FALSE).
 */
class AliasTag {
public:
  AliasTag( int tag, bool must ) : mTag(tag), mMustFlag(must) { }
  ~AliasTag() { }

  int getTag() { return mTag; }
  int getMustFlag() { return mMustFlag; }

private:
  int mTag;
  bool mMustFlag;
};

  } // end namespace
} // end namespace
#endif
