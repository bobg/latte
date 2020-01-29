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

Latte_Param::Latte_Param(const Latte_Wstate &ws,
			 const Latte_FileLoc &loc,
			 const shstring &name,
			 enum Latte_Param::Type type) :
  Latte_Tangible(ws, loc), shstring(name), m_type(type)
{
}

Latte_Param::Latte_Param(const Latte_Param &other) :
  Latte_Tangible(other), shstring(other.name()), m_type(other.m_type)
{
}
  
void
Latte_Param::render(ostream &out) const
{
  out << '\\';
  switch (m_type) {
   case type_named:
    out << '=';
    break;
   case type_rest:
    out << '&';
    break;

   default:
    // xxx impossible?
    break;
  }

  out << name();
}

void
Latte_Param::visit(Latte_Visitor &visitor)
{
  visitor.visit_param(*this);
}
