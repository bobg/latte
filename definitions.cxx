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
#include <latte-log.h>
#include <latte-fstream.h>
#include <cstdio>
#include <cstdlib>
#include <cctype>

#ifdef HAVE_WAIT_H
# include <wait.h>
#endif // HAVE_WAIT_H

#ifdef ENABLE_FLOATING_POINT
# include <cmath>
#endif // ENABLE_FLOATING_POINT

const shstring &
Latte_Lambda::name() const
{
  static const shstring lambda_name = "lambda";

  return lambda_name;
}

Refcounter<Latte_Obj>
Latte_Lambda::apply(const Latte_Wstate &ws,
		    const Latte_FileLoc &loc,
		    const Latte_List::const_iterator &args,
		    const Latte_List::const_iterator &args_end,
		    Latte_Activation &activation) const
{
  Latte_List::const_iterator i = args;

  if (i == args_end)
    throw NoParamList(loc);

  const Refcounter<Latte_Obj> &param_obj = *i++;
  Latte_List *param_list = param_obj->as_list();

  if (!param_list)
    throw NoParamList(loc);

  Refcounter<Latte_Closure> closure(new Latte_Closure(param_list->begin(),
						      param_list->end(),
						      i, args_end,
						      activation));

  return Refcounter<Latte_Obj>(closure.get());
}

class MacroOp : public Latte_Lambda {
  const shstring &name() const {
    static const shstring n = "macro";
    return n;
  }
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
			      const Latte_FileLoc &loc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    Refcounter<Latte_Obj> result = Latte_Lambda::apply(ws, loc,
                                                       args, args_end,
                                                       activation);
    Latte_Operator *op = result->as_operator();

    if (op) {
      Latte_Closure *closure = op->as_closure();

      if (closure)                // xxx else impossible?
        closure->set_macro(1);
    }

    return result;
  }
};

class DefOp : public Latte_Operator {
 public:
  DefOp() : Latte_Operator(0) {}

  const shstring &name() const { static const shstring n = "def"; return n; }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
			      const Latte_FileLoc &loc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    Latte_List::const_iterator i = args;

    if (i == args_end)
      throw InsufficientArgs(*this, loc);

    const Refcounter<Latte_Obj> &definee = *i++;
    Latte_VarRef *varref = definee->as_varref();
    Latte_List *list = definee->as_list();

    shstring name;
    Refcounter<Latte_Obj> val;

    if (varref) {		// {\def \var VALUE}
      name = varref->name();
      val = Refcounter<Latte_Obj>((i == args_end) ?
				  latte_false() :
				  (*i)->eval(activation));
    } else if (list) {		// {\def {\name ARG ...} BODY ...}
      Latte_List::const_iterator g = list->begin();

      if (g == list->end())
	throw BadType(*this, 0, loc);

      const Refcounter<Latte_Obj> &gfirst = *g++;
      Latte_VarRef *gvarref = gfirst->as_varref();

      if (!gvarref)
	throw BadType(*this, 0, loc);

      name = gvarref->name();
      val = Refcounter<Latte_Obj>(new Latte_Closure(g, list->end(),
						    i, args_end,
						    activation));
    } else
      throw BadType(*this, 0, loc);

    activation.define(name, val);

    return latte_false();
  }
};

class AddOp : public Latte_Operator {
 public:
  AddOp(bool add) : m_add(add) {}

  const shstring &name() const {
    static const shstring a = "add";
    static const shstring s = "subtract";

    return (m_add ? a : s);
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
			      const Latte_FileLoc &loc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    if (args == args_end)
      return Refcounter<Latte_Obj>(new Latte_Str(ws, loc, 0));

    Latte_List::const_iterator i = args;
    const Refcounter<Latte_Obj> &obj = *i;
    Latte_Number_t result = obj->numeric_val();

    bool subtrahend = 0;

    while (++i != args_end) {
      subtrahend = 1;

      const Refcounter<Latte_Obj> &obj = *i;
      Latte_Number_t num = obj->numeric_val();

      if (m_add)
        result += num;
      else
        result -= num;
    }

    if (!m_add && !subtrahend)
      result = (-result);

    return Refcounter<Latte_Obj>(new Latte_Str(ws, loc, result));
  }

 private:
  bool m_add;
};

class MultiplyOp : public Latte_Operator {
 public:
  MultiplyOp(bool multiply) : m_multiply(multiply) {}

  const shstring &name() const {
    static const shstring m = "multiply";
    static const shstring d = "divide";

    return (m_multiply ? m : d);
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
			      const Latte_FileLoc &loc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    if (args == args_end)
      return Refcounter<Latte_Obj>(new Latte_Str(ws, loc, 0));

    Latte_List::const_iterator i = args;
    const Refcounter<Latte_Obj> &obj = *i;
    Latte_Number_t result = obj->numeric_val();

    long argno = 1;

    while (++i != args_end) {
      const Refcounter<Latte_Obj> &obj = *i;
      Latte_Number_t num = obj->numeric_val();

      if (m_multiply)
        result *= num;
      else if (num == 0)
        throw OutOfRange(*this, argno, loc);
      else
        result /= num;

      ++argno;
    }

    return Refcounter<Latte_Obj>(new Latte_Str(ws, loc, result));
  }

