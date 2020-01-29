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

# Copyright 1998 Zanshin Inc.			      <http://www.zanshin.com>

# The contents of this file are subject to the Zanshin Public License Version
# 1.0 (the "License"); you may not use this file except in compliance with the
# License.  You should have received a copy of the License with Latte; see
# the file COPYING.  You may also obtain a copy of the License at
# <http://www.zanshin.com/ZPL.html>.
# 
# Software distributed under the License is distributed on an "AS IS" basis,
# WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
# for the specific language governing rights and limitations under the
# License.
# 
# The Original Code is Latte.
# 
# The Initial Developer of the Original Code is Zanshin, Inc.


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

DESTDIR =

pkgdatadir = $(datadir)/latte
pkglibdir = $(libdir)/latte
pkgincludedir = $(includedir)/latte

top_builddir = ..

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
MAKEINFO = makeinfo
NM = /usr/bin/nm -B
PACKAGE = latte
RANLIB = ranlib
RPM_REV = 0
USE_SYMBOL_UNDERSCORE = no
VERSION = 2.1
lispdir = $(prefix)/share/emacs/site-lisp

TESTS = a add and append apply car case concat cons def divide 	empty equal explode file-contents 	greater group if img include 	lambda 	length let 	lmap load-file load-library member modulo multiply not nth operator 	reverse set 	string subseq substr undefined-x while

EXTRA_DIST = runtest.sh 	a.latte a.exp add.latte add.exp and.latte and.exp append.latte 	append.exp apply.latte 	apply.exp car.latte car.exp case.latte case.exp 	concat.latte concat.exp 	cons.latte cons.exp 	def.latte def.exp divide.latte divide.exp 	empty.latte empty.exp equal.latte equal.exp explode.latte explode.exp 	file-contents.latte file-contents.exp 	greater.latte greater.exp group.latte group.exp 	if.latte if.exp 	img.latte img.exp include.latte include.exp include2.latte 	lambda.latte 	lambda.exp length.latte length.exp 	let.latte let.exp lmap.latte lmap.exp 	load-file.latte load-file.exp load-library.latte 	load-library.exp member.latte member.exp 	modulo.latte modulo.exp 	multiply.latte multiply.exp not.latte not.exp nth.latte nth.exp 	operator.latte operator.exp reverse.latte reverse.exp set.latte 	set.exp string.latte string.exp subseq.latte subseq.exp 	substr.latte substr.exp undefined-x.latte undefined-x.exp 	while.latte 	while.exp


LN_S = ln -s

CLEANFILES = $(TESTS) runtest

LATTE_URL = http://www.latte.org/
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = ../latte-conf.h
CONFIG_CLEAN_FILES = 
DIST_COMMON =  Makefile.am Makefile.in


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = gtar
GZIP_ENV = --best
all: all-redirect
.SUFFIXES:
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --gnu --include-deps tests/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status

tags: TAGS
TAGS:


distdir = $(top_builddir)/$(PACKAGE)-$(VERSION)/$(subdir)

subdir = tests

distdir: $(DISTFILES)
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
check-TESTS: $(TESTS)
	@failed=0; all=0; \
	srcdir=$(srcdir); export srcdir; \
	for tst in $(TESTS); do \
	  if test -f $$tst; then dir=.; \
	  else dir="$(srcdir)"; fi; \
	  if $(TESTS_ENVIRONMENT) $$dir/$$tst; then \
	    all=`expr $$all + 1`; \
	    echo "PASS: $$tst"; \
	  elif test $$? -ne 77; then \
	    all=`expr $$all + 1`; \
	    failed=`expr $$failed + 1`; \
	    echo "FAIL: $$tst"; \
	  fi; \
	done; \
	if test "$$failed" -eq 0; then \
	  banner="All $$all tests passed"; \
	else \
	  banner="$$failed of $$all tests failed"; \
	fi; \
	dashes=`echo "$$banner" | sed s/./=/g`; \
	echo "$$dashes"; \
	echo "$$banner"; \
	echo "$$dashes"; \
	test "$$failed" -eq 0
info-am:
info: info-am
dvi-am:
dvi: dvi-am
check-am: all-am
	$(MAKE) $(AM_MAKEFLAGS) check-TESTS check-local
check: check-am
installcheck-am:
installcheck: installcheck-am
install-exec-am:
install-exec: install-exec-am

install-data-am:
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am:
uninstall: uninstall-am
all-am: Makefile
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) INSTALL_STRIP_FLAG=-s install
installdirs:


mostlyclean-generic:

clean-generic:
	-test -z "$(CLEANFILES)" || rm -f $(CLEANFILES)

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean-am:  mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-generic mostlyclean-am

clean: clean-am

distclean-am:  distclean-generic clean-am
	-rm -f libtool

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-generic distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: tags distdir check-TESTS info-am info dvi-am dvi check-local \
check check-am installcheck-am installcheck install-exec-am \
install-exec install-data-am install-data install-am install \
uninstall-am uninstall all-redirect all-am all installdirs \
mostlyclean-generic distclean-generic clean-generic \
maintainer-clean-generic clean mostlyclean distclean maintainer-clean


runtest: runtest.sh Makefile
	-rm -f $@
	(echo '#!/bin/sh'; \
	 echo ' '; \
	 echo 'version=$(VERSION)'; \
	 echo 'url=$(LATTE_URL)'; \
	 echo ' '; \
	 cat $<) > $@
	chmod +x $@
	-rm -f log

$(TESTS): runtest
	-rm -f $@
	$(LN_S) $< $@

check-local:
	-rm -f runtest

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
