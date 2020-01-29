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

Latte_Group::Latte_Group(const Latte_Wstate &ws,
			 const Latte_FileLoc &loc) :
  Latte_Tangible(ws, loc),
  m_self_eval(Latte_Group::SelfEvalFn(*this))
{
}

Latte_Group::Latte_Group(const Latte_Wstate &ws,
			 const Latte_FileLoc &loc,
			 const Latte_List::const_iterator &a,
			 const Latte_List::const_iterator &b) :
  Latte_Tangible(ws, loc),
  Latte_List(a, b),
  m_self_eval(Latte_Group::SelfEvalFn(*this))
{
}

Latte_Group::Latte_Group(const Latte_Group &other) :
  Latte_Tangible(other),
  Latte_List(other.begin(), other.end()),
  m_self_eval(Latte_Group::SelfEvalFn(*this))
{
}

bool
Latte_Group::SelfEvalFn::operator () () const
{
  if (m_group.empty())
    return 1;

  const Refcounter<Latte_Obj> &obj = m_group.front();
  if (obj->get_operator())
    return 0;

  for (Latte_Group::const_iterator i = m_group.begin();
       i != m_group.end();
       ++i) {
    const Refcounter<Latte_Obj> &obj = *i;

    if (!(obj->self_evaluating()))
      return 0;
  }

  return 1;
}

Refcounter<Latte_Obj>
Latte_Group::do_eval(Latte_Activation &activation)
{
  if (self_evaluating())
    return Refcounter<Latte_Obj>(this);

  const_iterator i = begin();
  const Refcounter<Latte_Obj> &first = *i++;

  Refcounter<Latte_Obj> first_val = first->eval(activation);
  const Latte_Operator *as_operator = first_val->get_operator();

  if (as_operator) {
    Latte_List args;

    Latte_List::const_iterator args_begin = i;
    Latte_List::const_iterator args_end = end();

    if (as_operator->eval_args()) {
      while (i != end())
        args.push_back((*i++)->eval(activation));

      args_begin = args.begin();
      args_end = args.end();
    }

    return as_operator->call_apply(wstate(), fileloc(),
                                   args_begin, args_end,
                                   activation);
  }

  Refcounter<Latte_List> result_list(new Latte_List);

  result_list->push_back(first_val);

  while (i != end()) {
    const Refcounter<Latte_Obj> &obj = *i++;

    result_list->push_back(obj->eval(activation));
  }

  return Latte_WsNode::wrap(*result_list, wstate());
}

void
Latte_Group::visit(Latte_Visitor &visitor)
{
  visitor.visit_group(*this);
}

void
Latte_Group::render(ostream &out) const
{
  out << '{';
  Latte_List::render(out);
  out << '}';
}
