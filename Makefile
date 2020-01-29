# Generated automatically from Makefile.in by configure.
# Makefile.in generated automatically by automake 1.4a from Makefile.am

# Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

# Copyright 1998,1999 Zanshin Inc.		     <http://www.zanshin.com/>

# The contents of this file are subject to the Zanshin Public License Version
# 1.0 (the "License"); you may not use this file except in compliance with the
# License.  You should have received a copy of the License with Latte; see the
# file COPYING.  You may also obtain a copy of the License at
# <http://www.zanshin.com/ZPL.html>.
# 
# Software distributed under the License is distributed on an "AS IS" basis,
# WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for
# the specific language governing rights and limitations under the License.
# 
# The Original Code is Latte.
# 
# The Initial Developer of the Original Code is Zanshin, Inc.


SHELL = /bin/sh

srcdir = .
top_srcdir = .
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

DESTDIR =

pkgdatadir = $(datadir)/latte
pkglibdir = $(libdir)/latte
pkgincludedir = $(includedir)/latte

top_builddir = .

ACLOCAL = aclocal
AUTOCONF = autoconf
AUTOMAKE = automake
AUTOHEADER = autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL_PROGRAM}
INSTALL_STRIP_FLAG =
transform = s,x,x,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
host_alias = i686-pc-linux-gnu
host_triplet = i686-pc-linux-gnu
AS = 
CC = gcc
CONFIGURE_ARGS =   --enable-floating-point
CXX = c++
DLLTOOL = 
EMACS = /usr/local/bin/emacs
LD = /usr/bin/ld
LIBLATTE_VERSION = 5:1:0
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LN_S = ln -s
MAKEINFO = makeinfo
NM = /usr/bin/nm -B
PACKAGE = latte
RANLIB = ranlib
RPM_REV = 0
USE_SYMBOL_UNDERSCORE = no
VERSION = 2.1
lispdir = $(prefix)/share/emacs/site-lisp

AUTOMAKE_OPTIONS = dist-shar 1.3

SUBDIRS = doc tests

bin_PROGRAMS = latte-html latte-text
lib_LTLIBRARIES = liblatte.la
pkginclude_HEADERS = latte.h latte-conf.h latte-deque.h latte-fstream.h 		     latte-iosfwd.h latte-log.h latte-stack.h latte-string.h 		     latte-vector.h memo.h mutable.h refcount.h restorer.h 		     shdeque.h shstring.h latte-html.h latte-text.h

pkgdata_DATA = html.latte text.latte standard.latte

lisp_LISP = latte.el

EXTRA_DIST = COPYING.LIB FlexLexer.h getopt.c getopt.h getopt1.c grammar.cxx 	grammar.tab.h grammar.y html-latte.pl html.latte latte.el 	standard.latte syntax.cxx syntax.lxx text.latte

#GETOPT_SRCS = getopt.c getopt1.c
GETOPT_SRCS = 
#GETOPT_OBJS = getopt.o getopt1.o
GETOPT_OBJS = 

BUILT_SOURCES = conf.cxx grammar.cxx syntax.cxx

latte_html_SOURCES = latte-html.cxx latte-html-cli.cxx $(GETOPT_SRCS)
latte_html_LDADD = liblatte.la -lstdc++

latte_text_SOURCES = latte-text.cxx latte-text-cli.cxx $(GETOPT_SRCS)
latte_text_LDADD = liblatte.la -lstdc++

liblatte_la_SOURCES = activation.cxx assignment.cxx boolean.cxx closure.cxx 		      conf.cxx definitions.cxx env.cxx fileloc.cxx 		      grammar.cxx group.cxx latte.cxx latte-log.cxx 		      latte-string.cxx list.cxx load.cxx memo.cxx nested.cxx 		      operator.cxx param.cxx quote.cxx reader.cxx 		      shstring.cxx str.cxx syntax.cxx tangible.cxx 		      varref.cxx visitor.cxx wsnode.cxx wstate.cxx

liblatte_la_LDFLAGS = -version-info 5:1:0

LATTE_URL = http://www.latte.org/

ETAGS_ARGS = --c++
#LDCONFIG_REQ = Requires: ldconfig
LDCONFIG_REQ = 
#LDCONFIG_RUN = /sbin/ldconfig
LDCONFIG_RUN = 
#STATIC_ERROR = LDFLAGS=$(LDFLAGS) requires configure --enable-static
#STATIC_TEST = case "$(LDFLAGS)" in 	      *-static*) 		echo >&2 "error: $(STATIC_ERROR)"; 		false;; 	      *) true;; 	      esac
STATIC_TEST = true
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = latte-conf.h
CONFIG_CLEAN_FILES = 
LTLIBRARIES =  $(lib_LTLIBRARIES)


DEFS = -DHAVE_CONFIG_H -I. -I$(srcdir) -I.
CPPFLAGS = 
LDFLAGS = 
LIBS =  -lm
liblatte_la_LIBADD = 
liblatte_la_OBJECTS =  activation.lo assignment.lo boolean.lo closure.lo \
conf.lo definitions.lo env.lo fileloc.lo grammar.lo group.lo latte.lo \
latte-log.lo latte-string.lo list.lo load.lo memo.lo nested.lo \
operator.lo param.lo quote.lo reader.lo shstring.lo str.lo syntax.lo \
tangible.lo varref.lo visitor.lo wsnode.lo wstate.lo
PROGRAMS =  $(bin_PROGRAMS)

