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

#include <cstdlib>
#include <iostream>
#include <strstream>
#include <cctype>
#include <stdexcept>

#include "ChessPosition.h"
#include "ChessMove.h"
#include "ChessGame.h"

// A FEN for the position at the start of a game
static const char *start_FEN =
"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

static const ChessPiece all_white_pieces[] = { ChessPiece( ChessPiece::Bishop,
							   ChessPiece::White ),
					       ChessPiece( ChessPiece::Knight,
							   ChessPiece::White ),
					       ChessPiece( ChessPiece::Rook,
							   ChessPiece::White ),
					       ChessPiece( ChessPiece::Queen,
							   ChessPiece::White ),
					       ChessPiece( ChessPiece::King,
							   ChessPiece::White ),
					       ChessPiece( ChessPiece::Pawn,
							   ChessPiece::White ) };

static const ChessPiece all_black_pieces[] = { ChessPiece( ChessPiece::Bishop,
							   ChessPiece::Black ),
					       ChessPiece( ChessPiece::Knight,
							   ChessPiece::Black ),
					       ChessPiece( ChessPiece::Rook,
							   ChessPiece::Black ),
					       ChessPiece( ChessPiece::Queen,
							   ChessPiece::Black ),
					       ChessPiece( ChessPiece::King,
							   ChessPiece::Black ),
					       ChessPiece( ChessPiece::Pawn,
							   ChessPiece::Black ) };

ChessPosition::ChessPosition() throw (ChessPosition::InvalidPosition):
  en_passant_x(0),
  en_passant_y(0),
  halfmove_clock(0),
  fullmove_number(1),
  check(0) {
  
  set_castling( White, Kingside, 1 );
  set_castling( White, Queenside, 1 );
  set_castling( Black, Kingside, 1 );
  set_castling( Black, Queenside, 1 );

  // There ought to be a faster way to initialize to the default
  // starting position, but this is a good self-test for now
  istrstream s( start_FEN );
  read_FEN( s );
}

const ChessPiece &ChessPosition::get_piece_at( const char *square ) const {
  return get_piece_at( square_x( square ), square_y( square ) );
}

int ChessPosition::get_castling( Color color,
				 Castling castling ) const {
  assert( color == White || color == Black );
  assert( castling == Kingside || castling == Queenside );

  if (color == White)
    switch ( castling ) {
    case Kingside:
      return castling_white_kingside;
    case Queenside:
      return castling_white_queenside;
    default:
      throw invalid_argument("castling");
    }
  else
    switch ( castling ) {
    case Kingside:
      return castling_black_kingside;
    case Queenside:
      return castling_black_queenside;
    default:
      throw invalid_argument("castling");
    }
}
  
int ChessPosition::get_en_passant_x() const {
  return en_passant_x;
}

int ChessPosition::get_en_passant_y() const {
  return en_passant_y;
}

int ChessPosition::get_halfmove_clock() const {
  return halfmove_clock;
}

int ChessPosition::get_fullmove_number() const {
  return fullmove_number;
}

void ChessPosition::write_FEN( ostream &output ) const {
  int x, y;
  int empty_ctr = 0;
  int any_castling = 0;

  //
  // Piece placement data
  //
  for ( y = 7 ; y >= 0 ; --y ) {
    for ( x = 0 ; x < 8 ; ++x ) {
      if ( board[x][y].get_type() == ChessPiece::Empty ) {
	++empty_ctr;
      } else {
	if ( empty_ctr != 0) { // End of a series of empty squares
	  output << empty_ctr;
	  empty_ctr = 0;
	}

	output << board[x][y].piece_char();
      }
    }

    if ( empty_ctr != 0 ) { // Empty rank
      output << empty_ctr;
      empty_ctr = 0;
    }

    if (y > 0)
      output << '/';
  }

  output << ' '; // Field separator

  //
  // Active color
  //
  if ( active_color == White ) {
    output << 'w';
  } else if ( active_color == Black ) {
    output << 'b';
  } else { // Should never happen
    throw InvalidPosition("Unable to determine active color");
  }

  output << ' '; // Field separator

  //
  // Castling availability
  //
  if ( get_castling( White, Kingside ) ) {
    any_castling = 1;
    output << 'K';
  }

  if ( get_castling( White, Queenside ) ) {
    any_castling = 1;
    output << 'Q';
  }
  
  if ( get_castling( Black, Kingside ) ) {
    any_castling = 1;
    output << 'k';
  }

  if ( get_castling( Black, Queenside ) ) {
    any_castling = 1;
    output << 'q';
  }

  if (!any_castling) {
    output << '-';
  }

  output << ' '; // Field separator

  //
  // En passant target square
  //
  if ( en_passant_x != 0 
       && en_passant_y != 0 ) {

    output << static_cast<char>('a' + en_passant_x - 1);
    output << static_cast<char>('1' + en_passant_y - 1);

  } else {
    output << '-';
  }

  output << ' ';

  //
  // Halfmove clock
  //
  output << halfmove_clock << ' ';

  //
  // Fullmove number
  //
  output << fullmove_number << endl;
}

