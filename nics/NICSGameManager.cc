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

#include "NICSCore.h"
#include "NICSGameManager.h"
#include "NICSSessionManager.h"
#include "NICSLog.h"
#include "Game.h"
#include "UserSession.h"
#include <algorithm>
#include <stdexcept>
#include <sstream>

GameID NICSGameManager::next_game_id() const throw(GameLimitReached) {
  for( GameID i = 0 ; i < 1024 ; ++i )
    if ( games.find(i) == games.end() )
      return i;

  throw GameLimitReached();
}

GameID
NICSGameManager::register_game(Game *game) throw(GameLimitReached) {
  GameID id;

  WriteLock();

  try {
    id = next_game_id();
    games[id] = game;
  } catch (GameLimitReached e) {
    Unlock();
    throw e;
  }

  Unlock();

  NICSCore::log->log( NICSLog::Game, NICSLog::Info, "Game registered" );
  return id;
}

void NICSGameManager::unregister_game(GameID game) {

  WriteLock();

  map<GameID, Game *>::iterator it = games.find(game);
  
  if (it != games.end()) {
    games.erase( it );
    Unlock();
    NICSCore::log->log( NICSLog::Game, NICSLog::Debug,
			"Game unregistered" );
  } else {
      Unlock();
      NICSCore::log->log( NICSLog::Game, NICSLog::Warning,
			  "Game not found in NICSGameManager::unregister_game" );
  }
}

void NICSGameManager::dump_game_info(ostream &out) {
  ReadLock();

  for( map<GameID, Game *>::const_iterator it = games.begin() ;
       it != games.end() ; ++it ) {

    out << "Game ID " << it->first << ": " << rlock(it->first).description()
	<< endl;

    unlock(it->first);
  }

  Unlock();
}

void NICSGameManager::show_games( SessionID session ) {
  ReadLock();
  
  for( map<GameID, Game *>::const_iterator it = games.begin() ;
       it != games.end() ; ++it ) {
    UserSession::rlock(session)
      .send_game( it->first );
    UserSession::unlock(session);
  }

  Unlock();
}

void NICSGameManager::user_left( SessionID session ) {
  ReadLock();

  for( map<GameID, Game *>::iterator it = games.begin() ;
       it != games.end() ; ++it ) {

    it->second->user_left( session );
  }

  Unlock();
}
