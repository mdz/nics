#CXX = g++-3.0
CXXFLAGS = @CXXFLAGS@ -Wall -Woverloaded-virtual -ansi -pedantic
lib_LTLIBRARIES = libmchess.la

libmchess_la_SOURCES = ChessPiece.cc ChessPiece.h \
			ChessMove.cc ChessMove.h \
			ChessMoveAlgebraic.cc ChessMoveAlgebraic.h \
			ChessMoveCoordinateAlgebraic.cc \
			ChessMoveCoordinateAlgebraic.h \
			ChessPosition.cc ChessPosition.h \
			AlgebraicParser.h AlgebraicParser.cc \
			alg_parse.yy \
			ChessMove.cc ChessMove.h \
			ChessGame.cc ChessGame.h \
			pgn_parse.yy PGNParser.h \
			PGNLexer.ll PGNLexer.h

EXTRA_DIST = pgn_parse.h alg_parse.h

YACC += -d -p $(patsubst %_parse.yy,%_yy,$(<F))