void ChessPosition::set_piece_at( const char *square,
				  const ChessPiece &piece ) {
  set_piece_at( square_x( square ), square_y( square ), piece );
}

void ChessPosition::set_active_color( Color color ) {
  active_color = color;
}

void ChessPosition::set_castling( ChessPosition::Color color,
				  ChessPosition::Castling castling,
				  int on ) {
  assert( color == White || color == Black );
  assert( castling == Queenside || castling == Kingside );

  if ( color == White )
    switch ( castling ) {
    case Kingside:
      castling_white_kingside = on;
      break;
    case Queenside:
      castling_white_queenside = on;
      break;
    default:
      throw invalid_argument("castling");
    }
  else
    switch ( castling ) {
    case Kingside:
      castling_black_kingside = on;
      break;
    case Queenside:
      castling_black_queenside = on;
      break;
    default:
      throw invalid_argument("castling");
    }
}

void ChessPosition::set_en_passant( int x, int y ) {
  en_passant_x = x;
  en_passant_y = y;
}

void ChessPosition::set_halfmove_clock( int value ) {
  halfmove_clock = value;
}

void ChessPosition::set_fullmove_number( int value ) {
  fullmove_number = value;
}

void ChessPosition::read_FEN( istream &input )
  throw (ChessPosition::InvalidPosition) {
  char ch;
  string str;
  int num;

  for( int y = 7 ; y >= 0 ; --y ) {
    for( int x = 0 ; x < 8 ;  ) {
      ch = input.get();

      if ( isdigit( ch ) ) {
	int count = ch -  '1' + 1;
	x += count;
	//cerr << "Skipping " << count << " squares" << endl;
      } else {
	board[x][y].set( ch );
	//cerr << "Got piece (" << *current << "): "
	//     << board[x][y].get_type() << ','
	++x;
      }
    }

    if ( y > 0 ) {
      ch = input.get();
      if ( ch != '/' ) { // Bad FEN
	throw InvalidPosition("Expected slash at end of rank in FEN");
      }

    }
  }

  if ( input.get() != ' ' ) { // Check field separator
    throw InvalidPosition("Missing field separator in FEN");
  }

  ch = input.get();
  if ( ch == 'w' ) {
    set_active_color( White );
  } else if ( ch == 'b' ) {
    set_active_color( Black );
  } else {
    throw InvalidPosition("Couldn't determine active color from FEN");
  }

  ch = input.get();
  if ( ch != ' ' ) {
    throw InvalidPosition("Missing field separator in FEN");
  }

  input >> str;

  // Castling
  if ( str.find('K') != string::npos ) {
    set_castling( White, Kingside, 1 );
  }
  if ( str.find('Q') != string::npos ) {
    set_castling( White, Queenside, 1 );
  }

  if ( str.find('k') != string::npos ) {
    set_castling( Black,  Kingside, 1 );
  }
  if ( str.find('q') != string::npos ) {
    set_castling( Black, Queenside, 1 );
  }

  // En passant square
  input >> str;
  if ( str == "-" ) {
    set_en_passant( 0, 0 );
  } else if ( str.length() == 2 ) {
    
    set_en_passant( str[0] - 'a' + 1,
		    str[1] - '1' + 1 );
  } else {
//      cerr << "ChessPosition::read_FEN: invalid en passant square" << endl;
    throw InvalidPosition("Invalid en passant square in FEN");
  }

  input >> num;

  set_halfmove_clock( num );

  input >> num;

  set_fullmove_number( num );

  validate();
}

