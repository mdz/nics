#ifndef _PGNLEXER_H
#define _PGNLEXER_H

//  libmchess - a library for abstraction of operations on a chessboard    
//  Copyright (C) 2000, 2001  Matt Zimmerman

//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.

//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA

#include <iostream>
#include "ChessMove.h"

#ifndef yyFlexLexer
#define yyFlexLexer PGNFlexLexer
#include <FlexLexer.h>
#undef yyFlexLexer
#endif

class PGNLexer: public PGNFlexLexer {
public:
  PGNLexer(istream& arg_yyin): PGNFlexLexer(&arg_yyin) {};

  typedef struct {
    int num;
    ChessMove move;
    string str;
  } yystype;

  // flex's code
  int yylex();

  inline int yylex_helper(yystype* lvalp) {
    int result=yylex();
    *lvalp = lval;
    return result;
  };

protected:
  yystype lval;
  string str;
};

#endif /* !PGNLEXER_H */
