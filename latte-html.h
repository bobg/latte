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

#ifndef LATTE_HTML_H
# define LATTE_HTML_H

#include <latte.h>

class LatteHtml_PreObj : public Latte_Nested {
 public:
  LatteHtml_PreObj(Latte_Obj &obj) : Latte_Nested(obj) {}
  LatteHtml_PreObj(const LatteHtml_PreObj &other) : Latte_Nested(other) {}

  void visit(Latte_Visitor &);

 protected:
  Refcounter<Latte_Obj> do_eval(Latte_Activation &);
};

class LatteHtml_HtmlObj : public Latte_Nested {
 public:
  LatteHtml_HtmlObj(Latte_Obj &obj) : Latte_Nested(obj) {}
  LatteHtml_HtmlObj(const LatteHtml_HtmlObj &other) : Latte_Nested(other) {}

  void visit(Latte_Visitor &);

 protected:
  Refcounter<Latte_Obj> do_eval(Latte_Activation &);
};

class LatteHtml_HtmlVisitor : public Latte_Visitor {
 public:
  LatteHtml_HtmlVisitor(ostream &out, bool close_par = 0) :
    m_out(out), m_close_par(close_par) {}

  void visit_str(Latte_Str &);

 private:
  ostream &m_out;
  bool m_close_par;
};

extern void
latte_html(const char *lang,
           bool strict, bool fragment, bool no_default, bool close_par,
           unsigned long log_flags,
           const deque<latte_string>::const_iterator &loads_begin,
           const deque<latte_string>::const_iterator &loads_end,
           const char *filename, istream &in, ostream &out,
           Latte_Activation *global_activation = 0);

extern void
latte_html_init_globals(Latte_Activation &, bool, bool,
                        const deque<latte_string>::const_iterator &,
                        const deque<latte_string>::const_iterator &);

#endif // LATTE_HTML_H
