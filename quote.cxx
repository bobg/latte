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

Latte_Quote::Latte_Quote(const Latte_Wstate &ws,
                         const Latte_FileLoc &loc,
                         enum Latte_Quote::Type type) :
  Latte_Tangible(ws, loc), m_type(type) {}

Latte_Quote::Latte_Quote(const Latte_Quote &other) :
  Latte_Tangible(other), m_obj(other.m_obj), m_type(other.m_type) {}

void
Latte_Quote::set_obj(const Refcounter<Latte_Obj> &obj)
{
  m_obj = obj;
}

void
Latte_Quote::visit(Latte_Visitor &visitor)
{
  visitor.visit_quote(*this);
}

void
Latte_Quote::render(ostream &out) const
{
  out << '\\';

  switch (m_type) {
   case type_quote:
    out << '\'';
    break;

   case type_quasiquote:
    out << '`';
    break;

   case type_unquote:
    out << ',';
    break;

   case type_unquote_splicing:
    out << ",@";
    break;
  }

  out << *m_obj;
}

typedef pair<Refcounter<Latte_Obj>, bool> qqpair;

static qqpair
quasiquote(const Refcounter<Latte_Obj> &obj,
           Latte_Activation &activation)
{
  Latte_Group *group = obj->as_group();
  Latte_Nested *nested = obj->as_nested();
  Latte_Quote *quote = obj->as_quote();
  Latte_VarRef *varref = obj->as_varref();

  if (group) {
    Refcounter<Latte_Group> result(new Latte_Group(group->wstate(),
                                                   group->fileloc()));

    for (Latte_Group::const_iterator i = group->begin();
         i != group->end();
         ++i) {
      const Refcounter<Latte_Obj> &elt = *i;
      qqpair q = quasiquote(elt, activation);
      Latte_List *qlist = q.first->as_list();

      if (qlist && q.second)
        result->append(qlist->begin(), qlist->end());
      else
        result->push_back(q.first);
    }

    return qqpair(Refcounter<Latte_Obj>(result.get()), 0);
  }

  if (quote) {
    switch (quote->type()) {
     case Latte_Quote::type_quote: {
       Refcounter<Latte_Quote> result(new Latte_Quote(*quote));

       result->set_obj(quasiquote(quote->obj(), activation).first);

       return qqpair(Refcounter<Latte_Obj>(result.get()), 0);
     }

     case Latte_Quote::type_quasiquote:
      return qqpair(obj, 0);

     case Latte_Quote::type_unquote: {
       Refcounter<Latte_Obj> val = quote->obj()->eval(activation);

       return qqpair(Refcounter<Latte_Obj>(new Latte_WsNode(*val,
                                                            quote->wstate())),
                     0);
     }

     case Latte_Quote::type_unquote_splicing: {
       Refcounter<Latte_Obj> val = quote->obj()->eval(activation);

       return qqpair(Refcounter<Latte_Obj>(new Latte_WsNode(*val,
                                                            quote->wstate())),
                     1);
     }

     // xxx not reached

    }
  }

  if (varref)
    return qqpair(Refcounter<Latte_Obj>(new Latte_VarRef(*varref)), 0);

  if (nested) {
    Refcounter<Latte_Nested> result(new Latte_Nested(*nested));
    qqpair q = quasiquote(nested->nested_obj(), activation);
    result->set_nested_obj(q.first);
    return qqpair(Refcounter<Latte_Obj>(result.get()), 0);
  }

  return qqpair(obj, 0);
}

Refcounter<Latte_Obj>
Latte_Quote::do_eval(Latte_Activation &activation)
{
  switch (m_type) {
   case type_quote:
    return m_obj;

   case type_quasiquote:
    return quasiquote(m_obj, activation).first;
  }

  // xxx error
}

bool
Latte_Quote::side_effects() const
{
  return ((m_type == type_quote) ? 0 : m_obj->side_effects());
}
