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
#include <string>

#include "ChessGame.h"
#include "ChessMoveAlgebraic.h"
#include "PGNParser.h"

#define YYERROR_VERBOSE
#ifdef DEBUG_pgn_parse
#define YYDEBUG 1
#endif

#define YYPARSE_PARAM data
#define YYLEX_PARAM data

#define YYSTYPE PGNLexer::yystype

int yylex(YYSTYPE *lvalp, void *data);
int yyerror(char *);

%}

%pure_parser

%token <num> NUM
%token <str> MOVE
%token <str> SYMBOL
%token <str> STR
%token <str> RESULT

%type <move> move

%%

   input: game  { YYACCEPT; }
	|       { YYACCEPT; }
	;

   game: tag_section movetext RESULT { }
	| tag_section { }
	;

   tag_section: tag_pair       { }
        | tag_section tag_pair { }
	;

   tag_pair: '[' SYMBOL STR ']'     {
	((PGNParser *)data)->game.pgn_tag($2, $3);
   }
	;

   movetext: NUM '.' move move { }
	| NUM '.' move { }
	| movetext NUM '.' move move { }
        | movetext NUM '.' move { }
	;

   move: MOVE				{
	((PGNParser *)data)->game.make_move(
		ChessMoveAlgebraic($1,
			((PGNParser *)data)->game.current_position()) );
#ifdef DEBUG_pgn_parse
	((PGNParser*)data)->game.current_position().write_FEN(cout);
#endif
   }
	;

%%

int yylex(YYSTYPE* lvalp, void *data) {
  return ((PGNParser *)data)->lexer.yylex_helper(lvalp);
}

int yyerror( char *s ) {
  throw PGNParser::ParseError(s);
}
