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
#include "NICSGameManager.h"
#include "User.h"
#include "GameFilter.h"
#include "GameOffer.h"
#include "ReferenceCounted.h"

class UserSession: public ReferenceCounted::object<UserSession> {
public:

  UserSession(TCPStream &input_, TCPStream &output_);
  virtual ~UserSession();

  const User &user() const { return my.user; };

  // Process commands from the user until they're done
  virtual void Run() = 0;

  // Update board
  //virtual void send_refresh(NICSGameManager::GameID game) = 0;

  // Inbound messages
  virtual void send_tell( const string &fromuser, const string &message ) = 0;
  virtual void send_text( const string &text ) = 0;
  virtual void send_text_long( const string &text ) = 0;
  virtual void game_start_message( ReferenceCounted::pointer<Game> game ) = 0;
  virtual void game_end_message( ReferenceCounted::pointer<Game> game ) = 0;
  virtual void send_offer( ReferenceCounted::pointer<GameOffer> offer ) = 0;
  virtual void send_session( pointer session ) = 0;
  virtual void send_game( ReferenceCounted::pointer<Game> game ) = 0;

  void end() { end_session = true; };
  const string &name() { return my.user.name(); };

  class LoginFailed: public runtime_error {
  public: LoginFailed( const string &arg_what ): runtime_error( arg_what ) {};
  };

  // Authenticate and establish our identity
  void login(const string &user, const string &password) throw(LoginFailed);

  // Outbound tell
  void tell(const string &user, const string &message) const;

  void join(NICSGameManager::GameID game)
  { current_game = game; gaming = true; };
  void leave(NICSGameManager::GameID game)
  { gaming = false; };

  NICSSessionManager::SessionID id() const { return my.id; };

protected:
  TCPStream &input;
  TCPStream &output;
  bool end_session;

  // So that we can name our accessors sensibly without conflicts
  struct {
    User user;
    GameFilter game_filter;
    NICSSessionManager::SessionID id;
  } my;

  // The game currently being operated on (for move commands, etc.)
  bool gaming;
  NICSGameManager::GameID current_game;
};

#endif // !_USERSESSION_H