#latte_html_OBJECTS =  latte-html.o latte-html-cli.o \
#getopt.o getopt1.o
latte_html_OBJECTS =  latte-html.o latte-html-cli.o
latte_html_DEPENDENCIES =  liblatte.la
latte_html_LDFLAGS = 
#latte_text_OBJECTS =  latte-text.o latte-text-cli.o \
#getopt.o getopt1.o
latte_text_OBJECTS =  latte-text.o latte-text-cli.o
latte_text_DEPENDENCIES =  liblatte.la
latte_text_LDFLAGS = 
CXXFLAGS = -O6
CXXCOMPILE = $(CXX) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
LTCXXCOMPILE = $(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
CXXLD = $(CXX)
CXXLINK = $(LIBTOOL) --mode=link $(CXXLD) $(AM_CXXFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@
CFLAGS = -g -O2
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
LTCOMPILE = $(LIBTOOL) --mode=compile $(CC) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(LIBTOOL) --mode=link $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@
LISP =  $(lisp_LISP)

ELCFILES =  latte.elc
DATA =  $(pkgdata_DATA)

HEADERS =  $(pkginclude_HEADERS)

DIST_COMMON =  README ./stamp-h.in AUTHORS COPYING COPYING.LIB ChangeLog \
INSTALL Makefile.am Makefile.in NEWS TODO acconfig.h aclocal.m4 \
config.guess config.sub configure configure.in elisp-comp install-sh \
latte-conf.h.in ltconfig ltmain.sh missing mkinstalldirs


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = gtar
GZIP_ENV = --best
SOURCES = $(liblatte_la_SOURCES) $(latte_html_SOURCES) $(latte_text_SOURCES)
OBJECTS = $(liblatte_la_OBJECTS) $(latte_html_OBJECTS) $(latte_text_OBJECTS)

all: all-redirect
.SUFFIXES:
.SUFFIXES: .S .c .cxx .el .elc .lo .o .s
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --gnu --include-deps Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status

$(ACLOCAL_M4):  configure.in 
	cd $(srcdir) && $(ACLOCAL)

config.status: $(srcdir)/configure $(CONFIG_STATUS_DEPENDENCIES)
	$(SHELL) ./config.status --recheck
$(srcdir)/configure: $(srcdir)/configure.in $(ACLOCAL_M4) $(CONFIGURE_DEPENDENCIES)
	cd $(srcdir) && $(AUTOCONF)

latte-conf.h: stamp-h
	@if test ! -f $@; then \
		rm -f stamp-h; \
		$(MAKE) stamp-h; \
	else :; fi
stamp-h: $(srcdir)/latte-conf.h.in $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES= CONFIG_HEADERS=latte-conf.h \
	     $(SHELL) ./config.status
	@echo timestamp > stamp-h 2> /dev/null
$(srcdir)/latte-conf.h.in: $(srcdir)/stamp-h.in
	@if test ! -f $@; then \
		rm -f $(srcdir)/stamp-h.in; \
		$(MAKE) $(srcdir)/stamp-h.in; \
	else :; fi
$(srcdir)/stamp-h.in: $(top_srcdir)/configure.in $(ACLOCAL_M4) acconfig.h
	cd $(top_srcdir) && $(AUTOHEADER)
	@echo timestamp > $(srcdir)/stamp-h.in 2> /dev/null

mostlyclean-hdr:

clean-hdr:

distclean-hdr:
	-rm -f latte-conf.h

maintainer-clean-hdr:

mostlyclean-libLTLIBRARIES:

clean-libLTLIBRARIES:
	-test -z "$(lib_LTLIBRARIES)" || rm -f $(lib_LTLIBRARIES)

distclean-libLTLIBRARIES:

maintainer-clean-libLTLIBRARIES:

install-libLTLIBRARIES: $(lib_LTLIBRARIES)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libdir)
	@list='$(lib_LTLIBRARIES)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo "$(LIBTOOL)  --mode=install $(INSTALL) $$p $(DESTDIR)$(libdir)/$$p"; \
	    $(LIBTOOL)  --mode=install $(INSTALL) $$p $(DESTDIR)$(libdir)/$$p; \
	  else :; fi; \
	done

uninstall-libLTLIBRARIES:
	@$(NORMAL_UNINSTALL)
	list='$(lib_LTLIBRARIES)'; for p in $$list; do \
	  $(LIBTOOL)  --mode=uninstall rm -f $(DESTDIR)$(libdir)/$$p; \
	done

.c.o:
	$(COMPILE) -c $<

.s.o:
	$(COMPILE) -c $<

.S.o:
	$(COMPILE) -c $<

mostlyclean-compile:
	-rm -f *.o core *.core

clean-compile:

distclean-compile:
	-rm -f *.tab.c

maintainer-clean-compile:

.c.lo:
	$(LIBTOOL) --mode=compile $(COMPILE) -c $<

.s.lo:
	$(LIBTOOL) --mode=compile $(COMPILE) -c $<

.S.lo:
	$(LIBTOOL) --mode=compile $(COMPILE) -c $<

mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs

distclean-libtool:

maintainer-clean-libtool:

liblatte.la: $(liblatte_la_OBJECTS) $(liblatte_la_DEPENDENCIES)
	$(CXXLINK) -rpath $(libdir) $(liblatte_la_LDFLAGS) $(liblatte_la_OBJECTS) $(liblatte_la_LIBADD) $(LIBS)

mostlyclean-binPROGRAMS:

clean-binPROGRAMS:
	-test -z "$(bin_PROGRAMS)" || rm -f $(bin_PROGRAMS)

distclean-binPROGRAMS:

maintainer-clean-binPROGRAMS:

install-binPROGRAMS: $(bin_PROGRAMS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	@list='$(bin_PROGRAMS)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo " $(LIBTOOL)  --mode=install $(INSTALL_PROGRAM) $(INSTALL_STRIP_FLAG) $$p $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`"; \
	    $(LIBTOOL)  --mode=install $(INSTALL_PROGRAM) $(INSTALL_STRIP_FLAG) $$p $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	  else :; fi; \
	done

uninstall-binPROGRAMS:
	@$(NORMAL_UNINSTALL)
	list='$(bin_PROGRAMS)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	done

latte-html: $(latte_html_OBJECTS) $(latte_html_DEPENDENCIES)
	@rm -f latte-html
	$(CXXLINK) $(latte_html_LDFLAGS) $(latte_html_OBJECTS) $(latte_html_LDADD) $(LIBS)

latte-text: $(latte_text_OBJECTS) $(latte_text_DEPENDENCIES)
	@rm -f latte-text
	$(CXXLINK) $(latte_text_LDFLAGS) $(latte_text_OBJECTS) $(latte_text_LDADD) $(LIBS)
.cxx.o:
	$(CXXCOMPILE) -c $<
.cxx.lo:
	$(LTCXXCOMPILE) -c $<

install-lispLISP: $(lisp_LISP) $(ELCFILES)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(lispdir)
	@list='$(lisp_LISP)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(lispdir)/$$p"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(lispdir)/$$p; \
	  if test -f $${p}c; then \
	    echo " $(INSTALL_DATA) $${p}c $(DESTDIR)$(lispdir)/$${p}c"; \
	    $(INSTALL_DATA) $${p}c $(DESTDIR)$(lispdir)/$${p}c; \
	  else : ; fi; \
	done

uninstall-lispLISP:
	@$(NORMAL_UNINSTALL)
	list='$(lisp_LISP)'; for p in $$list; do \
	  rm -f $(lispdir)/$$p $(DESTDIR)$(lispdir)/$${p}c; \
	done
.el.elc:
	@echo 'WARNING: Warnings can be ignored. :-)'
	if test $(EMACS) != no; then \
	  EMACS=$(EMACS) $(SHELL) $(srcdir)/elisp-comp $<; \
	fi

mostlyclean-lisp:

clean-lisp:
	-test -z "$(ELCFILES)" || rm -f $(ELCFILES)

distclean-lisp:

maintainer-clean-lisp:

install-pkgdataDATA: $(pkgdata_DATA)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)
	@list='$(pkgdata_DATA)'; for p in $$list; do \
	  if test -f $(srcdir)/$$p; then \
	    echo " $(INSTALL_DATA) $(srcdir)/$$p $(DESTDIR)$(pkgdatadir)/$$p"; \
	    $(INSTALL_DATA) $(srcdir)/$$p $(DESTDIR)$(pkgdatadir)/$$p; \
	  else if test -f $$p; then \
	    echo " $(INSTALL_DATA) $$p $(DESTDIR)$(pkgdatadir)/$$p"; \
	    $(INSTALL_DATA) $$p $(DESTDIR)$(pkgdatadir)/$$p; \
	  fi; fi; \
	done

uninstall-pkgdataDATA:
	@$(NORMAL_UNINSTALL)
	list='$(pkgdata_DATA)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(pkgdatadir)/$$p; \
	done

install-pkgincludeHEADERS: $(pkginclude_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(pkgincludedir)
	@list='$(pkginclude_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(pkgincludedir)/$$p"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(pkgincludedir)/$$p; \
	done

uninstall-pkgincludeHEADERS:
	@$(NORMAL_UNINSTALL)
	list='$(pkginclude_HEADERS)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(pkgincludedir)/$$p; \
	done

# This directory's subdirectories are mostly independent; you can cd
# into them and run `make' without going through this Makefile.
# To change the values of `make' variables: instead of editing Makefiles,
# (1) if the variable is set in `config.status', edit `config.status'
#     (which will cause the Makefiles to be regenerated when you run `make');
# (2) otherwise, pass the desired values on the `make' command line.



all-recursive install-data-recursive install-exec-recursive \
installdirs-recursive install-recursive uninstall-recursive  \
check-recursive installcheck-recursive info-recursive dvi-recursive:
	@set fnord $(MAKEFLAGS); amf=$$2; \
	dot_seen=no; \
	target=`echo $@ | sed s/-recursive//`; \
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  echo "Making $$target in $$subdir"; \
	  if test "$$subdir" = "."; then \
	    dot_seen=yes; \
	    local_target="$$target-am"; \
	  else \
	    local_target="$$target"; \
	  fi; \
	  (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
	   || case "$$amf" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac; \
	done; \
	if test "$$dot_seen" = "no"; then \
	  $(MAKE) $(AM_MAKEFLAGS) "$$target-am" || exit 1; \
	fi; test -z "$$fail"

mostlyclean-recursive clean-recursive distclean-recursive \
maintainer-clean-recursive:
	@set fnord $(MAKEFLAGS); amf=$$2; \
	dot_seen=no; \
	rev=''; list='$(SUBDIRS)'; for subdir in $$list; do \
	  rev="$$subdir $$rev"; \
	  test "$$subdir" = "." && dot_seen=yes; \
	done; \
	test "$$dot_seen" = "no" && rev=". $$rev"; \
	target=`echo $@ | sed s/-recursive//`; \
	for subdir in $$rev; do \
	  echo "Making $$target in $$subdir"; \
	  if test "$$subdir" = "."; then \
	    local_target="$$target-am"; \
	  else \
	    local_target="$$target"; \
	  fi; \
	  (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
	   || case "$$amf" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"
tags-recursive:
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) tags); \
	done

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP)
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	here=`pwd` && cd $(srcdir) \
	  && mkid -f$$here/ID $$unique $(LISP)

TAGS: tags-recursive $(HEADERS) $(SOURCES) latte-conf.h.in $(TAGS_DEPENDENCIES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SUBDIRS)'; for subdir in $$list; do \
   if test "$$subdir" = .; then :; else \
	    test -f $$subdir/TAGS && tags="$$tags -i $$here/$$subdir/TAGS"; \
   fi; \
	done; \
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)latte-conf.h.in$$unique$(LISP)$$tags" \
	  || (cd $(srcdir) && etags $(ETAGS_ARGS) $$tags latte-conf.h.in $$unique $(LISP) -o $$here/TAGS)

mostlyclean-tags:

clean-tags:

distclean-tags:
	-rm -f TAGS ID

maintainer-clean-tags:

distdir = $(PACKAGE)-$(VERSION)
top_distdir = $(distdir)

# This target untars the dist file and tries a VPATH configuration.  Then
# it guarantees that the distribution is self-contained by making another
# tarfile.
distcheck: dist
	-rm -rf $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) zxf $(distdir).tar.gz
	mkdir $(distdir)/=build
	mkdir $(distdir)/=inst
	dc_install_base=`cd $(distdir)/=inst && pwd`; \
	cd $(distdir)/=build \
	  && ../configure --srcdir=.. --prefix=$$dc_install_base \
	  && $(MAKE) $(AM_MAKEFLAGS) \
	  && $(MAKE) $(AM_MAKEFLAGS) dvi \
	  && $(MAKE) $(AM_MAKEFLAGS) check \
	  && $(MAKE) $(AM_MAKEFLAGS) install \
	  && $(MAKE) $(AM_MAKEFLAGS) installcheck \
	  && $(MAKE) $(AM_MAKEFLAGS) dist
	-rm -rf $(distdir)
	@banner="$(distdir).tar.gz is ready for distribution"; \
	dashes=`echo "$$banner" | sed s/./=/g`; \
	echo "$$dashes"; \
	echo "$$banner"; \
	echo "$$dashes"