 private:
  bool m_multiply;
};

class ModuloOp : public Latte_Operator {
 public:
  const shstring &name() const {
    static const shstring n = "modulo";
    return n;
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
			      const Latte_FileLoc &loc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    Latte_List::const_iterator i = args;

    if (i == args_end)
      throw InsufficientArgs(*this, loc);

    const Refcounter<Latte_Obj> &a = *i++;

    if (i == args_end)
      throw InsufficientArgs(*this, loc);

    const Refcounter<Latte_Obj> &b = *i;

    long a_num = static_cast<long>(a->numeric_val());

    long b_num = static_cast<long>(b->numeric_val());

    if (b_num == 0)
      throw OutOfRange(*this, 1, loc);

    return Refcounter<Latte_Obj>(new Latte_Str(ws, loc, a_num % b_num));
  }
};

class ConsOp : public Latte_Operator {
 public:
  ConsOp(bool front) : m_front(front) {}

  const shstring &name() const {
    static const shstring c = "cons";
    static const shstring p = "push-back";

    return (m_front ? c : p);
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
			      const Latte_FileLoc &loc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    Latte_List::const_iterator i = args;
    if (i == args_end)
      throw InsufficientArgs(*this, loc);

    const Refcounter<Latte_Obj> &first = *i++;

    if (i == args_end)
      throw InsufficientArgs(*this, loc);

    const Refcounter<Latte_Obj> &second = *i;

    const Latte_List *second_list = second->as_list();
    if (!second_list)
      throw BadType(*this, 1, loc);

    Refcounter<Latte_List> result_list(new Latte_List);

    if (m_front) {
      Latte_Tangible *tangible = second->as_tangible();

      if (tangible) {
        Latte_List::const_iterator j = second_list->begin();

        if (j != second_list->end()) {
          result_list->push_back(Latte_WsNode::wrap(**j, tangible->wstate()));
          result_list->append(++j, second_list->end());
        }
      } else
        result_list->append(second_list->begin(),
                            second_list->end());

      result_list->push_front(first);
    } else {
      result_list->append(second_list->begin(),
                          second_list->end());
      result_list->push_back(first);
    }
    return Latte_WsNode::wrap(*result_list, ws);
  }

 private:
  bool m_front;
};

class SetOp : public Latte_Operator {
 public:
  SetOp() : Latte_Operator(0) {}

  const shstring &name() const { static const shstring n = "set!"; return n; }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
			      const Latte_FileLoc &loc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    Latte_List::const_iterator i = args;

    if (i == args_end)
      throw InsufficientArgs(*this, loc);

    const Refcounter<Latte_Obj> &obj = *i++;
    Latte_VarRef *varref = obj->as_varref();

    if (!varref)
      throw BadType(*this, 0, loc);

    if (i == args_end)
      throw InsufficientArgs(*this, loc);

    const Refcounter<Latte_Obj> &valobj = *i;

    varref->lookup_rw(activation) = valobj->eval(activation);

    return latte_false();
  }
};

class IfOp : public Latte_Operator {
 public:
  IfOp() : Latte_Operator(0) {}

  const shstring &name() const { static const shstring n = "if"; return n; }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
			      const Latte_FileLoc &loc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    Latte_List::const_iterator i = args;
    if (i == args_end)
      throw InsufficientArgs(*this, loc);
    const Refcounter<Latte_Obj> &test_obj = *i++;
    Refcounter<Latte_Obj> val = test_obj->eval(activation);

    if (i != args_end) {
      if (val->bool_val())
	val = (*i)->eval(activation);
      else if (++i != args_end)
        val = (*i)->eval(activation);
    }

    return Latte_WsNode::wrap(*val, ws);
  }
};

class WhileOp : public Latte_Operator {
 public:
  WhileOp() : Latte_Operator(0) {}

  const shstring &name() const { static const shstring n = "while"; return n; }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
                              const Latte_FileLoc &loc,
                              const Latte_List::const_iterator &args,
                              const Latte_List::const_iterator &args_end,
                              Latte_Activation &activation) const {
    Latte_List::const_iterator i = args;
    if (i == args_end)
      throw InsufficientArgs(*this, loc);
    const Refcounter<Latte_Obj> &test_obj = *i++;
    const Latte_List::const_iterator body_start = i;

    Refcounter<Latte_List> result(new Latte_List);

    while (1) {
      Refcounter<Latte_Obj> val = test_obj->eval(activation);
      if (!(val->bool_val()))
        break;
      for (i = body_start; i != args_end; ++i) {
        const Refcounter<Latte_Obj> &body_obj = *i;
        Refcounter<Latte_Obj> body_val = body_obj->eval(activation);

        if (body_val->bool_val())
          result->push_back(body_val);
      }
    }

    return Latte_WsNode::wrap(*result, ws);
  }
};

class NotOp : public Latte_Operator {
 public:
  const shstring &name() const { static const shstring n = "not"; return n; }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
			      const Latte_FileLoc &loc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    Latte_List::const_iterator i = args;
    if (i == args_end)
      throw InsufficientArgs(*this, loc);
    const Refcounter<Latte_Obj> &obj = *i;
    return (obj->bool_val() ? latte_false() : latte_true());
  }
};

