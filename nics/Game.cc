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

#include "Game.h"
#include "NICSCore.h"
#include "NICSGameManager.h"
#include "NICSLog.h"
#include "UserSession.h"

#include <cstdlib>
#include <sstream>

using namespace ost;

Game::Game() {
  my.initial_time = 10;
  my.increment = 0;
  my.game_over = false;
  my.id = NICSCore::gm->register_game(this);
  my.status = Initial;
}

Game::~Game() {
  Terminate();
  NICSCore::gm->unregister_game(my.id);
}

void Game::start() {
  // call this first so that description() will work for the
  // announcement
  start_hook();

  my.status = InProgress;

  // Spread the word
  for( map<SessionID, Relationship>::iterator
	 it = users.begin() ;
       it != users.end() ; ++it ) {
    UserSession::rlock(it->first)
      .game_start_message(my.id);
    UserSession::unlock(it->first);
  }
  NICSCore::sm->game_notify(my.id);

  // Start checking for flag
  Start();
}

void Game::end() {
  // Tell our thread to stop checking the clocks and destroy this
  // object (will be delayed until the calling thread releases its
  // lock)
  my.game_over = true;

  for( map<SessionID, Relationship>::iterator
	 it = users.begin() ;
       it != users.end() ; ++it ) {
    try {
      UserSession &user = UserSession::rlock(it->first);
	user.game_end_message(my.id);
	user.leave(my.id);
	user.unlock();
      } catch (NICSSessionManager::UserNotFound) {
      }
    users.erase(it);
  }

  end_hook();

  while ( clocks.size() > 0 ) {
    clocks.begin()->second.stop();
    clocks.erase(clocks.begin());
  }
}

void Game::add_user(SessionID user, Relationship rel) {
  if (users.find(user) != users.end()) {
    // XXX - do something sensible
    return;
  }

  users[user] = rel;

  if ( rel == Play ) {
    clocks[user] = Clock(my.initial_time);
  } else if (my.status == InProgress) {
    UserSession::rlock(user)
      .send_refresh(my.id);
    UserSession::unlock(user);
  }
}

void Game::delete_user(SessionID user) {
  map<SessionID, Relationship>::iterator it =
    users.find(user);

  if (it == users.end())
    return;

  users.erase(it);
  
  unsigned int players = 0;
  for(it = users.begin() ; 
      it != users.end() ;
      ++players, ++it )
    ;

  // XXX
  if (players < 2)
    end();
}

void Game::Run() {
  setCancel(THREAD_CANCEL_IMMEDIATE);
  for(;;) {
    if (my.game_over) {
      //cerr << "Game over, destroying" << endl;
      return;
    } else {
      check_flags();
    }
    sleep(1);
  }
}

void Game::check_flags() {
  for( map<SessionID,Clock>::iterator it =
	 clocks.begin() ;
       it != clocks.end() ; ++it )
    
    if ( it->second.seconds() < 0 ) {
      flag( it->first );
      return;
    }
}

unsigned int Game::flip_coin() const {
  return (static_cast<int>(2.0*rand()/(RAND_MAX+1.0)));
}

void Game::refresh() const {

  for( map<SessionID, Relationship>::const_iterator
	 it = users.begin() ;
       it != users.end() ;
       ++it ) {
    UserSession::rlock(it->first)
      .send_refresh(my.id);
    UserSession::unlock(it->first);
  }
}

void Game::user_left(SessionID session) {
  ostringstream logmsg;
  logmsg << "user_left() called for game ID " << my.id;

  NICSCore::log->log( NICSLog::Game, NICSLog::Debug,
		      logmsg.str() );

  map<SessionID, Relationship>::const_iterator it = users.find(session);

  if (it == users.end())
    return;

  if (it->second == Play) {
    my.status = Aborted;
    end();
  }
}

void Game::kibitz(SessionID fromuser, const string& message) const {
  for( map<SessionID, Relationship>::const_iterator
	 it = users.begin() ;
       it != users.end() ;
       ++it ) {
    UserSession::rlock(it->first)
      .send_kibitz(fromuser, my.id, message);
    UserSession::unlock(it->first);
  }
}

void Game::whisper(SessionID fromuser, const string& message) const {
  for( map<SessionID, Relationship>::const_iterator
	 it = users.begin() ;
       it != users.end() ;
       ++it ) {
    if (it->second == Play)
      continue;
    UserSession::rlock(it->first)
      .send_whisper(fromuser, my.id, message);
    UserSession::unlock(it->first);
  }
}
