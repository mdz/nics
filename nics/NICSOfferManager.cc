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
#include "NICSOfferManager.h"
#include "NICSSessionManager.h"
#include "GameOffer.h"
#include "UserSession.h"
#include "NICSLog.h"
#include "Chess.h"

NICSOfferManager::OfferID NICSOfferManager::next_offer_id() const
  throw(OfferLimitReached) {

  for( OfferID i = 0 ; i < 1024 ; ++i )
    if ( offers.find(i) == offers.end() )
      return i;

  throw OfferLimitReached();
}

NICSOfferManager::OfferID NICSOfferManager::register_offer(GameOffer *offer)
  throw(OfferLimitReached) {
  OfferID id;

  WriteLock();

  try {
    id = next_offer_id();
    offers[id] = offer;
  } catch (OfferLimitReached e) {
    Unlock();
    throw e;
  }

  Unlock();

  try {
    NICSCore::sm->send_offer(rlock(id));
    unlock(id);

  } catch ( NICSSessionManager::UserNotFound e ) {
    try {

      UserSession::rlock(offer->session())
	.send_text("Note: Requested opponent is not logged in.");
      UserSession::unlock(offer->session());

    } catch ( NICSSessionManager::UserNotFound ) {
      // Offering user must have left already, ignore
    }
  } catch ( OfferNotFound ) {
  }

  NICSCore::log->log( NICSLog::Game, NICSLog::Debug,
		      "Offer registered" );
  return id;
}

void NICSOfferManager::unregister_offer(OfferID id) {
  WriteLock();
  remove_offer(id);
  Unlock();
}

void NICSOfferManager::dump_offer_info(ostream &out) {
  ReadLock();
  for( map<OfferID, GameOffer *>::const_iterator it = offers.begin() ;
       it != offers.end() ; ++it )
    out << "Offer ID " << it->first << endl;
  Unlock();
}

void NICSOfferManager::show_offers( SessionID session ) {
  int found = 0;

  ReadLock();

  try {
    for( map<OfferID, GameOffer *>::const_iterator it = offers.begin() ;
	 it != offers.end() ; ++it )
      if ( it->second->match( session ) ) {
	UserSession::rlock(session).send_offer( *it->second );
	UserSession::unlock(session);
	
	++found;
      }
    
    if (found == 0) {
      UserSession::rlock(session)
	.send_text( "No matching offers" );
      UserSession::unlock(session);
    }
    
  } catch (NICSSessionManager::UserNotFound e) {
    // They went away, forget it
  }

  Unlock();
}

GameID
NICSOfferManager::accept( OfferID id, SessionID session )
  throw(OfferNotFound) {

  ReadLock();

  map<OfferID, GameOffer *>::iterator it = offers.find(id);

  if (it == offers.end()) {
    Unlock();
    throw OfferNotFound("");
  }

  GameOffer *offer = it->second;

  if ( session == offer->session() ) {
    Unlock();
    throw OfferNotFound("User attempted to accept own offer");
  }

  Unlock();

  GameID gid = (new Chess)->id();
  Game &game = Game::rlock(gid);
  UserSession &offering_user = UserSession::rlock(offer->session());
  UserSession &accepting_user = UserSession::rlock(session);

  game.add_user(offering_user.id(), Game::Play);
  offering_user.join(gid);

  game.add_user(accepting_user.id(), Game::Play);
  accepting_user.join(gid);
  
  remove_offers( session );
  remove_offers( offer->session() );

  game.start();

  game.unlock();
  offering_user.unlock();
  accepting_user.unlock();

  return gid;
}

unsigned int
NICSOfferManager::find_matching_offers(SessionID session,
				       set<OfferID> &result) {

  unsigned int found = 0;

  ReadLock();
  for( map<OfferID, GameOffer *>::const_iterator it = offers.begin() ;
       it != offers.end() ; ++it )
    if ( it->second->session() == session )
      // An offer from us
      continue;
    else if ( it->second->match( session ) ) {
      result.insert(it->first);
      ++found;
    }

  Unlock();
  return found;
}

void NICSOfferManager::remove_offers( SessionID session ) {
  WriteLock();

  for( map<OfferID, GameOffer *>::iterator it = offers.begin() ;
       it != offers.end() ; ++it )
    if (it->second->session() == session)
      remove_offer(it->first);

  Unlock();
}

void NICSOfferManager::remove_offer(OfferID offer) {
  map<OfferID, GameOffer *>::iterator it = offers.find(offer);
  if (it != offers.end()) {
    NICSCore::log->log( NICSLog::Game, NICSLog::Debug,
			"Offer unregistered" );
    delete it->second;
    offers.erase( it );
  } else {
    NICSCore::log->log( NICSLog::Game, NICSLog::Warning,
			"Offer not found in remove_offer" );
  }
}