class ApplyOp : public Latte_Operator {
 public:
  ApplyOp(bool funcall) : m_funcall(funcall) {}

  const shstring &name() const {
    static const shstring a = "apply";
    static const shstring f = "funcall";

    return m_funcall ? f : a;
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
			      const Latte_FileLoc &loc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    Latte_List::const_iterator i = args;
    if (i == args_end)
      throw InsufficientArgs(*this, loc);
    const Refcounter<Latte_Obj> &op_obj = *i++;
    Latte_Operator *op = op_obj->as_operator();

    if (!op)
      throw BadType(*this, 0, loc);

    Refcounter<Latte_List> new_args(new Latte_List);

    while (i != args_end) {
      const Refcounter<Latte_Obj> &arg = *i++;

      if (!m_funcall && (i == args_end)) {
	// treat last arg specially: if it's a group/list, its members get
	// added to new_args.
        const Latte_List *arg_list = arg->as_list();
	if (arg_list)
	  new_args->append(arg_list->begin(), arg_list->end());
	else
	  new_args->push_back(arg);
      } else
	new_args->push_back(arg);
    }

    return op->call_apply(ws, loc,
			  new_args->begin(), new_args->end(),
			  activation);
  }

 private:
  bool m_funcall;
};

class LoadFileOp : public Latte_Operator {
 public:
  LoadFileOp(bool file) : m_file(file) {}

  const shstring &name() const {
    static const shstring f = "load-file";
    static const shstring l = "load-library";
    return (m_file ? f : l);
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
			      const Latte_FileLoc &loc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    if (args == args_end)
      throw InsufficientArgs(*this, loc);

    Latte_Stringify sv(0);

    for (Latte_List::const_iterator i = args; i != args_end; ++i) {
      const Refcounter<Latte_Obj> &obj = *i;
      obj->visit(sv);
    }

    return (m_file ?
            latte_load_file(sv.str(), activation) :
            latte_load_library(sv.str(), activation));
  }

 private:
  bool m_file;
};

// This can be defined with a macro in terms of lambda, but this
// implementation is quite a bit faster
class LetOp : public Latte_Operator {
 public:
  LetOp() : Latte_Operator(0) {}

  const shstring &name() const { static const shstring n = "let"; return n; }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
			      const Latte_FileLoc &loc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    Latte_List::const_iterator i = args;
    if (i == args_end)
      throw InsufficientArgs(*this, loc);

    const Refcounter<Latte_Obj> &bindings_obj = *i++;
    Latte_List *bindings_list = bindings_obj->as_list();

    if (!bindings_list)
      throw BadType(*this, 0, loc);

    Refcounter<Latte_List> params(new Latte_List);
    Refcounter<Latte_List> vals(new Latte_List);

    for (Latte_List::const_iterator j = bindings_list->begin();
	 j != bindings_list->end();
	 ++j) {
      const Refcounter<Latte_Obj> &binding_obj = *j;
      Latte_List *binding_list = binding_obj->as_list();

      if (!binding_list)
	throw BadType(*this, 0, loc);

      Latte_List::const_iterator k = binding_list->begin();

      if (k == binding_list->end())
	throw BadType(*this, 0, loc);

      params->push_back(*k++);
      vals->push_back((k == binding_list->end()) ?
                      latte_false() : (*k)->eval(activation));
    }

    Refcounter<Latte_Closure> closure(new Latte_Closure(params->begin(),
							params->end(),
							i, args_end,
							activation));
    return closure->call_apply(ws, loc,
			       vals->begin(), vals->end(),
			       activation);
  }
};

class LengthOp : public Latte_Operator {
 public:
  const shstring &name() const {
    static const shstring n = "length";
    return n;
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
			      const Latte_FileLoc &loc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    Latte_List::const_iterator i = args;
    if (i == args_end)
      throw InsufficientArgs(*this, loc);

    const Refcounter<Latte_Obj> &obj = *i;

    if (obj->as_list())
      return Refcounter<Latte_Obj>(new Latte_Str(ws, loc,
						 obj->as_list()->size()));
    if (obj->as_str())
      return Refcounter<Latte_Obj>(new Latte_Str(ws, loc,
						 obj->as_str()->size()));

    throw BadType(*this, 0, loc);
  }
};

class IneqOp : public Latte_Operator {
 public:
  IneqOp(bool greater, bool equal) : m_greater(greater), m_equal(equal) {}

  const shstring &name() const {
    static const shstring ge = "greater-equal?";
    static const shstring gt = "greater?";
    static const shstring le = "less-equal?";
    static const shstring lt = "less?";

    if (m_greater) {
      if (m_equal)
        return ge;
      return gt;
    }
    if (m_equal)
      return le;
    return lt;
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
			      const Latte_FileLoc &loc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    Latte_List::const_iterator i = args;
    if (i == args_end)
      throw InsufficientArgs(*this, loc);

    const Refcounter<Latte_Obj> &obj = *i++;
    Latte_Number_t val = obj->numeric_val();

    while (i != args_end) {
      Latte_Number_t newval = (*i++)->numeric_val();

      if (m_greater) {
        if (m_equal) {
          if (!(val >= newval))
            return latte_false();
        } else {
          if (!(val > newval))
            return latte_false();
        }
      } else {
        if (m_equal) {
          if (!(val <= newval))
            return latte_false();
        } else {
          if (!(val < newval))
            return latte_false();
        }
      }

      val = newval;
    }
    return latte_true();
  }

