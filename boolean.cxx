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

const Refcounter<Latte_Obj> &
latte_false()
{
  static Refcounter<Latte_Obj> b(new Latte_Boolean(0));

  return b;
}

const Refcounter<Latte_Obj> &
latte_true()
{
  static Refcounter<Latte_Obj> b(new Latte_Boolean(1));

  return b;
}

Latte_Boolean::Latte_Boolean(bool val) : m_val(val)
{
}

Latte_Boolean::Latte_Boolean(const Latte_Boolean &other) : m_val(other.m_val)
{
}

void
Latte_Boolean::visit(Latte_Visitor &visitor)
{
  visitor.visit_boolean(*this);
}

void
Latte_Boolean::render(ostream &out) const
{
  if (m_val)
    out << "[true]";
  else
    out << "[false]";
}
