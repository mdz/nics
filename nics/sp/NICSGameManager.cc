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
#include "Chess.h"
#include "GameOffer.h"
#include "UserSession.h"
#include <algorithm>
#include <stdexcept>
#include <strstream>

NICSGameManager::OfferID NICSGameManager::next_offer_id() const
  throw(OfferLimitReached) {

  for( OfferID i = 0 ; i < 1024 ; ++i )
    if ( offers.find(i) == offers.end() )
      return i;

  throw OfferLimitReached();
}

NICSGameManager::OfferID
NICSGameManager::register_offer(GameOffer::pointer offer)
  throw(OfferLimitReached) {
  OfferID id;

  WriteLock();

  try {
    id = next_offer_id();
    map<OfferID, GameOffer::pointer>::value_type val(id, offer);
    offers.insert(val);
  } catch (OfferLimitReached e) {
    Unlock();
    throw e;
  }

  try {
    NICSCore::session_manager->send_offer(offer);

  } catch ( NICSSessionManager::UserNotFound e ) {
    try {
      // XXX
      NICSCore::session_manager->find_session(offer->session())
	->send_text("Note: Requested opponent is not logged in.");
    } catch ( NICSSessionManager::UserNotFound ) {
      // Offering user must have left already, ignore
    }
  }

  Unlock();

  NICSCore::log->log( NICSLog::Game, NICSLog::Debug,
		      "Offer registered" );
  return id;
}

void NICSGameManager::unregister_offer(OfferID id) {
  //WriteLock();

  map<OfferID, GameOffer::pointer>::iterator it = offers.find(id);
  if (it != offers.end()) {
    GameOffer *ptr = it->second.ptr();
    offers.erase( it );
    delete ptr;
    //Unlock();
    
    NICSCore::log->log( NICSLog::Game, NICSLog::Debug,
			"Offer unregistered" );
  } else {
    //Unlock();

    NICSCore::log->log( NICSLog::Game, NICSLog::Warning,
			"Offer not found in NICSGameManager::unregister_offer" );
  }
}

void NICSGameManager::dump_offer_info(ostream &out) {

  for( map<OfferID, GameOffer::pointer>::const_iterator it = offers.begin() ;
       it != offers.end() ; ++it )
    out << "Offer ID " << it->first << endl;

}

void NICSGameManager::show_offers( NICSSessionManager::SessionID session ) {
  ostrstream out;
  int found = 0;

  for( map<OfferID, GameOffer::pointer>::const_iterator it = offers.begin() ;
       it != offers.end() ; ++it )
    if ( (*it).second->match( session ) ) {
      NICSCore::session_manager->find_session(session)
	->send_offer( it->second );
      
      ++found;
    }

  if (found == 0)
    NICSCore::session_manager->find_session(session)
      ->send_text( "No matching offers" );
}

NICSGameManager::GameID NICSGameManager::next_game_id() const throw(GameLimitReached) {
  for( GameID i = 0 ; i < 1024 ; ++i )
    if ( games.find(i) == games.end() )
      return i;

  throw GameLimitReached();
}

NICSGameManager::GameID
NICSGameManager::register_game(Game::pointer game) throw(GameLimitReached) {
  GameID id;

  ReadLock();

  try {
    id = next_game_id();
    games.insert( map<GameID, Game::pointer>::value_type(id, game) );
  } catch (GameLimitReached e) {
    Unlock();
    throw e;
  }

  Unlock();

  NICSCore::log->log( NICSLog::Game, NICSLog::Info, "Game added" );
  return id;
}

void NICSGameManager::unregister_game(GameID game) {

  ReadLock();

  map<GameID, Game::pointer>::iterator it = games.find(game);
  
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

  for( map<GameID, Game::pointer>::const_iterator it = games.begin() ;
       it != games.end() ; ++it )
    out << "Game ID " << it->first << ": " << it->second->description()
	<< endl;
  Unlock();
}

unsigned int
NICSGameManager::find_matching_offers(NICSSessionManager::SessionID session,
				      vector<GameID> &result) {

  unsigned int found = 0;

  ReadLock();
  for( map<OfferID, GameOffer::pointer>::const_iterator it = offers.begin() ;
       it != offers.end() ; ++it )
    if ( it->second->session() == session )
      // An offer from us
      continue;
    else if ( it->second->match( session ) ) {
      result.insert(result.end(), it->first);
      ++found;
    }

  Unlock();
  return found;
}

void NICSGameManager::remove_offers( NICSSessionManager::SessionID session ) {
  WriteLock();

  for( map<OfferID, GameOffer::pointer>::iterator it = offers.begin() ;
       it != offers.end() ; ++it )
    if (it->second->session() == session)
      unregister_offer(it->first);

  Unlock();
}

//  void NICSGameManager::join( GameID game, NICSSessionManager::SessionID user,
//  			    GameRelationship rel ) throw(GameNotFound) {
//    ReadLock();
//    find(game).add_user( user, rel );
//    Unlock();
//  }

//  void NICSGameManager::leave( GameID game, NICSSessionManager::SessionID user,
//  			     GameRelationship rel ) throw(GameNotFound) {
//    ReadLock();
//    find(game).delete_user( user, rel );
//    Unlock();
//  }

NICSGameManager::GameID
NICSGameManager::accept( OfferID id, UserSession::pointer session )
  throw(OfferNotFound) {

  ReadLock();

  map<OfferID, GameOffer::pointer>::iterator it = offers.find(id);

  if (it == offers.end()) {
    Unlock();
    throw OfferNotFound("");
  }

  GameOffer::pointer offer = it->second;

  if ( session->id() == offer->session() ) {
    Unlock();
    throw OfferNotFound("User attempted to accept own offer");
  }

  Unlock();

  Game::pointer game = new Chess;

  GameID gid = game->id();

  game->add_user(offer->session(), Game::Play);
  NICSCore::session_manager->find_session(offer->session())
    ->join(gid);

  game->add_user(session->id(), Game::Play);
  session->join(gid);
  
  remove_offers( session->id() );
  remove_offers( offer->session() );

  game->start();

  return gid;
}

void NICSGameManager::show_games( UserSession::pointer session ) {
  ReadLock();
  
  for( map<GameID, Game::pointer>::const_iterator it = games.begin() ;
       it != games.end() ; ++it )
    session->send_game( it->second );

  Unlock();
}
