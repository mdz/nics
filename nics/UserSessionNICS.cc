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

#include <string>
#include "NICSCore.h"
#include "UserSessionNICS.h"
#include "NICSDatabase.h"
#include "Chess.h"

void UserSessionNICS::Run() {
  output << "200 Ready" << endl;

  for(;;) {
    string command;
    input >> command;

    if (command == "NOOP") {
      output << "200 NOOP successful" << endl;
    } else if (command == "QUIT") {
      output << "200 Signing off" << endl;
      break;
    } else if (command == "USER") {
      string name, password;
      input >> name >> password;

      try {
	login(name, password);
	output << "200 Login successful" << endl;
      } catch (LoginFailed e) {
	output << "400 Login failed: " << e.what() << endl;
      }

    } else if (command == "ADDUSER") {
      User newuser;

      string name, password;
      input >> name >> password;

      newuser.name(name);
      newuser.password(password);

      NICSCore::database->add_user(newuser);
      output << "200 User added" << endl;
    } else if (command == "PLAY") {
      string gametype;
      input >> gametype;

      if (current_game != NULL) {
	output << "400 Game already in progress" << endl;
      } else {
	if (gametype == "CHESS") {
	  current_game = new Chess;

	  send_refresh( current_game );
	} else {
	  output << "400 Unknown game type: " << gametype << endl;
	}
      }
    } else if (command == "MOVE") {
      string move;
      input >> move;

      if ( current_game == NULL ) {
	output << "400 No game in progress" << endl;
      } else {
	try {
	  current_game->make_move(this, move);
	} catch ( Chess::InvalidMove e ) {
	  output << "400 Invalid move (" << move << "): " << e.what() << endl;
	} catch ( Chess::IllegalMove e ) {
	  output << "400 Illegal move (" << move << "): " << e.what() << endl;
	}
      }
    } else {
      output << "400 Unrecognized command (" << command << ')' << endl;
    }
  }
}

void UserSessionNICS::send_refresh(const Game *game_) {
  output << "300 Board follows" << endl;
  game_->dump_style12(output, this);
  output << "301 End board" << endl;
}

void UserSessionNICS::send_tell(const UserSession *from, const string &message) {
}

void UserSessionNICS::send_text( const string &text ) {
  output << "200 " << text << endl;
}

void UserSessionNICS::send_text_long( const string &text ) {
  output << "300 Text follows" << endl
	 << text
	 << endl;
}

void UserSessionNICS::game_start_message( const Game *game_ ) {
}

void UserSessionNICS::game_end_message( const Game *game_ ) {
}
