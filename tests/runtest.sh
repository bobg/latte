# Copyright (c) 1998 Zanshin Inc.             <http://www.zanshin.com>
# 
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

LATTE_PATH=$srcdir/..
export LATTE_PATH

name=`basename $0`

rm -f $name.log
../latte-html --fragment $srcdir/$name.latte > $name.log 2>&1
result=$?

case $name in
 *-x) expfail=1 ;;
 *) expfail=0 ;;
esac

if test $expfail -eq 0
then
  if test $result -eq 0
  then
    ok=1
  else
    ok=0
  fi
else
  if test $result -eq 0
  then
    ok=0
  else
    ok=1
  fi
fi

if test $ok -eq 1
then
    rm -f $name.out
    sed -e "s/@VERSION@/$version/g" \
        -e "s,@URL@,$url,g" \
        -e "s,@SRCDIR@,$srcdir,g" $srcdir/$name.exp > $name.out
    if cmp -s $name.log $name.out > /dev/null 2>&1
    then
	echo "*** Test $name SUCCEEDED" >> log
	rm -f $name.log $name.out
	exit 0
    else
	echo "*** Test $name FAILED" >> log
	echo "**** Expected output was" >> log
	cat $name.out >> log
	echo " " >> log
	echo "**** Actual output was" >> log
	cat $name.log >> log
	echo " " >> log
	rm -f $name.log $name.out
	exit 1
    fi
else
    echo "*** Test $name FAILED" >> log
    cat $name.log >> log
    echo " " >> log
    rm -f $name.log
    exit 2
fi
