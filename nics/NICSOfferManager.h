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

#ifndef _NICSOFFERMANAGER_H
#define _NICSOFFERMANAGER_H

#include <map>
#include <set>
#include <cc++/thread.h>
#include <stdexcept>

#include "NICSModule.h"
#include "typedefs.h"

using namespace std;

class NICSOfferManager: public NICSModule, protected ost::ThreadLock {
public:
  NICSOfferManager(): NICSModule("Offer Manager") {};

  class OfferNotFound: public runtime_error {
  public: OfferNotFound( const string &arg_what ): runtime_error( arg_what ) {};
  };
  class OfferLimitReached: public runtime_error {
  public: OfferLimitReached(): runtime_error( "Offer limit reached" ) {};
  };

  typedef unsigned int OfferID;

  OfferID register_offer(class GameOffer *offer) throw(OfferLimitReached);
  void unregister_offer(OfferID offer);
  void dump_offer_info(ostream &out);
  void show_offers( SessionID session );

  unsigned int find_matching_offers(SessionID session,
				    set<OfferID> &result);
  void remove_offers(SessionID session);

  GameID accept(OfferID offer, SessionID session) throw(OfferNotFound);

  inline GameOffer& rlock(OfferID offer) throw(OfferNotFound)
  { ReadLock(); return find(offer); };
  inline GameOffer& wlock(OfferID offer) throw(OfferNotFound)
  { WriteLock(); return find(offer); };
  inline void unlock(OfferID offer) throw()
  { Unlock(); };

protected:
  inline GameOffer& find(OfferID offer) throw(OfferNotFound) {
    map<OfferID, GameOffer *>::iterator it = offers.find(offer);
    if (it == offers.end()) {
      Unlock();
      throw OfferNotFound("Offer not found");
    }
    return *(it->second);
  };

  void remove_offer(OfferID offer);

private:
  map<OfferID, GameOffer *> offers;

  OfferID next_offer_id() const throw(OfferLimitReached);
};

#endif // !_NICSGAMEMANAGER_H
