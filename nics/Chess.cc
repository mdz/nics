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

#include "Chess.h"
#include "NICSCore.h"
#include "NICSGameManager.h"
#include "NICSLog.h"
#include "NICSDatabase.h"
#include "UserSession.h"
#include <ChessMoveAlgebraic.h>
#include <ChessMoveCoordinateAlgebraic.h>
#include <cctype>
#include <iostream>
#include <sstream>
#include <iomanip>

Chess::Chess(): Game() {
  result = InProgress;
}

Chess::~Chess() {
}

void Chess::make_move(SessionID player,
		      const string &movetext) {

  if ( clocks[white].zero() ) {
    flag(white);
    return;
  } else if ( clocks[black].zero() ) {
    flag(black);
    return;
  }

  if ( (player == white
	&& game.current_position().get_active_color() != ChessPosition::White)
       ||
       (player == black
	&& game.current_position().get_active_color() != ChessPosition::Black) )
    throw IllegalMove("It is not your turn");

  // First, try xboard-style
  try {

    ChessMoveCoordinateAlgebraic move_xboard(movetext);
    // Convert to algebraic
    ChessMoveAlgebraic tmp_move(move_xboard, game.current_position());

//      cerr << "Received move: \"" << movetext << "\", converted to algebraic \""
//  	 << tmp_move.text() << "\"" << endl;

    game.make_move(move_xboard);
    last_move = tmp_move;
//      cerr << "Position: ";
//      game.current_position().write_FEN(cerr);

  } catch ( ChessGame::IllegalMove e) {

    throw IllegalMove(e.what());

  } catch ( ChessMove::InvalidMove e ) {
      // Try algebraic
      try {

	ChessMoveAlgebraic move_algebraic(movetext, game.current_position() );

	game.make_move(move_algebraic);
	last_move = move_algebraic;

      } catch ( ChessMove::InvalidMove e ) {
	throw InvalidMove(e.what());
      } catch ( ChessGame::IllegalMove e ) {
	throw IllegalMove(e.what());
      }
  }

  ChessPosition::Status status = game.current_position().get_status();
  string msg;
  if (status == ChessPosition::Checkmate) {
    result = Checkmate;

    if ( game.current_position().in_check( ChessPosition::White ) )
      winner = black;
    else
      winner = white;
    
  } else if (status == ChessPosition::Stalemate) {
    result = Stalemate;
  }

  if (result != InProgress) {

    Game::refresh();

    end();
    return;
  }

  toggle_clocks();
  Game::refresh();
}

// ICS style 12
const string Chess::get_board( SessionID session,
		       Relationship rel, int style ) {

  const ChessPosition &pos(game.current_position());

  ostringstream out;

  out << endl << "<12> ";
  for( int rank = 8 ; rank >= 1 ; --rank ) {
    for( int file = 1 ; file <= 8 ; ++file ) {
      out << pos.get_piece_at( file, rank ).piece_char();
    }

    out << " ";
  }

  if ( pos.get_active_color() == ChessPosition::White )
    out << "W ";
  else
    out << "B ";

  if ( pos.get_en_passant_x() == 0 )
    out << "-1";
  else
    out << pos.get_en_passant_x() - 1;

  out << ' ';

  if ( pos.get_castling( ChessPosition::White, ChessPosition::Kingside ) )
    out << "1 ";
  else
    out << "0 ";

  if ( pos.get_castling( ChessPosition::White, ChessPosition::Queenside ) )
    out << "1 ";
  else
    out << "0 ";

  if (pos.get_castling( ChessPosition::Black, ChessPosition::Kingside ) )
    out << "1 ";
  else
    out << "0 ";

  if (pos.get_castling( ChessPosition::Black, ChessPosition::Queenside ) )
    out << "1 ";
  else
    out << "0 ";

  out << pos.get_halfmove_clock() << ' ';

  // Game number
  out << my.id << ' ';

  out << game.white() << ' '
      << game.black() << ' ';

  // Relation to game
  //      -3 isolated position, such as for "ref 3" or the "sposition" command
  //      -2 I am observing game being examined
  //       2 I am the examiner of this game
  //      -1 I am playing, it is my opponent's move
  //       1 I am playing and it is my move
  //       0 I am observing a game being played
  if ( rel == Play )

    if ( session == white && pos.get_active_color() == ChessPosition::White
	 ||
	 session == black && pos.get_active_color() == ChessPosition::Black )
      out << "1 ";
    else
      out << "-1 ";

  else if ( rel == Observe )
    out << "0 ";

  else if ( rel == Examine )
    out << "2 ";

  else
    out << "-3 ";

  // XXX - initial game time
  out << my.initial_time << ' ';

  // XXX - increment
  out << "0 ";
  //<< increment.seconds() << ' ';

  // XXX - material strength
  out << "42 42 ";

  // remaining time (white black)
  out << clocks[white].remaining_seconds() << ' '
      << clocks[black].remaining_seconds() << ' ';

  out << pos.get_fullmove_number() << ' ';

  // previous move
  if ( game.moves() == 0 )
    out << "none ";
  else
    out << last_move.text() << ' ';

  // time taken for previous move
  if ( game.moves() == 0 ) {
    out << "(0:00) ";
  } else {
    unsigned long elapsed;
    if ( pos.get_active_color() == ChessPosition::White )
      elapsed = clocks[black].elapsed_seconds();
    else
      elapsed = clocks[white].elapsed_seconds();
    
    out << '(' << elapsed / 60 << ':'
	<< setfill('0') << setw(2) << elapsed % 60 << ") ";
  }

  // pretty notation for previous move
  if ( game.moves() == 0 )
    out << "none ";
  else
      out << last_move.text() << ' ';

  // XXX - board orientation
  out << "0 ";

  // XXX - new fields?
  //out << "0 0 ";

  return out.str();
}

