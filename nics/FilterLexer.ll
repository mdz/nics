%{

#include <iostream>
#include "FilterLexer.h"
#include "Filter.h"
#include "FilterParserDefs.h"

%}

%option outfile="lex.yy.c"
%option c++
%option yyclass="FilterLexer"
%option never-interactive
%option prefix="Filter"

ws [ \t\n]
variable [a-zA-Z][A-Za-z0-9_]+
number [0-9]+
string \"[^\"]*\"
ne !=
le <=
ge >=
other .

%%

{ws} // skip whitespace

{variable} {
  string name(yytext, yyleng);
  lval = name;

  switch (filter.vartype(name)) {
  case Filter::variable::Integer: return(NUM_VAR);
  case Filter::variable::String: return(STR_VAR);
  case Filter::variable::Boolean: return(BOOL_VAR);
  default: throw Error("Undefined variable " + name);
  }
}

{number} {
  lval = string(yytext, yyleng);
  return(NUM);
}

{string} {
  lval = string(yytext + 1, yyleng - 2);
  return(STR);
}

{ne} {
  return(NOTEQUAL);
}

{ge} {
  return(GREATEREQUAL);
}

{le} {
  return(LESSEQUAL);
}

{other} return yytext[0];

%%

static int yywrap() {
  return 1;
}
