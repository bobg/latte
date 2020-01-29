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

#include <shstring.h>
#include <iostream.h>
#include <cstring>

shstring::shstring() : rep(0)
{
}

shstring::shstring(const char *str) : rep(new Rep(str))
{
  rep->inc_refcount();
}

shstring::shstring(const char *str, size_t n) : rep(new Rep(str, n))
{
  rep->inc_refcount();
}

shstring::shstring(const string &str) : rep(new Rep(str))
{
  rep->inc_refcount();
}

shstring::shstring(const shstring &other) : rep(other.rep)
{
  if (rep.get())
    rep->inc_refcount();
}

shstring::shstring(const shstring::const_iterator &b,
		   const shstring::const_iterator &e) : rep(new Rep(b, e))
{
  rep->inc_refcount();
}

shstring::~shstring()
{
  if (rep.get())
    rep->dec_refcount();
}

shstring &
shstring::operator = (const shstring &other)
{
  if (this != &other) {
    if (other.rep.get())
      other.rep->inc_refcount();
    if (rep.get())
      rep->dec_refcount();
    rep = other.rep;
  }
  return *this;
}

shstring::Rep::Rep(const char *s) : latte_string(s)
{
}

shstring::Rep::Rep(const char *s, size_t n) : latte_string(s, n)
{
}

shstring::Rep::Rep(const string &s) : latte_string(s)
{
}

shstring::Rep::Rep(const latte_string::const_iterator &b,
		   const latte_string::const_iterator &e) : latte_string(b, e)
{
}

ostream &
operator << (ostream &out, const shstring &s)
{
  if (s.rep.get())
    out << s.rep->str();
  return out;
}

bool
shstring::operator < (const shstring &other) const
{
  if (rep.get()) {
    if (other.rep.get())
      return rep->str() < other.rep->str();
    return 0;
  } else if (other.rep.get())
    return 1;
  else
    return 0;
}

bool
shstring::operator == (const shstring &other) const
{
  if (this == &other)
    return 1;

  if (rep.get()) {
    if (other.rep.get())
      return rep->str() == other.rep->str();
    return 0;
  } else if (other.rep.get())
    return 0;
  else
    return 1;
}