 private:
  bool m_greater, m_equal;
};

class StrIneqOp : public Latte_Operator {
 public:
  StrIneqOp(bool greater, bool equal) : m_greater(greater), m_equal(equal) {}

  const shstring &name() const {
    static const shstring ge = "string-greater-equal?";
    static const shstring gt = "string-greater?";
    static const shstring le = "string-less-equal?";
    static const shstring lt = "string-less?";

    if (m_greater) {
      if (m_equal)
        return ge;
      return gt;
    }
    if (m_equal)
      return le;
    return lt;
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
			      const Latte_FileLoc &loc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    Latte_List::const_iterator i = args;
    if (i == args_end)
      throw InsufficientArgs(*this, loc);

    const Refcounter<Latte_Obj> &obj = *i++;
    Latte_Str *refstr = obj->as_str();

    long argno = 0;

    if (!refstr)
      throw BadType(*this, 0, loc);

    while (i != args_end) {
      ++argno;

      Latte_Str *newstr = (*i++)->as_str();

      if (!newstr)
        throw BadType(*this, argno, loc);

      if (m_greater) {
        if (m_equal) {
          if (!(*refstr >= *newstr))
            return latte_false();
        } else {
          if (!(*refstr > *newstr))
            return latte_false();
        }
      } else {
        if (m_equal) {
          if (!(*refstr <= *newstr))
            return latte_false();
        } else {
          if (!(*refstr < *newstr))
            return latte_false();
        }
      }

      refstr = newstr;
    }
    return latte_true();
  }

 private:
  bool m_greater, m_equal;
};

class AndOp : public Latte_Operator {
 public:
  AndOp() : Latte_Operator(0) {}

  const shstring &name() const { static const shstring n = "and"; return n; }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
			      const Latte_FileLoc &loc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    Refcounter<Latte_Obj> result = latte_true();

    Latte_List::const_iterator i = args;
    while ((i != args_end) && result->bool_val())
      result = (*i++)->eval(activation);

    return Latte_WsNode::wrap(*result, ws);
  }
};

class OrOp : public Latte_Operator {
 public:
  OrOp() : Latte_Operator(0) {}

  const shstring &name() const { static const shstring n = "or"; return n; }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
			      const Latte_FileLoc &loc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    Refcounter<Latte_Obj> result = latte_false();

    Latte_List::const_iterator i = args;
    while ((i != args_end) && !(result->bool_val()))
      result = (*i++)->eval(activation);

    return Latte_WsNode::wrap(*result, ws);
  }
};

class WarnOp;

class WarnOpVisitor : public Latte_Visitor {
 public:
  void visit_str(Latte_Str &str) {
    cerr << suggest_wstate(str.wstate()) << str.str();
  }
};

class WarnOp : public Latte_Operator {
 public:
  WarnOp(bool die) : m_die(die) {}

  const shstring &name() const {
    static const shstring w = "warn";
    static const shstring e = "error";

    return (m_die ? e : w);
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
                              const Latte_FileLoc &loc,
                              const Latte_List::const_iterator &args,
                              const Latte_List::const_iterator &args_end,
                              Latte_Activation &activation) const {
    cerr << endl;               // ensure new line

    if (m_die)
      cerr << "Error: ";        // don't print loc; that gets thrown
    else
      cerr << "Warning at " << loc << ": ";

    for (Latte_List::const_iterator i = args; i != args_end; ++i) {
      const Refcounter<Latte_Obj> &obj = *i;
      WarnOpVisitor wov;
      obj->visit(wov);
    }
    cerr << endl;

    if (m_die)
      throw Latte_Error(loc);   // no more detail than that

    return latte_false();
  }

 private:
  bool m_die;
};

class RandomOp : public Latte_Operator {
 public:
  const shstring &name() const {
    static const shstring n = "random";
    return n;
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
			      const Latte_FileLoc &loc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    if (args == args_end)
      throw InsufficientArgs(*this, loc);

    const Refcounter<Latte_Obj> &obj = *args;
    long val = static_cast<long>(obj->numeric_val());

    if (val < 1)
      throw OutOfRange(*this, 0, loc);

    char buf[32];
    sprintf(buf, "%ld", random() % val);
    return Refcounter<Latte_Obj>(new Latte_Str(ws, loc, shstring(buf)));
  }
};

class FileContentsOp : public Latte_Operator {
 public:
  const shstring &name() const {
    static const shstring n = "file-contents";
    return n;
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
                              const Latte_FileLoc &loc,
                              const Latte_List::const_iterator &args,
                              const Latte_List::const_iterator &args_end,
                              Latte_Activation &activation) const {
    if (args == args_end)
      throw InsufficientArgs(*this, loc);

    Latte_Stringify sv(0);

    for (Latte_List::const_iterator i = args; i != args_end; ++i) {
      const Refcounter<Latte_Obj> &obj = *i;
      obj->visit(sv);
    }

    ifstream fstrm(sv.str().c_str());
    if (fstrm.fail())
      throw Latte_FileError(loc, sv.str());

    latte_string result;
    char buf[1024];
    size_t bytes;

    while (1) {
      fstrm.read(buf, sizeof buf);
      bytes = fstrm.gcount();

      if (!bytes)
        break;

      result.append(buf, bytes);
    }

    fstrm.close();

    return Refcounter<Latte_Obj>(new Latte_Str(ws, loc, shstring(result)));
  }
};

