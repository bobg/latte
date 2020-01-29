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

Latte_Closure::Latte_Closure(const Latte_List::const_iterator &params,
			     const Latte_List::const_iterator &params_end,
			     const Latte_List::const_iterator &body,
			     const Latte_List::const_iterator &body_end,
			     Latte_Activation &activation) :
  m_env(new Latte_BindingEnv(*(activation.env()))),
  m_activation(&activation),
  m_body(new Latte_List(body, body_end)),
  m_rest_offset(-1), m_positional_params(0),
  m_macro(0)
{
  Refcounter<Latte_Param> rest_param(0);

  // Build the binding environment for calls to this closure.
  // Start with ordinary positional parameters.
  for (Latte_List::const_iterator i = params; i != params_end; ++i) {
    const Refcounter<Latte_Obj> &obj = *i;
    Latte_VarRef *varref = obj->as_varref();
    Latte_Param *param = obj->as_param();

    if (param && param->is_rest())
      rest_param = Refcounter<Latte_Param>(param);
    else if (varref) {
      m_env->extend(varref->name());
      ++m_positional_params;
    }

    // silently ignore other kinds of objects
  }

  // Now the named params
  for (Latte_List::const_iterator i = params; i != params_end; ++i) {
    const Refcounter<Latte_Obj> &obj = *i;
    Latte_Param *param = obj->as_param();

    if (param && param->is_named())
      m_env->extend(param->name());
  }

  if (rest_param.get())
    m_rest_offset = m_env->extend(rest_param->name());
}

Latte_Closure::Latte_Closure(const Latte_Closure &other) :
  m_env(other.m_env),
  m_activation(other.m_activation),
  m_body(other.m_body),
  m_rest_offset(other.m_rest_offset),
  m_positional_params(other.m_positional_params),
  m_macro(other.m_macro)
{
}

Refcounter<Latte_Obj>
Latte_Closure::apply(const Latte_Wstate &ws,
		     const Latte_FileLoc &loc,
		     const Latte_List::const_iterator &args,
		     const Latte_List::const_iterator &args_end,
		     Latte_Activation &activation) const
{
  Refcounter<Latte_Activation> new_activation(new
					      Latte_Activation(*m_env,
							       *m_activation));
  Refcounter<Latte_List> rest_list((m_rest_offset >= 0) ?
				   (new Latte_List) :
				   0);

  long n = 0;
  for (Latte_List::const_iterator i = args; i != args_end; ++i) {
    const Refcounter<Latte_Obj> &obj = *i;
    Latte_Assignment *assignment = obj->as_assignment();

    if (assignment) {
      try {
        Refcounter<Latte_Obj> val = assignment->nested_obj();

        new_activation->lookup_rw(assignment->name()) = val;
      } catch (const Latte_BindingEnv::NotFound &err) {
        throw IllegalAssignment(*this, err, loc);
      };
    } else if (n >= m_positional_params) {
      if (m_rest_offset >= 0) {
        rest_list->push_back(obj);
        ++n;
      } // else silently drop it
    } else {
      new_activation->lookup_rw(0, n) = obj;
      ++n;
    }
  }

  if (m_rest_offset >= 0)
    new_activation->lookup_rw(0, m_rest_offset) =
				       Refcounter<Latte_Obj>(rest_list.get());

  Refcounter<Latte_Obj> result = latte_false();

  for (Latte_List::const_iterator i = m_body->begin(); i != m_body->end(); ) {
    const Refcounter<Latte_Obj> &obj = *i++;

    if (!((i == m_body->end()) || obj->side_effects())) {
      Latte_Tangible *tangible = obj->as_tangible();

      throw Useless(*this, (tangible ? tangible->fileloc() : loc));
    }

    result = obj->eval(*new_activation);
  }

  return Latte_WsNode::wrap(*(m_macro ? result->eval(activation) : result),
                            ws);
}

void
Latte_Closure::visit(Latte_Visitor &visitor)
{
  visitor.visit_closure(*this);
}

const shstring &
Latte_Closure::name() const
{
  static const shstring n = "closure";
  return n;
}
