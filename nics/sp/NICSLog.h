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

#ifndef _NICSLOG_H
#define _NICSLOG_H

#include "NICSModule.h"

#include <fstream>
#include <string>

class NICSLog: public NICSModule {
public:

  enum Facility { Game, Network, User, Database };
  enum Severity { Debug = 0, Info, Warning, Error, Critical };

  NICSLog( const string &filename, Severity loglevel );

  void log(Facility facility, Severity severity, const string &message);

private:
  Severity loglevel;
  ofstream logfile;
};

#endif // !_NICSLOG_H
