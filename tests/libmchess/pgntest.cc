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

#include <iostream>
#include <fstream>
#include <stdexcept>

#include "ChessGame.h"
#include "PGNParser.h"
#include "AlgebraicParser.h"

int main(int argc, char *argv[]) {
  for( int i = 1; i < argc ; ++i ) {
    ifstream file(argv[i]);

    while (!file.eof()) {
      try {
	ChessGame game;
	file >> game;

	if (file.eof())
	  return 0;

	game.current_position().write_FEN( cout );
	cout << "Final game status: ";
	switch (game.current_position().get_status()) {
	case ChessPosition::Unknown:
	  cout << "(unknown)";break;
	case ChessPosition::InProgress:
	  cout << "in progress";break;
	case ChessPosition::Checkmate:
	  cout << "checkmate";break;
	case ChessPosition::Stalemate:
	  cout << "stalemate";break;
	default:
	  cout << "(really unknown)";
	}
	cout << endl;
      } catch (PGNParser::ParseError e) {
	cerr << "Parse error: " << e.what() << endl;
	return 1;
      } catch (AlgebraicParser::ParseError e) {
	cerr << "Parse error: " << e.what() << endl;
	return 1;
      } catch (ChessGame::IllegalMove e) {
	cerr << "Illegal move: " << e.what() << endl;
	return 1;
      } catch (exception e) {
	cerr << "Unexpected exception: " << e.what() << endl;
	return 1;
      } catch (...) {
	cout << "Unrecognized exception" << endl;
	return 1;
      }
    }
  }

  return 0;
}
