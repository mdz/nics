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
#include <sstream>

using namespace ost;

class TestSession: public TCPSession {
public:
  TestSession(TCPSocket &server): TCPSession(server) {
  }
  ~TestSession() { Terminate(); endStream(); cerr << "~TestSession" << endl; };

  void Run() { cerr << "TestSession::Run()" << endl; };
  void Final() { delete this; };
};
    

template<class T>
NetworkListener<T>::NetworkListener( const InetAddress &address,
				     tpport_t port ):
  TCPSocket( address, port ) {
}

template<class T>
NetworkListener<T>::~NetworkListener() {
  *NICSCore::log << NICSLog::msg( NICSLog::Network, NICSLog::Debug,
				  "NetworkListener destroyed" );

  Terminate();
}

template<class T>
void NetworkListener<T>::Run() {
  ostringstream logmsg;
  tpport_t port;
  
  getLocal(&port);
  logmsg << "NetworkListener waiting for a connection on port " << port;
    
  NICSCore::log->log( NICSLog::Network, NICSLog::Info,
		      logmsg.str() );


  for(;;)
    if (isPendingConnection(1000))
      try {
	NetworkSession<T> *s =
	  new NetworkSession<T>( *dynamic_cast<TCPSocket *>(this) );
	s->Start();
	//        (new TestSession( *dynamic_cast<TCPSocket *>(this) ))
	//  	->Start();
      } catch (...) {
	NICSCore::log->log( NICSLog::Network, NICSLog::Error,
			    "Error creating a NetworkSession" );
      }
    else
      testCancel();
}

template class NetworkListener<class UserSessionFICS>;
