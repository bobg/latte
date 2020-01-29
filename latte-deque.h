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

#ifndef LATTE_DEQUE_H
# define LATTE_DEQUE_H

#ifndef CONFIGURE_IS_RUNNING    // hack hack hack
# include <latte-conf.h>
#endif // CONFIGURE_IS_RUNNING
#include <deque>

template <class T>
class latte_deque : public deque<T> {
 public:
  latte_deque() {}

#ifdef HAVE_NOV96_DEQUE
  latte_deque(const const_iterator &a,
	      const const_iterator &b) : deque<T>(a, b) {}
#else // HAVE_NOV96_DEQUE
  latte_deque(const const_iterator &a,
	      const const_iterator &b) {
    for (const_iterator i = a; i != b; ++i)
      push_back(*i);
  }
#endif // HAVE_NOV96_DEQUE

  void append(const const_iterator &a,
	      const const_iterator &b) {
#ifdef HAVE_NOV96_DEQUE
    insert(end(), a, b);
#else // HAVE_NOV96_DEQUE
    for (const_iterator i = a; i != b; ++i)
      push_back(*i);
#endif // HAVE_NOV96_DEQUE
  }
};

#endif // LATTE_DEQUE_H