int ChessPosition::find_moves( const ChessPiece *pieces,
			       unsigned int num_pieces,
                               int origin_x, int origin_y,
                               int file,
                               int rank,
			       int direction,
			       int capture_only,
			       int check_legal,
			       int find_max,
			       ChessPositionCoords *found_pieces )
  const {

  // TODO: as a performance optimization, we should find a way to
  // special-case the search for any piece that can attack a given
  // square.  This is used on every move to test for check.  We should
  // be able to retrieve a single list of vectors, merging
  // queen/rook/bishop and minimizing the search space.  This function
  // gets called a lot.

  if ( direction < 0 )
    direction = -1; // Looking for a piece to move to origin
  else if ( direction > 0 )
    direction = 1; // Looking for a place for piece at origin to move to
  else
    throw invalid_argument("direction must be nonzero");

  int found = 0;
  const ChessPiece *piece = pieces;
  for( unsigned int i = 0 ; i < num_pieces ; ++i, ++piece ) {
    // Retrieve piece-movement vectors
    ChessPieceVector const *vectors = NULL;
    unsigned int numvec = piece->get_vectors( &vectors, capture_only );
    
#ifdef DEBUG_ChessPosition
    cerr << "Searching for a piece " << piece->get_type()
	 << " within position " << this << ": " << endl;
  write_FEN( cerr );
#endif

    int x, y;
    unsigned int vec, len;
    for ( vec = 0 ; vec < numvec ; ++vec ) {
      for (    // Start off one square in the right direction
	   x = origin_x + vectors[ vec ].x * direction,
	     y = origin_y + vectors[ vec ].y * direction,
	      len = 1 ;
	    
	   // Stay on the board, and within the length of the vector
	    x >= 1 && x <= 8 && y >= 1 && y <= 8
	      && len <= vectors[ vec ].length ;
	    
	   // Move in the opposite direction of the current vector
	   x += vectors[ vec ].x * direction,
	     y += vectors[ vec ].y * direction,
	     ++len ) {
	
	// It's somewhat inefficient to check the rank/file inside the
	// loop like this, but they're relatively rare in practice
	// (mostly for a clarifier in PGN notation)
	
	if ( file != 0 && file != x )
	  continue;
	if ( rank != 0 && rank != y )
	  continue;

	// For direction > 0, we are finding empty squares that the
	// piece on the origin can move to.  For direction < 0, we are
	// finding pieces that can move to the origin.

	int start_x, start_y, end_x, end_y;
	if ( direction > 0 ) {
	  start_x = origin_x;
	  start_y = origin_y;
	  end_x = x;
	  end_y = y;
	} else {
	  start_x = x;
	  start_y = y;
	  end_x = origin_x;
	  end_y = origin_y;
	}

	const ChessPiece &found_piece = get_piece_at( x, y );

	if ( direction < 0 && found_piece.get_type() == ChessPiece::Empty ) {

	  // Skip empty squares early, since most of them are
	  continue;

	} else if ( direction > 0
		    && found_piece.get_type() != ChessPiece::Empty
		    && found_piece.get_color() == piece->get_color() ) {

	  // Can't move onto or over our own piece
	  break;

	} else if ( direction > 0 || found_piece == *piece ) {

#ifdef DEBUG_ChessPosition
	  cerr << "(found candidate on " << x << ',' << y << ')' << endl;
#endif

	  if ( check_legal ) {
	    try {
#ifdef DEBUG_ChessPosition
	      cerr << "Trying move: " << start_x << ',' << start_y
		   << " => " << end_x << ',' << end_y << endl;
#endif
	      ChessMove move( start_x, start_y, end_x, end_y );
	      ChessGame game( *this );

	      // Avoid recursion
	      game.make_move(move, 0);

	    } catch (ChessGame::IllegalMove e) {
	      // We're only looking for legal moves, so skip
#ifdef DEBUG_ChessPosition
	      cerr << "Skipping illegal move: " << e.what() << endl;
#endif
	      continue;
	    }
	  }
	    
	  if ( found_pieces != NULL ) {
	    found_pieces[ found ].x = x;
	    found_pieces[ found ].y = y;
	  }
	  
	  ++found;
	  
	  // The caller has found out all that she wanted to know
	  if ( find_max > 0 && found >= find_max )
	    return found;

	  // Stop searching this vector (we found a piece)
	  if ( direction < 0 )
	    break;
	  
	} else { // direction > 0

	  // Stop searching this vector (there is a piece in the way)
	  break;

	}
      }
    }
  }

  return found;
}

