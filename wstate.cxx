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

unsigned int Latte_Wstate::tab_width = 8;

Latte_Wstate::Latte_Wstate() :
  m_spaces(0), m_newlines(0)
{
}

Latte_Wstate::Latte_Wstate(const Latte_Wstate &other) :
#ifndef ENABLE_WHITESPACE_OPT
  m_str(other.m_str),
#endif // ENABLE_WHITESPACE_OPT
  m_spaces(other.m_spaces),
  m_newlines(other.m_newlines)
{
}

Latte_Wstate &
Latte_Wstate::operator = (const Latte_Wstate &other)
{
  if (this != &other) {
#ifndef ENABLE_WHITESPACE_OPT
    m_str = other.m_str;
#endif // ENABLE_WHITESPACE_OPT
    m_spaces = other.m_spaces;
    m_newlines = other.m_newlines;
  }
  return *this;
}

bool
Latte_Wstate::operator == (const Latte_Wstate &other) const
{
  if (this == &other)
    return 1;

#ifndef ENABLE_WHITESPACE_OPT
  if (m_str != other.m_str)
    return 0;
#endif // ENABLE_WHITESPACE_OPT

  return ((m_newlines == other.m_newlines)
          && (m_spaces == other.m_spaces));
}

Latte_Wstate
Latte_Wstate::transfer()
{
  Latte_Wstate result(*this);

  reset_ws();

  return result;
}

void
Latte_Wstate::reset_ws()
{
#ifndef ENABLE_WHITESPACE_OPT
  m_str.erase();
#endif // ENABLE_WHITESPACE_OPT
  m_spaces = 0;
  m_newlines = 0;
}

void
Latte_Wstate::space(unsigned int n)
{
#ifndef ENABLE_WHITESPACE_OPT
  for (unsigned int i = 0; i < n; ++i)
    m_str += ' ';
#endif // ENABLE_WHITESPACE_OPT
  m_spaces += n;
}

unsigned int
Latte_Wstate::tab(unsigned int n, unsigned int column)
{
  if (n > 0) {
    unsigned int new_column = column / tab_width;
    ++new_column;
    new_column *= tab_width;
    m_spaces += (new_column - column);
    m_spaces += (tab_width * (n - 1));
    new_column += (tab_width * (n - 1));
#ifndef ENABLE_WHITESPACE_OPT
    for (unsigned int i = 0; i < n; ++i)
      m_str += '\t';
#endif // ENABLE_WHITESPACE_OPT
    return new_column;
  }
  return column;
}

void
Latte_Wstate::crlf(unsigned int n)
{
  m_newlines += n;
  m_spaces = 0;
#ifndef ENABLE_WHITESPACE_OPT
  for (unsigned int i = 0; i < n; ++i)
    m_str += "\r\n";
#endif // ENABLE_WHITESPACE_OPT
}

void
Latte_Wstate::cr(unsigned int n)
{
  m_newlines += n;
  m_spaces = 0;
#ifndef ENABLE_WHITESPACE_OPT
  for (unsigned int i = 0; i < n; ++i)
    m_str += '\r';
#endif // ENABLE_WHITESPACE_OPT
}

void
Latte_Wstate::lf(unsigned int n)
{
  m_newlines += n;
  m_spaces = 0;
#ifndef ENABLE_WHITESPACE_OPT
  for (unsigned int i = 0; i < n; ++i)
    m_str += '\n';
#endif // ENABLE_WHITESPACE_OPT
}

ostream &
operator << (ostream &out, const Latte_Wstate &wstate)
{
  for (unsigned int i = 0; i < wstate.newlines(); ++i)
    out << endl;
  for (unsigned int i = 0; i < wstate.m_spaces; ++i)
    out << ' ';
#ifndef ENABLE_WHITESPACE_OPT
  out << wstate.m_str;
#endif // ENABLE_WHITESPACE_OPT
  return out;
}
