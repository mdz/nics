# Makefile.in generated automatically by automake 1.5 from Makefile.am.

# Copyright 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001
# Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.



#CXX=g++-3.0

SHELL = /bin/sh

srcdir = .
top_srcdir = ..

prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include
pkgdatadir = $(datadir)/nics
pkglibdir = $(libdir)/nics
pkgincludedir = $(includedir)/nics
top_builddir = ..

ACLOCAL = ${SHELL} /home/mdz/data/src/mine/nics/missing --run aclocal
AUTOCONF = ${SHELL} /home/mdz/data/src/mine/nics/missing --run autoconf
AUTOMAKE = ${SHELL} /home/mdz/data/src/mine/nics/missing --run automake
AUTOHEADER = ${SHELL} /home/mdz/data/src/mine/nics/missing --run autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
INSTALL_HEADER = $(INSTALL_DATA)
transform = s,x,x,
NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
host_alias = 
host_triplet = i686-pc-linux-gnu
AMTAR = ${SHELL} /home/mdz/data/src/mine/nics/missing --run tar
AS = @AS@
AWK = mawk
CC = gcc
CXX = g++
DEPDIR = .deps
DLLTOOL = @DLLTOOL@
ECHO = /bin/echo
EXEEXT = 
INSTALL_STRIP_PROGRAM = ${SHELL} $(install_sh) -c -s
LEX = flex
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LN_S = ln -s
OBJDUMP = @OBJDUMP@
OBJEXT = o
PACKAGE = nics
RANLIB = ranlib
STRIP = strip
VERSION = 0.1
YACC = bison -y
am__include = include
am__quote = 
install_sh = /home/mdz/data/src/mine/nics/install-sh

CXXFLAGS = -g -O2 -D_REENTRANT -D_THREAD_SAFE -D_GNU_SOURCE -pg \
	-Wall -Woverloaded-virtual \
	-Werror-implicit-function-declaration

INCLUDES = -I/usr/include/postgresql -I$(top_srcdir)/libmchess -I/home/mdz/include

bin_PROGRAMS = nics
nics_SOURCES = NetworkSession.cc NetworkSession.h \
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
subdir = nics
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = $(top_builddir)/config.h
CONFIG_CLEAN_FILES =
bin_PROGRAMS = nics$(EXEEXT)
PROGRAMS = $(bin_PROGRAMS)

am_nics_OBJECTS = NetworkSession.$(OBJEXT) NetworkListener.$(OBJEXT) \
	UserSession.$(OBJEXT) UserSessionFICS.$(OBJEXT) \
	NICSDatabase.$(OBJEXT) PostgreSQL.$(OBJEXT) Game.$(OBJEXT) \
	Chess.$(OBJEXT) GameOffer.$(OBJEXT) Clock.$(OBJEXT) \
	Filter.$(OBJEXT) GameFilter.$(OBJEXT) \
	UserSessionFilter.$(OBJEXT) main.$(OBJEXT) NICSCore.$(OBJEXT) \
	NICSModule.$(OBJEXT) NICSNetwork.$(OBJEXT) NICSLog.$(OBJEXT) \
	NICSSessionManager.$(OBJEXT) NICSGameManager.$(OBJEXT) \
	NICSOfferManager.$(OBJEXT) NICSGuile.$(OBJEXT) \
	FilterParser.$(OBJEXT) FilterLexer.$(OBJEXT)
nics_OBJECTS = $(am_nics_OBJECTS)
nics_DEPENDENCIES = ../libmchess/libmchess.la

DEFS = -DHAVE_CONFIG_H
DEFAULT_INCLUDES =  -I. -I$(srcdir) -I$(top_builddir)
CPPFLAGS = 
LDFLAGS = 
LIBS = 
depcomp = $(SHELL) $(top_srcdir)/depcomp
DEP_FILES = $(DEPDIR)/Chess.Po $(DEPDIR)/Clock.Po \
	$(DEPDIR)/Filter.Po $(DEPDIR)/FilterLexer.Po \
	$(DEPDIR)/FilterParser.Po $(DEPDIR)/Game.Po \
	$(DEPDIR)/GameFilter.Po $(DEPDIR)/GameOffer.Po \
	$(DEPDIR)/NICSCore.Po $(DEPDIR)/NICSDatabase.Po \
	$(DEPDIR)/NICSGameManager.Po $(DEPDIR)/NICSGuile.Po \
	$(DEPDIR)/NICSLog.Po $(DEPDIR)/NICSModule.Po \
	$(DEPDIR)/NICSNetwork.Po \
	$(DEPDIR)/NICSOfferManager.Po \
	$(DEPDIR)/NICSSessionManager.Po \
	$(DEPDIR)/NetworkListener.Po \
	$(DEPDIR)/NetworkSession.Po $(DEPDIR)/PostgreSQL.Po \
	$(DEPDIR)/UserSession.Po \
	$(DEPDIR)/UserSessionFICS.Po \
	$(DEPDIR)/UserSessionFilter.Po $(DEPDIR)/main.Po
