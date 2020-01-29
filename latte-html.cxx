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

#include <latte-html.h>
#include <latte-log.h>
#include <restorer.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

static bool in_pre = 0;
static bool in_html = 0;

void
LatteHtml_PreObj::visit(Latte_Visitor &visitor)
{
  Restorer<bool> in_pre_restorer(in_pre);
  in_pre = 1;
  nested_obj()->visit(visitor);
}

Refcounter<Latte_Obj>
LatteHtml_PreObj::do_eval(Latte_Activation &activation)
{
  if (self_evaluating())
    return Refcounter<Latte_Obj>(this);
  return Refcounter<Latte_Obj>(new
                         LatteHtml_PreObj(*(nested_obj()->eval(activation))));
}

void
LatteHtml_HtmlObj::visit(Latte_Visitor &visitor)
{
  Restorer<bool> in_html_restorer(in_html);
  in_html = 1;
  nested_obj()->visit(visitor);
}

Refcounter<Latte_Obj>
LatteHtml_HtmlObj::do_eval(Latte_Activation &activation)
{
  if (self_evaluating())
    return Refcounter<Latte_Obj>(this);
  return Refcounter<Latte_Obj>(new
                        LatteHtml_HtmlObj(*(nested_obj()->eval(activation))));
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
                           LatteHtml_PreObj(*(Latte_WsNode::wrap(*result_list,
                                                                   wstate))));
  }
};

class HtmlOp : public Latte_Operator {
 public:
  const shstring &name() const { static const shstring n = "html"; return n; }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &wstate,
			      const Latte_FileLoc &fileloc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    Refcounter<Latte_List> result_list(new Latte_List);

    for (Latte_List::const_iterator i = args; i != args_end; ++i)
      result_list->push_back((*i)->eval(activation));

    Refcounter<Latte_WsNode> wsnode(new Latte_WsNode(*result_list, wstate));

    return Refcounter<Latte_Obj>(new
                          LatteHtml_HtmlObj(*(Latte_WsNode::wrap(*result_list,
                                                                 wstate))));
  }
};

void
LatteHtml_HtmlVisitor::visit_str(Latte_Str &str)
{
  static bool ever_called = 0;
  static bool p_begun = 0;
  const Latte_Wstate &ws = suggest_wstate(str.wstate());

  if (ever_called) {
    bool do_p = (!in_pre && (ws.newlines() > 1));

    if (do_p && p_begun && m_close_par)
      m_out << "</p>";
    m_out << ws;
    if (do_p) {
      m_out << "<p>";
      p_begun = 1;
    }
  }

  ever_called = 1;

  if (in_html)
    m_out << str.str();
  else
    for (Latte_Str::const_iterator i = str.begin(); i != str.end(); ++i) {
      const char &c = *i;

      switch (c) {
       case '<':
        m_out << "&lt;";
        break;
       case '>':
        m_out << "&gt;";
        break;
       case '&':
        m_out << "&amp;";
        break;
       case '"':
        m_out << "&quot;";
        break;
       default:
        m_out << c;
        break;
      }
    }
}

void
latte_html_init_globals(Latte_Activation &activation,
                        bool no_default, bool strict,
                        const deque<latte_string>::const_iterator &loads_begin,
                        const deque<latte_string>::const_iterator &loads_end)
{
  activation.install_standard_definitions();

  activation.define("html", Refcounter<Latte_Obj>(new HtmlOp));
  activation.define("_pre", Refcounter<Latte_Obj>(new PreOp));
  activation.define("strict-html4", strict ? latte_true() : latte_false());

  if (!no_default) {
    latte_load_library("standard", activation);
    latte_load_library("html", activation);
  }

  for (deque<latte_string>::const_iterator i = loads_begin;
       i != loads_end;
       ++i)
    latte_load_library(*i, activation);
}

void
latte_html(const char *lang,
           bool strict, bool fragment, bool no_default, bool close_par,
           unsigned long log_flags,
           const deque<latte_string>::const_iterator &loads_begin,
           const deque<latte_string>::const_iterator &loads_end,
           const char *filename, istream &in, ostream &out,
           Latte_Activation *global_activation)
{
  srandom(getpid() ^ time(0));  // should the UI do this?

  if (!global_activation) {
    global_activation = new Latte_Activation;
    latte_html_init_globals(*global_activation, no_default, strict,
                            loads_begin, loads_end);
  }

  Latte_Reader reader(in, filename, *global_activation);

  reader.define_global("__FILE__",
                       Refcounter<Latte_Obj>(new
                                             Latte_Str(Latte_Wstate(),
                                                       Latte_FileLoc(),
                                                       filename)));

  latte_log_flags = log_flags;

  LatteHtml_HtmlVisitor visitor(out, close_par);

  if (!fragment) {
    out << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0";

    if (!strict)
      out << " Transitional";

    out << "//EN\"" << endl;
    out << "                      \"http://www.w3.org/TR/REC-html40/"
         << (strict ? "strict" : "loose")
         << ".dtd\">"
         << endl;
    out << "<!-- This document was created with Latte version "
         << latte_version << " -->" << endl;
    out << "<!-- For information on Latte, see "
         << latte_url << " -->" << endl;

    out << "<html";
    if (lang)
      out << " lang=\"" << lang << "\"";
    out << ">" << endl;
  }

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
  if (!fragment)
    out << "</html>" << endl;
}
