#CXX=g++-3.0
CXXFLAGS = @CXXFLAGS@ \
	-Wall -Woverloaded-virtual \
	-Werror-implicit-function-declaration
INCLUDES = -I/usr/include/postgresql -I$(top_srcdir)/libmchess -I/home/mdz/include

bin_PROGRAMS = nics
nics_SOURCES = 	NetworkSession.cc NetworkSession.h \
	NetworkListener.cc NetworkListener.h \
	UserSession.cc UserSession.h \
	UserSessionFICS.cc UserSessionFICS.h \
	NICSDatabase.cc NICSDatabase.h \
	PostgreSQL.cc PostgreSQL.h \
	Game.cc Game.h Chess.cc Chess.h \
	GameOffer.cc GameOffer.h \
	Clock.cc Clock.h \
	Filter.cc Filter.h \
	GameFilter.cc GameFilter.h \
	UserSessionFilter.cc UserSessionFilter.h \
	User.h \
	main.cc NICSCore.cc NICSCore.h \
	NICSModule.cc NICSModule.h \
	NICSNetwork.cc NICSNetwork.h \
	NICSLog.cc NICSLog.h \
	NICSSessionManager.cc NICSSessionManager.h \
	NICSGameManager.cc NICSGameManager.h \
	NICSOfferManager.cc NICSOfferManager.h \
	NICSGuile.cc NICSGuile.h \
	FilterParser.h FilterParser.yy \
	FilterLexer.h FilterLexer.ll
#	thread.cc
#	dmallocc.cc
#	UserSessionNICS.cc UserSessionNICS.h
nics_LDFLAGS = -L/home/mdz/opt/lib $(shell ccgnu-config --libs)
nics_LDADD = -lpq++ -lguile -lqthreads \
		../libmchess/libmchess.la
#-ldmallocth -ldmallocxx

EXTRA_DIST = FilterParserDefs.h

FilterParser.cc FilterParserDefs.h: FilterParser.yy
	bison -d -p filter_yy FilterParser.yy
	mv FilterParser.yy.tab.c FilterParser.cc
	mv FilterParser.yy.tab.h FilterParserDefs.h
