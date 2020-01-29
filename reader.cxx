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

Latte_Reader::Latte_Reader(istream &i, const shstring &f) :
  yyFlexLexer(&i),
  m_column(0), m_filename(f)
{
  m_global_activation =
    Refcounter<Latte_Activation>(new Latte_Activation(*(new
                                                        Latte_BindingEnv)));
}

Latte_Reader::Latte_Reader(istream &i,
			   const shstring &f,
			   Latte_Activation &activation) :
  yyFlexLexer(&i),
  m_column(0), m_filename(f),
  m_global_activation(&activation)
{
  if (m_global_activation->frozen()) {
    Latte_BindingEnv *new_env(new
                              Latte_BindingEnv(*(m_global_activation->env())));

    m_global_activation = Refcounter<Latte_Activation>(new
                                                    Latte_Activation(*new_env,
                                                       *m_global_activation));
  }
}

void
Latte_Reader::define_global(const shstring &name,
			    const Refcounter<Latte_Obj> &obj)
{
  m_global_activation->define(name, obj);
}
