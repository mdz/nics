#ifndef _ALGEBRAICPARSER_H
#define _ALGEBRAICPARSER_H

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

#include <string>
#include "ChessPiece.h"
#include "ChessMoveAlgebraic.h"

int alg_yyparse(void*);

class AlgebraicParser {
public:
  class ParseError: public runtime_error {
  public: ParseError(const char* what): runtime_error(what) {};
  };

  AlgebraicParser(const string& str, const ChessPosition& pos)
    throw(ParseError);

  //const ChessMoveAlgebraic& operator() () const { return move; };

public:
  // These are filled in by the bison parser
  ChessPiece::Type piece;
  struct {
    int rank;
    int file;
  } origin_square;
  struct {
    int rank;
    int file;
  } dest_square;
  bool capture;
  bool check;
  bool checkmate;
  ChessPiece::Type promote;
  ChessPosition::Castling castling;

  // Get a token for the parser
  inline int lex() {
    if (current_char == str.end())
      return 0;
    return *(current_char++);
  };

protected:
  const string& str;
  string::const_iterator current_char;
  const ChessPosition& pos;
};

#endif /* !PGNPARSER_H */
