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

#ifndef _USERSESSION_H
#define _USERSESSION_H

#include <cc++/socket.h>

#include <iostream>
#include <vector>
#include <stdexcept>

#include "NICSCore.h"
#include "NICSSessionManager.h"
#include "Game.h"
#include "User.h"
#include "GameFilter.h"

using namespace std;

class UserSession {
public:

  UserSession(ost::TCPStream &input_, ost::TCPStream &output_);
  virtual ~UserSession();

  const User &user() const { return my.user; };

  // Synchronization with NICSSessionManager
  static inline UserSession &rlock(SessionID id)
  { return NICSCore::sm->rlock(id); };

  static inline UserSession &wlock(SessionID id)
  { return NICSCore::sm->wlock(id); };

  static inline void unlock(SessionID id)
  { NICSCore::sm->unlock(id); };

  inline void unlock()
  { unlock(my.id); };

  inline SessionID id()
  { return my.id; };

  // Process commands from the user until they're done
  virtual void Run() = 0;

  // Update board
  virtual void send_refresh(GameID game) = 0;

  // Inbound messages
  virtual void send_tell(const string &fromuser, const string &message) = 0;
  virtual void send_text( const string &text ) = 0;
  virtual void send_text_long( const string &text ) = 0;
  virtual void game_start_message( GameID game ) = 0;
  virtual void game_end_message( GameID game ) = 0;
  virtual void send_offer( const class GameOffer &offer ) = 0;
  virtual void send_session( SessionID session ) = 0;
  virtual void send_game( GameID game ) = 0;
  virtual void send_kibitz( SessionID fromuser,
			    GameID game,
			    const string& message ) = 0;
  virtual void send_whisper( SessionID fromuser,
			     GameID game,
			     const string& message ) = 0;
  void game_notify( GameID game );

  // Delayed termination
  void end() { end_session = true; };

  // XXX, should support multiple games eventually
  void join(GameID game)
  { game_focus = game; have_focus = true; };
  void leave(GameID game)
  { have_focus = false; };

  bool match(const GameFilter &filter)
  { return true; };

protected:
  class LoginFailed: public runtime_error {
  public: LoginFailed( const string &arg_what ): runtime_error( arg_what ) {};
  };

  // Authenticate and establish our identity
  void login(const string &user, const string &password) throw(LoginFailed);

  // Outbound tell
  void tell(const string &user, const string &message) const;

  ost::TCPStream &input;
  ost::TCPStream &output;
  bool end_session;

  // So that we can name our accessors sensibly without conflicts
  struct {
    User user;
    GameFilter game_filter;
    SessionID id;
  } my;

  // The game currently being operated on (for move commands, etc.)
  bool have_focus;
  GameID game_focus;
};

#endif // !_USERSESSION_H
