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

#include "NICSCore.h"
#include "NICSDatabase.h"
#include "NICSNetwork.h"
#include "NICSSessionManager.h"
#include "NICSGameManager.h"
#include "NICSOfferManager.h"
#include "NICSLog.h"
#include "NICSGuile.h"

#include <iostream>
#include <cstdlib>

#include "PostgreSQL.h"

int NICSCore::exists = 0;
//NICSLoader *NICSCore::loader = NULL;
NICSNetwork *NICSCore::network = NULL;
NICSDatabase *NICSCore::database = NULL;
NICSSessionManager *NICSCore::sm = NULL;
NICSGameManager *NICSCore::gm = NULL;
NICSOfferManager *NICSCore::om = NULL;
NICSLog *NICSCore::log = NULL;
NICSGuile *NICSCore::guile = NULL;

int NICSCore::Run(void) {
  try {
    //loader = new NICSLoader;
    //config = new NICSConfig;
    log = new NICSLog("/dev/stdout", NICSLog::Debug);
    database = new PostgreSQL("dbname=nics user=postgres");
    network = new NICSNetwork;
    guile = new NICSGuile;
    guile->Start();
    sm = new NICSSessionManager;
    gm = new NICSGameManager;
    om = new NICSOfferManager;

    *log << NICSLog::msg( NICSLog::Internal, NICSLog::Info,
			  "Initialization complete" );
    
    for(;;) {
      if (!cin || cin.get() == 'q')
	break;
      cout << "Session dump:" << endl
	   << "=============" << endl;
      sm->dump_session_info(cout);
      cout << "Game offer dump:" << endl
	   << "================" << endl;
      om->dump_offer_info(cout);
      cout << "Game dump:" << endl
	   << "==========" << endl;
      gm->dump_game_info(cout);
    }

    delete om; om = NULL;
    delete gm; gm = NULL;
    delete sm; sm = NULL;
    delete guile; guile = NULL;
    delete network; network = NULL;
    delete database; database = NULL;
    delete log; log = NULL;

    return EXIT_SUCCESS;
  } catch (exception e) {
    cerr << "Unexpected exception: e.what()" << endl;

    return EXIT_FAILURE;
  }
}