// Run tests for check, mate, etc.
void ChessPosition::validate( int check_legal ) {
  ChessPiece::Color active_piece_color = get_active_color() == White ?
    ChessPiece::White : ChessPiece::Black;

#ifdef DEBUG_ChessPosition
  ChessPositionCoords coords;
  ChessPositionCoords *found = &coords;
#else
  ChessPositionCoords *found = NULL;
#endif

  // Initialize
  invalidate();

  // Are there any legal moves for the active player?
  int legal_move = 0;

  int white_king_x = 0, white_king_y = 0;
  int black_king_x = 0, black_king_y = 0;

  //
  // Find the king.  We could store its location always, but this
  // should run pretty fast.
  //

  for( int x = 1 ; x <= 8 ; ++x ) {
    for( int y = 1 ; y <= 8 ; ++y ) {
      const ChessPiece &piece = get_piece_at( x, y );

      if ( piece ==
	   ChessPiece( ChessPiece::King, ChessPiece::White ) ) {

//  	// XXX - variants
//  	if ( white_king_x > 0 )
//  	  throw InvalidPosition("Two white kings");

	white_king_x = x;
	white_king_y = y;
      } else if ( piece ==
		  ChessPiece( ChessPiece::King, ChessPiece::Black ) ) {

//  	// XXX - variants
//  	if ( black_king_x > 0 )
//  	  throw InvalidPosition("Two black kings");

	black_king_x = x;
	black_king_y = y;
      }

      if (check_legal && piece.get_type() != ChessPiece::Empty
	  && piece.get_color() == active_piece_color ) {
	// This will cause us to be called by find_moves while testing
	// legality, as it creates a game, whose position must then be
	// validated.  That way madness lies.  For this reason,
	// find_moves must ensure that ChessGame calls us with
	// check_legal = 0.

	// Can this piece legally move?
	if ( !legal_move
	     && find_moves( &piece, 1, x, y, 0, 0, 1, 0, 1, 1,
			    found ) ) {
	  
#ifdef DEBUG_ChessPosition
	  cerr << "Active player has a legal move: "
	       << coords.x << ',' << coords.y << endl;
#endif
	  
	  legal_move = 1;
	  
	// Cut the search short if we've answered all of the questions
	  if ( legal_move && white_king_x > 0 && black_king_x > 0 )
	    break;
	} else {
	  if ( white_king_x > 0 && black_king_x > 0 )
	    break;
	}
      }
    }
  }


  if ( white_king_x == 0 )
    throw InvalidPosition("Missing white king");
  if ( black_king_x == 0 )
    throw InvalidPosition("Missing black king");
  
#ifdef DEBUG_ChessPosition
  cerr << "Testing for check: kings are on "
       << white_king_x << ',' << white_king_y << " and "
       << black_king_x << ',' << black_king_y << endl;
#endif

  if ( is_attacked( white_king_x, white_king_y, Black ) ) {
    // Check
#ifdef DEBUG_ChessPosition
  cerr << "White is in check" << endl;
#endif

    check = 1;
    check_who = White;

    if (active_color == Black)
      throw InvalidPosition("White has moved into check");
  }

  if ( is_attacked( black_king_x, black_king_y, White ) ) {

#ifdef DEBUG_ChessPosition
    cerr << "Black is in check" << endl;
#endif

    if (check == 1)
      throw InvalidPosition("Both kings are in check");

    if (active_color == White)
      throw InvalidPosition("Black has moved into check");

    check = 1;
    check_who = Black;
  }

  if ( check_legal ) {
    if (!legal_move)
      if (check)
	status = Checkmate;
      else
	status = Stalemate;
    else
      status = InProgress;
  }

  return;
}

int ChessPosition::in_check( Color color ) const {
  if ( status == Unknown )
    throw logic_error("Position must be validated for in_check()");

  if ( color == AnyColor )
    return check;

  return ( check == 1 && check_who == color );
}

ChessPosition::Status ChessPosition::get_status() const {
  if ( status == Unknown )
    throw logic_error("Position must be validated for get_status()");

  return status;
}

int ChessPosition::is_attacked( int x, int y, Color color )
  const {

  const ChessPiece *pieces = color == White ? all_white_pieces
    : all_black_pieces;

  return find_moves( pieces, 6, x, y, 0, 0, -1, 1, 0, 1, NULL );
}
