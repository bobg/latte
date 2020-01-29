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

Latte_FileLoc::Latte_FileLoc(const shstring &filename,
			     unsigned int lineno) :
  m_filename(filename), m_lineno(lineno)
{
}

Latte_FileLoc::Latte_FileLoc(const Latte_FileLoc &other) :
  m_filename(other.m_filename), m_lineno(other.m_lineno)
{
}

Latte_FileLoc &
Latte_FileLoc::operator = (const Latte_FileLoc &other)
{
  if (this != &other) {
    m_filename = other.m_filename;
    m_lineno = other.m_lineno;
  }
  return *this;
}

bool
Latte_FileLoc::operator == (const Latte_FileLoc &other) const
{
  return ((this == &other)
          || ((m_filename == other.m_filename)
              && (m_lineno == other.m_lineno)));
}

ostream &
operator << (ostream &out, const Latte_FileLoc &loc)
{
  if (loc.filename().empty())
    out << "[no filename]";
  else
    out << loc.filename();

  out << ", line " << loc.lineno();
  return out;
}