dist: distdir
	-chmod -R a+r $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) chozf $(distdir).tar.gz $(distdir)
	-rm -rf $(distdir)
dist-shar: distdir
	-chmod -R a+r $(distdir)
	shar $(distdir) | gzip > $(distdir).shar.gz
	-rm -rf $(distdir)
dist-all: distdir
	-chmod -R a+r $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) chozf $(distdir).tar.gz $(distdir)
	shar $(distdir) | gzip > $(distdir).shar.gz
	-rm -rf $(distdir)
distdir: $(DISTFILES)
	-rm -rf $(distdir)
	mkdir $(distdir)
	-chmod 777 $(distdir)
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -pr $$d/$$file $(distdir)/$$file; \
	  else \
	    test -f $(distdir)/$$file \
	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	    || cp -p $$d/$$file $(distdir)/$$file || :; \
	  fi; \
	done
	for subdir in $(SUBDIRS); do \
	  if test "$$subdir" = .; then :; else \
	    test -d $(distdir)/$$subdir \
	    || mkdir $(distdir)/$$subdir \
	    || exit 1; \
	    chmod 777 $(distdir)/$$subdir; \
	    (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) top_distdir=../$(distdir) distdir=../$(distdir)/$$subdir distdir) \
	      || exit 1; \
	  fi; \
	done
	$(MAKE) $(AM_MAKEFLAGS) top_distdir="$(top_distdir)" distdir="$(distdir)" dist-hook
