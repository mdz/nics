%pure_parser

%left '&'
%left '|'
%nonassoc '<' '>' '='
%nonassoc NOTEQUAL "!="
%nonassoc LESSEQUAL "<="
%nonassoc GREATEREQUAL ">="
%left '-' '+'
%left '*' '/'

%token BOOL_VAR
%token STR_VAR
%token NUM_VAR
%token STR
%token NUM

%{

#include <string>
#include "FilterParser.h"

#define YYSTYPE string

int yyerror(char*);
int yylex(YYSTYPE* lvalp, void* parser);

#define YYPARSE_PARAM parser
#define YYLEX_PARAM parser
#define YYERROR_VERBOSE

#define PARSER (*static_cast<FilterParser*>(parser))

%}

%%
input:	/* empty */		{ }
	| boolexp      		{ PARSER.scheme = $1; }
;

boolexp: '(' boolexp ')'      	{ $$ = $2; };
        | boolexp '&' boolexp   { $$ = "(and " + $1 + ' ' + $3 + ")"; }
	| boolexp '|' boolexp	{ $$ = "(or " + $1 + ' ' + $3 + ")"; }
	| numexp '=' numexp	{ $$ = "(eq? " + $1 + ' ' + $3 + ")"; }
	| numexp "!=" numexp	{ $$ = "(not (eq? " + $1 + ' ' + $3 + "))"; }
	| numexp '<' numexp	{ $$ = "(< " + $1 + ' ' + $3 + ")"; }
	| numexp '>' numexp	{ $$ = "(> " + $1 + ' ' + $3 + ")"; }
	| numexp "<=" numexp	{ $$ = "(<= " + $1 + ' ' + $3 + ")"; }
	| numexp ">=" numexp	{ $$ = "(>= " + $1 + ' ' + $3 + ")"; }
	| strexp '=' strexp	{ $$ = "(string=? " + $1 + ' ' + $3 + ")"; }
	| strexp "!=" strexp	{ $$ = "(not (string=? " + $1 + ' ' + $3 + "))"; }
	| BOOL_VAR		{ $$ = $1; }
;

strexp: STR			{ $$ = "\"" + $1 + "\""; }
	| STR_VAR		{ $$ = $1; }
;

numexp:	NUM			{ $$ = $1; }
	| '(' numexp ')'	{ $$ = $2; }
	| numexp '+' numexp	{ $$ = "(+ " + $1 + ' ' + $3 + ")"; }
	| numexp '-' numexp	{ $$ = "(- " + $1 + ' ' + $3 + ")"; }
	| numexp '*' numexp	{ $$ = "(* " + $1 + ' ' + $3 + ")"; }
	| numexp '/' numexp	{ $$ = "(/ " + $1 + ' ' + $3 + ")"; }
	| NUM_VAR		{ $$ = $1; }
;

%%

int yyerror(char* s) {
  throw FilterParser::ParseError(s);
}

int yylex(YYSTYPE* lvalp, void* parser) {
  return PARSER.lexer->yylex_helper(lvalp);
}
