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
#include "UserSession.h"
#include "Clock.h"
#include <string>
#include <map>
#include <set>
#include <stdexcept>
#include "ReferenceCounted.h"

//class Game: private Thread {
class Game: public ReferenceCounted::object<Game> {
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

public:
  // Pure virtual methods

  virtual void make_move( NICSSessionManager::SessionID user,
			  const string &move ) = 0;
  virtual const string get_board( ReferenceCounted::pointer<UserSession> user,
				  Relationship rel,
				  int style = 0 ) = 0;
  virtual string description() const = 0;
  virtual string score_text() const = 0;
  virtual string result_text() const = 0;
  virtual void save() const = 0;
  virtual void resign( NICSSessionManager::SessionID player ) = 0;
  virtual void flag( NICSSessionManager::SessionID player ) = 0;
  virtual void start() = 0;
  virtual void end() = 0;

  // Normal methods
  void add_user(NICSSessionManager::SessionID user, Relationship rel);
  void delete_user(NICSSessionManager::SessionID user) {};
//    void delete_user(ReferenceCounted::pointer<UserSession> user);
  
  // A little help
  void refresh() const;
  string get_board( ReferenceCounted::pointer<UserSession> user, int style )
  { return get_board( user, relationship(user), style ); };
  Relationship relationship(ReferenceCounted::pointer<UserSession> user)
  { return Play; };

  // General purpose game control
  void start_game();
  void end_game();

  // Accessors
  unsigned int initial_time() const { return my.initial_time; };
  unsigned int increment() const { return my.increment; };

  //void Run();
  unsigned int flip_coin() const;

  NICSGameManager::GameID id() const { return my.id; };

  map<NICSSessionManager::SessionID, Clock> clocks;
  set<NICSSessionManager::SessionID> players;
//    set<ReferenceCounted::pointer<UserSession>> observers;
//    set<ReferenceCounted::pointer<UserSession>> examiners

  struct {
    unsigned int initial_time;
    unsigned int increment;
    NICSGameManager::GameID id;
  } my;
};

#endif // !_GAME_H
