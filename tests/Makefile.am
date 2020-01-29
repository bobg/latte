## Process this file with automake to produce Makefile.in

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

TESTS = a add and append apply car case concat cons def divide \
	empty equal explode file-contents \
	greater group if img include \
	lambda \
	length let \
	lmap load-file load-library member modulo multiply not nth operator \
	reverse set \
	string subseq substr undefined-x while
EXTRA_DIST = runtest.sh \
	a.latte a.exp add.latte add.exp and.latte and.exp append.latte \
	append.exp apply.latte \
	apply.exp car.latte car.exp case.latte case.exp \
	concat.latte concat.exp \
	cons.latte cons.exp \
	def.latte def.exp divide.latte divide.exp \
	empty.latte empty.exp equal.latte equal.exp explode.latte explode.exp \
	file-contents.latte file-contents.exp \
	greater.latte greater.exp group.latte group.exp \
	if.latte if.exp \
	img.latte img.exp include.latte include.exp include2.latte \
	lambda.latte \
	lambda.exp length.latte length.exp \
	let.latte let.exp lmap.latte lmap.exp \
	load-file.latte load-file.exp load-library.latte \
	load-library.exp member.latte member.exp \
	modulo.latte modulo.exp \
	multiply.latte multiply.exp not.latte not.exp nth.latte nth.exp \
	operator.latte operator.exp reverse.latte reverse.exp set.latte \
	set.exp string.latte string.exp subseq.latte subseq.exp \
	substr.latte substr.exp undefined-x.latte undefined-x.exp \
	while.latte \
	while.exp

LN_S = @LN_S@

CLEANFILES = $(TESTS) runtest

LATTE_URL = @LATTE_URL@

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
