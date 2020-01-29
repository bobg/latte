// -*- c++ -*-

// Copyright 1998,1999 Zanshin Inc.                  <http://www.zanshin.com/>

// The contents of this file are subject to the Zanshin Public License Version
// 1.0 (the "License"); you may not use this file except in compliance with the
// License.  You should have received a copy of the License with Latte; see
// the file COPYING.  You may also obtain a copy of the License at
// <http://www.zanshin.com/ZPL.html>.
// 
// Software distributed under the License is distributed on an "AS IS" basis,
// WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
// for the specific language governing rights and limitations under the
// License.
// 
// The Original Code is Latte.
// 
// The Initial Developer of the Original Code is Zanshin, Inc.

#ifndef LATTE_STRING_H
# define LATTE_STRING_H

#ifndef CONFIGURE_IS_RUNNING
# include <latte-conf.h>
#endif // CONFIGURE_IS_RUNNING
#include <string>

#undef latte_use_intrinsic_string

#if defined(HAVE_TWO_ITERATOR_STRING_CTOR) && !defined(USE_STRING_REMOVE)
# define latte_use_intrinsic_string 1
#endif // HAVE_TWO_ITERATOR_STRING_CTOR && !USE_STRING_REMOVE

#ifdef latte_use_intrinsic_string

typedef string latte_string;

#else // latte_use_intrinsic_string

class latte_string : public string {
 public:
  latte_string() {}
  latte_string(const char *str) : string(str) {}
  latte_string(const char *str, size_t n) : string(str, n) {}
  latte_string(const string &other) : string(other) {}

# ifdef HAVE_TWO_ITERATOR_STRING_CTOR
  latte_string(const const_iterator &a, const const_iterator &b) :
    string(a, b) {}
# else // HAVE_TWO_ITERATOR_STRING_CTOR
  latte_string(const const_iterator &a, const const_iterator &b) {
    for (const_iterator i = a; i != b; ++i)
      insert(end(), *i);
  }
#endif // HAVE_TWO_ITERATOR_STRING_CTOR

#ifdef USE_STRING_REMOVE
  latte_string &erase(size_t pos) { remove(pos); return *this; }
#endif // USE_STRING_REMOVE
};

#include <iostream.h>

extern ostream &operator << (ostream &, const latte_string &);

extern bool operator < (const latte_string &, const latte_string &);
extern bool operator == (const latte_string &, const latte_string &);

#endif // latte_use_intrinsic_string

#endif // LATTE_STRING_H