#ifdef HAVE_WAIT_H
class ProcessOutputOp : public Latte_Operator {
 public:
  const shstring &name() const {
    static const shstring n = "process-output";
    return n;
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
                              const Latte_FileLoc &loc,
                              const Latte_List::const_iterator &args,
                              const Latte_List::const_iterator &args_end,
                              Latte_Activation &activation) const {
    Latte_Stringify sv(1);

    for (Latte_List::const_iterator i = args; i != args_end; ++i) {
      const Refcounter<Latte_Obj> &obj = *i;
      obj->visit(sv);
    }

    FILE *pstrm = popen(sv.str().c_str(), "r");
    if (!pstrm)
      throw Latte_SysError(loc);

    latte_string result;
    char buf[1024];
    size_t bytes;

    while (1) {
      bytes = fread(buf, 1, sizeof buf, pstrm);

      if (!bytes)
        break;

      result.append(buf, bytes);
    }

    int status = pclose(pstrm);

    if (!WIFEXITED(status) || WEXITSTATUS(status))
      throw Latte_SysError(loc);

    if ((result.size() > 0)
        && (result[result.size() - 1] == 10)) // linefeed
      result.erase(result.size() - 1);
    if ((result.size() > 0)
        && (result[result.size() - 1] == 13)) // carriage return
      result.erase(result.size() - 1);

    return Refcounter<Latte_Obj>(new Latte_Str(ws, loc, shstring(result)));
  }
};
#endif // HAVE_WAIT_H

class AppendOp : public Latte_Operator {
 public:
  const shstring &name() const {
    static const shstring n = "append";
    return n;
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
                              const Latte_FileLoc &loc,
                              const Latte_List::const_iterator &args,
                              const Latte_List::const_iterator &args_end,
                              Latte_Activation &activation) const {
    Refcounter<Latte_List> result(new Latte_List);

    for (Latte_List::const_iterator i = args; i != args_end; ++i) {
      const Refcounter<Latte_Obj> &obj = *i;

      const Latte_List *val_list = obj->as_list();
      if (val_list) {
        Latte_Tangible *tangible = obj->as_tangible();

        if (tangible) {
          Latte_List::const_iterator j = val_list->begin();

          if (j != val_list->end()) {
            result->push_back(Latte_WsNode::wrap(**j, tangible->wstate()));
            result->append(++j, val_list->end());
          }
        } else
          result->append(val_list->begin(), val_list->end());
      } else
        result->push_back(obj);
    }

    return Latte_WsNode::wrap(*result, ws);
  }
};

class StringAppendOp : public Latte_Operator {
 public:
  const shstring &name() const {
    static const shstring n = "string-append";
    return n;
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
                              const Latte_FileLoc &loc,
                              const Latte_List::const_iterator &args,
                              const Latte_List::const_iterator &args_end,
                              Latte_Activation &activation) const {
    latte_string result;

    long argno = 0;
    for (Latte_List::const_iterator i = args; i != args_end; ++i, ++argno) {
      const Refcounter<Latte_Obj> &obj = *i;

      if (!(obj->as_str()))
        throw BadType(*this, argno, loc);

      result += obj->as_str()->str().str();
    }

    return Refcounter<Latte_Obj>(new Latte_Str(ws, loc, result));
  }
};

class TypePredicateOp : public Latte_Operator {
 public:
  enum type_type {
    tp_string, tp_group, tp_operator
  };

  TypePredicateOp(type_type type) : m_type(type) {}

  const shstring &name() const {
    static const shstring s = "string?";
    static const shstring g = "group?";
    static const shstring o = "operator?";

    if (m_type == tp_string)
      return s;
    if (m_type == tp_group)
      return g;
    // assume tp_operator
    return o;
  }

 protected:  
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
                              const Latte_FileLoc &loc,
                              const Latte_List::const_iterator &args,
                              const Latte_List::const_iterator &args_end,
                              Latte_Activation &activation) const {
    if (args == args_end)
      throw InsufficientArgs(*this, loc);

    const Refcounter<Latte_Obj> &obj = *args;

    if (m_type == tp_group)
      return (obj->as_list() ? latte_true() : latte_false());

    if (m_type == tp_string)
      return (obj->as_str() ? latte_true() : latte_false());

    // assume tp_operator
    return (obj->as_operator() ? latte_true() : latte_false());
  }

 private:
  type_type m_type;
};

