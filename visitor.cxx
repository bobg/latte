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

void
Latte_Visitor::visit_nested(Latte_Nested &nested)
{
  nested.nested_obj()->visit(*this);
}

void
Latte_Visitor::visit_group(Latte_Group &group)
{
  if (group.empty())
    return;

  bool set_use_my_wstate = 0;

  if (!use_my_wstate) {
    use_my_wstate = 1;
    my_wstate = group.wstate();
    set_use_my_wstate = 1;
  }

  for (Latte_Group::const_iterator i = group.begin();
       i != group.end();
       ++i) {
    const Refcounter<Latte_Obj> &obj = *i;

    obj->visit(*this);
  }

  if (set_use_my_wstate)
    use_my_wstate = 0;
}

const Latte_Wstate &
Latte_Visitor::suggest_wstate(const Latte_Wstate &fallback)
{
  if (use_my_wstate) {
    use_my_wstate = 0;
    return my_wstate;
  }
  return fallback;
}

void
Latte_Visitor::visit_wsnode(Latte_WsNode &wsnode)
{
  bool set_use_my_wstate = 0;

  if (!use_my_wstate) {
    use_my_wstate = 1;
    my_wstate = wsnode.wstate();
    set_use_my_wstate = 1;
  }

  wsnode.nested_obj()->visit(*this);

  if (set_use_my_wstate)
    use_my_wstate = 0;
}

void
Latte_Visitor::visit_list(Latte_List &list)
{
  for (Latte_List::const_iterator i = list.begin(); i != list.end(); ++i) {
    const Refcounter<Latte_Obj> &obj = *i;
    obj->visit(*this);
  }
}

void
Latte_Visitor::visit_quote(Latte_Quote &quote)
{
  if (quote.type() == Latte_Quote::type_quote)
    quote.obj()->visit(*this);
}

void
Latte_Stringify::visit_str(Latte_Str &str)
{
  if (m_space_separated && m_string.size()) {
    const Latte_Wstate &ws = suggest_wstate(str.wstate());

    if (ws.spaces() || ws.newlines())
      m_string += ' ';
  }
  m_string += str.str().str();
}

void
Latte_Listify::visit_assignment(Latte_Assignment &obj)
{
  m_list->push_back(Refcounter<Latte_Obj>(&obj));
}
void
Latte_Listify::visit_boolean(Latte_Boolean &obj)
{
  m_list->push_back(Refcounter<Latte_Obj>(&obj));
}
void
Latte_Listify::visit_closure(Latte_Closure &obj)
{
  m_list->push_back(Refcounter<Latte_Obj>(&obj));
}
void
Latte_Listify::visit_group(Latte_Group &obj)
{
  m_list->push_back(Refcounter<Latte_Obj>(&obj));
}
void
Latte_Listify::visit_list(Latte_List &obj)
{
  m_list->push_back(Refcounter<Latte_Obj>(&obj));
}
void
Latte_Listify::visit_nested(Latte_Nested &obj)
{
  m_list->push_back(Refcounter<Latte_Obj>(&obj));
}
void
Latte_Listify::visit_operator(Latte_Operator &obj)
{
  m_list->push_back(Refcounter<Latte_Obj>(&obj));
}
void
Latte_Listify::visit_param(Latte_Param &obj)
{
  m_list->push_back(Refcounter<Latte_Obj>(&obj));
}
void
Latte_Listify::visit_varref(Latte_VarRef &obj)
{
  m_list->push_back(Refcounter<Latte_Obj>(&obj));
}
void
Latte_Listify::visit_wsnode(Latte_WsNode &obj)
{
  m_list->push_back(Refcounter<Latte_Obj>(&obj));
}
void
Latte_Listify::visit_str(Latte_Str &obj)
{
  m_list->push_back(Refcounter<Latte_Obj>(&obj));
}
