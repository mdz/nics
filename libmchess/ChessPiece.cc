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

#ifdef HAVE_ASSERT_H
#include <assert.h>
#else
#define assert(x)
#endif

#include <ctype.h>
#include <iostream.h>

#include "ChessPiece.h"

static const ChessPieceVector king_vectors[] = {
  { 0, 1, 1 },
  { 0, -1, 1 },
  { 1, 0, 1 },
  { -1, 0, 1 },
  { 1, 1, 1 },
  { -1, 1, 1 },
  { 1, -1, 1 },
  { -1, -1, 1 } };
static const ChessPieceVector queen_vectors[] = {
  { 1, 1, 8 },
  { -1, 1, 8 },
  { 1, -1, 8 },
  { -1, -1, 8 },
  { 1, 0, 8 },
  { -1, 0, 8 },
  { 0, 1, 8 },
  { 0, -1, 8 } };
static const ChessPieceVector bishop_vectors[] = {
  { 1, 1, 8 },
  { -1, 1, 8 },
  { 1, -1, 8 },
  { -1, -1, 8 } };
static const ChessPieceVector rook_vectors[] = {
  { 1, 0, 8 },
  { -1, 0, 8 },
  { 0, 1, 8 },
  { 0, -1, 8 } };
static const ChessPieceVector knight_vectors[] = {
  { 1, 2, 1 },
  { -1, 2, 1 },
  { 1, -2, 1 },
  { -1, -2, 1 },
  { 2, 1, 1 },
  { -2, 1, 1 },
  { 2, -1, 1 },
  { -2, -1, 1 } };
static const ChessPieceVector white_pawn_vectors[] = {
  { 1, 1, 1 },
  { -1, 1, 1 },
  { 0, 1, 2 } };
static const ChessPieceVector black_pawn_vectors[] = {
  { 1, -1, 1 },
  { -1, -1, 1 },
  { 0, -1, 2 } };

char ChessPiece::piece_char() const {
  char ret;

  switch ( type ) {
  case Empty:
    return ' ';
  case Pawn:
    ret = 'P';break;
  case Knight:
    ret = 'N';break;
  case Bishop:
    ret = 'B';break;
  case Rook:
    ret = 'R';break;
  case Queen:
    ret = 'Q';break;
  case King:
    ret = 'K';break;
  default:
    // Should never happen
    return 'X';
  }

  if ( color == Black )
    ret = tolower(ret);

  return ret;
}

void ChessPiece::set(char c) {
  switch ( toupper(c) ) {
  case 'P':
    set_type( Pawn );break;
  case 'N':
    set_type( Knight );break;
  case 'B':
    set_type( Bishop );break;
  case 'R':
    set_type( Rook );break;
  case 'Q':
    set_type( Queen );break;
  case 'K':
    set_type( King );break;
  default:
    set_type( Empty );
  }

  if ( c == tolower(c) )
    set_color( Black );
  else
    set_color( White );
}
  
unsigned int ChessPiece::get_vectors( const ChessPieceVector **dst,
				      int capture_only ) const {
  unsigned int size = 0;

  switch ( type ) {
  case King:
    *dst = king_vectors;
    size = sizeof(king_vectors);
    break;
  case Queen:
    *dst = queen_vectors;
    size = sizeof(queen_vectors);
    break;
  case Rook:
    *dst = rook_vectors;
    size = sizeof(rook_vectors);
    break;
  case Knight:
    *dst = knight_vectors;
    size = sizeof(knight_vectors);
    break;
  case Bishop:
    *dst = bishop_vectors;
    size = sizeof(bishop_vectors);
    break;
  case Pawn:
    if ( color == White ) {
      *dst = white_pawn_vectors;
      size = sizeof(white_pawn_vectors);
    } else {
      *dst = black_pawn_vectors;
      size = sizeof(black_pawn_vectors);
    }

    // Hack hack hack...ignore pawn advances when checking captures
    if ( capture_only )
      size -= sizeof(ChessPieceVector);
	
    break;
  default:
    assert(0);
  }

  return size / sizeof(ChessPieceVector);
}