activation.lo activation.o : activation.cxx latte.h latte-conf.h \
	shdeque.h latte-deque.h refcount.h mutable.h shstring.h \
	latte-string.h memo.h latte-stack.h latte-vector.h FlexLexer.h
assignment.lo assignment.o : assignment.cxx latte.h latte-conf.h \
	shdeque.h latte-deque.h refcount.h mutable.h shstring.h \
	latte-string.h memo.h latte-stack.h latte-vector.h FlexLexer.h
boolean.lo boolean.o : boolean.cxx latte.h latte-conf.h shdeque.h \
	latte-deque.h refcount.h mutable.h shstring.h latte-string.h \
	memo.h latte-stack.h latte-vector.h FlexLexer.h
closure.lo closure.o : closure.cxx latte.h latte-conf.h shdeque.h \
	latte-deque.h refcount.h mutable.h shstring.h latte-string.h \
	memo.h latte-stack.h latte-vector.h FlexLexer.h
conf.lo conf.o : conf.cxx
definitions.lo definitions.o : definitions.cxx latte.h latte-conf.h \
	shdeque.h latte-deque.h refcount.h mutable.h shstring.h \
	latte-string.h memo.h latte-stack.h latte-vector.h FlexLexer.h \
	latte-log.h latte-fstream.h
