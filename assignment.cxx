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

Latte_Assignment::Latte_Assignment(const Latte_Wstate &ws,
			 const Latte_FileLoc &loc,
			 const shstring &name) :
  Latte_Tangible(ws, loc), shstring(name)
{
}

Latte_Assignment::Latte_Assignment(const Latte_Assignment &other) :
  Latte_Tangible(other), shstring(other)
{
}

void
Latte_Assignment::render(ostream &out) const
{
  out << '\\' << name() << '=' << *(nested_obj());
}

void
Latte_Assignment::visit(Latte_Visitor &visitor)
{
  visitor.visit_assignment(*this);
}

Refcounter<Latte_Obj>
Latte_Assignment::do_eval(Latte_Activation &activation)
{
  Refcounter<Latte_Assignment> result(new Latte_Assignment(*this));

  result->set_nested_obj(nested_obj()->eval(activation));

  return Refcounter<Latte_Obj>(result.get());
}
