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

#ifndef _CHESSMOVE_H
#define _CHESSMOVE_H

//#include <cstdlib>
#include <stdexcept>
#include "ChessPiece.h"
#include "ChessPosition.h"

class ChessMove {
  //
  // Public data types
  //
public:
  class InvalidMove: public runtime_error {
  public: InvalidMove( const string &what_arg ): runtime_error( what_arg ) {};
  };

  //
  // Constructors and destructors
  //
public:
  ChessMove() throw() {
    start_x = start_y = end_x = end_y = 0;
    castling = ChessPosition::NoCastling;
    promote = ChessPiece::Empty;
  }

  ChessMove( int start_x_, int start_y_, int end_x_, int end_y_,
	     ChessPiece::Type promote_ = ChessPiece::Empty )
    throw(InvalidMove):
    start_x(start_x_), start_y(start_y_), end_x(end_x_), end_y(end_y_),
    promote(promote_), castling( ChessPosition::NoCastling) {};

  ChessMove( ChessPosition::Castling c ) throw() {
    ChessMove();
    castling = c;
  };

  //
  // Accessors
  //
public:
  // These are guaranteed to return values in the range [1,8]
  int get_start_x() const { return start_x; };
  int get_start_y() const { return start_y; };
  int get_end_x() const   { return end_x; };
  int get_end_y() const   { return end_y; };

  // Returns the piece to promote to, for pawn moves
  ChessPiece::Type get_promote() const { return promote; };

  // Returns the type of castling done on the CURRENT move
  // (only valid if the king is being moved)
  ChessPosition::Castling get_castling() const { return castling; };

  // Kludge to let ChessGame recognize castling when it isn't
  // explicitly specified
  void set_castling( ChessPosition::Castling c ) { castling = c; };

  //
  // Protected data
  //
protected:
  int start_x;
  int start_y;
  int end_x;
  int end_y;

  // Promoted piece, for pawn moves
  ChessPiece::Type promote;

  // Castling type, for king moves
  ChessPosition::Castling castling;
};

#endif /* !_CHESSMOVE_H */
