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

#ifndef _SQLDATABASE_H
#define _SQLDATABASE_H

#include "NICSDatabase.h"

class SQLDatabase: public NICSDatabase {
public:

  virtual void get_user( const string &name, User &user )
    throw(UserNotFound) = 0;
  virtual void get_user( int uid, User &user )
    throw(UserNotFound) = 0;

  virtual void add_user( const User &user ) throw() = 0;
  virtual void save_game( const class Chess &game ) throw() = 0;
  virtual void dump_history( const string &name, ostream &out )
    throw(UserNotFound) = 0;
  virtual void dump_history( const User &user, ostream &out ) throw() = 0;

  virtual int played_white_against( const User &user1, const User &user2,
};

#endif // !_SQLDATABASE_H
