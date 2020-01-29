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

Refcounter<Latte_Obj>
Latte_Operator::call_apply(const Latte_Wstate &ws,
			   const Latte_FileLoc &loc,
			   const Latte_List::const_iterator &args,
			   const Latte_List::const_iterator &args_end,
			   Latte_Activation &activation) const
{
  return apply(ws, loc, args, args_end, activation);
}

void
Latte_Operator::visit(Latte_Visitor &visitor)
{
  visitor.visit_operator(*this);
}
