// -*- c++ -*-

// Copyright 1997,1998 Zanshin Inc.                  <http://www.zanshin.com/>

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

#ifndef LATTE_IOSFWD_H
# define LATTE_IOSFWD_H

#ifdef HAVE_IOSFWD
# include <iosfwd>
#else // HAVE_IOSFWD
# ifdef HAVE_IOSFWD_H
#  include <iosfwd.h>
# else // HAVE_IOSFWD_H
class istream;
class ostream;
# endif // HAVE_IOSFWD_H
#endif // HAVE_IOSFWD

#endif // LATTE_IOSFWD_H
