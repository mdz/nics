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

#ifndef _USERSESSIONFILTER_H
#define _USERSESSIONFILTER_H

#include "NICSSessionManager.h"
#include "Filter.h"

class UserSessionFilter: public Filter {
public:
  UserSessionFilter() {
    d.session_specific = false;
  };

  UserSessionFilter( NICSSessionManager::SessionID session ) {
    d.session_specific = true;
    d.session = session;
  };

  bool match(NICSSessionManager::SessionID session) const;

  NICSSessionManager::SessionID session() const { return d.session; };
  bool session_specific() const { return d.session_specific; };

  friend ostream &operator<<(ostream &o, const class UserSessionFilter &me );

private:
  struct {
    NICSSessionManager::SessionID session;
    bool session_specific;
  } d;
};

#endif // !_USERSESSIONFILTER_H
