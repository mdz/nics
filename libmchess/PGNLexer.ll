%option outfile="lex.yy.c"
%option c++
%option yyclass="PGNLexer"
%option never-interactive
%option prefix="PGN"

%{

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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include "PGNLexer.h"

#include "pgn_parse.h" // yacc's stuff

// Since we could be reading multiple games, only read one character
// at a time so that no input gets swallowed.  Theoretically, we
// should be able to read a line at a time, but istream's interface
// doesn't seem to fit well enough.
#define YY_READ_BUF_SIZE 1
%}

ws [ \t\n]
integer [0-9]+
move ([KkqQrRnNbB]?[a-h1-8]?x?[a-h][1-8]=?[qQrRnNbB]?|O-O|O-O-O)[+#]?
tag [a-zA-Z0-9][A-Za-z0-9_+#=:-]*
comment \{[^\}]*\}
result 1-0|0-1|1\/2-1\/2|\*
quoted_string \"[^\"]*\"
other .

%%

{ws} // skip whitespace

{comment} // discard comments

{result} {
  lval.str = string(yytext, yyleng);
  return(RESULT);
}

{integer} {
	lval.num = atoi(yytext);
	return(NUM);
}

{quoted_string} {
  // Strip quotes
  lval.str = string(yytext + 1, yyleng - 2);
  return(STR);
}

{move} {
    lval.str = string(yytext, yyleng);
#ifdef DEBUG_pgn_lex
    cerr << "Read move: " << lval.str << endl;
#endif
   return(MOVE);
}

{tag} {
  lval.str = string(yytext, yyleng);
  return(SYMBOL);
}

{other} return yytext[0];

%%

static int yywrap() {
  return 1;
}