class NthOp : public Latte_Operator {
 public:
  const shstring &name() const {
    static const shstring n = "nth";
    return n;
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
                              const Latte_FileLoc &loc,
                              const Latte_List::const_iterator &args,
                              const Latte_List::const_iterator &args_end,
                              Latte_Activation &activation) const {
    Latte_List::const_iterator i = args;

    if (i == args_end)
      throw InsufficientArgs(*this, loc);

    const Refcounter<Latte_Obj> &n_obj = *i++;
    if (i == args_end)
      throw InsufficientArgs(*this, loc);

    long n = static_cast<long>(n_obj->numeric_val());

    const Refcounter<Latte_Obj> &seq_obj = *i++;

    const Latte_List *seq_list = seq_obj->as_list();
    if (seq_list) {
      if (n < 0)
        n += seq_list->size();

      if ((n < 0) || (n >= seq_list->size()))
        throw OutOfRange(*this, 0, loc);

      return Latte_WsNode::wrap(*((*seq_list)[n]), ws);
    }

    const Latte_Str *seq_str = seq_obj->as_str();
    if (seq_str) {
      const shstring &shstr = *seq_str;

      if (n < 0)
        n += shstr.size();

      if ((n < 0) || (n >= shstr.size()))
        throw OutOfRange(*this, 0, loc);

      char x[2];
      x[0] = shstr.str()[n];
      x[1] = 0;

      return Refcounter<Latte_Obj>(new Latte_Str(ws, loc, shstring(x)));
    }

    throw BadType(*this, 1, loc);
  }
};

class CaseOp : public Latte_Operator {
 public:
  CaseOp(bool up) : m_up(up) {}

  const shstring &name() const {
    static const shstring d = "downcase";
    static const shstring u = "upcase";
    return m_up ? u : d;
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
                              const Latte_FileLoc &loc,
                              const Latte_List::const_iterator &args,
                              const Latte_List::const_iterator &args_end,
                              Latte_Activation &activation) const {
    Refcounter<Latte_List> result(new Latte_List);
    long argno = 0;

    for (Latte_List::const_iterator i = args; i != args_end; ++i, ++argno) {
      const Refcounter<Latte_Obj> &obj = *i;
      Latte_Str *valstr = obj->as_str();

      if (!valstr)
        throw BadType(*this, argno, loc);

      const shstring &shstr = *valstr;
      latte_string newstr;

      for (latte_string::const_iterator j = shstr.begin();
           j != shstr.end();
           ++j) {
        const char &c = *j;
        char c2 = c;

        if (isalpha(c)) {
          if (m_up && islower(c))
            c2 = toupper(c);
          else if (!m_up && isupper(c))
            c2 = tolower(c);
        }

        newstr.insert(newstr.end(), c2);
      }

      result->push_back(Refcounter<Latte_Obj>(new Latte_Str(valstr->wstate(),
                                                            valstr->fileloc(),
                                                            newstr)));
    }

    return Latte_WsNode::wrap(*result, ws);
  }

 private:
  bool m_up;
};

static bool
equalp(const Latte_Obj &a, const Latte_Obj &b)
{
  if (&a == &b)
    return 1;

  const Latte_Str *a_str = a.as_str();
  const Latte_Str *b_str = b.as_str();

  if (a_str)
    return (b_str &&
            ((a_str->null() && b_str->null())
             || (!(a_str->null())
                 && !(b_str->null())
                 && (a_str->str().str().compare(b_str->str().str()) == 0))));

  const Latte_List *a_list = a.as_list();
  const Latte_List *b_list = b.as_list();

  if (a_list && b_list) {
    Latte_List::const_iterator i = a_list->begin();
    Latte_List::const_iterator j = b_list->begin();

    while ((i != a_list->end()) && (j != b_list->end())) {
      const Refcounter<Latte_Obj> &p = *i++;
      const Refcounter<Latte_Obj> &q = *j++;
      if (!equalp(*p, *q))
        return 0;
    }

    return ((i == a_list->end()) && (j == b_list->end()));
  }

  return 0;
}

class EqualOp : public Latte_Operator {
 public:
  const shstring &name() const {
    static const shstring n = "equal?";
    return n;
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
                              const Latte_FileLoc &loc,
                              const Latte_List::const_iterator &args,
                              const Latte_List::const_iterator &args_end,
                              Latte_Activation &activation) const {
    Latte_List::const_iterator i = args;

    if (i == args_end)
      throw InsufficientArgs(*this, loc);

    const Refcounter<Latte_Obj> &obj = *i++;

    if (i == args_end)
      throw InsufficientArgs(*this, loc);

    do {
      if (!equalp(*obj, *(*i++)))
        return latte_false();
    } while (i != args_end);

    return latte_true();
  }
};

class SubseqOp : public Latte_Operator {
 public:
  const shstring &name() const {
    static const shstring n = "subseq";
    return n;
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
                              const Latte_FileLoc &loc,
                              const Latte_List::const_iterator &args,
                              const Latte_List::const_iterator &args_end,
                              Latte_Activation &activation) const {
    Latte_List::const_iterator i = args;

    if (i == args_end)
      throw InsufficientArgs(*this, loc);

    const Refcounter<Latte_Obj> &seq_obj = *i++;

    if (i == args_end)
      throw InsufficientArgs(*this, loc);

    const Refcounter<Latte_Obj> &from_obj = *i++;

    const Latte_List *seq_list = seq_obj->as_list();
    if (!seq_list)
      throw BadType(*this, 0, loc);

    long from = static_cast<long>(from_obj->numeric_val());

    bool have_to = (i != args_end);
    long to = 0;

    if (have_to) {
      const Refcounter<Latte_Obj> &to_obj = *i;
      to = static_cast<long>(to_obj->numeric_val());
    }

    if (from < 0)
      from += seq_list->size();

    if ((from < 0) || (from > seq_list->size()))
      throw OutOfRange(*this, 1, loc);

    if (have_to) {
      if (to < 0)
        to += seq_list->size();
      if ((to < 0) || (to > seq_list->size()) || (to < from))
        throw OutOfRange(*this, 2, loc);
    }

    Refcounter<Latte_List> result(new Latte_List(seq_list->begin() + from,
                                                 (have_to ?
                                                  (seq_list->begin() + to) :
                                                  seq_list->end())));

    return Latte_WsNode::wrap(*result, ws);
  }
};

