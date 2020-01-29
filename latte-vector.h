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

#ifndef LATTE_VECTOR_H
# define LATTE_VECTOR_H

#ifndef CONFIGURE_IS_RUNNING
# include <latte-conf.h>
#endif // CONFIGURE_IS_RUNNING
#include <vector>

#ifdef HAVE_VECTOR_RESIZE

template <class T>
class latte_vector : public vector<T> {
  // empty (I wish there were template typedefs)
};

#else // HAVE_VECTOR_RESIZE

template <class T>
class latte_vector : public vector<T> {
 public:
  void resize(size_t n, const T &obj) {
    if (n > size())
      insert(end(), n - size(), obj);
    else
      erase(begin() + n, end());
  }
};

#endif // HAVE_VECTOR_RESIZE

#endif // LATTE_VECTOR_H
