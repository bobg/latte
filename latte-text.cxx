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

#include <latte-text.h>
#include <latte-log.h>
#include <restorer.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

static bool in_pre = 0;
static unsigned int left_margin = 0;

void
LatteText_PreObj::visit(Latte_Visitor &visitor)
{
  Restorer<bool> in_pre_restorer(in_pre);
  in_pre = 1;
  nested_obj()->visit(visitor);
}

Refcounter<Latte_Obj>
LatteText_PreObj::do_eval(Latte_Activation &activation)
{
  if (self_evaluating())
    return Refcounter<Latte_Obj>(this);
  return Refcounter<Latte_Obj>(new
                         LatteText_PreObj(*(nested_obj()->eval(activation))));
}

void
LatteText_IndentObj::visit(Latte_Visitor &visitor)
{
  Restorer<unsigned int> left_margin_restorer(left_margin);
  left_margin += m_level;
  nested_obj()->visit(visitor);
}

Refcounter<Latte_Obj>
LatteText_IndentObj::do_eval(Latte_Activation &activation)
{
  if (self_evaluating())
    return Refcounter<Latte_Obj>(this);
  return Refcounter<Latte_Obj>(new
                      LatteText_IndentObj(*(nested_obj()->eval(activation))));
}

class PreOp : public Latte_Operator {
 public:
  const shstring &name() const { static const shstring n = "_pre"; return n; }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &wstate,
			      const Latte_FileLoc &fileloc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    Refcounter<Latte_List> result_list(new Latte_List);

    for (Latte_List::const_iterator i = args; i != args_end; ++i)
      result_list->push_back((*i)->eval(activation));

    return Refcounter<Latte_Obj>(new
                           LatteText_PreObj(*(Latte_WsNode::wrap(*result_list,
                                                                 wstate))));
  }
};

class IndentOp : public Latte_Operator {
 public:
  const shstring &name() const {
    static const shstring n = "_indent";
    return n;
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &wstate,
			      const Latte_FileLoc &fileloc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    Refcounter<Latte_List> result_list(new Latte_List);

    Refcounter<Latte_Obj> level_arg;

    for (Latte_List::const_iterator i = args; i != args_end; ++i) {
      if ((*i)->as_assignment()) {
        const Latte_Assignment *assignment = (*i)->as_assignment();

        if (assignment->name().str().compare("level") == 0)
          level_arg = assignment->nested_obj();
        else
          throw IllegalAssignment(*this, assignment->name(), fileloc);
      } else
        result_list->push_back((*i)->eval(activation));
    }

    unsigned int level = 2;

    if (level_arg.get()) {
      Refcounter<Latte_Obj> level_val = level_arg->eval(activation);
      level = static_cast<unsigned int>(level_val->numeric_val());
    }

    return Refcounter<Latte_Obj>(new
                        LatteText_IndentObj(*(Latte_WsNode::wrap(*result_list,
                                                                 wstate)),
                                            level));
  }
};

void
LatteText_TextVisitor::visit_str(Latte_Str &str)
{
  Latte_Wstate ws = suggest_wstate(str.wstate());
  static bool ever_called = 0;

  if (in_pre) {
    cout << ws;
    if (ws.newlines() > 0)
      m_column = 0;
    m_column += ws.spaces();
  } else if (ws.newlines() > 1) {
    if (ever_called)
      m_out << endl << endl;
    m_column = 0;
  } else if (ws.newlines() == 1) {
    ws = Latte_Wstate();
    ws.space(m_new_sentence ? 2 : 1);
  }

  ever_called = 1;

  m_new_sentence = 0;
  if (m_sentence_end_chars) {
    shstring::const_iterator p = str.end();
    do {
      --p;
      if (!m_sentence_end_transparent_chars
          || !strchr(m_sentence_end_transparent_chars, *p)) {
        if (strchr(m_sentence_end_chars, *p))
          m_new_sentence = 1;
        break;
      }
    } while (p != str.begin());
  }

  if (!in_pre && (m_column > left_margin)) {
    unsigned int new_column = m_column + ws.spaces();
    new_column += str.size();
    if (new_column < m_wrapcol) {
      m_out << ws << str.str();
      m_column = new_column;
      return;
    }
    m_out << endl;
    m_column = 0;
  }

  // in_pre || (m_column == 0)

  while (m_column < left_margin) {
    m_out << ' ';
    ++m_column;
  }

  m_out << str.str();
  m_column += str.size();
}

void
latte_text_init_globals(Latte_Activation &activation,
                        bool no_default,
                        const deque<latte_string>::const_iterator &loads_begin,
                        const deque<latte_string>::const_iterator &loads_end)
{
  activation.install_standard_definitions();

  activation.define("_pre", Refcounter<Latte_Obj>(new PreOp));
  activation.define("_indent", Refcounter<Latte_Obj>(new IndentOp));

  if (!no_default) {
    latte_load_library("standard", activation);
    latte_load_library("text", activation);
  }

  for (deque<latte_string>::const_iterator i = loads_begin;
       i != loads_end;
       ++i)
    latte_load_library(*i, activation);
}

void
latte_text(bool no_default,
           unsigned long log_flags,
           const deque<latte_string>::const_iterator &loads_begin,
           const deque<latte_string>::const_iterator &loads_end,
           unsigned int wrapcol,
           const char *sentence_end_chars,
           const char *sentence_end_transparent_chars,
           const char *filename, istream &in, ostream &out,
           Latte_Activation *global_activation)
{
  srandom(getpid() ^ time(0));  // should the UI do this?
  latte_init(1);

  if (!global_activation) {
    global_activation = new Latte_Activation;
    latte_text_init_globals(*global_activation, no_default,
                            loads_begin, loads_end);
  }

  Latte_Reader reader(in, filename, *global_activation);

  latte_log_flags = log_flags;

  reader.define_global("__FILE__",
                       Refcounter<Latte_Obj>(new
                                             Latte_Str(Latte_Wstate(),
                                                       Latte_FileLoc(),
                                                       filename)));

  LatteText_TextVisitor visitor(out, wrapcol,
                                sentence_end_chars,
                                sentence_end_transparent_chars);

  reader.process(visitor);

#if 0
  if (!reader.processed()) {
    const Latte_Obj &unprocessed = reader.unprocessed();

    const Latte_Tangible *tangible = unprocessed.as_tangible();
    const Latte_Group *group = unprocessed.as_group();
    const Latte_Assignment *assignment = unprocessed.as_assignment();

    if (group)
      throw Latte_Reader::IncompleteGroup(group->fileloc());
    if (assignment)
      throw Latte_Reader::IncompleteAssignment(assignment->fileloc());
    if (tangible)
      throw Latte_Reader::Incomplete(tangible->fileloc());

    throw Latte_Reader::Incomplete(Latte_FileLoc());
  }
#endif

  out << endl;
}
