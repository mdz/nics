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

#ifndef _USERSESSIONFICS_H
#define _USERSESSIONFICS_H

#include "UserSession.h"

class UserSessionFICS: public UserSession {
public:

  UserSessionFICS(TCPStream &input, TCPStream &output):
    UserSession(input, output) {};

  virtual void Run();
  virtual void send_refresh(NICSGameManager::GameID game);
  virtual void send_tell(const string &fromuser, const string &message);
  virtual void send_text( const string &text );
  virtual void send_text_long( const string &text );
  virtual void game_start_message( NICSGameManager::GameID game );
  virtual void game_end_message( NICSGameManager::GameID game );
  virtual void send_offer( const class GameOffer &offer );
  virtual void send_session( NICSSessionManager::SessionID sid );
  virtual void send_game( NICSGameManager::GameID gid);

private:
  void send_prompt();
  string prompt;
};

#endif // !_USERSESSIONFICS_H
