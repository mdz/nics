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

#ifndef _NICSGUILE_H
#define _NICSGUILE_H

#include "NICSModule.h"
#include <cc++/thread.h>
#include <string>
#include <guile/gh.h>
#include <stdexcept>

class NICSGuile:
  public NICSModule, public virtual ost::Thread {
public:
  NICSGuile(): NICSModule("Guile") {};

  class Error: public std::runtime_error {
  public: Error(const std::string& str): runtime_error(str) {};
  };

  void Run(void);
  bool eval_bool(const std::string& expr) throw(Error) {
    SCM val = eval(expr);
    if (gh_boolean_p(val))
      return gh_scm2bool(val);
    else
      throw Error("expression did not evaluate to a boolean value");
  };
    

  friend void nicsguile_run(int, char **);

protected:
  SCM eval(const std::string& expr);
private:
  ost::Mutex mutex;
  ost::Semaphore request_sem;
  ost::Semaphore result_sem;
  string request;
  SCM result;
};

#endif // !_NICSNETWORK_H
