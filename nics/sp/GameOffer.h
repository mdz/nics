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

#ifndef _GAMEOFFER_H
#define _GAMEOFFER_H

#include "NICSSessionManager.h"
#include "NICSGameManager.h"
#include "GameFilter.h"
#include "UserSessionFilter.h"
#include "ReferenceCounted.h"

class GameOffer: public ReferenceCounted::object<GameOffer> {
public:

  GameOffer( NICSSessionManager::SessionID session,
	     const GameFilter& game_filter,
	     const UserSessionFilter& user_session_filter );
  ~GameOffer();

  NICSSessionManager::SessionID session() const { return my_session; };

  const GameFilter &game_filter() const { return my_game_filter; };
  const UserSessionFilter &user_session_filter() const
  { return my_user_session_filter; };

  bool match( NICSSessionManager::SessionID session ) const;

private:
  // The user who is offering a game
  NICSSessionManager::SessionID my_session;

  // What sort of games she is looking for
  const GameFilter my_game_filter;

  // What sort of people she wants to play
  const UserSessionFilter my_user_session_filter;

  NICSGameManager::OfferID my_id;
};

#endif // !_GAMEOFFER_H
