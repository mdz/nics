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

#ifndef _POSTGRESQL_H
#define _POSTGRESQL_H

#include <string>
#include "NICSDatabase.h"
#include <libpq++.h>

class PostgreSQL: public NICSDatabase {
 public:
  PostgreSQL( const char *connect );
  void get_user( const string &name, User &user ) throw(UserNotFound);
  void get_user( int uid, User &user ) throw(UserNotFound);
  void add_user( const User &user );
  void save_game( const class Chess &game );

  void dump_history( const User &user, ostream &out, unsigned int howmany );
  string last_opponent( const User &user );

private:
  int next_game();

 private:
  PgDatabase database;
};

#endif // !_POSTGRESQL_H
