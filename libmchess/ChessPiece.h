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

#ifndef _CHESSPIECE_H
#define _CHESSPIECE_H

struct ChessPieceVector {
  int x;
  int y;
  unsigned int length;
};

class ChessPiece {
  //
  // Public data types
  //
 public:
  enum Type { Empty, Pawn, Knight, Bishop, Rook, Queen, King, AnyType };
  enum Color { White, Black, AnyColor };

  //
  // Constructors and destructors
  //
 public:
  // For type = Empty, color is ignored
  ChessPiece(Type _type = Empty, Color _color = White):
    type( _type ), color( _color ) {};

  //
  // Accessors
  //
 public:
  Type get_type() const { return type; };
  Color get_color() const { return color; };
  // Return the character symbol for a piece (SAN notation)
  // lowercase p, n, b, etc. for black, uppercase for white
  char piece_char() const;

  // Set a pointer to an array of vectors for this piece, return the
  // number of vectors
  unsigned int get_vectors(const ChessPieceVector **dst,
			   int capture_only ) const;

  //
  // Mutators
  //
 public:
  void set_type(Type type_) { type = type_; };
  void set(char);
  void set_color(Color color_) { color = color_; };

  //
  // Operators
  //
public:
  inline friend int operator==( const ChessPiece &a, const ChessPiece &b ) {
    if ( a.type == ChessPiece::Empty && b.type == ChessPiece::Empty )
      return 1;
    
    return ( a.type == b.type && a.color == b.color );
  }


  //
  // Private data
  //
 private:
  Type type;
  Color color;
};

#endif /* !_CHESSPIECE_H */
