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

#ifndef _NICSDATABASE_H
#define _NICSDATABASE_H

#include "NICSModule.h"
#include "User.h"
#include "Game.h"
#include <stdexcept>

class NICSDatabase: public NICSModule, protected Mutex {
public:

  virtual ~NICSDatabase() {};

  class UserNotFound: public runtime_error {
  public:
    UserNotFound( const string &arg_what ): runtime_error (arg_what) {};
  };

  class GameNotFound: public runtime_error {
  public:
    GameNotFound( const string &arg_what ): runtime_error (arg_what) {};
  };

  // User data
  virtual void get_user( const string &name, User &user )
    throw(UserNotFound) = 0;
  virtual void get_user( int uid, User &user )
    throw(UserNotFound) = 0;
  virtual void add_user( const User &user ) throw() = 0;

  // Game data
  virtual void save_game( const class Chess &game ) throw() = 0;
  virtual void dump_history( const User &user, ostream &out,
			     unsigned int howmany = 10 ) throw() = 0;
  //virtual Game *load_game( int game_id ) throw(GameNotFound) = 0;
  virtual string last_opponent( const User &user ) throw() = 0;

  // Wrappers for virtual functions
  void dump_history( const string &name, ostream &out )
    throw(UserNotFound);
  //int find_game( ...various criteria... )

protected:
  inline void lock() throw() { EnterMutex(); };
  inline void unlock() throw() { LeaveMutex(); };
};

#endif // !_NICSDATABASE_H
