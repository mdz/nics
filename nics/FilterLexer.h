// -*- c++ -*-

#ifndef _FILTERLEXER_H
#define _FILTERLEXER_H

//  NICS - The Next Internet Chess Server
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
#include <string>
#include <set>
#include <stdexcept>

#ifndef yyFlexLexer
#define yyFlexLexer FilterFlexLexer
#include <FlexLexer.h>
#endif

class FilterLexer: public FilterFlexLexer {
public:
  FilterLexer(istream& arg_yyin, const class Filter& filter_):
    FilterFlexLexer(&arg_yyin), filter(filter_) {};

  class Error: public std::runtime_error {
  public: Error(const string& what): runtime_error(what) {};
  };

  // flex's code
  int yylex();

  inline int yylex_helper(string* lvalp) {
    int result=yylex();
    *lvalp = lval;
    return result;
  };

protected:
  const class Filter& filter;
  string lval;
};

#endif /* !FILTERLEXER_H */
