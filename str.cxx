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

#include <latte.h>
#include <cstdio>
#include <cstdlib>

Latte_Str::Latte_Str(const Latte_Wstate &ws,
		     const Latte_FileLoc &loc,
		     const shstring &str) :
  Latte_Tangible(ws, loc), shstring(str), m_numval_known(0), m_numval(0)
{
}

Latte_Str::Latte_Str(const Latte_Wstate &ws,
		     const Latte_FileLoc &loc,
		     const Latte_Number_t &num) :
  Latte_Tangible(ws, loc), m_numval_known(1), m_numval(num)
{
  char buf[32];

#ifdef ENABLE_FLOATING_POINT
  sprintf(buf, "%.f", num);
#else // ENABLE_FLOATING_POINT
  sprintf(buf, "%ld", num);
#endif // ENABLE_FLOATING_POINT

  str() = shstring(buf);
}

Latte_Str::Latte_Str(const Latte_Str &other) :
  Latte_Tangible(other), shstring(other.str()),
  m_numval_known(other.m_numval_known), m_numval(other.m_numval)
{
}

void
Latte_Str::visit(Latte_Visitor &visitor)
{
  visitor.visit_str(*this);
}

void
Latte_Str::render(ostream &out) const
{
  out << str();
}

Latte_Number_t
Latte_Str::numeric_val() const
{
  if (m_numval_known)
    return m_numval;

#ifdef ENABLE_FLOATING_POINT
  mutate(m_numval) = strtod(str().str().c_str(), 0);
#else // ENABLE_FLOATING_POINT
  mutate(m_numval) = strtol(str().str().c_str(), 0, 10);
#endif // ENABLE_FLOATING_POINT

  mutate(m_numval_known) = 1;

  return m_numval;
}
