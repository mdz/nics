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

#include "ChessMoveAlgebraic.h"
#include "ChessGame.h"
#include "AlgebraicParser.h"
#include <cctype>

ChessMoveAlgebraic::ChessMoveAlgebraic( const string &str,
					const ChessPosition &pos )
  throw (ChessMove::InvalidMove) {

  try {
    AlgebraicParser parser(str, pos);

    ChessPosition::Color pos_color = pos.get_active_color();
    ChessPiece::Color piece_color = pos_color == ChessPosition::White ?
      ChessPiece::White : ChessPiece::Black;
    
#ifdef DEBUG_ChessMove
    cerr << "Parsed move:" << endl
	 << "piece: " << (int)parser.piece << endl
	 << "origin rank: " << parser.origin_square.rank << endl
	 << "origin file: " << parser.origin_square.file << endl
	 << "square.rank: " << parser.dest_square.rank << endl
	 << "square.file: " << parser.dest_square.file << endl
	 << "capture: " << parser.capture << endl
	 << "check: " << parser.check << endl
	 << "promote: " << parser.promote << endl
	 << "castling: " << parser.castling << endl;
#endif
  
    ChessPiece piece( parser.piece, piece_color );
  
    // Make sure we got a valid piece
  
    switch ( piece.get_type() ) {
    case ChessPiece::Pawn:
    case ChessPiece::Bishop:
    case ChessPiece::Knight:
    case ChessPiece::Rook:
    case ChessPiece::Queen:
    case ChessPiece::King:
      break;
    default:
      throw InvalidMove("Unable to determine piece type");
    }
    
    //
    // Sanity checks...trust no one...not even the parser.
    //

    if ( parser.castling != ChessPosition::NoCastling
	 && ( parser.castling == ChessPosition::Kingside
	      || parser.castling == ChessPosition::Queenside ) )

      castling = parser.castling;

    if ( castling == ChessPosition::NoCastling ) {

      if ( parser.dest_square.file >= 1 && parser.dest_square.file <= 8 )
	end_x = parser.dest_square.file;
      else
	throw InvalidMove("Move coordinate out of range");

      if (  parser.dest_square.rank >= 1 && parser.dest_square.rank <= 8 )
	end_y = parser.dest_square.rank;
      else 
	throw InvalidMove("Move coordinate out of range");
    }

    // The origin square should only ever give a rank or a file...a
    // problem here would be indicative of a programmatic error in the
    // parser
    assert( !( parser.origin_square.rank && parser.origin_square.file ) );

    // Check active color
    assert( pos_color == ChessPosition::White
	    || pos_color == ChessPosition::Black );

    // Make sure we're not trying to capture our own piece, or make a
    // null move

    if ( castling == ChessPosition::NoCastling ) {
      ChessPiece end_piece = pos.get_piece_at( end_x, end_y );

      if ( end_piece.get_type() != ChessPiece::Empty &&
	   end_piece.get_color() == piece_color )
	throw InvalidMove("Attempt to move onto a piece of the same color");

      if ( start_x == end_x && start_y == end_y )
	throw InvalidMove("Attempt to move a piece onto the square it already occupies");
    }

    if ( castling == ChessPosition::NoCastling ) {
      //
      // Determine start square
      //
    
      int found = 0;
      ChessPositionCoords found_pieces[8];
      switch ( piece.get_type() ) {
	//
	// Pawn moves
	//
      case ChessPiece::Pawn:
	int dir, eighth_rank, second_rank;
      
	if ( pos_color == ChessPosition::White ) {
	  dir = 1;
	  second_rank = 2;
	} else {
	  dir = -1;
	  second_rank = 7;
	}

	eighth_rank = second_rank + ( 6 * dir );

	if ( end_y == ( second_rank ) || end_y == (second_rank - dir) )
	  throw InvalidMove("Attempt to move pawn onto player's first or second rank");

	if ( parser.capture ) {
	  //
	  // Pawn captures
	  //

	  start_x = parser.origin_square.file;
	  start_y = end_y - dir;

	  if ( !( start_x >= 1 && start_x <= 8 ) ) {
	    throw InvalidMove("Move coordinate out of range");
	  }

	} else {
	  //
	  // Pawn moves without capture
	  //

	  start_x = end_x;
	
	  if ( ( end_y == second_rank + ( dir * 2 ) )
	       && ( pos.get_piece_at( end_x, second_rank + dir )
		    .get_type() == ChessPiece::Empty )
	       && ( pos.get_piece_at( end_x, second_rank )
		    == ChessPiece( ChessPiece::Pawn, piece_color ) ) ) {
	    //
	    // Initial two-square advance
	    //

	    start_y = second_rank;

	  } else if ( pos.get_piece_at( end_x, end_y - dir )
		      == ChessPiece( ChessPiece::Pawn, piece_color )
		      && pos.get_piece_at( end_x, end_y ).get_type() 
		      == ChessPiece::Empty ) {
	    //
	    // Normal pawn move
	    //

	    start_y = end_y - dir;

	  } else {
	    throw InvalidMove("Invalid pawn move");
	  }
	}
      
	if ( pos.get_piece_at( start_x, start_y ).get_type()
	     != ChessPiece::Pawn ) {
	  throw InvalidMove("Invalid pawn move");
	}

	if ( end_y == eighth_rank )
	  if ( parser.promote == ChessPiece::Empty )
	    throw InvalidMove("Pawn move onto eighth rank without promotion");
	  else
	    promote = parser.promote;

	break;

      case ChessPiece::Knight:
      case ChessPiece::Bishop:
      case ChessPiece::Rook:
      case ChessPiece::Queen:
      case ChessPiece::King:
	found = pos.find_moves( &piece,
				1,
				end_x,
				end_y,
				parser.origin_square.file,
				parser.origin_square.rank,
				-1,
				0,
				1,
				0,
				found_pieces );
    
	if ( !found )
	  throw InvalidMove("No piece found capable of legally moving onto destination square");
    
	else if ( found > 1 ) {
	
	  throw InvalidMove("Algebraic move is ambiguous");

	} else {
      
	  // We found exactly one matching piece
	  start_x = found_pieces[0].x;
	  start_y = found_pieces[0].y;
	}
    
	break;

      default:
	// Should never happen, since we checked this case above
	assert(0);
      }
    }
    
    //      cerr << "start_x: " << start_x << endl;
    //      cerr << "start_y: " << start_y << endl;

    data.text = str;

  } catch (AlgebraicParser::ParseError e) {
    throw InvalidMove(string("Parse error: ") + e.what());
  }
}