CXXCOMPILE = $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
LTCXXCOMPILE = $(LIBTOOL) --mode=compile $(CXX) $(DEFS) \
	$(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) \
	$(AM_CXXFLAGS) $(CXXFLAGS)
CXXLD = $(CXX)
CXXLINK = $(LIBTOOL) --mode=link $(CXXLD) $(AM_CXXFLAGS) $(CXXFLAGS) \
	$(AM_LDFLAGS) $(LDFLAGS) -o $@
LEXCOMPILE = $(LEX) $(LFLAGS) $(AM_LFLAGS)
LTLEXCOMPILE = $(LIBTOOL) --mode=compile $(LEX) $(LFLAGS) $(AM_LFLAGS)
YACCCOMPILE = $(YACC) $(YFLAGS) $(AM_YFLAGS)
LTYACCCOMPILE = $(LIBTOOL) --mode=compile $(YACC) $(YFLAGS) $(AM_YFLAGS)
CFLAGS = -g -O2
COMPILE = $(CC) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	$(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
LTCOMPILE = $(LIBTOOL) --mode=compile $(CC) $(DEFS) $(DEFAULT_INCLUDES) \
	$(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(LIBTOOL) --mode=link $(CCLD) $(AM_CFLAGS) $(CFLAGS) \
	$(AM_LDFLAGS) $(LDFLAGS) -o $@
DIST_SOURCES = $(nics_SOURCES)
DIST_COMMON = FilterLexer.cc FilterParser.cc Makefile.am Makefile.in
SOURCES = $(nics_SOURCES)

all: all-am

.SUFFIXES:
.SUFFIXES: .cc .ll .lo .o .obj .yy

mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs

distclean-libtool:
	-rm -f libtool
$(srcdir)/Makefile.in:  Makefile.am  $(top_srcdir)/configure.ac $(ACLOCAL_M4)
	cd $(top_srcdir) && \
	  $(AUTOMAKE) --foreign  nics/Makefile
Makefile:  $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) && \
	  CONFIG_HEADERS= CONFIG_LINKS= \
	  CONFIG_FILES=$(subdir)/$@ $(SHELL) ./config.status
install-binPROGRAMS: $(bin_PROGRAMS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	@list='$(bin_PROGRAMS)'; for p in $$list; do \
	  p1=`echo $$p|sed 's/$(EXEEXT)$$//'`; \
	  if test -f $$p \
	     || test -f $$p1 \
	  ; then \
	    f=`echo $$p1|sed '$(transform);s/$$/$(EXEEXT)/'`; \
	   echo " $(INSTALL_PROGRAM_ENV) $(LIBTOOL) --mode=install $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/$$f"; \
	   $(INSTALL_PROGRAM_ENV) $(LIBTOOL) --mode=install $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/$$f; \
	  else :; fi; \
	done

uninstall-binPROGRAMS:
	@$(NORMAL_UNINSTALL)
	@list='$(bin_PROGRAMS)'; for p in $$list; do \
	  f=`echo $$p|sed 's/$(EXEEXT)$$//;$(transform);s/$$/$(EXEEXT)/'`; \
	  echo " rm -f $(DESTDIR)$(bindir)/$$f"; \
	  rm -f $(DESTDIR)$(bindir)/$$f; \
	done

clean-binPROGRAMS:
	-test -z "$(bin_PROGRAMS)" || rm -f $(bin_PROGRAMS)
nics$(EXEEXT): $(nics_OBJECTS) $(nics_DEPENDENCIES) 
	@rm -f nics$(EXEEXT)
	$(CXXLINK) $(nics_LDFLAGS) $(nics_OBJECTS) $(nics_LDADD) $(LIBS)

mostlyclean-compile:
	-rm -f *.$(OBJEXT) core *.core

distclean-compile:
	-rm -f *.tab.c

include $(DEPDIR)/Chess.Po
include $(DEPDIR)/Clock.Po
include $(DEPDIR)/Filter.Po
include $(DEPDIR)/FilterLexer.Po
include $(DEPDIR)/FilterParser.Po
include $(DEPDIR)/Game.Po
include $(DEPDIR)/GameFilter.Po
include $(DEPDIR)/GameOffer.Po
include $(DEPDIR)/NICSCore.Po
include $(DEPDIR)/NICSDatabase.Po
include $(DEPDIR)/NICSGameManager.Po
include $(DEPDIR)/NICSGuile.Po
include $(DEPDIR)/NICSLog.Po
include $(DEPDIR)/NICSModule.Po
include $(DEPDIR)/NICSNetwork.Po
include $(DEPDIR)/NICSOfferManager.Po
include $(DEPDIR)/NICSSessionManager.Po
include $(DEPDIR)/NetworkListener.Po
include $(DEPDIR)/NetworkSession.Po
include $(DEPDIR)/PostgreSQL.Po
include $(DEPDIR)/UserSession.Po
include $(DEPDIR)/UserSessionFICS.Po
include $(DEPDIR)/UserSessionFilter.Po
include $(DEPDIR)/main.Po

distclean-depend:
	-rm -rf $(DEPDIR)

.cc.o:
	source='$<' object='$@' libtool=no \
	depfile='$(DEPDIR)/$*.Po' tmpdepfile='$(DEPDIR)/$*.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXXCOMPILE) -c -o $@ `test -f $< || echo '$(srcdir)/'`$<

.cc.obj:
	source='$<' object='$@' libtool=no \
	depfile='$(DEPDIR)/$*.Po' tmpdepfile='$(DEPDIR)/$*.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXXCOMPILE) -c -o $@ `cygpath -w $<`

.cc.lo:
	source='$<' object='$@' libtool=yes \
	depfile='$(DEPDIR)/$*.Plo' tmpdepfile='$(DEPDIR)/$*.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LTCXXCOMPILE) -c -o $@ `test -f $< || echo '$(srcdir)/'`$<
CXXDEPMODE = depmode=gcc

LEX_OUTPUT_ROOT = lex.yy
LEXLIB = -lfl

.ll.cc:
	$(LEXCOMPILE) $< && mv $(LEX_OUTPUT_ROOT).c $@

.yy.cc:
	$(YACCCOMPILE) $< && mv y.tab.c $@
	if test -f y.tab.h; then \
	  if cmp -s y.tab.h $*.h; then \
	    rm -f y.tab.h; \
	  else \
	    mv y.tab.h $*.h; \
	  fi; \
	fi
uninstall-info-am:

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP) $(TAGS_FILES)
	list='$(SOURCES) $(HEADERS) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	mkid -fID $$unique $(LISP)

TAGS:  $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) \
		$(TAGS_FILES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)$$unique$(LISP)$$tags" \
	  || etags $(ETAGS_ARGS) $$tags  $$unique $(LISP)

