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
#include <latte-log.h>
#include <mutable.h>

Latte_VarRef::Latte_VarRef(const Latte_Wstate &ws,
			   const Latte_FileLoc &loc,
			   const shstring &name) :
  Latte_Tangible(ws, loc), shstring(name), m_depth_offset_known(0)
{
}

Latte_VarRef::Latte_VarRef(const Latte_VarRef &other) :
  Latte_Tangible(other), shstring(other.name()),
  m_depth(other.m_depth), m_offset(other.m_offset),
  m_depth_offset_known(other.m_depth_offset_known)
{
}

Refcounter<Latte_Obj> &
Latte_VarRef::lookup_rw_aux(Latte_Activation &activation,
                            bool err_if_frozen) const
{
  if (!m_depth_offset_known) {
    try {
      activation.env()->lookup(*this, mutate(m_depth), mutate(m_offset));
    } catch(const Latte_BindingEnv::NotFound &) {
      throw Undefined(fileloc(), name());
    }
    mutate(m_depth_offset_known) = 1;
  }

  Refcounter<Latte_Obj> *result;

  try {
    result = &(activation.lookup_rw_aux(m_depth, m_offset, err_if_frozen));
  } catch (const Latte_Activation::Frozen &) {
    throw Frozen(fileloc(), name());
  }

  return *result;
}

Refcounter<Latte_Obj> &
Latte_VarRef::lookup_rw(Latte_Activation &activation) const
{
  return lookup_rw_aux(activation, 1);
}

const Refcounter<Latte_Obj> &
Latte_VarRef::lookup(Latte_Activation &activation) const
{
  return lookup_rw_aux(activation, 0);
}

Refcounter<Latte_Obj>
Latte_VarRef::do_eval(Latte_Activation &activation)
{
  return Latte_WsNode::wrap(*(lookup(activation)), wstate());
}

void
Latte_VarRef::visit(Latte_Visitor &visitor)
{
  visitor.visit_varref(*this);
}

void
Latte_VarRef::render(ostream &out) const
{
  out << '\\';
  out << name();
}
