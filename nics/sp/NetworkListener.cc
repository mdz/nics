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

#include "NetworkListener.h"
#include "NetworkSession.h"
#include "NICSCore.h"
#include "NICSLog.h"
#include <strstream>

NetworkListener::NetworkListener( const InetHostAddress &address,
				  tpport_t port,
				  NetworkSession::SessionType type):
  TCPSocket( address, port ),
  session_type(type) {
  this->Start();
}

void NetworkListener::Run() {
  ostrstream logmsg;
  tpport_t port;
  
  getLocal(&port);
  logmsg << "NetworkListener waiting for a connection on port " << port;
    
  NICSCore::log->log( NICSLog::Network, NICSLog::Info,
		      string(logmsg.str(), logmsg.pcount()) );

  for(;;) {
    try {
      NetworkSession *session =
	new NetworkSession( *dynamic_cast<TCPSocket *>(this), session_type );
      session->Start();
    } catch (...) {
      NICSCore::log->log( NICSLog::Network, NICSLog::Error,
			  "Error creating a NetworkSession" );
    }
  }
}