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

#ifndef _NICSSESSIONMANAGER_H
#define _NICSSESSIONMANAGER_H

#include "NICSModule.h"
#include "typedefs.h"
#include <stdexcept>
#include <map>
#include <cc++/thread.h>

using namespace std;

class UserSession;
class GameOffer;
class GameFilter;

class NICSSessionManager: public NICSModule, protected ost::ThreadLock {
 public:

  NICSSessionManager(): NICSModule("Session Manager") {};

  //typedef unsigned int SessionID;

  class UserNotFound: public runtime_error {
  public: UserNotFound( const string &arg_what ): runtime_error( arg_what ) {};
  };
  class SessionLimitReached: public runtime_error {
  public: SessionLimitReached(): runtime_error("Session limit reached") {};
  };

  SessionID register_session(UserSession *session) throw(SessionLimitReached);
  void unregister_session(SessionID session) throw();
  void dump_session_info(ostream &out) throw();
  SessionID next_session_id() const throw(SessionLimitReached);
  void show_sessions(SessionID session) throw(UserNotFound);

  // Returns the number of sessions that received the offer
  unsigned int send_offer(const GameOffer &offer) throw(UserNotFound);
  void send_offer(SessionID session, const GameOffer &offer)
    throw(UserNotFound);
  void game_notify(GameID game);

  // Debugging
//    void WriteLock() { cerr << "WLocking session manager for pid " << getpid() << endl; ThreadLock::WriteLock(); };
//    void ReadLock() { cerr << "RLocking session manager for pid " << getpid() << endl; ThreadLock::ReadLock(); };
//    void Unlock() { cerr << "Unlocking session manager for pid " << getpid() << endl; ThreadLock::Unlock(); };

  inline UserSession &rlock(SessionID id) throw(UserNotFound)
  { ReadLock(); return find(id); };
  inline UserSession &wlock(SessionID id) throw(UserNotFound)
  { WriteLock(); return find(id); };
  inline void unlock(SessionID id) throw()
  { Unlock(); };

  SessionID find_session(const string &user) throw(UserNotFound);

protected:
  inline UserSession &find(SessionID id) {
    map<SessionID, UserSession *>::iterator it = sessions.find(id);
    if (it == sessions.end()) {
      Unlock();
      throw UserNotFound("User not found");
    }
    return *(it->second);
  }


 private:
  map<SessionID, UserSession *> sessions;
  map<const string, unsigned int> users;
};

#endif // !_NICSSESSIONMANAGER_H