void Chess::start_hook() {
  bool coin = flip_coin();
  unsigned int players_found = 0;

  for( map<SessionID, Relationship>::iterator it =
	 users.begin() ; 
       it != users.end() ;
       ++it )

    if (it->second == Play) {
      if (players_found == 0)
	if (coin)
	  white = it->first;
	else
	  black = it->first;
      else if (players_found == 1)
	if (coin)
	  black = it->first;
	else
	  white = it->first;
      else
	break;

      ++players_found;
    }


  if (players_found != 2) {
    NICSCore::log->log( NICSLog::Game, NICSLog::Error,
			"Chess::start() called with bad number of players" );
    result = Aborted;
    end();
  }

  game.white(UserSession::rlock(white).user().name());
  UserSession::unlock(white);
  game.black(UserSession::rlock(black).user().name());
  UserSession::unlock(black);

  NICSCore::log->log( NICSLog::Game, NICSLog::Info,
		      "Chess::start(): white=" + game.white()
		      + ", black=" + game.black() );

  clocks[white].start();

  Game::refresh();
}

void Chess::toggle_clocks() {
  if (clocks[white].is_running()) {
    clocks[white].stop();
    clocks[black].start();
  } else if (clocks[black].is_running()) {
    clocks[black].stop();
    clocks[white].start();
  }
}

void Chess::flag( SessionID player ) {
  Game::refresh();

  if ( clocks.find(player) == clocks.end() )
    return; // XXX, throw a meaningful exception

  if ( !clocks[player].zero() )
    return; // XXX, throw a meaningful exception

  result = TimeForfeit;

  if ( player == white )
    winner = black;
  else if ( player == black )
    winner = white;
  else
    throw logic_error("Flagged player is neither white nor black");

  end();
}

string Chess::description() const {
  return game.white() + " vs. " + game.black();
}

void Chess::resign( SessionID player ) {
  if ( player == white ) {
    winner = black;
  } else {
    winner = white;
  }

  result = Resign;

  end();
}

string Chess::score_text() const {
  if ( winner == white )
    return "1-0";
  else if ( winner == static_cast<unsigned>(-1) )
    return "1/2-1/2";
  else if ( winner == black )
    return "0-1";
  else
    throw logic_error("score_text: winner is funky");
}

string Chess::result_text() const {
  if (my.status == Game::Aborted)
    return string("Game aborted");

//    if ( winner != black && winner != white )
//      throw logic_error("result_text: winner is neither white nor black");

  switch ( result ) {
  case TimeForfeit:
    if ( winner == white )
      return string(game.black() + " forfeits on time");
    else
      return string(game.white() + " forfeits on time");
  case Checkmate:
    if ( winner == white )
      return string(game.black() + " checkmated");
    else
      return string(game.white() + " checkmated");
  case Stalemate:
    return "Stalemate";
  case InProgress:
    return "Still in progress";
  case Resign:
    if ( winner == white )
      return string(game.black() + " resigns");
    else
      return string(game.white() + " resigns");
  default:
    throw logic_error("Unknown result value");
  }
}

void Chess::save() const {
  NICSCore::database->save_game( *this );
}
