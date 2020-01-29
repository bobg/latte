// -*- c++ -*-

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

#ifndef LATTE_MEMO_H
# define LATTE_MEMO_H

// Horrors, a global!  I'd use a namespace to contain this, if namespaces were
// more widely implemented.
extern unsigned int g_memo_last_reset;

template <class T, class F>
class memo {
 public:
  memo(const F &fn) : m_known(0), m_fn(fn) {}

  T &val() {
    if (m_known >= g_memo_last_reset)
      return m_val;
    m_known = g_memo_last_reset;
    return ((m_val = m_fn()));
  }
  const T &val() const {
    if (m_known >= g_memo_last_reset)
      return m_val;
    mutate(m_known) = g_memo_last_reset;
    return ((mutate(m_val) = m_fn()));
  }

  void reset() { m_known = 0; }

  static void reset_all() { ++g_memo_last_reset; }

 private:
  mutable unsigned int m_known;
  mutable T m_val;
  F m_fn;
};

#endif // LATTE_MEMO_H
