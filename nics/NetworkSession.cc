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
#include <sstream>

using namespace std;
using namespace ost;

template<class T>
NetworkSession<T>::NetworkSession( TCPSocket &socket ):
  TCPSession( socket ),
  user_session(NULL) {

  tpport_t port;
  ostringstream logmsg;

  InetHostAddress ia = getPeer( &port );
  logmsg << "Connection from " << ia.getHostname() << ':' << port;
  NICSCore::log->log( NICSLog::Network, NICSLog::Info,
  		      logmsg.str() );

//    ostringstream debug;
//    debug << "cat /proc/" << getpid() << "/status";
//    system(debug.str().c_str());
}

template<class T>
NetworkSession<T>::~NetworkSession() {
  Terminate();
  endStream();
  if ( user_session != NULL )
    delete user_session;
  NICSCore::log->log( NICSLog::Network, NICSLog::Debug,
		      "NetworkSession destroyed" );
}

template<class T>
void NetworkSession<T>::Final() {
  delete this;
}

template<class T>
void NetworkSession<T>::Run() {
  try {

      user_session = new T(*this, *this);
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
}

template class NetworkSession<class UserSessionFICS>;
