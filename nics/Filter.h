// -*- C++ -*-

// NICS - the Next Internet Chess Server
// Copyright (C) 2001  Matt Zimmerman <mdz@csh.rit.edu>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//

#ifndef _FILTER_H
#define _FILTER_H

#include <iostream>
#include <string>
#include <map>
#include "FilterParser.h"

class Filter {
public:
  class Error: public runtime_error {
  public: Error(const string& s): runtime_error(s) {};
  };

  class variable {
  public:
    enum Type { Invalid, Integer, String , Boolean };

    variable(): type(Invalid) {};
    variable(string stringval_): type(String), stringval(stringval_) {};
    variable(int intval_): type(Integer), intval(intval_) {};
    variable(bool boolval_): type(Boolean), boolval(boolval_) {};

    Type type;
    string stringval;
    int intval;
    bool boolval;
  };

  variable::Type vartype(const string& name) const {
    LegalVars::const_iterator it = legalvars.find(name);
    if (it != legalvars.end())
      return it->second;
    return variable::Invalid;
  };

protected:
  Filter() {};

  void parse(istream& input) throw(FilterParser::ParseError) {
    FilterParser parser;
    scheme_text = parser.parse(input, *this);
  };

  typedef map<string, variable> BoundVars;
  typedef map<string, variable::Type> LegalVars;
  LegalVars legalvars;

  string scheme_text;

  bool eval( const BoundVars& boundvars ) const throw(Error);
};

#endif // !_FILTER_H
