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

#ifdef STDC_HEADERS
#include <string.h>
#endif

#include "ChessPiece.h"
#include "AlgebraicParser.h"

#define YYERROR_VERBOSE
#ifdef DEBUG_alg_parse
#define YYDEBUG 1
#endif

#define YYPARSE_PARAM parser
#define YYLEX_PARAM parser

#define MAX_MOVE_LEN 16

#define ALGPARSER (*(AlgebraicParser*)parser)

%}  

%pure_parser

%union {
  int num;
  ChessPiece::Type piece;
}

%type <num> rank file
%type <piece> piece

%{
  static int yylex(YYSTYPE *lvalp, void *move_str);
  static int yyerror(char *);
%}

%%

   input: move_check { YYACCEPT; }
	;

   move_check: move	{ }
	| move '+'	{ }
	| move '#'	{ }
	;

   move: piece 'x' dest_square	        { ALGPARSER.piece = $1;
					  ALGPARSER.capture = 1; }
	| piece dest_square		{ ALGPARSER.piece = $1; }

	| piece file maybe_capture dest_square { ALGPARSER.piece = $1;
					    ALGPARSER.origin_square.file =  $2;
					}

	| piece rank maybe_capture dest_square { ALGPARSER.piece = $1;
					    ALGPARSER.origin_square.rank = $2;
						}

	| dest_square maybe_promote		{ ALGPARSER.piece = ChessPiece::Pawn; }

        | file 'x' dest_square maybe_promote	{ ALGPARSER.piece = ChessPiece::Pawn;
					  ALGPARSER.origin_square.file = $1;
					  ALGPARSER.capture = 1; }

	| 'O' '-' 'O'			{ ALGPARSER.piece = ChessPiece::King;
					  ALGPARSER.castling =
						ChessPosition::Kingside; }

	| 'O' '-' 'O' '-' 'O'		{ ALGPARSER.piece = ChessPiece::King;
					  ALGPARSER.castling =
						ChessPosition::Queenside; }
					  
	;

   piece: 'N'	{ $$ = ChessPiece::Knight; }
        | 'B'	{ $$ = ChessPiece::Bishop; }
	| 'R'	{ $$ = ChessPiece::Rook; }
	| 'Q'	{ $$ = ChessPiece::Queen; }
	| 'K'	{ $$ = ChessPiece::King; }
	;

   maybe_capture: 'x' { ALGPARSER.capture = true; }
	| /* empty */ { ALGPARSER.capture = false; }

   dest_square: file rank	{ ALGPARSER.dest_square.file = $1;
				  ALGPARSER.dest_square.rank = $2; }

   maybe_promote: /* empty */ { ALGPARSER.promote = ChessPiece::Empty; }
	| '=' piece	      { ALGPARSER.promote = $2; };

   rank: '1'		{ $$ = 1; }
	| '2'		{ $$ = 2; }
	| '3'		{ $$ = 3; }
	| '4'		{ $$ = 4; }
	| '5'		{ $$ = 5; }
	| '6'		{ $$ = 6; }
	| '7'		{ $$ = 7; }
	| '8'		{ $$ = 8; }
	;

   file: 'a'		{ $$ = 1; }
	| 'b'		{ $$ = 2; }
	| 'c'		{ $$ = 3; }
	| 'd'		{ $$ = 4; }
	| 'e'		{ $$ = 5; }
	| 'f'		{ $$ = 6; }
	| 'g'		{ $$ = 7; }
	| 'h'		{ $$ = 8; }
	;
       
%%

int yylex(YYSTYPE* lval, void* parser) {
  return ((AlgebraicParser*)parser)->lex();
}

int yyerror( char *s ) {
  throw AlgebraicParser::ParseError(s);
}