env.lo env.o : env.cxx latte.h latte-conf.h shdeque.h latte-deque.h \
	refcount.h mutable.h shstring.h latte-string.h memo.h \
	latte-stack.h latte-vector.h FlexLexer.h
fileloc.lo fileloc.o : fileloc.cxx latte.h latte-conf.h shdeque.h \
	latte-deque.h refcount.h mutable.h shstring.h latte-string.h \
	memo.h latte-stack.h latte-vector.h FlexLexer.h
grammar.lo grammar.o : grammar.cxx latte.h latte-conf.h shdeque.h \
	latte-deque.h refcount.h mutable.h shstring.h latte-string.h \
	memo.h latte-stack.h latte-vector.h FlexLexer.h
group.lo group.o : group.cxx latte.h latte-conf.h shdeque.h \
	latte-deque.h refcount.h mutable.h shstring.h latte-string.h \
	memo.h latte-stack.h latte-vector.h FlexLexer.h
latte-html-cli.o: latte-html-cli.cxx latte-html.h latte.h latte-conf.h \
	shdeque.h latte-deque.h refcount.h mutable.h shstring.h \
	latte-string.h memo.h latte-stack.h latte-vector.h FlexLexer.h \
	latte-log.h latte-fstream.h getopt.h
latte-html.o: latte-html.cxx latte-html.h latte.h latte-conf.h shdeque.h \
	latte-deque.h refcount.h mutable.h shstring.h latte-string.h \
	memo.h latte-stack.h latte-vector.h FlexLexer.h latte-log.h \
	restorer.h getopt.h
latte-log.lo latte-log.o : latte-log.cxx latte-log.h latte.h \
	latte-conf.h shdeque.h latte-deque.h refcount.h mutable.h \
	shstring.h latte-string.h memo.h latte-stack.h latte-vector.h \
	FlexLexer.h
latte-string.lo latte-string.o : latte-string.cxx latte-string.h \
	latte-conf.h
latte-text-cli.o: latte-text-cli.cxx latte-text.h latte.h latte-conf.h \
	shdeque.h latte-deque.h refcount.h mutable.h shstring.h \
	latte-string.h memo.h latte-stack.h latte-vector.h FlexLexer.h \
	latte-log.h latte-fstream.h getopt.h
latte-text.o: latte-text.cxx latte-text.h latte.h latte-conf.h shdeque.h \
	latte-deque.h refcount.h mutable.h shstring.h latte-string.h \
	memo.h latte-stack.h latte-vector.h FlexLexer.h latte-log.h \
	restorer.h getopt.h
latte.lo latte.o : latte.cxx latte.h latte-conf.h shdeque.h \
	latte-deque.h refcount.h mutable.h shstring.h latte-string.h \
	memo.h latte-stack.h latte-vector.h FlexLexer.h latte-log.h \
	restorer.h
list.lo list.o : list.cxx latte.h latte-conf.h shdeque.h latte-deque.h \
	refcount.h mutable.h shstring.h latte-string.h memo.h \
	latte-stack.h latte-vector.h FlexLexer.h
load.lo load.o : load.cxx latte.h latte-conf.h shdeque.h latte-deque.h \
	refcount.h mutable.h shstring.h latte-string.h memo.h \
	latte-stack.h latte-vector.h FlexLexer.h latte-fstream.h \
	getopt.h
memo.lo memo.o : memo.cxx memo.h
nested.lo nested.o : nested.cxx latte.h latte-conf.h shdeque.h \
	latte-deque.h refcount.h mutable.h shstring.h latte-string.h \
	memo.h latte-stack.h latte-vector.h FlexLexer.h
operator.lo operator.o : operator.cxx latte.h latte-conf.h shdeque.h \
	latte-deque.h refcount.h mutable.h shstring.h latte-string.h \
	memo.h latte-stack.h latte-vector.h FlexLexer.h
param.lo param.o : param.cxx latte.h latte-conf.h shdeque.h \
	latte-deque.h refcount.h mutable.h shstring.h latte-string.h \
	memo.h latte-stack.h latte-vector.h FlexLexer.h
quote.lo quote.o : quote.cxx latte.h latte-conf.h shdeque.h \
	latte-deque.h refcount.h mutable.h shstring.h latte-string.h \
	memo.h latte-stack.h latte-vector.h FlexLexer.h
reader.lo reader.o : reader.cxx latte.h latte-conf.h shdeque.h \
	latte-deque.h refcount.h mutable.h shstring.h latte-string.h \
	memo.h latte-stack.h latte-vector.h FlexLexer.h
