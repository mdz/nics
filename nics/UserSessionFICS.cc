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
#include "FilterParser.h" // XXX
#include "Chess.h" // XXX
#include "NICSGuile.h" // XXX

#include <string>
#include <sstream>

using namespace ost;

enum RunState { Login, Password, CommandLoop };

UserSessionFICS::UserSessionFICS(TCPStream& input, TCPStream& output):
  UserSession(input, output) {
  
  regcomp(&xboard_move,
	  "^([a-h][1-8][a-h][1-8]=?[qrnb]?|o-o|o-o-o)$",
	  REG_EXTENDED | REG_ICASE | REG_NOSUB );
}

UserSessionFICS::~UserSessionFICS() {
  regfree(&xboard_move);
}

void UserSessionFICS::Run(void) {
  string username, password;
  RunState state = Login;

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
      if (!input.good()
	  ||input.isPending(SOCKET_PENDING_ERROR, 0)) {
	return;
      } else if (input.isPending(SOCKET_PENDING_INPUT, 500)) {
	break;
      }
    }

    getline(input, line_raw);

    // Strip carriage return (what is it doing there?)
    if (line_raw[line_raw.length() - 1] == '\r')
      line_raw.resize(line_raw.length() - 1);

    // xboard/winboard's secret way of hiding commands
    if (line_raw[0] == '$')
      line_raw.erase(0, 1);

    istringstream line(line_raw);
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

      NICSOfferManager::OfferID offer = id;

      try {
	NICSCore::om->accept(offer, my.id);
      } catch (NICSOfferManager::OfferNotFound e) {
	send_text("No such offer");
      } catch (NICSGameManager::GameLimitReached e) {
	send_text(e.what());
      }
    } else if (command == "accept") {
      try {
	set<NICSOfferManager::OfferID> offers;
	unsigned int ret = NICSCore::om->find_matching_offers(my.id, offers);

	if (ret <= 0)
	  send_text("There are no offers to accept.");
	else if (ret == 1)
	  NICSCore::om->accept(*offers.begin(), my.id);
	else
	  // XXX - show a list
	  send_text("There are several offers matching your formula.");

      } catch (NICSOfferManager::OfferNotFound e) {
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

      if ( have_focus )
	try {
	  send_refresh( game_focus );
	} catch (NICSGameManager::GameNotFound ) {
	  send_text("Not playing a game");
	}
      else
	send_text("Not playing a game");

    } else if (regexec(&xboard_move, command.c_str(), 0, 0, 0) == 0) {

      if (have_focus) {
	try {
	  NICSCore::gm->rlock(game_focus)
	    .make_move(my.id, command);
	  NICSCore::gm->unlock(game_focus);
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

      NICSCore::om->register_offer(new GameOffer(my.id,
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
	  NICSCore::om->
	    register_offer(new GameOffer( my.id, GameFilter(),
					  UserSessionFilter( NICSCore::sm->
							     find_session(opp) ) ));
	  send_text("Offer sent.");
	} catch (NICSSessionManager::UserNotFound) {
	  send_text("Requested opponent is not logged in");
	}
      }

    } else if (command == "resign") {

      if (have_focus)
	try {
	  // XXX - check if we are actually playing
	  NICSCore::gm->rlock(game_focus)
	    .resign(my.id);
	  NICSCore::gm->unlock(game_focus);
	} catch (NICSGameManager::GameNotFound e) {
	  send_text("You are not playing a game");
	}
      else
	send_text("You are not playing a game");

    } else if (command == "who") {
      NICSCore::sm->show_sessions( my.id );
    } else if (command == "sought") {
      NICSCore::om->show_offers( my.id );
    } else if (command == "games") {
      NICSCore::gm->show_games( my.id );
    } else if (command == "observe") {
      unsigned int id;
      line >> id;

      try {
	Game &g = NICSCore::gm->rlock(id);
	g.add_user( my.id, Game::Observe );
	g.unlock();
	game_focus = id;
	have_focus = true;
      } catch ( NICSGameManager::GameNotFound e ) {
	send_text(e.what());
      }

    } else if (command == "unobserve") {
      unsigned int id;
      line >> id;

      try {
	Game &g = NICSCore::gm->rlock(id);
	g.delete_user(my.id);
	g.unlock();
	have_focus = false;
      } catch ( NICSGameManager::GameNotFound e) {
	send_text(e.what());
      }

    } else if (command == "history") {
      string user;
      line >> user;
      
      if (user == "")
	user = my.user.name();
      
      NICSCore::database->dump_history( user, output );
    } else if (command == "kibitz") {

      if (have_focus) {
	string message;
	
	getline(line, message);
	
	try {
	  Game &g = NICSCore::gm->rlock(game_focus);
	  g.kibitz(my.id, message);
	  g.unlock();
	} catch (NICSSessionManager::UserNotFound e) {
	  send_text(e.what());
	}
      } else {
	send_text("Where?");
      }
    } else if (command == "whisper") {

      if (have_focus) {
	string message;

	getline(line, message);

	try {
	  Game &g = NICSCore::gm->rlock(game_focus);
	  g.whisper(my.id, message);
	  g.unlock();
	} catch (NICSSessionManager::UserNotFound e) {
	  send_text(e.what());
	}
      } else {
	send_text("Where?");
      }
    } else if (command == "test") {
      try {
	GameFilter filter(line);
	Chess game;

	send_text(filter.match(game) ? "true" : "false");
	
      } catch (FilterParser::ParseError e) {
	send_text(string("Parse error: ") + e.what());
      }
    } else if (command == "test2") {
      string expr;
      getline(line, expr);
      try {
	bool result = NICSCore::guile->eval_bool(expr);
	
	send_text(result ? "True" : "False");
      } catch (NICSGuile::Error e) {
	send_text(string("Guile error: ") + e.what());
      }
    }
    else {
      send_text("Unrecognized command: " + command);
    }
  }
}
  

void UserSessionFICS::send_prompt() {
  output << "ics% " << flush;
}

void UserSessionFICS::send_refresh( GameID gid ) {
  Game &game = NICSCore::gm->rlock(gid);
  string board = game.get_board(my.id);

  game.unlock();

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

void UserSessionFICS::game_start_message( GameID gid ) {
  Game &game = NICSCore::gm->rlock(gid);

  // Creating: docz (1826) GuestBYKG (++++) unrated lightning 1 0
  // {Game 34 (docz vs. GuestBYKG) Creating unrated lightning match.}
  output << "{Game " << game.id() << " ("
	 << game.description()
	 << ") " << "Creating match}" << endl;

  game.unlock();
  send_prompt();
}

void UserSessionFICS::game_end_message( GameID gid ) {
  Game &game = NICSCore::gm->rlock(gid);
  // {Game 34 (docz vs. GuestBYKG) GuestBYKG resigns} 1-0
  output << "{Game " << game.id() << " ("
	 << game.description()
	 << ") " << game.result_text() << "} "
	 << game.score_text()
	 << endl;
  game.unlock();
  send_prompt();
}

void UserSessionFICS::send_offer( const GameOffer &offer ) {
  if ( offer.user_session_filter().session_specific()
       && offer.user_session_filter().session() == my.id ) {
    UserSession &offerer = NICSCore::sm->rlock(offer.session());
    // Challenge: GuestHQXW (++++) docz (1614) unrated blitz 2 12.
    output << "Challenge: "
	   << offerer.user().name() << " (++++) "
	   << my.user.name() << " (++++) "
	   << "unrated blitz 1 0." << endl;
    offerer.unlock();
  } else {
    output << "TODO: General game offer" << endl;
  }

  send_prompt();
}

void UserSessionFICS::send_session( SessionID sid ) {
  try {
    UserSession &session = NICSCore::sm->rlock(sid);
    output << session.user().name() << endl;
    session.unlock();
  } catch (NICSSessionManager::UserNotFound) {
  }
}

void UserSessionFICS::send_game( GameID gid ) {
  try {
    Game &game = NICSCore::gm->rlock(gid);
    output << "Game " << game.id() << ": " << game.description() << endl;
    game.unlock();
  } catch (NICSGameManager::GameNotFound) {
  }
}

void UserSessionFICS::send_kibitz( SessionID fromuser,
				   GameID game,
				   const string& message ) {
  try {
    UserSession &session = NICSCore::sm->rlock(fromuser);
    output << session.user().name() << "(xxxx)[" << game
	   << "] kibitzes: " << message << endl;
    session.unlock();
  } catch (NICSSessionManager::UserNotFound) {
  }
}

void UserSessionFICS::send_whisper( SessionID fromuser,
				    GameID game,
				    const string& message ) {
  try {
    UserSession &session = NICSCore::sm->rlock(fromuser);
    output << session.user().name() << "(xxxx)[" << game
	   << "] whispers: " << message << endl;
    session.unlock();
  } catch (NICSSessionManager::UserNotFound) {
  }
}
