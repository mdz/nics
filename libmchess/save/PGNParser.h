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
#include "pgn_parse.h"

class PGNParser {
public:
  PGNParser(std::istream& input_):
    input(input_) {};

  const ChessGame& operator() const { return game; };

protected:
  std::istream& input;
  ChessGame game;
  PGNLexer lexer;

  friend pgn_yyparse(class PGNParser& parser);
};

#endif /* !PGNPARSER_H */