shstring.lo shstring.o : shstring.cxx shstring.h latte-conf.h \
	latte-string.h refcount.h mutable.h
str.lo str.o : str.cxx latte.h latte-conf.h shdeque.h latte-deque.h \
	refcount.h mutable.h shstring.h latte-string.h memo.h \
	latte-stack.h latte-vector.h FlexLexer.h
syntax.lo syntax.o : syntax.cxx latte-iosfwd.h getopt.h FlexLexer.h \
	latte.h latte-conf.h shdeque.h latte-deque.h refcount.h \
	mutable.h shstring.h latte-string.h memo.h latte-stack.h \
	latte-vector.h grammar.tab.h
tangible.lo tangible.o : tangible.cxx latte.h latte-conf.h shdeque.h \
	latte-deque.h refcount.h mutable.h shstring.h latte-string.h \
	memo.h latte-stack.h latte-vector.h FlexLexer.h
varref.lo varref.o : varref.cxx latte.h latte-conf.h shdeque.h \
	latte-deque.h refcount.h mutable.h shstring.h latte-string.h \
	memo.h latte-stack.h latte-vector.h FlexLexer.h latte-log.h
visitor.lo visitor.o : visitor.cxx latte.h latte-conf.h shdeque.h \
	latte-deque.h refcount.h mutable.h shstring.h latte-string.h \
	memo.h latte-stack.h latte-vector.h FlexLexer.h
wsnode.lo wsnode.o : wsnode.cxx latte.h latte-conf.h shdeque.h \
	latte-deque.h refcount.h mutable.h shstring.h latte-string.h \
	memo.h latte-stack.h latte-vector.h FlexLexer.h
wstate.lo wstate.o : wstate.cxx latte.h latte-conf.h shdeque.h \
	latte-deque.h refcount.h mutable.h shstring.h latte-string.h \
	memo.h latte-stack.h latte-vector.h FlexLexer.h

info-am:
info: info-recursive
dvi-am:
dvi: dvi-recursive
check-am: all-am
check: check-recursive
installcheck-am:
installcheck: installcheck-recursive
all-recursive-am: latte-conf.h
	$(MAKE) $(AM_MAKEFLAGS) all-recursive

install-exec-am: install-libLTLIBRARIES install-binPROGRAMS
install-exec: install-exec-recursive

install-data-am: install-lispLISP install-pkgdataDATA \
		install-pkgincludeHEADERS
install-data: install-data-recursive

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-recursive
uninstall-am: uninstall-libLTLIBRARIES uninstall-binPROGRAMS \
		uninstall-lispLISP uninstall-pkgdataDATA \
		uninstall-pkgincludeHEADERS
uninstall: uninstall-recursive
all-am: Makefile $(LTLIBRARIES) $(PROGRAMS) $(LISP) $(ELCFILES) $(DATA) \
		$(HEADERS) latte-conf.h
all-redirect: all-recursive-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) INSTALL_STRIP_FLAG=-s install
installdirs: installdirs-recursive
installdirs-am:
	$(mkinstalldirs)  $(DESTDIR)$(libdir) $(DESTDIR)$(bindir) \
		$(DESTDIR)$(lispdir) $(DESTDIR)$(pkgdatadir) \
		$(DESTDIR)$(pkgincludedir)


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
	-test -z "$(BUILT_SOURCES)" || rm -f $(BUILT_SOURCES)
mostlyclean-am:  mostlyclean-hdr mostlyclean-libLTLIBRARIES \
		mostlyclean-compile mostlyclean-libtool \
		mostlyclean-binPROGRAMS mostlyclean-lisp \
		mostlyclean-tags mostlyclean-generic

mostlyclean: mostlyclean-recursive

clean-am:  clean-hdr clean-libLTLIBRARIES clean-compile clean-libtool \
		clean-binPROGRAMS clean-lisp clean-tags clean-generic \
		mostlyclean-am

clean: clean-recursive

distclean-am:  distclean-hdr distclean-libLTLIBRARIES distclean-compile \
		distclean-libtool distclean-binPROGRAMS distclean-lisp \
		distclean-tags distclean-generic clean-am
	-rm -f libtool

distclean: distclean-recursive
	-rm -f config.status

maintainer-clean-am:  maintainer-clean-hdr \
		maintainer-clean-libLTLIBRARIES \
		maintainer-clean-compile maintainer-clean-libtool \
		maintainer-clean-binPROGRAMS maintainer-clean-lisp \
		maintainer-clean-tags maintainer-clean-generic \
		distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-recursive
	-rm -f config.status

