// -*- C++ -*-

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

#ifndef _CHESS_H
#define _CHESS_H

#include <ChessGame.h>
#include <ChessMoveAlgebraic.h>
#include "NICSSessionManager.h"
#include "Game.h"

class Chess: public Game {
public:
  Chess();
  ~Chess();

  enum Result { InProgress, Resign, Checkmate, Stalemate, TimeForfeit };
  Result get_result() const { return result; };
  enum Winner { White, Black, Neither };
  Winner get_winner() const {
    if ( winner == white )
      return White;
    else if ( winner == black )
      return Black;
    else
      return Neither;
  };

protected:
  virtual void make_move(NICSSessionManager::SessionID player,
			 const string &move);
  virtual const string get_board(ReferenceCounted::pointer<UserSession> player,
				 Relationship rel, int style = 0 );
  virtual void refresh( ReferenceCounted::pointer<UserSession> player,
			Relationship rel ) const {};
  virtual void flag( NICSSessionManager::SessionID player );
  virtual void resign( NICSSessionManager::SessionID player );
  virtual string description() const;
  virtual string score_text() const;
  virtual string result_text() const;
  virtual void save() const;
  virtual void start();
  virtual void end() {};

  // These are intended for NICSDatabase::save_game
  unsigned int moves() const { return game.moves(); };
  const ChessMove &move( unsigned int i ) const { return game.move(i); };
  const string &white_name() const { return game.white(); };
  const string &black_name() const { return game.black(); };

private:

  void toggle_clocks();

  ChessGame game;
  ChessMoveAlgebraic last_move;
  NICSSessionManager::SessionID white;
  NICSSessionManager::SessionID black;
  unsigned int winner;
  Result result;
};

#endif // !_CHESS_H
