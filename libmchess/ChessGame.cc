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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_ASSERT_H
#include <assert.h>
#else
#define assert(x)
#endif

#include <iostream>

#include "PGNParser.h"
#include "ChessGame.h"

ChessGame::ChessGame() {
}

ChessGame::ChessGame( const ChessPosition &pos ):
  position(pos) {
}

void ChessGame::make_move( const ChessMove &move_in, int validate )
  throw(IllegalMove) {
  ChessPosition::Color active_color = position.get_active_color()
;
  ChessPosition old_position(position);
  ChessMove move(move_in);
  int start_x = move.get_start_x();
  int start_y = move.get_start_y();
  int end_x = move.get_end_x();
  int end_y = move.get_end_y();

  // Test for castling moves that aren't explicitly tagged as such.
  // This is needed because some move formats, like xboard's
  // "coordinate algebraic", don't always supply this information

  if ( start_x == 5 &&
       ( ( start_y == 8 && end_y == 8 && active_color == ChessPosition::Black )
	 ||
	 ( start_y == 1 && end_y == 1 && active_color == ChessPosition::White )
	 )
       && position.get_piece_at( start_x, start_y ).get_type() ==
       ChessPiece::King ) {

    if ( end_x == 7 )
      move.set_castling( ChessPosition::Kingside );
    else if ( end_x == 3 )
      move.set_castling( ChessPosition::Queenside );
  }

  if ( move.get_castling() != ChessPosition::NoCastling ) {
    int old_king_file = 5, new_king_file;
    int old_rook_file, new_rook_file;
    int back_rank;

    if ( active_color == ChessPosition::White )
      back_rank = 1;
    else
      back_rank = 8;

    // Are we allowed to castle?
    if ( !position.get_castling( active_color, move.get_castling() ) )
      throw IllegalMove("Attempt to castle when castling has been disallowed");

    if ( position.in_check( active_color ) )
      throw IllegalMove("Attempt to castle when in check");

    // XXX - variants
    switch ( move.get_castling() ) {
    case ChessPosition::Kingside:
      new_king_file = 7;
      old_rook_file = 8;
      new_rook_file = 6;
      break;
    case ChessPosition::Queenside:
      new_king_file = 3;
      old_rook_file = 1;
      new_rook_file = 4;
      break;
    default:
      // ChessMove will not allow this
      assert( 0 );
    }

    if ( position.in_check( position.get_active_color() ) )
      throw IllegalMove("Attempted to castle out of check");

    // new_rook_file also happens to be the square that the king
    // "passes through"
    if ( position.is_attacked( new_rook_file, back_rank,
			       position.get_active_color() ==
			       ChessPosition::White ?
			       ChessPosition::Black : ChessPosition::White ) )
      throw IllegalMove("Attempted to castle through check");

    // Castling into check should be caught by validate() at the end
    // of this function

    // Move king
    position.set_piece_at( new_king_file, back_rank,
			   position.get_piece_at( old_king_file, back_rank ) );
    position.set_piece_at( old_king_file, back_rank,
			   ChessPiece( ChessPiece::Empty ) );

    // Move rook
    position.set_piece_at( new_rook_file, back_rank,
			   position.get_piece_at( old_rook_file, back_rank ) );
    position.set_piece_at( old_rook_file, back_rank,
			   ChessPiece( ChessPiece::Empty ) );

    // No more castling
    position.set_castling( active_color, ChessPosition::Kingside, 0 );
    position.set_castling( active_color, ChessPosition::Queenside, 0 );
  } else {
    // Handle normal moves
    ChessPiece captured_piece( position.get_piece_at( end_x, end_y ) );
    ChessPiece moved_piece( position.get_piece_at( start_x,
						   start_y ) );
    int dir = (moved_piece.get_color()
	       == ChessPiece::White) ? 1 : -1;

    assert( moved_piece.get_type() != ChessPiece::Empty );

    if ( position.find_moves( &moved_piece, 1,
			      end_x, end_y,
			      start_x, start_y,
			      -1, 0, 0,
			      1, NULL ) < 1 )
      throw IllegalMove("Piece did not follow a valid piece vector");

#ifdef DEBUG_ChessGame
    cerr << "Making move: " << endl
	 << "start_x: " << start_x << endl
	 << "start_y: " << start_y << endl
	 << "end_x: " << end_x << endl
	 << "end_y: " << end_y << endl;
#endif

    if ( moved_piece.get_type() == ChessPiece::Pawn ) {
      int second_rank = active_color == ChessPosition::White ? 2 : 7;
      if ( start_x == end_x && captured_piece.get_type() != ChessPiece::Empty )
	throw IllegalMove("Pawn attempted to march over another piece");

      if ( ( (end_y - start_y) == 2 || (end_y - start_y) == -2 )
	   && start_y != second_rank )
	throw IllegalMove("Pawn attempted to advance two squares when not on second rank");

      if (  end_x != start_x
	   && position.get_en_passant_x() == 0
	   && captured_piece.get_type() == ChessPiece::Empty )
	throw IllegalMove("Pawn attempted to move diagonally with no capture");

      // Handle en passant captures (capture the pawn)
      if ( end_x == position.get_en_passant_x()
	   && end_y == position.get_en_passant_y() + dir )
	position.set_piece_at( end_x, end_y - dir,
			       ChessPiece( ChessPiece::Empty ) );
      
      // Handle pawn promotion (change the recently placed pawn into
      // another piece)
      if ( move.get_promote() != ChessPiece::Empty )
	position.set_piece_at( end_x, end_y,
			       ChessPiece( move.get_promote(),
					   moved_piece.get_color() ) );
      
      // Handle en passant setting for next move
      if ( ( moved_piece.get_color() == ChessPiece::White
	     && start_y == 2 && end_y == 4 )
	   ||
	   ( moved_piece.get_color() == ChessPiece::Black
	     && start_y == 7 && end_y == 5 ) )
	position.set_en_passant( end_x, end_y );
      else
	position.set_en_passant( 0, 0 );
    
    } else {
      position.set_en_passant( 0, 0 );
    }

    position.set_piece_at( start_x,
			   start_y,
			   ChessPiece( ChessPiece::Empty ) );

    if ( move.get_promote() == ChessPiece::Empty )
      // Don't undo promotion, done above
      position.set_piece_at( end_x,
			     end_y,
			     moved_piece );
    
    // No more castling if we move the king
    if ( moved_piece.get_type() == ChessPiece::King ) {
      position.set_castling( active_color, ChessPosition::Kingside, 0 );
      position.set_castling( active_color, ChessPosition::Queenside, 0 );
    }

    // No more castling if anything moves onto or off of the rook squares
    // XXX - variants
    if ( start_y == 1 && start_x == 8
	 || end_y == 1 && end_x == 8 )
      position.set_castling( ChessPosition::White,
			     ChessPosition::Kingside, 0 );
    if ( start_y == 1 && start_x == 1
	 || end_y == 1 && end_x == 1 )
      position.set_castling( ChessPosition::White,
			     ChessPosition::Queenside, 0 );

    if ( start_y == 8 && start_x == 8
	 || end_y == 8 && end_x == 8 )
      position.set_castling( ChessPosition::Black,
			     ChessPosition::Kingside, 0 );

    if (start_y == 8 && start_x == 1
	|| end_y == 8 && end_x == 1 )
      position.set_castling( ChessPosition::Black,
			     ChessPosition::Queenside, 0 );
    
    // Set halfmove clock
    if ( captured_piece.get_type() != ChessPiece::Empty ||
	 moved_piece.get_type() == ChessPiece::Pawn ) 
      position.set_halfmove_clock( 0 );
    else
      position.set_halfmove_clock( position.get_halfmove_clock() + 1 );
  
  }
  
  // Increment fullmove clock if appropriate
  position.increment_move();

    // Validate the position
  try {
    position.validate( validate );
  } catch (ChessPosition::InvalidPosition e) {
    // Roll back
    position = old_position;
    throw IllegalMove(string("Move resulted in invalid position: ")
		      + e.what());
  }

#ifdef DEBUG_ChessGame
  cerr << "Game " << this << " Position after move: " << endl;
  position.write_FEN( cerr );
#endif

  data.moves.insert(data.moves.end(), move);
}

void ChessGame::pgn_tag( const string &name, const string &value ) {
  if ( name == "Event" ) {
    event(value);
  } else if ( name == "Site" ) {
    site(value);
  } else if ( name == "Date" ) {
    date(value);
  } else if ( name == "Round" ) {
    round(value);
  } else if ( name == "White" ) {
    white(value);
  } else if ( name == "Black" ) {
    black(value);
  } else if ( name == "Result" ) {
    result(value);
  }

  // Throw away unknown tags
}

istream& operator>>(istream& is, ChessGame& game) {
  PGNParser parser(is);
  game = (ChessGame&)parser;
  return is;
}