.PHONY: mostlyclean-hdr distclean-hdr clean-hdr maintainer-clean-hdr \
mostlyclean-libLTLIBRARIES distclean-libLTLIBRARIES \
clean-libLTLIBRARIES maintainer-clean-libLTLIBRARIES \
uninstall-libLTLIBRARIES install-libLTLIBRARIES mostlyclean-compile \
distclean-compile clean-compile maintainer-clean-compile \
mostlyclean-libtool distclean-libtool clean-libtool \
maintainer-clean-libtool mostlyclean-binPROGRAMS distclean-binPROGRAMS \
clean-binPROGRAMS maintainer-clean-binPROGRAMS uninstall-binPROGRAMS \
install-binPROGRAMS uninstall-lispLISP install-lispLISP \
mostlyclean-lisp distclean-lisp clean-lisp maintainer-clean-lisp \
uninstall-pkgdataDATA install-pkgdataDATA uninstall-pkgincludeHEADERS \
install-pkgincludeHEADERS install-data-recursive \
uninstall-data-recursive install-exec-recursive \
uninstall-exec-recursive installdirs-recursive uninstalldirs-recursive \
all-recursive check-recursive installcheck-recursive info-recursive \
dvi-recursive mostlyclean-recursive distclean-recursive clean-recursive \
maintainer-clean-recursive tags tags-recursive mostlyclean-tags \
distclean-tags clean-tags maintainer-clean-tags distdir info-am info \
dvi-am dvi check check-am installcheck-am installcheck all-recursive-am \
install-exec-am install-exec install-data-am install-data install-am \
install uninstall-am uninstall all-redirect all-am all installdirs-am \
installdirs mostlyclean-generic distclean-generic clean-generic \
maintainer-clean-generic clean mostlyclean distclean maintainer-clean


conf.cxx: config.status
	-rm -f $@
	(echo '// Automatically generated by Latte makefile'; \
	 echo 'extern const char latte_pkgdatadir[] = "$(pkgdatadir)";' ; \
	 echo 'extern const char latte_version[] = "$(VERSION)";'; \
	 echo 'extern const char latte_url[] = "$(LATTE_URL)";') > $@

grammar.tab.h grammar.cxx: grammar.y
	-rm -f grammar.tab.c grammar.tab.h grammar.cxx
	bison -d -p latte $<
	mv grammar.tab.c grammar.cxx
	@rm -f grammar.tab.c

syntax.cxx: syntax.lxx
	-rm -f $@ $@.tmp
	flex -B -o$@ $<
	sed 's/^class istream;/#include <latte-iosfwd.h>/' $@ > $@.tmp
	mv $@.tmp $@

dist-hook:
	@echo " "
	@echo "  If this is a real distribution, did you remember to:"
	@echo "    - Update VERSION (in AM_INIT_AUTOMAKE in configure.in)?"
	@echo "    - Update LIBLATTE_VERSION (in configure.in)?"
	@echo "    - Reset RPM_REV (in configure.in)?"
	@echo "    - Update NEWS?"
	@echo "    - Make sure conf.cxx is up to date?"
	@echo "    - make distcheck?"
	@echo "    - Update ChangeLog?"
	@echo "    - cvs tag the sources?"
	@echo "    - Update the Latte website?"
	@echo "    - Notify latte mailing list subscribers?"
	@echo " "

# Tweak the source for other platforms

win-src: distdir
	cd $(distdir); \
	cr=`echo "" | tr '\012' '\015'`; \
	for name in standard html; \
	do \
	  (sed 's/$$/'"$$cr"'/' < $$name.latte > $$name.lat \
		&& rm $$name.latte); \
	done; \
	for file in AUTHORS COPYING NEWS README TODO; \
	do \
	  (sed 's/$$/'"$$cr"'/' < $$file > $$file.win \
		&& mv $$file.win $$file); \
	done; \
	cd doc; \
	for name in latte liblatte; \
	do \
	  (sed 's/$$/'"$$cr"'/' < $$name.info > $$name.inf \
		&& rm $$name.info); \
	done; \
	cd ../tests; \
	for file in *.exp; \
	do \
	  (sed 's/$$/'"$$cr"'/' < $$file > $$file.win \
		&& mv $$file.win $$file); \
	done

win-dist: latte.zip

latte.zip: win-distdir 
	-rm -f latte.zip
	zip -r latte latte
	-rm -rf latte

win-distdir: win-src latte-$(VERSION).exe
	-rm -rf latte
	mkdir latte
	rm -f latte/latte.exe; ln latte-$(VERSION).exe latte/latte.exe
	for file in standard.lat html.lat; \
	do \
	  rm -f latte/$$file; ln $(distdir)/$$file latte/$$file; \
	done
	for file in AUTHORS COPYING NEWS README TODO; \
	do \
	  rm -f latte/$$file.txt; ln $(distdir)/$$file latte/$$file.txt; \
	done
	rm -f latte/latte.inf; ln $(distdir)/doc/latte.inf latte/latte.inf
	rm -f latte/liblatte.inf; ln $(distdir)/doc/liblatte.inf latte/liblatte.inf
	-rm -rf $(distdir)

mac-src: distdir
	cd $(distdir); \
	for file in standard.latte html.latte; \
	do \
	  (tr '\012' '\015' < $$file > $$file.mac && mv $$file.mac $$file); \
	done; \
	cd tests; \
	for file in *.exp; \
	do \
	  (tr '\012' '\015' < $$file > $$file.mac && mv $$file.mac $$file); \
	done

# Some variables needed by latte.spec are not available at configure time.

