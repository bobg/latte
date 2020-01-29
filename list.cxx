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

Latte_List::Latte_List() :
  m_self_eval(Latte_List::SelfEvalFn(*this)),
  m_operator(Latte_List::GetOperatorFn(*this))
{
}

Latte_List::Latte_List(const const_iterator &a,
		       const const_iterator &b) :
  Latte_ObjDeque(a, b),
  m_self_eval(Latte_List::SelfEvalFn(*this)),
  m_operator(Latte_List::GetOperatorFn(*this))
{
}

Latte_List::Latte_List(const Latte_ObjDeque &other) :
  Latte_ObjDeque(other),
  m_self_eval(Latte_List::SelfEvalFn(*this)),
  m_operator(Latte_List::GetOperatorFn(*this))
{
}

bool
Latte_List::SelfEvalFn::operator () () const
{
  if (m_list.empty())
    return 1;

  for (Latte_List::const_iterator i = m_list.begin(); i != m_list.end(); ++i) {
    const Refcounter<Latte_Obj> &obj = *i;

    if (!(obj->self_evaluating()))
      return 0;
  }

  return 1;
}

Latte_Operator *
Latte_List::GetOperatorFn::operator () () const
{
  if (m_list.size() == 1)
    return m_list.front()->get_operator();

  return 0;
}

Refcounter<Latte_Obj>
Latte_List::do_eval(Latte_Activation &activation)
{
  if (self_evaluating())
    return Refcounter<Latte_Obj>(this);

  Refcounter<Latte_List> result(new Latte_List);

  for (const_iterator i = begin(); i != end(); ++i) {
    const Refcounter<Latte_Obj> &obj = *i;

    result->push_back(obj->eval(activation));
  }

  return Refcounter<Latte_Obj>(result.get());
}

void
Latte_List::visit(Latte_Visitor &visitor)
{
  visitor.visit_list(*this);
}

void
Latte_List::render(ostream &out) const
{
  for (const_iterator i = begin(); i != end(); ++i) {
    const Refcounter<Latte_Obj> &obj = *i;
    if (i != begin())
      out << ' ';
    obj->render(out);
  }
}
