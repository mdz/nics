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

#include "NetworkSession.h"
#include "UserSessionFICS.h"
//#include "UserSessionNICS.h"
#include "NICSCore.h"
#include "NICSLog.h"
#include <string>
#include <strstream>

NetworkSession::NetworkSession( TCPSocket &socket,
				SessionType session_type_ ):
  TCPSession( NULL, socket ),
  session_type(session_type_),
  user_session(NULL) {

  tpport_t port;
  ostrstream logmsg;

  InetHostAddress ia = getPeer( &port );
  logmsg << "Connection from " << ia.getHostname() << ':' << port;

  NICSCore::log->log( NICSLog::Network, NICSLog::Info,
		      string(logmsg.str(), logmsg.pcount()) );
}

NetworkSession::~NetworkSession() {
  if ( user_session != NULL )
    delete user_session;
}

void NetworkSession::Final() {
  delete this;
}

void NetworkSession::Run() {
  switch (session_type) {
  case FICS:
    user_session = new UserSessionFICS(*this, *this);
    break;
  case NICS:
    //user_session = new UserSessionNICS(*this, *this);
    break;
  }

  try {
    user_session->Run();
  } catch (runtime_error e) {
    NICSCore::log->log( NICSLog::User, NICSLog::Error,
			string("Runtime error: ") + e.what() );
  } catch (logic_error e) {
    NICSCore::log->log( NICSLog::User, NICSLog::Error,
			string("Logic error: ") + e.what() );
  } catch (...) {
    NICSCore::log->log( NICSLog::User, NICSLog::Critical,
			"Unknown exception from UserSession::Run" );
  }

  delete user_session;
  user_session = NULL;
}
