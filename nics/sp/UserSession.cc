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
#include "NICSDatabase.h"
#include "NetworkSession.h"

#include <algorithm>
#include <strstream>

UserSession::UserSession(TCPStream &input_, TCPStream &output_):
  ReferenceCounted::object<UserSession>(*this),
  input(input_), output(output_), end_session(false), gaming(false) {

  NICSCore::session_manager->register_session(this);
}

UserSession::~UserSession() {

  NICSCore::game_manager->remove_offers(my.id);
  NICSCore::session_manager->unregister_session(my.id);
}

void UserSession::tell(const string &user, const string &message) const {
  NICSCore::session_manager->find_session(user)->send_tell(my.user.name(),
							   message);
}

void UserSession::login(const string &username, const string &password)
  throw(LoginFailed) {

  User tmpuser;

//    try {

//      NICSCore::session_manager->kick(username);

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

    ostrstream pid;
    pid << getpid() << ends;
    NICSCore::log->log( NICSLog::User, NICSLog::Info,
			username + " logged in " + pid.str() );

  } else {

    NICSCore::log->log( NICSLog::User, NICSLog::Info,
			"Failed login: " + username );
    throw LoginFailed("Incorrect password");
  }

}
