#ifndef _FILTERPARSER_H
#define _FILTERPARSER_H

#include <iostream>
#include <stdexcept>
#include "FilterLexer.h"

int filter_yyparse(void*);

class FilterParser {
public:

  class ParseError: public runtime_error {
  public: ParseError(const char* what): runtime_error(what) {};
  };

  std::string parse(std::istream& input,
		    const class Filter& filter) throw(ParseError) {
    FilterLexer lexer_(input, filter);
    lexer = &lexer_;
    scheme = string();

    filter_yyparse(this);
    return scheme;
  };

protected:
  FilterLexer* lexer;
  std::string scheme;

  struct yystype {
    int i;
    bool b;
    std::string str;
  };

  friend int filter_yyparse(void*);
  friend int filter_yylex(std::string*,void*);
};

#endif /* !FILTERPARSER_H */