class SubstrOp : public Latte_Operator {
 public:
  const shstring &name() const {
    static const shstring n = "substr";
    return n;
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
                              const Latte_FileLoc &loc,
                              const Latte_List::const_iterator &args,
                              const Latte_List::const_iterator &args_end,
                              Latte_Activation &activation) const {
    Latte_List::const_iterator i = args;

    if (i == args_end)
      throw InsufficientArgs(*this, loc);

    const Refcounter<Latte_Obj> &str_obj = *i++;

    if (i == args_end)
      throw InsufficientArgs(*this, loc);

    const Refcounter<Latte_Obj> &from_obj = *i++;

    const Latte_Str *str_str = str_obj->as_str();
    if (!str_str)
      throw BadType(*this, 0, loc);

    const shstring &shstr = str_str->str();

    long from = static_cast<long>(from_obj->numeric_val());

    bool have_to = (i != args_end);
    long to = 0;

    if (have_to) {
      const Refcounter<Latte_Obj> &to_obj = *i;
      to = static_cast<long>(to_obj->numeric_val());
    }

    if (from < 0)
      from += shstr.size();

    if ((from < 0) || (from > shstr.size()))
      throw OutOfRange(*this, 1, loc);

    if (have_to) {
      if (to < 0)
        to += shstr.size();
      if ((to < 0) || (to > shstr.size()) || (to < from))
        throw OutOfRange(*this, 2, loc);
    }

    const latte_string &shstrstr = shstr.str();

    return Refcounter<Latte_Obj>(new Latte_Str(ws, loc,
                                               (have_to ?
                                                shstrstr.substr(from,
                                                                to - from) :
                                                shstrstr.substr(from))));
  }
};

class TruncOp : public Latte_Operator {
 public:
  TruncOp(bool floor) : m_floor(floor) {}

  const shstring &name() const {
    static const shstring f = "floor";
    static const shstring c = "ceiling";

    return (m_floor ? f : c);
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
                              const Latte_FileLoc &loc,
                              const Latte_List::const_iterator &args,
                              const Latte_List::const_iterator &args_end,
                              Latte_Activation &activation) const {
    if (args == args_end)
      throw InsufficientArgs(*this, loc);

    const Refcounter<Latte_Obj> &obj = *args;
    Latte_Number_t num = obj->numeric_val();

#ifdef ENABLE_FLOATING_POINT
    if (m_floor)
      num = floor(num);
    else
      num = ceil(num);
#endif // ENABLE_FLOATING_POINT

    return Refcounter<Latte_Obj>(new Latte_Str(ws, loc, num));
  }

 private:
  bool m_floor;
};

class ExplodeOp : public Latte_Operator {
 public:
  const shstring &name() const {
    static const shstring n = "explode";
    return n;
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
                              const Latte_FileLoc &loc,
                              const Latte_List::const_iterator &args,
                              const Latte_List::const_iterator &args_end,
                              Latte_Activation &activation) const {
    Refcounter<Latte_List> result(new Latte_List);

    Latte_Wstate just_one_space;
    just_one_space.space(1);

    long argno = 0;
    for (Latte_List::const_iterator i = args; i != args_end; ++i, ++argno) {
      const Refcounter<Latte_Obj> &obj = *i;
      Latte_Str *lstr = obj->as_str();

      if (!lstr)
        throw BadType(*this, argno, loc);

      const latte_string &str = lstr->str().str();

      for (latte_string::const_iterator j = str.begin();
           j != str.end();
           ++j) {
        char x[2];

        x[0] = *j;
        x[1] = 0;

        result->push_back(Refcounter<Latte_Obj>(new
                                                Latte_Str(just_one_space,
                                                          loc,
                                                          x)));
      }
    }

    return Latte_WsNode::wrap(*result, ws);
  }
};

class IncludeOp : public Latte_Operator {
 public:
  const shstring &name() const {
    static const shstring n = "include";
    return n;
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
			      const Latte_FileLoc &loc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    if (args == args_end)
      throw InsufficientArgs(*this, loc);

    Latte_Stringify sv(0);

    for (Latte_List::const_iterator i = args; i != args_end; ++i) {
      const Refcounter<Latte_Obj> &obj = *i;
      obj->visit(sv);
    }

    Latte_Listify lv;
    latte_load_file(sv.str(), activation, lv);

    return Latte_WsNode::wrap(lv.release(), ws);
  }
};

