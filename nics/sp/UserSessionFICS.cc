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

#include "UserSessionFICS.h"
#include "NICSCore.h"
#include "NICSLog.h"
#include "NICSGameManager.h"
#include "NICSSessionManager.h"
#include "NICSDatabase.h"
#include "GameOffer.h"

#include <string>
#include <strstream>
#include <regex.h>

enum RunState { Login, Password, CommandLoop };

void UserSessionFICS::Run(void) {
  string username, password;
  regex_t xboard_move;
  RunState state = Login;

  if (regcomp(&xboard_move,
    	      "^([a-h][1-8][a-h][1-8]=?[qrnb]?|o-o|o-o-o)$",
	      REG_EXTENDED | REG_ICASE | REG_NOSUB ) != 0) {
    NICSCore::log->log( NICSLog::User, NICSLog::Error,
			"regcomp failed in UserSessionFICS" );
  }

  //
  // Main command loop
  //
  for(;;) {
    string line_raw;

    if (state == Login)
      output << "login: " << flush;
    else if (state == Password)
      output << "Password: " << flush;
    else if (state == CommandLoop)
      send_prompt();

    for(;;) {
      if (input.isPending(SOCKET_PENDING_ERROR, 0)) {
	return;
      } else if (input.isPending(SOCKET_PENDING_INPUT, 500)) {
	break;
      }
    }

    getline(input, line_raw);

    // Strip carriage return (what is it doing there?)
    if (line_raw[line_raw.length() - 1] == '\r')
      line_raw.erase(line_raw.length() - 1);

    // xboard/winboard's secret way of hiding commands
    if (line_raw[0] == '$')
      line_raw.erase(0, 1);

    istrstream line(line_raw.c_str());
    string command;

    line >> command;

    if (command.length() == 0) {

      // Do nothing

    } else if (state == Login) {

      username = command;
      state = Password;

    } else if (state == Password) {

      password = command;

      try {
	login(username, password);
	output << "Login successful" << endl;
	state = CommandLoop;
      } catch (LoginFailed e) {
	output << "Login failed: " << e.what() << endl;
	state = Login;
      }

    } else if (command == "hello") {

      send_text("Pleased to meet you");

    } else if (command == "play") {
      unsigned int id;
      line >> id;

      NICSGameManager::OfferID offer = id;

      try {
	NICSCore::game_manager->accept(offer, my.id);
      } catch (NICSGameManager::OfferNotFound e) {
	send_text("No such offer");
      } catch (NICSGameManager::GameLimitReached e) {
	send_text(e.what());
      }
    } else if (command == "accept") {
      // XXX
      try {
	NICSCore::game_manager->accept(0, my.id);
      } catch (NICSGameManager::OfferNotFound e) {
	send_text("There are no offers to accept.");
      } catch (NICSGameManager::GameLimitReached e) {
	send_text(e.what());
      }
    } else if (command == "bye" || command == "quit") {
      NICSCore::log->log( NICSLog::User, NICSLog::Debug,
			  "Got goodbye command" );
      send_text("Goodbye");

      break;
    } else if (command == "refresh") {

      if ( gaming )
	try {
	  send_refresh( current_game );
	} catch (NICSGameManager::GameNotFound ) {
	  send_text("Not playing a game");
	}
      else
	send_text("Not playing a game");

    } else if (regexec(&xboard_move, command.c_str(), 0, 0, 0) == 0) {

      if (gaming) {
	try {
	  Game::make_move(current_game, my.id, command);
	} catch ( Game::IllegalMove e ) {
	  send_text("Illegal move (" + command + "): " + e.what());
	} catch ( Game::InvalidMove e ) {
	  send_text("Invalid move (" + command + "): " + e.what());
	} catch ( NICSGameManager::GameNotFound ) {
	  send_text("Not playing a game");
	}
      } else {
	send_text("Not playing a game");
      }
    } else if (command == "seek") {

      NICSCore::game_manager->register_offer(new GameOffer(my.id,
							   GameFilter(),
							   UserSessionFilter()));

      send_text("Offer posted.");

    } else if (command == "tell") {
      string user;
      string message;

      line >> user;

      getline(line, message);

      try {
	tell(user, message);
	send_text("(told " + user + ")");
      } catch (NICSSessionManager::UserNotFound e) {
	send_text(e.what());
      }
    } else if (command == "match") {
      string opp;
      line >> opp;

      if ( opp == "" ) {
	send_text("Usage: match <opponent>");
      } else {

	try {
	  NICSCore::game_manager->
	    register_offer(new GameOffer( my.id, GameFilter(),
					  UserSessionFilter( NICSCore::session_manager->
							     find_session(opp) ) ));
	  send_text("Offer sent.");
	} catch (NICSSessionManager::UserNotFound) {
	  send_text("Requested opponent is not logged in");
	}
      }

    } else if (command == "resign") {

      if (gaming)
	try {
	  Game::resign( current_game, my.id );
	  continue;
	} catch (NICSGameManager::GameNotFound e) {
	  send_text("You are not playing a game");
	}
      else
	send_text("You are not playing a game");

    } else if (command == "who") {
      NICSCore::session_manager->show_sessions( my.id );
    } else if (command == "sought") {
      NICSCore::game_manager->show_offers( my.id );
    } else if (command == "games") {
      NICSCore::game_manager->show_games( my.id );
//      } else if (command == "observe") {
//        unsigned int id;
//        line >> id;

//        try {
//  	Game *g = NICSCore::game_manager->find_game( id );
//  	NICSCore::game_manager->join( g, this, Observe );
//        } catch ( NICSGameManager::GameNotFound e ) {
//  	send_text(e.what());
//        }

//      } else if (command == "unobserve") {
//        unsigned int id;
//        line >> id;

//        try {
//        Game *g = NICSCore::game_manager->find_game( id );
//        NICSCore::game_manager->leave( g, this, Observe );
//        } catch ( NICSGameManager::GameNotFound e) {
//  	send_text(e.what());
//        }

    } else if (command == "history") {
      string user;
      line >> user;
      
      if (user == "")
	user = my.user.name();
      
      NICSCore::database->dump_history( user, output );

    } else {
      send_text("Unrecognized command: " + command);
    }
  }
  regfree(&xboard_move);
}
  

