#ifndef _PGNPARSER_H
#define _PGNPARSER_H

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
#include "ChessGame.h"
#include "PGNLexer.h"

int pgn_yyparse(void*);

class PGNParser {
public:
  class ParseError: public runtime_error {
  public: ParseError(const char* what): runtime_error(what) {};
  };

  PGNParser(std::istream& input) throw(ParseError):
    lexer(input) {
    pgn_yyparse(this);
  };

  const ChessGame& operator() () const { return game; };

protected:
  ChessGame game;
  PGNLexer lexer;

  friend int pgn_yyparse(void*);
  friend int pgn_yylex(PGNLexer::yystype* lvalp, void* data);
};

#endif /* !PGNPARSER_H */
