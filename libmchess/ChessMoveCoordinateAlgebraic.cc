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

#include "ChessMoveCoordinateAlgebraic.h"

ChessMoveCoordinateAlgebraic::ChessMoveCoordinateAlgebraic(
							   const string &data ) {
  if ( data.length() < 3)
    throw InvalidMove("String too short to contain a valid move");
  if ( data.length() > 6 )
    throw InvalidMove("String too long to contain a valid move");

  if ( data == "o-o" ) {
    castling = ChessPosition::Kingside;
    return;
  }
  if ( data == "o-o-o" ) {
    castling = ChessPosition::Queenside;
    return;
  }

  if ( data[0] < 'a' || data[0] > 'h' )
    throw InvalidMove("Invalid file specification");

  start_x = data[0] - 'a' + 1;

  if ( data[1] < '1' || data[1] > '8' )
    throw InvalidMove("Invalid rank specification");

  start_y = data[1] - '1' + 1;

  if ( data[2] < 'a' || data[2] > 'h' )
    throw InvalidMove("Invalid file specification");

  end_x = data[2] - 'a' + 1;

  if ( data[3] < '1' || data[3] > '8' )
    throw InvalidMove("Invalid rank specification");

  end_y = data[3] - '1' + 1;

  if ( data.length() > 4 ) {
    if ( data[4] != '=' )
      throw InvalidMove("Expected: '='");
    else
      switch ( data[5] ) {
      case 'Q':case 'q':
	promote = ChessPiece::Queen;break;
      case 'R':case 'r':
	promote = ChessPiece::Rook;break;
      case 'N':case 'n':
	promote = ChessPiece::Knight;break;
      case 'B':case 'b':
	promote = ChessPiece::Bishop;break;
      default:
	throw InvalidMove("Invalid piece specification");
      }
  }
}
