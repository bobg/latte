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
#include <latte-log.h>
#include <restorer.h>

void
latte_init(bool path)
{
  latte_log_flags = 0;
  if (path)
    latte_default_path();
}

Refcounter<Latte_Obj>
Latte_Obj::eval(Latte_Activation &activation)
{
  if (self_evaluating())
    return Refcounter<Latte_Obj>(this);

  static unsigned int depth = 0;
  Restorer<unsigned int> depth_restorer(depth);

  ++depth;

  if (latte_log_flags & latte_log_EVAL) {
    for (unsigned int i = 0; i < depth - 1; ++i)
      LatteLog(latte_log_EVAL) << ' ';
    LatteLog(latte_log_EVAL) << "> " << *this << endl;
  }

  Refcounter<Latte_Obj> val;

  try {

    val = do_eval(activation);

  } catch (const Latte_Error &err) {

    Latte_Tangible *tangible = as_tangible();

    if (tangible)
      err.add_fileloc(tangible->fileloc());

    throw;
  };

  if (latte_log_flags & latte_log_EVAL) {
    for (unsigned int i = 0; i < depth - 1; ++i)
      LatteLog(latte_log_EVAL) << ' ';
    LatteLog(latte_log_EVAL) << "< " << *val << endl;
  }

  return val;
}

Refcounter<Latte_Obj>
Latte_Obj::do_eval(Latte_Activation &)
{
  return Refcounter<Latte_Obj>(this);
}

void
Latte_Obj::render(ostream &out) const
{
  out << static_cast<const void *>(this);
}

ostream &
operator << (ostream &out, const Latte_Obj &obj)
{
  obj.render(out);
  return out;
}

static unsigned long extant_objs = 0;
static unsigned long ctor_dtor_calls = 0;
static unsigned long all_objs = 0;

static void
maybe_report_objs()
{
  if ((latte_log_flags & latte_log_MEM)
      && (!(ctor_dtor_calls % 100))) {
    LatteLog(latte_log_MEM) << "Latte_Objs: "
                            << extant_objs << '/' << all_objs << endl;
  }
}

Latte_Obj::Latte_Obj()
{
  ++extant_objs;
  ++all_objs;
  ++ctor_dtor_calls;

  maybe_report_objs();
}

Latte_Obj::~Latte_Obj()
{
  --extant_objs;
  ++ctor_dtor_calls;

  maybe_report_objs();
}

void
Latte_Error::add_fileloc(const Latte_FileLoc &loc) const
{
  if (m_backtrace.empty()) {
    if (loc == fileloc())
      return;
  } else if (loc == m_backtrace.back())
    return;

  mutate(m_backtrace).push_back(loc);
}
