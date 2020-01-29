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

Latte_Nested::Latte_Nested() :
  m_self_eval(Latte_Nested::SelfEvalFn(*this)),
  m_side_effect(Latte_Nested::SideEffectFn(*this)),
  m_operator(Latte_Nested::GetOperatorFn(*this))
{
}

Latte_Nested::Latte_Nested(Latte_Obj &obj) :
  m_nested(&obj),
  m_self_eval(Latte_Nested::SelfEvalFn(*this)),
  m_side_effect(Latte_Nested::SideEffectFn(*this)),
  m_operator(Latte_Nested::GetOperatorFn(*this))
{
}

Latte_Nested::Latte_Nested(const Latte_Nested &other) :
  m_nested(other.m_nested),
  m_self_eval(Latte_Nested::SelfEvalFn(*this)),
  m_side_effect(Latte_Nested::SideEffectFn(*this)),
  m_operator(Latte_Nested::GetOperatorFn(*this))
{
}

bool
Latte_Nested::SelfEvalFn::operator () () const
{
  return m_nested.m_nested->self_evaluating();
}

bool
Latte_Nested::SideEffectFn::operator () () const
{
  return m_nested.m_nested->side_effects();
}

Latte_Operator *
Latte_Nested::GetOperatorFn::operator () () const
{
  return m_nested.m_nested->get_operator();
}

Refcounter<Latte_Obj>
Latte_Nested::do_eval(Latte_Activation &activation)
{
  return m_nested->do_eval(activation);
}

void
Latte_Nested::visit(Latte_Visitor &visitor)
{
  visitor.visit_nested(*this);
}

Latte_Number_t
Latte_Nested::numeric_val() const
{
  return m_nested->numeric_val();
}

bool
Latte_Nested::bool_val() const
{
  return m_nested->bool_val();
}

void
Latte_Nested::set_nested_obj(const Refcounter<Latte_Obj> &obj)
{
  m_nested = obj;
  m_self_eval.reset();
  m_side_effect.reset();
  m_operator.reset();
}
