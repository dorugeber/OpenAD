/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Id: ParenToken.cpp,v 1.1 2008/02/19 19:02:23 utke Exp $
 */

// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include <xercesc/util/regx/ParenToken.hpp>

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  ParenToken: Constructors and Destructors
// ---------------------------------------------------------------------------
ParenToken::ParenToken(const unsigned short tokType,
                       Token* const tok, const int noParen, MemoryManager* const manager)
    : Token(tokType, manager)
    , fNoParen(noParen)
    , fChild(tok)
{

}


ParenToken::~ParenToken() {

}

XERCES_CPP_NAMESPACE_END

/**
  *	End of file ParenToken.cpp
  */