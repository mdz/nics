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

#include "GameOffer.h"
#include "NICSCore.h"
#include "NICSGameManager.h"
#include "UserSession.h"
#include "Chess.h"

GameOffer::GameOffer( NICSSessionManager::SessionID session,
		      const GameFilter &game_filter,
		      const UserSessionFilter &user_session_filter ):
  my_session( session ),
  my_game_filter( game_filter ),
  my_user_session_filter( user_session_filter ) {

  //  my_id = NICSCore::game_manager->register_offer( this );

}

GameOffer::~GameOffer() {
  //  NICSCore::game_manager->unregister_offer( my_id );
}

bool GameOffer::match( NICSSessionManager::SessionID session ) const {
  if ( session == my_session )
    return false;

  try {
    return UserSession::match(session, my_game_filter);
  } catch (NICSSessionManager::UserNotFound) {
    return false;
  }
}
