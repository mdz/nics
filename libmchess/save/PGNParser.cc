#include "PGNParser.h"

using namespace std;

PGNParser::PGNParser(istream& input_, ChessGame& game_):
  input(input_), game(game_) {
}
