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

#include "NICSSessionManager.h"
#include "UserSession.h"
#include "NICSCore.h"
#include "NICSLog.h"
#include "GameOffer.h"
#include <algorithm>

SessionID NICSSessionManager::next_session_id() const
  throw(SessionLimitReached) {
  // Should only be called from register_session, does no locking
  for( SessionID i = 0 ; i < 1024 ; ++i )
    if ( sessions.find(i) == sessions.end() )
      return i;

  throw SessionLimitReached();
}

SessionID
NICSSessionManager::register_session(UserSession *session)
  throw(SessionLimitReached) {

  SessionID id;

  WriteLock();

  try {
    id = next_session_id();
    sessions[id] = session;
  } catch (SessionLimitReached e) {
    Unlock();
    throw e;
  }

  Unlock();
  return id;
}

void NICSSessionManager::unregister_session(SessionID session) throw() {
  WriteLock();
  map<SessionID, UserSession *>::iterator it = sessions.find(session);

  if (it == sessions.end()) {
    Unlock();
    // log session not found
  } else {
      sessions.erase( it );
      Unlock();
      NICSCore::log->log( NICSLog::User, NICSLog::Info,
			  "End session" );
  }
}

void NICSSessionManager::dump_session_info(ostream &out) throw() {
  ReadLock();
  for( map<SessionID, UserSession *>::iterator it = sessions.begin() ;
       it != sessions.end() ; ++it ) {

    out << "Session ID " << it->first << ": "
	<< it->second->user().name() << endl;

  }
  Unlock();
}

void NICSSessionManager::show_sessions(SessionID session)
  throw(UserNotFound) {

  ReadLock();
  for( map<SessionID, UserSession *>::iterator it = sessions.begin() ;
       it != sessions.end() ; ++it ) {
    rlock(session)
      .send_session(it->first);
    unlock(session);
  }
  Unlock();
}

//  void NICSSessionManager::tell(SessionID from, SessionID to,
//  			      const string &message)
//    throw(UserNotFound) {

//    EnterMutex();

//    map<SessionID, UserSession *>::iterator from_iter = sessions.find(from);
//    map<SessionID, UserSession *>::iterator to_iter = sessions.find(to);

//    if (from_iter == sessions.end()) {
//      // Telling user is already gone, just ignore
//      LeaveMutex();
//      return;
//    }

//    if (to_iter == sessions.end()) {
//      LeaveMutex();
//      throw UserNotFound("Invalid session ID");
//    }

//    UserSession *from_session = from_iter->second;
//    UserSession *to_session = to_iter->second;

//    to_session->send_tell(from_session->user().name(), message);
//    LeaveMutex();
//  }

unsigned int NICSSessionManager::send_offer( const GameOffer &offer )
  throw(UserNotFound) {
  ReadLock();

  unsigned int found = 0;

  if ( offer.user_session_filter().session_specific() ) {
    // Offer specific to one session

    map<SessionID, UserSession *>::iterator it =
      sessions.find(offer.user_session_filter().session());

    if ( it == sessions.end() ) {
      Unlock();
      throw UserNotFound("Opponent is not logged in");
    }

    rlock(it->first)
      .send_offer(offer);
    unlock(it->first);

    found = 1;

  } else {
    // Broadcast to all matching users

    for( map<SessionID, UserSession *>::iterator it = sessions.begin() ;
	 it != sessions.end() ; ++it )
      if ( offer.user_session_filter().match( it->first ) ) {
	rlock(it->first)
	  .send_offer(offer);
	unlock(it->first);

	++found;
      }

  }

  Unlock();
  return found;
}

SessionID
NICSSessionManager::find_session(const string &user)
  throw(UserNotFound) {
  ReadLock();
  for( map<SessionID, UserSession *>::iterator it = sessions.begin() ;
       it != sessions.end() ;
       ++it )
    if ( it->second->user().name() == user ) {
      Unlock();
      return it->first;
    }
  Unlock();
  throw UserNotFound("No user \"" + user + "\" is logged in");
}

void NICSSessionManager::game_notify(GameID game) {
  ReadLock();

  for( map<SessionID, UserSession *>::iterator it = sessions.begin() ;
       it != sessions.end() ;
       ++it )
    it->second->game_notify(game);
  
  Unlock();
}
