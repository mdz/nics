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

#ifndef _CHESSPOSITION_H
#define _CHESSPOSITION_H

#include "ChessPiece.h"
#include <string>
#include <assert.h>
#include <stdexcept>
#include <cctype>

using namespace std;

struct ChessPositionCoords {
  int x;
  int y;
};

//
// Note: all positions are specified in rank-file notation (e.g., a4,
// h7, etc.), except in the (x, y) forms of certain functions, (which
// accept numerical coordinates in the range [1,8]
//

class ChessPosition {
  //
  // Public data types
  //
public:
  enum Color { White, Black, AnyColor };
  enum Castling { NoCastling, Queenside, Kingside, AnyCastling };
  enum Status { Unknown, InProgress, Checkmate, Stalemate };

  class InvalidPosition: public runtime_error {
  public:
    InvalidPosition( const string &what_arg ):
      runtime_error( what_arg ) {};
  };

  //
  // Constructors and destructors
  //
public:

  // Default constructor
  // (start-of-game position)
  ChessPosition() throw (InvalidPosition);

  // FEN constructor
  ChessPosition( const string &fen ) throw (InvalidPosition);

  //
  // Accessors
  //
public:
  // What sort of piece is at position (x, y)
  // x and y have the range [1,8]
  inline const ChessPiece &get_piece_at( int x, int y ) const {
    assert( x >= 1 && x <= 8);
    assert( y >= 1 && y <= 8);
    return board[x - 1][y - 1];
  }

  // What piece is at <square>?
  const ChessPiece &get_piece_at( const char *square ) const;

  // Whose turn is it?
  Color get_active_color() const { return active_color; };

  // Can the specified player castle in the specified manner?
  int get_castling( Color, Castling ) const;

  // If an en passant capture is possible, on which square?
  // If none, returns 0
  int get_en_passant_x() const;
  int get_en_passant_y() const;

  // How many half-moves since the last pawn advance or capturing move?
  int get_halfmove_clock() const;

  // How many full moves so far in the game?
  int get_fullmove_number() const;

  // Write position data as a FEN to a stream
  void write_FEN( ostream &output ) const;

  // Validate the position, and set check and status flags
  void validate( int check_legal = 1 );

  // Is the specified color king in check?
  int in_check( Color color ) const;

  // Does this position represent checkmate, stalemate, a game in
  // progress...?
  Status get_status() const;

  // Find pieces that are capable of moving to (end_x, end_y) The
  // search can be restricted to a specified rank or file. On success,
  // the coordinates of the pieces are stored in the array pointed to
  // by found_pieces, and the number of pieces found is returned

  int find_moves( const ChessPiece *pieces,
		  unsigned int num_pieces,
		  int start_x, int start_y,
		  int file,
		  int rank,
		  int direction,  // -1 => toward square, 1 => away from
		  int capture_only,
		  int check_legal,
		  int find_max,
		  ChessPositionCoords *found_pieces ) const;

  // Degenerate case of find_moves, for convenience
  int is_attacked( int file, int rank, Color color ) const;

  //
  // Mutators
  //
public:
  // Place a piece at position (x, y)
  // (replacing any existing piece on that square)
  // x and y have the range [1,8]
  void set_piece_at( int x, int y, const ChessPiece &piece ) {
    assert(x >= 1 && x <= 8);
    assert(y >= 1 && y <= 8);
    board[x - 1][y - 1] = piece;
    invalidate();
  }

  // Place a piece at <square>
  // (replacing any existing piece on that square)
  void set_piece_at( const char *square, const ChessPiece &piece );

  // Set active color
  void set_active_color( Color );

  // Set castling availability (on=1 to enable, on=0 to disable)
  void set_castling( Color, Castling, int on );

  // Set en passant move (0, 0 for none)
  void set_en_passant( int x, int y );

  // Set the halfmove clock
  void set_halfmove_clock( int );

  // Set the fullmove number
  void set_fullmove_number( int );

  // Increment the halfmove clock, and (if appropriate) the fullmove
  // number
  void increment_move() {
    switch ( active_color ) {
    case White:
      active_color = Black;
      break;
    case Black:
      ++fullmove_number;
      active_color = White;
      break;
    default:
      assert(0);
    }
  }

  // Read a FEN from an in-memory buffer Returns 1 if successful, 0 if
  // the buffer does not contain a valid FEN
  void read_FEN( istream &input ) throw (InvalidPosition);

  //
  // Protected member functions
  //
protected:
  // Convert between SAN square notation and integer coordinates
  inline static int square_x( const char *square ) {
    char file;
    assert( strlen(square) == 2 );
    
    file = tolower(square[0]);
    assert( file >= 'a' && file <= 'h');
    return file - 'a' + 1;
  };

  inline static int square_y( const char *square ) {
    char rank;
    assert( strlen(square) == 2 );
    
    rank = tolower(square[1]);
    assert( rank >= '1' && rank <= '8' );
    return rank - '1' + 1;
  };

  static const char *make_square( int x, int y );

  inline void invalidate() {
    check = 0;
    status = Unknown;
  }


  //
  // Private data
  //
private:
  ChessPiece board[8][8];
  Color active_color;
  int castling_white_kingside;
  int castling_white_queenside;
  int castling_black_kingside;
  int castling_black_queenside;
  int en_passant_x;
  int en_passant_y;
  int halfmove_clock;
  int fullmove_number;
  int check; // Is someone in check? -1 => no, 0 => don't know, 1 => yes
  Color check_who; // If yes, whom?
  Status status;
};

#endif /* !_CHESSPOSITION_H */
