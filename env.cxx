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

void
Latte_BindingEnv::lookup(const shstring &name,
			 unsigned long &depth,
			 unsigned long &offset) const
{
  offset = 0;

  for (const_iterator i = begin(); i != end(); ++i, ++offset) {
    const shstring &s = *i;

    if (name == s) {
      depth = 0;
      return;
    }
  }

  if (!m_outer.get())
    throw NotFound(name);

  m_outer->lookup(name, depth, offset);
  ++depth;
}

unsigned long
Latte_BindingEnv::extend(const shstring &name)
{
  push_back(name);
  return (size() - 1);
}

unsigned long
Latte_BindingEnv::define(const shstring &name)
{
  unsigned long offset = 0;

  for (const_iterator i = begin(); i != end(); ++i, ++offset) {
    const shstring &s = *i;

    if (name == s)
      return offset;
  }

  return extend(name);
}
