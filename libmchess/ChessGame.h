// -*- C++ -*-

//  libmchess - a library for abstraction of operations on a chessboard    
//  Copyright (C) 2000, 2001  Matt Zimmerman

//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.

//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA

#ifndef _CHESSGAME_H
#define _CHESSGAME_H

#include "ChessPosition.h"
#include "ChessMove.h"
#include <iostream>
#include <string>
#include <vector>

class ChessGame {
  //
  // Constructors and destructors
  //
public:
  // Default constructor (initial game setup, move 1)
  ChessGame();

  // Read PGN data
  ChessGame( istream &input );

  // Start from a position
  ChessGame( const ChessPosition & );

  enum Winner { WhiteWins, BlackWins, Draw, AnyWinner };
  enum Ending { Checkmate, Stalemate, Flag };

  //
  // Exceptions
  //
  class IllegalMove: public runtime_error {
  public: IllegalMove( const string &what_arg ): runtime_error( what_arg ) {};
  };

  //
  // Accessors
  //
  const ChessPosition &current_position() const { return position; };

  const string &event() const { return data.event; };
  const string &site() const { return data.site; };
  const string &date() const { return data.date; };
  const string &round() const { return data.round; };
  const string &white() const { return data.white; };
  const string &black() const { return data.black; };
  const string &result() const { return data.result; };
  

  //
  // Mutators
  //
  void make_move( const class ChessMove &, int validate = 1 )
    throw(IllegalMove);

  void event( const string &value ) { data.event = value; };
  void site( const string &value ) { data.site = value; };
  void date( const string &value ) { data.date = value; };
  void round( const string &value ) { data.round = value; };
  void white( const string &value ) { data.white = value; };
  void black( const string &value ) { data.black = value; };
  void result( const string &value ) { data.result = value; };

  void pgn_tag( const string &name, const string &value );

  const ChessMove &move( unsigned int halfmove ) const {
    if ( halfmove > data.moves.size() - 1 )
      throw range_error("Invalid move number");
    return data.moves[halfmove];
  };

  unsigned int moves() const { return data.moves.size(); };

  // Read a single game
  friend istream& operator>>(istream& is, ChessGame& game);

  //
  // Protected data
  //
protected:
  ChessPosition position;
  struct {
    vector<ChessMove> moves;

    string event;
    string site;
    string date;
    string round;
    string white;
    string black;
    string result;
  } data;
};

istream& operator>>(istream& is, ChessGame& game);

#endif /* !_CHESSGAME_H */
