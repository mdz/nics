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

#ifndef _GAME_H
#define _GAME_H

#include "NICSCore.h"
#include "NICSGameManager.h"
#include "Clock.h"
#include "typedefs.h"
#include <string>
#include <map>
#include <set>
#include <stdexcept>

using namespace std;

class Game: private virtual ost::Thread {
public:
  Game();
  virtual ~Game();

  // Exceptions
  class InvalidMove: public runtime_error {
  public: InvalidMove( const string &arg_what ): runtime_error( arg_what ) {};
  };
  class IllegalMove: public runtime_error {
  public: IllegalMove( const string &arg_what ): runtime_error( arg_what ) {};
  };


  enum Type { Chess };
  enum Relationship { Play, Observe, Examine };

  static inline Game &rlock(GameID id)
  { return NICSCore::gm->rlock(id); };

  static inline Game &wlock(GameID id)
  { return NICSCore::gm->wlock(id); };

  static inline void unlock(GameID id)
  { NICSCore::gm->unlock(id); };

  inline void unlock()
  { unlock(my.id); };

  // XXX - this should go away
  GameID id() const { return my.id; };

  //
  // Pure virtual methods
  //

  virtual void make_move( SessionID user, const string &move ) = 0;
  virtual const string get_board( SessionID user,
				  Relationship rel,
				  int style = 0 ) = 0;
  virtual string description() const = 0;
  virtual string score_text() const = 0;
  virtual string result_text() const = 0;
  virtual void save() const = 0;
  virtual void resign( SessionID player ) = 0;
  virtual void flag( SessionID player ) = 0;
  virtual void start_hook() = 0;
  virtual void end_hook() = 0;

  //
  // Normal methods
  //
  void add_user(SessionID user, Relationship rel);
  void delete_user(SessionID user);
  void start();
  void end();
  void kibitz(SessionID fromuser, const string& message) const;
  void whisper(SessionID fromuser, const string& message) const;

  // Notification when a user disconnects or logs out
  void user_left(SessionID session);

  //
  // Helpers
  //
  
  // Refresh all users
  void refresh() const;

  // Lookup user relationship and pass on
  string get_board( SessionID user, int style = 0)
  { return get_board( user, relationship(user), style ); };

  // Lookup user relationship
  Relationship relationship(SessionID user) {
    if (users.find(user) == users.end())
      // XXX - throw an exception
      return Observe;
    return users[user];
  };

  // Accessors
  unsigned int initial_time() const { return my.initial_time; };
  unsigned int increment() const { return my.increment; };

protected:
  virtual void Run();
  virtual void Final() { delete this; };
  void check_flags();
  unsigned int flip_coin() const;

  map<SessionID, Clock> clocks;

  map<SessionID, Relationship> users;
  enum Status { Initial, InProgress, Aborted };

  struct {
    unsigned int initial_time;
    unsigned int increment;
    GameID id;
    Status status;
    bool game_over;
  } my;
};

#endif // !_GAME_H