void UserSessionFICS::send_prompt() {
  output << "ics% " << flush;
}

void UserSessionFICS::send_refresh( NICSGameManager::GameID game ) {
  string board = Game::get_board(game, my.id);
  output << endl << board << endl;
  send_prompt();
}

void UserSessionFICS::send_tell(const string &fromuser, const string &message) {
  output << endl
	 << fromuser << " tells you: " << message << endl;
  send_prompt();
}

void UserSessionFICS::send_text_long( const string &text ) {
  output << text;
}

void UserSessionFICS::send_text( const string &text ) {
  output << text << endl;
  //send_prompt();
}

void UserSessionFICS::game_start_message( NICSGameManager::GameID game ) {
  // Creating: docz (1826) GuestBYKG (++++) unrated lightning 1 0
  // {Game 34 (docz vs. GuestBYKG) Creating unrated lightning match.}
  output << "{Game " << game << " ("
	 << Game::description(game)
	 << ") " << "Creating match}" << endl;
  send_prompt();
}

void UserSessionFICS::game_end_message( NICSGameManager::GameID game ) {
  // {Game 34 (docz vs. GuestBYKG) GuestBYKG resigns} 1-0
  output << "{Game " << game << " ("
	 << Game::description(game)
	 << ") " << Game::result_text(game) << "} "
	 << Game::score_text(game)
	 << endl;
  send_prompt();
}

void UserSessionFICS::send_offer( const GameOffer &offer ) {
  if ( offer.user_session_filter().session_specific()
       && offer.user_session_filter().session() == my.id )
    // Challenge: GuestHQXW (++++) docz (1614) unrated blitz 2 12.
    output << "Challenge: "
	   << name(offer.user_session_filter().session()) << " (++++) "
	   << my.user.name() << " (++++) "
	   << "unrated blitz 1 0." << endl;
  else
    output << "TODO: General game offer" << endl;
  send_prompt();
}

void UserSessionFICS::send_session( NICSSessionManager::SessionID session ) {
  try {
    output << name(session) << endl;
  } catch (NICSSessionManager::UserNotFound) {
  }
}

void UserSessionFICS::send_game( NICSGameManager::GameID game ) {
  try {
    output << "Game " << game << ": " << Game::description(game) << endl;
  } catch (NICSGameManager::GameNotFound) {
  }
}