$(PACKAGE).spec: Makefile $(PACKAGE).spec.in lib$(PACKAGE).la
	@$(STATIC_TEST)
	-rm -f $@
	@. $(top_builddir)/lib$(PACKAGE).la ; \
	echo "sed < $@.in > $@" ; \
	sed < $@.in > $@ \
		-e 's%#PACKAGE#%$(PACKAGE)%g' \
		-e 's%#VERSION#%$(VERSION)%g' \
		-e 's%#RPM_REV#%$(RPM_REV)%g' \
		-e 's%#LATTE_URL#%$(LATTE_URL)%g' \
		-e 's%#LDFLAGS#%$(LDFLAGS)%g' \
		-e 's%#LDCONFIG_REQ#%$(LDCONFIG_REQ)%g' \
		-e 's%#LDCONFIG_RUN#%$(LDCONFIG_RUN)%g' \
		-e 's%#prefix#%$(prefix)%g' \
		-e 's%#libdir#%$(libdir)%g' \
		-e 's%#infodir#%$(infodir)%g' \
		-e 's%#pkgdatadir#%$(pkgdatadir)%g' \
		-e 's%#pkglibdir#%$(pkglibdir)%g' \
		-e 's%#pkgincludedir#%$(pkgincludedir)%g' \
		-e 's%#CONFIGURE_ARGS#%$(CONFIGURE_ARGS)%g' \
		-e "s%#LIBLATTE_A#%$(libdir)/$$old_library%g" \
		-e h \
		`for f in $(bin_PROGRAMS); do \
		  echo " -e s%#BINPROGRAMS#%$(bindir)/$$f%gp"; \
		  echo " -e g"; \
		 done` \
		-e '/#BINPROGRAMS#/d' \
		`for f in $(pkginclude_HEADERS); do \
		  echo " -e s%#PKGINCLUDES#%$(pkgincludedir)/$$f%gp"; \
		  echo " -e g"; \
		 done` \
		-e '/#PKGINCLUDES#/d' \
		`for f in $(lib_LTLIBRARIES); do \
		  echo " -e s%#LIBLATTE_LA#%$(libdir)/$$f%gp"; \
		  echo " -e g"; \
		 done` \
		-e '/#LIBLATTE_LA#/d' \
		`for f in $$library_names; do \
		  echo " -e s%#LIBLATTE_SO#%$(libdir)/$$f%gp"; \
		  echo " -e g"; \
		 done` \
		-e '/#LIBLATTE_SO#/d' \
		`for f in $(lisp_LISP); do \
		  echo " -e s%#LATTELISP#%$(lispdir)/$$f%gp"; \
		  echo " -e g"; \
		 done` \
		-e '/#LATTELISP#/d' \
		`for f in $(pkgdata_DATA); do \
		  echo " -e s%#PKGDATA#%$(pkgdatadir)/$$f%gp"; \
		  echo " -e g"; \
		 done` \
		-e '/#PKGDATA#/d'

# RPM won't build the .src.rpm without verifying that the binary .rpm can
# also be successfully compiled.  So the binaries depend on the source,
# and all the build rules for both are under the source target.

$(PACKAGE)-$(VERSION)-$(RPM_REV).i386.rpm \
$(PACKAGE)-$(VERSION)-$(RPM_REV).i686.rpm \
$(PACKAGE)-$(VERSION)-$(RPM_REV).ppc.rpm \
$(PACKAGE)-$(VERSION)-$(RPM_REV).sparc.rpm \
$(PACKAGE)-$(VERSION)-$(RPM_REV).alpha.rpm : $(PACKAGE)-$(VERSION)-$(RPM_REV).src.rpm
	@-test -f $@ || $(MAKE) $(PACKAGE)-rpm
	@test -f $@

$(PACKAGE)-$(VERSION)-$(RPM_REV).src.rpm : dist $(distdir).tar.gz $(PACKAGE).spec
	@arch=`arch` ; case $$arch in \
	i?86) ;; \
	ppc) ;; \
	*) echo >&2 "Architecture $$arch not yet supported"; exit 1;; \
	esac
	@-case "$(LDFLAGS)" in \
	*-all-static*) ;; \
	*) echo >&2 "WARNING: LDFLAGS=-all-static required for portability"; \
	   sleep 5;; \
	esac
	@-uid=`id -u` ; case $$uid in \
	0) ;; \
	*) echo >&2 "WARNING: Build as root to avoid install-time warnings"; \
	   sleep 5;; \
	esac
	@-rm -rf rpmrc BUILD RPMS SOURCES SPECS SRPMS
	mkdir BUILD RPMS SOURCES SPECS SRPMS
	mkdir RPMS/alpha RPMS/i386 RPMS/i686 RPMS/ppc RPMS/sparc
	ln $(distdir).tar.gz SOURCES/$(distdir).tar.gz
	echo "topdir: `pwd`" > rpmrc
	echo "`grep '^macrofiles:' /usr/lib/rpm/rpmrc`:`pwd`/rpm-macros" \
								      >> rpmrc
	echo "%_topdir `pwd`" > rpm-macros
	rpm -ba --rcfile rpmrc --clean $(PACKAGE).spec
	mv SRPMS/*.rpm .
	-mv RPMS/*/*.rpm .
	rm -rf rpmrc BUILD RPMS SOURCES SPECS SRPMS

# A shorthand rule -- it's easier to type "make latte-rpm".
# This rule also assures that $(PACKAGE).spec is rebuilt.

$(PACKAGE)-rpm:
	@-rm -f $(PACKAGE).spec
	$(MAKE) $(PACKAGE)-$(VERSION)-$(RPM_REV).src.rpm

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
