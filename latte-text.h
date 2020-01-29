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

#ifndef LATTE_TEXT_H
# define LATTE_TEXT_H

#include <latte.h>

class LatteText_PreObj : public Latte_Nested {
 public:
  LatteText_PreObj(Latte_Obj &obj) : Latte_Nested(obj) {}
  LatteText_PreObj(const LatteText_PreObj &other) : Latte_Nested(other) {}

  void visit(Latte_Visitor &);

 protected:
  Refcounter<Latte_Obj> do_eval(Latte_Activation &);
};

class LatteText_IndentObj : public Latte_Nested {
 public:
  LatteText_IndentObj(Latte_Obj &obj, unsigned int level = 2) :
    Latte_Nested(obj), m_level(level) {}
  LatteText_IndentObj(const LatteText_IndentObj &other) :
    Latte_Nested(other), m_level(other.m_level) {}

  void visit(Latte_Visitor &);

 protected:
  Refcounter<Latte_Obj> do_eval(Latte_Activation &);

 private:
  unsigned int m_level;
};

class LatteText_TextVisitor : public Latte_Visitor {
 public:
  LatteText_TextVisitor(ostream &out,
                        unsigned int wrapcol = 72,
                        const char *sentence_end_chars = 0,
                        const char *sentence_end_transparent_chars = 0) :
    m_out(out), m_wrapcol(wrapcol), m_column(0),
    m_sentence_end_chars(sentence_end_chars),
    m_sentence_end_transparent_chars(sentence_end_transparent_chars),
    m_new_sentence(1) {}

  void visit_str(Latte_Str &);

 private:
  ostream &m_out;
  unsigned int m_wrapcol;
  unsigned int m_column;
  const char *m_sentence_end_chars;
  const char *m_sentence_end_transparent_chars;
  bool m_new_sentence;
};

void
latte_text_init_globals(Latte_Activation &, bool,
                        const deque<latte_string>::const_iterator &,
                        const deque<latte_string>::const_iterator &);

extern void
latte_text(bool no_default, unsigned long log_flags,
           const deque<latte_string>::const_iterator &loads_begin,
           const deque<latte_string>::const_iterator &loads_end,
           unsigned int wrapcol,
           const char *sentence_end_chars,
           const char *sentence_end_transparent_chars,
           const char *filename, istream &in, ostream &out,
           Latte_Activation *activation = 0);

#endif // LATTE_TEXT_H
