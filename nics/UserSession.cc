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

#include "UserSession.h"
#include "NICSCore.h"
#include "NICSLog.h"
#include "NICSGameManager.h"
#include "NICSSessionManager.h"
#include "NICSOfferManager.h"
#include "NICSDatabase.h"
#include "NetworkSession.h"

#include <algorithm>
#include <sstream>

UserSession::UserSession(ost::TCPStream& input_, ost::TCPStream& output_):
  input(input_), output(output_), end_session(false), have_focus(false) {

  my.id = NICSCore::sm->register_session(this);
}

UserSession::~UserSession() {
  NICSCore::om->remove_offers(my.id);
  NICSCore::gm->user_left(my.id);
  NICSCore::sm->unregister_session(my.id);
  NICSCore::log->log( NICSLog::User, NICSLog::Debug,
		      "UserSession destroyed" );
}

void UserSession::tell(const string &user, const string &message) const {
  UserSession &session = rlock(NICSCore::sm->find_session(user));
  session.send_tell(my.user.name(), message);
  session.unlock();
}

void UserSession::login(const string &username, const string &password)
  throw(LoginFailed) {
  User tmpuser;

//    try {

//      NICSCore::sm->kick(username);

//    } catch (NICSSessionManager::UserNotFound e) {
//    }

  try {
    NICSCore::database->get_user(username, tmpuser);
  } catch (NICSDatabase::UserNotFound) {
    // XXX - for now, just add the new user

    tmpuser.name(username);
    tmpuser.password(password);
    NICSCore::database->add_user(tmpuser);
    try {
      NICSCore::database->get_user(username, tmpuser);
    } catch (NICSDatabase::UserNotFound) {
      throw LoginFailed("Failed to add user");
    }
  }

  if (tmpuser.password() == password) {

    my.user = tmpuser;

    ostringstream logmsg;
    logmsg << username << " logged in, PID " << getpid();
    NICSCore::log->log( NICSLog::User, NICSLog::Info,
			logmsg.str() );

  } else {

    NICSCore::log->log( NICSLog::User, NICSLog::Info,
			"Failed login: " + username );
    throw LoginFailed("Incorrect password");
  }

}

void UserSession::game_notify( GameID game ) {
  // Here is where checks for following, gnotify and such will go
  send_game(game);
}
