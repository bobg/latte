// Copyright 1998 Zanshin Inc.                       <http://www.zanshin.com/>

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

#include <latte-string.h>

#ifndef latte_use_intrinsic_string

// I shouldn't need to define these, but some compilers are stupid
bool
operator < (const latte_string &a, const latte_string &b)
{
  return (static_cast<const string &>(a) < static_cast<const string &>(b));
}

bool
operator == (const latte_string &a, const latte_string &b)
{
  return ((&a == &b)
          || (static_cast<const string &>(a) ==
              static_cast<const string &>(b)));
}

ostream &
operator << (ostream &out, const latte_string &str)
{
  return (out << static_cast<const string &>(str));
}

#endif // latte_use_intrinsic_string