class GetenvOp : public Latte_Operator {
 public:
  const shstring &name() const {
    static const shstring n = "getenv";
    return n;
  }

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &ws,
			      const Latte_FileLoc &loc,
			      const Latte_List::const_iterator &args,
			      const Latte_List::const_iterator &args_end,
			      Latte_Activation &activation) const {
    if (args == args_end)
      throw InsufficientArgs(*this, loc);

    Latte_Stringify sv(0);

    for (Latte_List::const_iterator i = args; i != args_end; ++i) {
      const Refcounter<Latte_Obj> &obj = *i;
      obj->visit(sv);
    }

    const char *env = getenv(sv.str().c_str());

    if (env)
      return Refcounter<Latte_Obj>(new Latte_Str(ws, loc, shstring(env)));

    return latte_false();
  }
};

void
Latte_Activation::install_standard_definitions()
{
  // these are operators with multiple names
  Refcounter<Latte_Obj> cons_op(new ConsOp(1));
  Refcounter<Latte_Obj> greater_equal_op(new IneqOp(1, 1));
  Refcounter<Latte_Obj> greater_op(new IneqOp(1, 0));
  Refcounter<Latte_Obj> less_equal_op(new IneqOp(0, 1));
  Refcounter<Latte_Obj> less_op(new IneqOp(0, 0));
  Refcounter<Latte_Obj> not_op(new NotOp);
  Refcounter<Latte_Obj> snoc_op(new ConsOp(0));
  Refcounter<Latte_Obj> string_append_op(new StringAppendOp);
  Refcounter<Latte_Obj> str_ge_op(new StrIneqOp(1, 1));
  Refcounter<Latte_Obj> str_gt_op(new StrIneqOp(1, 0));
  Refcounter<Latte_Obj> str_le_op(new StrIneqOp(0, 1));
  Refcounter<Latte_Obj> str_lt_op(new StrIneqOp(0, 0));

#define REF Refcounter<Latte_Obj>

  define("add",            REF(new AddOp(1)));
  define("and",            REF(new AndOp));
  define("append",         REF(new AppendOp));
  define("apply",          REF(new ApplyOp(0)));
  define("ceil",           REF(new TruncOp(0)));
  define("concat",         string_append_op);
  define("cons",           cons_op);
  define("def",            REF(new DefOp));
  define("divide",         REF(new MultiplyOp(0)));
  define("downcase",       REF(new CaseOp(0)));
  define("empty?",         not_op);
  define("equal?",         REF(new EqualOp));
  define("error",          REF(new WarnOp(1)));
  define("explode",        REF(new ExplodeOp));
  define("floor",          REF(new TruncOp(1)));
  define("funcall",        REF(new ApplyOp(1)));
  define("ge?",            greater_equal_op);
  define("getenv",         REF(new GetenvOp));
  define("greater-equal?", greater_equal_op);
  define("greater?",       greater_op);
  define("group?",         REF(new TypePredicateOp(TypePredicateOp::tp_group)));
  define("gt?",            greater_op);
  define("if",             REF(new IfOp));
  define("include",        REF(new IncludeOp));
  define("file-contents",  REF(new FileContentsOp));

#ifdef HAVE_WAIT_H
  define("process-output", REF(new ProcessOutputOp));
#endif // HAVE_WAIT_H

  define("lambda",         REF(new Latte_Lambda));
  define("le?",            less_equal_op);
  define("length",         REF(new LengthOp));
  define("less-equal?",    less_equal_op);
  define("less?",          less_op);
  define("let",            REF(new LetOp));
  define("load-file",      REF(new LoadFileOp(1)));
  define("load-library",   REF(new LoadFileOp(0)));
  define("lt?",            less_op);
  define("macro",          REF(new MacroOp));
  define("modulo",         REF(new ModuloOp));
  define("multiply",       REF(new MultiplyOp(1)));
  define("not",            not_op);
  define("nth",            REF(new NthOp));
  define("operator?",      REF(new TypePredicateOp(TypePredicateOp::tp_operator)));
  define("or",             REF(new OrOp));
  define("push-back",      snoc_op);
  define("push-front",     cons_op);
  define("random",         REF(new RandomOp));
  define("set!",           REF(new SetOp));
  define("snoc",           snoc_op);
  define("string-append",  string_append_op);
  define("string-ge?",     str_ge_op);
  define("string-greater-equal?", str_ge_op);
  define("string-greater?", str_gt_op);
  define("string-gt?",     str_gt_op);
  define("string-le?",     str_le_op);
  define("string-less-equal?", str_le_op);
  define("string-less?",   str_lt_op);
  define("string-lt?",     str_lt_op);
  define("string?",        REF(new TypePredicateOp(TypePredicateOp::tp_string)));
  define("subseq",         REF(new SubseqOp));
  define("substr",         REF(new SubstrOp));
  define("subtract",       REF(new AddOp(0)));
  define("upcase",         REF(new CaseOp(1)));
  define("warn",           REF(new WarnOp(0)));
  define("while",          REF(new WhileOp));

  define("__latte-version__",
         Refcounter<Latte_Obj>(new Latte_Str(Latte_Wstate(),
                                             Latte_FileLoc(),
                                             latte_version)));
}
