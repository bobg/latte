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

Latte_WsNode::Latte_WsNode(Latte_Obj &obj, const Latte_Wstate &ws) :
  Latte_Nested(obj), Latte_Wstate(ws)
{
}

Latte_WsNode::Latte_WsNode(const Latte_WsNode &other) :
  Latte_Nested(other), Latte_Wstate(other)
{
}

Refcounter<Latte_Obj>
Latte_WsNode::wrap(Latte_Obj &obj,
                   const Latte_Wstate &ws)
{
#if 0
  Latte_WsNode *wsnode = obj.as_wsnode();

  if (wsnode)
    return wrap(*(wsnode->nested()), ws);

  Latte_Tangible *tangible = obj.as_tangible();

  if (tangible)
    if (ws == tangible->wstate())
      return Refcounter<Latte_Obj>(&obj);
#endif

  return Refcounter<Latte_Obj>(new Latte_WsNode(obj, ws));
}

void
Latte_WsNode::visit(Latte_Visitor &visitor)
{
  visitor.visit_wsnode(*this);
}

Refcounter<Latte_Obj>
Latte_WsNode::do_eval(Latte_Activation &activation)
{
  if (self_evaluating())
    return Refcounter<Latte_Obj>(this);

  return Latte_WsNode::wrap(*(nested_obj()->eval(activation)), wstate());
}
