CXXFLAGS = @CXXFLAGS@ -I$(top_srcdir)/nics -g

noinst_PROGRAMS = clock

clock_SOURCES = clock.cc
clock_LDADD = ../../nics/Clock.o -lccxx -lpthread

TESTS = ./clock

#EXTRA_DIST = clock.sh
