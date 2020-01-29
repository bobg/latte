// Copyright 1998 Zanshin Inc.                       <http://www.zanshin.com/>

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

Latte_Activation::Latte_Activation() :
  m_env(new Latte_BindingEnv), m_outer(0), m_frozen(0)
{
}

Latte_Activation::Latte_Activation(Latte_BindingEnv &env) :
  m_env(&env), m_outer(0), m_frozen(0)
{
}

Latte_Activation::Latte_Activation(Latte_BindingEnv &env,
				   Latte_Activation &outer) :
  m_env(&env), m_outer(&outer), m_frozen(0)
{
}

Refcounter<Latte_Obj> &
Latte_Activation::lookup_rw_aux(unsigned long depth,
                                unsigned long offset,
                                bool err_if_frozen) const
{
  if (depth) {
    if (!m_outer.get())
      throw ImpossibleDepth();	// xxx should never occur?

    return m_outer->lookup_rw_aux(depth - 1, offset, err_if_frozen);
  }

  if (err_if_frozen && m_frozen)
    throw Frozen();

  if (offset >= m_objs.size())
    mutate(m_objs).resize(offset + 1, latte_false());

  return *(mutate(m_objs).begin() + offset);
}

Refcounter<Latte_Obj> &
Latte_Activation::lookup_rw_aux(const shstring &name,
                                bool err_if_frozen) const
{
  unsigned long depth;
  unsigned long offset;

  m_env->lookup(name, depth, offset);
  return lookup_rw_aux(depth, offset, err_if_frozen);
}

Refcounter<Latte_Obj> &
Latte_Activation::lookup_rw(unsigned long depth,
                            unsigned long offset) const
{
  return lookup_rw_aux(depth, offset, 1);
}

Refcounter<Latte_Obj> &
Latte_Activation::lookup_rw(const shstring &name) const
{
  return lookup_rw_aux(name, 1);
}

const Refcounter<Latte_Obj> &
Latte_Activation::lookup(unsigned long depth,
                         unsigned long offset) const
{
  return lookup_rw_aux(depth, offset, 0);
}

const Refcounter<Latte_Obj> &
Latte_Activation::lookup(const shstring &name) const
{
  return lookup_rw_aux(name, 0);
}

unsigned long
Latte_Activation::define(const shstring &name,
			 const Refcounter<Latte_Obj> &val)
{
  if (m_frozen)
    throw Frozen();

  unsigned long offset = m_env->define(name);
  lookup_rw(0, offset) = val;

  return offset;
}
