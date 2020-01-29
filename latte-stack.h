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

#ifndef LATTE_STACK_H
# define LATTE_STACK_H

#ifndef CONFIGURE_IS_RUNNING
# include <latte-conf.h>
#endif // CONFIGURE_IS_RUNNING

#ifdef HAVE_NOV96_STACK

# include <stack>
template <class T>
class latte_stack : public stack<T> {
  // empty (I wish there were template typedefs)
};

#else // HAVE_NOV96_STACK

# include <deque>
# include <stack>
template <class T>
class latte_stack : public stack< deque<T> > {
  // empty (I wish there were template typedefs)
};

#endif // HAVE_NOV96_STACK

#endif // LATTE_STACK_H