GTAGS:
	here=`CDPATH=: && cd $(top_builddir) && pwd` \
	  && cd $(top_srcdir) \
	  && gtags -i $(GTAGS_ARGS) $$here

distclean-tags:
	-rm -f TAGS ID GTAGS GRTAGS GSYMS GPATH

DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)

top_distdir = ..
distdir = $(top_distdir)/$(PACKAGE)-$(VERSION)

distdir: $(DISTFILES)
	@for file in $(DISTFILES); do \
	  if test -f $$file; then d=.; else d=$(srcdir); fi; \
	  dir=`echo "$$file" | sed -e 's,/[^/]*$$,,'`; \
	  if test "$$dir" != "$$file" && test "$$dir" != "."; then \
	    $(mkinstalldirs) "$(distdir)/$$dir"; \
	  fi; \
	  if test -d $$d/$$file; then \
	    cp -pR $$d/$$file $(distdir) \
	    || exit 1; \
	  else \
	    test -f $(distdir)/$$file \
	    || cp -p $$d/$$file $(distdir)/$$file \
	    || exit 1; \
	  fi; \
	done
check-am: all-am
check: check-am
all-am: Makefile $(PROGRAMS)

installdirs:
	$(mkinstalldirs) $(DESTDIR)$(bindir)

install: install-am
install-exec: install-exec-am
install-data: install-data-am
uninstall: uninstall-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	  `test -z '$(STRIP)' || \
	    echo "INSTALL_PROGRAM_ENV=STRIPPROG='$(STRIP)'"` install
mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES) stamp-h stamp-h[0-9]*

maintainer-clean-generic:
	@echo "This command is intended for maintainers to use"
	@echo "it deletes files that may require special tools to rebuild."
clean: clean-am

clean-am: clean-binPROGRAMS clean-generic clean-libtool mostlyclean-am

distclean: distclean-am

distclean-am: clean-am distclean-compile distclean-depend \
	distclean-generic distclean-libtool distclean-tags

dvi: dvi-am

dvi-am:

info: info-am

info-am:

install-data-am:

install-exec-am: install-binPROGRAMS

install-info: install-info-am

install-man:

installcheck-am:

maintainer-clean: maintainer-clean-am

maintainer-clean-am: distclean-am maintainer-clean-generic

mostlyclean: mostlyclean-am

mostlyclean-am: mostlyclean-compile mostlyclean-generic \
	mostlyclean-libtool

uninstall-am: uninstall-binPROGRAMS uninstall-info-am

.PHONY: GTAGS all all-am check check-am clean clean-binPROGRAMS \
	clean-generic clean-libtool distclean distclean-compile \
	distclean-depend distclean-generic distclean-libtool \
	distclean-tags distdir dvi dvi-am info info-am install \
	install-am install-binPROGRAMS install-data install-data-am \
	install-exec install-exec-am install-info install-info-am \
	install-man install-strip installcheck installcheck-am \
	installdirs maintainer-clean maintainer-clean-generic \
	mostlyclean mostlyclean-compile mostlyclean-generic \
	mostlyclean-libtool tags uninstall uninstall-am \
	uninstall-binPROGRAMS uninstall-info-am


FilterParser.cc FilterParserDefs.h: FilterParser.yy
	bison -d -p filter_yy FilterParser.yy
	mv FilterParser.yy.tab.c FilterParser.cc
	mv FilterParser.yy.tab.h FilterParserDefs.h
# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
