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

#ifndef _NICSNETWORK_H
#define _NICSNETWORK_H

#include <vector>
#include <cc++/config.h>
#include <cc++/thread.h>
#include <cc++/socket.h>
#include "NICSModule.h"
#include "NetworkListener.h"

class NICSNetwork:
  public NICSModule {
public:
  NICSNetwork();
  ~NICSNetwork();

private:
  vector<NetworkListener<class UserSessionFICS> * > sessions_FICS;
};

#endif // !_NICSNETWORK_H
