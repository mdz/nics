SUBDIRS = . pgn

CXXFLAGS = @CXXFLAGS@ -I$(top_srcdir)/libmchess -g

noinst_PROGRAMS = declare_all pgntest

pgntest_SOURCES = pgntest.cc
pgntest_LDADD = ../../libmchess/libmchess.la

declare_all_SOURCES = declare_all.cc
declare_all_LDADD = ../../libmchess/libmchess.la

TESTS = ./declare_all ./pgntest.sh

EXTRA_DIST = pgntest.sh
