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

Game::Game() {
  my.initial_time = 60;
  my.increment = 0;
  NICSCore::game_manager->register_game(this);
}

Game::~Game() {
  NICSCore::game_manager->unregister_game(this);
  cerr << "Game destroyed" << endl;
}

void Game::start_game() {
  // virtual, call this first so that description() will work for the
  // announcement
  start();

  // Spread the word
  for( set<NICSSessionManager::SessionID>::iterator it = players.begin() ;
       it != players.end() ; ++it )
    it->game_start_message(this);

  // Start checking for flag
  //Start();
}

static void *end_thread( Game *game ) {
  //game->save();

  delete game;
  return NULL;
}

void Game::end_game() {
  pthread_t thread;

  // Stop checking the clocks
  //Suspend();

  while (players.size() > 0) {
    set<NICSSessionManager::SessionID>::iterator it = players.begin();
    it->game_end_message(my.id);
    it->leave(my.id);
    players.erase(it);
  }

//    while ( observers.size() > 0 ) {
//      (*observers.begin())->game_end_message( this );
//      (*observers.begin())->delete_game( this, Observe );
//      observers.erase(observers.begin());
//    }

//    while ( examiners.size() > 0 ) {
//      (*examiners.begin())->game_end_message( this );
//      (*examiners.begin())->delete_game( this, Examine );
//      examiners.erase(examiners.begin());
//    }

  end(); // virtual

  while ( clocks.size() > 0 ) {
    map<NICSSessionManager::SessionID, Clock>::iterator it = clocks.begin();
    it->second.stop();
    clocks.erase(it);
  }

  // Spawn a new thread to save the game and destroy it (this thread
  // will release its lock)
  pthread_create( &thread, NULL, end_thread, this );
}

void Game::add_user(UserSession::pointer user, Relationship rel) {
  set<UserSession::pointer> *user_list = NULL;

  switch ( rel ) {
  case Play: user_list = &players; break;
//    case Observe: user_list = &observers; break;
//    case Examine: user_list = &examiners; break;
  default: throw invalid_argument("Game::add_user: invalid relationship");
  }

  user_list->insert( user );

  if ( rel == Play )
    clocks[user] = Clock(my.initial_time);
}

//  void Game::delete_user(const UserSession *user, Relationship rel) {
//    vector<UserSession *> *user_list = NULL;

//    switch ( rel ) {
//    case Play: user_list = &players; break;
//    case Observe: user_list = &observers; break;
//    case Examine: user_list = &examiners; break;
//    default: throw invalid_argument("Game::add_user: invalid relationship");
//    }

//    for( unsigned int i = 0 ; i < user_list->size() ; )
//      if ( (*user_list)[i] == user ) {

//        user_list->erase(&(*user_list)[i]);

//        if ( rel == Play )
//  	clocks.erase(&clocks[i]);

//      } else {
//        ++i;
//      }
  
//    if (players.size() < 2)
//      end();
//  }

//  void Game::delete_user(const UserSession *user) {
//    delete_user(user, Play);
//    delete_user(user, Observe);
//    delete_user(user, Examine);
//  }

//  void Game::Run() {
//    for(;;) {
//      for( map<NICSSessionManager::SessionID,Clock>::iterator it =
//  	   clocks.begin() ;
//  	 it != clocks.end() ; ++it )

//        if ( it->second.seconds() < 0 ) {
//  	flag( it->first );
//  	return;
//        }
    
//      Sleep(1000);
//    }
//  }


unsigned int Game::flip_coin() const {
  return (static_cast<int>(2.0*rand()/(RAND_MAX+1.0)));
}

void Game::refresh() const {

  for( set<UserSession::pointer>::const_iterator it = players.begin() ;
       it != players.end() ; ++it )
    it->send_refresh(my.id);

//    for( set<NICSSessionManager::SessionID>::const_iterator it = examiners.begin() ;
//         it != examiners.end() ; ++it )
//      NICSCore::session_manager->send_refresh(*it, *this);

//    for( set<NICSSessionManager::SessionID>::const_iterator it = observers.begin() ;
//         it != observers.end() ; ++it )
//      NICSCore::session_manager->send_refresh(*it, *this);
}