ChessMoveAlgebraic::ChessMoveAlgebraic( const ChessMove &move,
					const ChessPosition &pos )
  throw (InvalidMove):
  ChessMove(move) {

  switch ( castling ) {

  case ChessPosition::Kingside:
    data.text = "O-O";break;

  case ChessPosition::Queenside:
    data.text = "O-O-O";break;

  case ChessPosition::AnyCastling:
    throw InvalidMove("ChessPosition::AnyCastling does not make sense in a move");

  case ChessPosition::NoCastling:
    // Non-castling moves
    
    const ChessPiece &moved_piece = pos.get_piece_at( start_x, start_y );
    
    if ( moved_piece.get_type() == ChessPiece::Empty )
      throw InvalidMove("Start square is empty");

    if ( moved_piece.get_type() != ChessPiece::Pawn )
      data.text += toupper(moved_piece.piece_char());
    
    // Determine whether a clarifier is needed
    ChessPositionCoords coords[8];
    if ( moved_piece.get_type() != ChessPiece::Pawn
	 && pos.find_moves( &moved_piece, 1,
			    end_x, end_y,
			    0, 0,
			    -1, 0, 1,
			    0,
			    coords ) > 1 ) {

      // XXX - this should calculate the minimum clarification needed
      data.text += 'a' + start_x - 1;
      data.text += '1' + start_y - 1;
    }

    // Capture
    int direction = pos.get_active_color() == ChessPosition::White ?
      1 : -1;
    if ( pos.get_piece_at( end_x , end_y ).get_type() != ChessPiece::Empty
	 ||
	 (end_x == pos.get_en_passant_x()
	  && end_y == pos.get_en_passant_y() + direction) ) {
      if ( moved_piece.get_type() == ChessPiece::Pawn )
	data.text += 'a' + start_x - 1;
      data.text += 'x';
    }

    data.text += 'a' + end_x - 1;
    data.text += '1' + end_y - 1;

    if ( promote != ChessPiece::Empty ) {
      data.text += '=';
      data.text += ChessPiece(promote, ChessPiece::White).piece_char();
    }
  }

  // Check?
  ChessGame game( pos );

  try {
    game.make_move( move );

    if ( game.current_position().get_status() == ChessPosition::Checkmate )
      data.text += '#';
    else if (game.current_position().in_check( ChessPosition::AnyColor ) )
      data.text += '+';
  } catch (ChessGame::IllegalMove) {
  }
  
}


string ChessMoveAlgebraic::text() const throw() {
  return data.text;
}
