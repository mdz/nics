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

#ifndef _NICSGAMEMANAGER_H
#define _NICSGAMEMANAGER_H

#include <vector>
#include <cc++/thread.h>
#include <stdexcept>

#include "NICSModule.h"
#include "NICSSessionManager.h"
#include "ReferenceCounted.h"
class UserSession;

class NICSGameManager: public NICSModule, protected ThreadLock {
public:
  class OfferNotFound: public runtime_error {
  public: OfferNotFound( const string &arg_what ): runtime_error( arg_what ) {};
  };
  class OfferLimitReached: public runtime_error {
  public: OfferLimitReached(): runtime_error( "Offer limit reached" ) {};
  };
  class GameNotFound: public runtime_error {
  public: GameNotFound( const string &arg_what ): runtime_error( arg_what ) {};
  };
  class GameLimitReached: public runtime_error {
  public: GameLimitReached(): runtime_error( "Game limit reached" ) {};
  };

  typedef unsigned int GameID;
  typedef unsigned int OfferID;

  // Offer stuff
  OfferID register_offer(ReferenceCounted::pointer<GameOffer> offer)
    throw(OfferLimitReached);
  void unregister_offer(OfferID offer);
  void dump_offer_info(ostream& out);
  void show_offers( NICSSessionManager::SessionID session );

  unsigned int find_matching_offers( NICSSessionManager::SessionID session,
				     vector<GameID>& result);
  void remove_offers(NICSSessionManager::SessionID session);

  GameID accept(OfferID offer, ReferenceCounted::pointer<UserSession> session)
    throw(OfferNotFound);


  // Game stuff
  GameID register_game(ReferenceCounted::pointer<Game> game)
    throw(GameLimitReached);
  void unregister_game(GameID);

  void dump_game_info(ostream& out);
  void show_games( ReferenceCounted::pointer<UserSession> session );

  inline Game& rlock(GameID game) throw(GameNotFound)
  { ReadLock(); return find(game); };
  inline Game& wlock(GameID game) throw(GameNotFound)
  { WriteLock(); return find(game); };
  inline void unlock(GameID game)
  { Unlock(); };

protected:
  inline Game &find(GameID game) throw(GameNotFound) {
    map<GameID, ReferenceCounted::pointer<Game> >::iterator it = games.find(game);
    if (it == games.end()) {
      Unlock();
      throw GameNotFound("Game not found");
    }
    return *(it->second);
  }

private:
  map<OfferID, ReferenceCounted::pointer<GameOffer> > offers;
  map<GameID, ReferenceCounted::pointer<Game> > games;
  multimap<GameID, ReferenceCounted::pointer<UserSession> > players;
//    multimap<GameID, NICSSessionManager::SessionID> observers;
//    multimap<GameID, NICSSessionManager::SessionID> examiners;

  OfferID next_offer_id() const throw(OfferLimitReached);
  GameID next_game_id() const throw(GameLimitReached);
};

#endif // !_NICSGAMEMANAGER_H
