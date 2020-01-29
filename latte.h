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

#ifndef LATTE_H
# define LATTE_H

#ifndef CONFIGURE_IS_RUNNING
# include <latte-conf.h>
#endif // CONFIGURE_IS_RUNNING

#ifndef SHDEQUE_IS_BEYOND_ME
# include <shdeque.h>
#endif // SHDEQUE_IS_BEYOND_ME

#include <shstring.h>
#include <refcount.h>
#include <mutable.h>
#include <memo.h>

#include <latte-stack.h>
#include <latte-vector.h>
#include <latte-deque.h>

#ifndef DONT_INCLUDE_FLEXLEXER
# undef yyFlexLexer
# define yyFlexLexer latteFlexLexer
# include <FlexLexer.h>
#endif // DONT_INCLUDE_FLEXLEXER

class Latte_Wstate {
 public:
  Latte_Wstate();
  Latte_Wstate(const Latte_Wstate &);

  Latte_Wstate &operator = (const Latte_Wstate &);

  Latte_Wstate transfer();

  void reset_ws();

  void crlf(unsigned int = 1);
  void cr(unsigned int = 1);
  void lf(unsigned int = 1);
  void space(unsigned int = 1);
  unsigned int tab(unsigned int = 1, unsigned int = 0);

  unsigned int newlines() const { return m_newlines; }
  unsigned int spaces() const { return m_spaces; }

  static void set_tab_width(unsigned int);

  bool operator == (const Latte_Wstate &) const;

 private:
  static unsigned int tab_width;

#ifndef ENABLE_WHITESPACE_OPT
  shstring m_str;
#endif // ENABLE_WHITESPACE_OPT
  unsigned int m_spaces;
  unsigned int m_newlines;

  friend ostream &operator << (ostream &, const Latte_Wstate &);
};

extern ostream &
operator << (ostream &, const Latte_Wstate &);

class Latte_FileLoc {
 public:
  Latte_FileLoc() : m_lineno(0) {}
  Latte_FileLoc(const shstring &, unsigned int);
  Latte_FileLoc(const Latte_FileLoc &);

  Latte_FileLoc &operator = (const Latte_FileLoc &);

  bool operator == (const Latte_FileLoc &) const;

  const shstring &filename() const { return m_filename; }
  unsigned int lineno() const { return m_lineno; }

 private:
  shstring m_filename;
  unsigned int m_lineno;
};

extern ostream &
operator << (ostream &, const Latte_FileLoc &);

class Latte_Error : public Latte_FileLoc {
 public:
  Latte_Error() {}
  Latte_Error(const Latte_FileLoc &loc) : Latte_FileLoc(loc) {}

  const Latte_FileLoc &fileloc() const { return *this; }

  void set_fileloc(const Latte_FileLoc &loc) { *this = loc; }
  void add_fileloc(const Latte_FileLoc &) const;

  latte_deque<Latte_FileLoc>::const_iterator backtrace_begin() const {
    return m_backtrace.begin();
  }
  latte_deque<Latte_FileLoc>::const_iterator backtrace_end() const {
    return m_backtrace.end();
  }

 private:
  mutable latte_deque<Latte_FileLoc> m_backtrace;
};

class Latte_SysError : public Latte_Error {
 public:
  Latte_SysError() {}
  Latte_SysError(const Latte_FileLoc &loc) : Latte_Error(loc) {}
};

class Latte_FileError : public Latte_SysError {
 public:
  Latte_FileError() {}
  Latte_FileError(const Latte_FileLoc &loc) : Latte_SysError(loc) {}
  Latte_FileError(const latte_string &filename) : m_filename(filename) {}
  Latte_FileError(const Latte_FileLoc &loc, const latte_string &filename) :
    Latte_SysError(loc), m_filename(filename) {}

  const latte_string &errfile() const { return m_filename; }

 private:
  latte_string m_filename;
};

class Latte_BindingEnv : public Refcounted,
			 public latte_vector<shstring> {
 public:
  Latte_BindingEnv() : m_outer(0) {}
  Latte_BindingEnv(Latte_BindingEnv &env) : m_outer(&env) {}

  void lookup(const shstring &, unsigned long &, unsigned long &) const;
  unsigned long extend(const shstring &);
  unsigned long define(const shstring &);

  class NotFound : public shstring {
   public:
    NotFound(const shstring &name) : shstring(name) {}
  };

 private:
  Refcounter<Latte_BindingEnv> m_outer;
};

// forward decls for Latte_Obj subtypes
class Latte_Nested;
class Latte_WsNode;
class Latte_Assignment;
class Latte_Group;
class Latte_List;
class Latte_Boolean;
class Latte_Operator;
class Latte_Param;
class Latte_Quote;
class Latte_Str;
class Latte_Tangible;
class Latte_VarRef;
// other forward decls
class Latte_Visitor;
class Latte_Activation;

#ifdef ENABLE_FLOATING_POINT
typedef double Latte_Number_t;
#else  // ENABLE_FLOATING_POINT
typedef long Latte_Number_t;
#endif // ENABLE_FLOATING_POINT

class Latte_Obj : public Refcounted {
 public:
  Latte_Obj();
  virtual ~Latte_Obj();

  virtual Latte_Nested *as_nested() { return 0; }
  virtual const Latte_Nested *as_nested() const { return 0; }

  virtual Latte_WsNode           *as_wsnode()           { return 0; }
  virtual const Latte_WsNode 	 *as_wsnode() const 	{ return 0; }

  virtual Latte_Group 		 *as_group() 		{ return 0; }
  virtual const Latte_Group 	 *as_group() const 	{ return 0; }

  virtual Latte_List 		 *as_list() 		{ return 0; }
  virtual const Latte_List 	 *as_list() const 	{ return 0; }

  virtual Latte_Boolean		 *as_boolean() 		{ return 0; }
  virtual const Latte_Boolean 	 *as_boolean() const 	{ return 0; }

  virtual Latte_Operator 	 *as_operator() 	{ return 0; }
  virtual const Latte_Operator 	 *as_operator() const 	{ return 0; }

  virtual Latte_Quote 	         *as_quote()     	{ return 0; }
  virtual const Latte_Quote      *as_quote() const      { return 0; }

  virtual Latte_Assignment 	 *as_assignment() 	{ return 0; }
  virtual const Latte_Assignment *as_assignment() const { return 0; }

  virtual Latte_Str 		 *as_str() 		{ return 0; }
  virtual const Latte_Str 	 *as_str() const 	{ return 0; }

  virtual Latte_Tangible 	 *as_tangible() 	{ return 0; }
  virtual const Latte_Tangible 	 *as_tangible() const 	{ return 0; }

  virtual Latte_Param 		 *as_param() 		{ return 0; }
  virtual const Latte_Param 	 *as_param() const 	{ return 0; }

  virtual Latte_VarRef 		 *as_varref() 		{ return 0; }
  virtual const Latte_VarRef 	 *as_varref() const 	{ return 0; }

  virtual Latte_Operator       *get_operator()       { return 0; }
  virtual const Latte_Operator *get_operator() const { return 0; }

  virtual bool bool_val() const { return 1; }
  virtual Latte_Number_t numeric_val() const { return 0; }

  virtual bool self_evaluating() const { return 1; }
  virtual bool side_effects() const { return !self_evaluating(); }

  virtual void visit(Latte_Visitor &) = 0;

  virtual void render(ostream &out) const;

  Refcounter<Latte_Obj> eval(Latte_Activation &);

 protected:
  virtual Refcounter<Latte_Obj> do_eval(Latte_Activation &);

  friend Latte_Nested;
  friend Latte_WsNode;
};

extern ostream &
operator << (ostream &, const Latte_Obj &);

class Latte_Nested : virtual public Latte_Obj {
 public:
  Latte_Nested();
  Latte_Nested(Latte_Obj &);
  Latte_Nested(const Latte_Nested &);

  Latte_Nested *as_nested() { return this; }
  const Latte_Nested *as_nested() const { return this; }

  Latte_WsNode *as_wsnode() { return m_nested->as_wsnode(); }
  const Latte_WsNode *as_wsnode() const { return m_nested->as_wsnode(); }

  Latte_Group 	    *as_group()       { return m_nested->as_group(); }
  const Latte_Group *as_group() const { return m_nested->as_group(); }

  Latte_List 	   *as_list() 	    { return m_nested->as_list(); }
  const Latte_List *as_list() const { return m_nested->as_list(); }

  Latte_Boolean       *as_boolean() 	  { return m_nested->as_boolean(); }
  const Latte_Boolean *as_boolean() const { return m_nested->as_boolean(); }

  Latte_Operator       *as_operator() 	    { return m_nested->as_operator(); }
  const Latte_Operator *as_operator() const { return m_nested->as_operator(); }

  Latte_Assignment *as_assignment() {
    return m_nested->as_assignment();
  }
  const Latte_Assignment *as_assignment() const {
    return m_nested->as_assignment();
  }

  Latte_Str *as_str()             { return m_nested->as_str(); }
  const Latte_Str *as_str() const { return m_nested->as_str(); }

  Latte_Tangible *as_tangible()             { return m_nested->as_tangible(); }
  const Latte_Tangible *as_tangible() const { return m_nested->as_tangible(); }

  Latte_Param *as_param()             { return m_nested->as_param(); }
  const Latte_Param *as_param() const { return m_nested->as_param(); }

  Latte_VarRef *as_varref()             { return m_nested->as_varref(); }
  const Latte_VarRef *as_varref() const { return m_nested->as_varref(); }

  Latte_Operator       *get_operator() { return m_operator.val(); }
  const Latte_Operator *get_operator() const { return m_operator.val(); }

  Latte_Number_t numeric_val() const;
  bool bool_val() const;

  bool self_evaluating() const { return m_self_eval.val(); }

  void visit(Latte_Visitor &);

  void render(ostream &out) const { m_nested->render(out); }

  void set_nested_obj(const Refcounter<Latte_Obj> &obj);
  const Refcounter<Latte_Obj> &nested_obj() const { return m_nested; }

 protected:
  Refcounter<Latte_Obj> do_eval(Latte_Activation &);

 private:
  struct SelfEvalFn {
    SelfEvalFn(const Latte_Nested &nested) : m_nested(nested) {}

    bool operator () () const;

   private:
    const Latte_Nested &m_nested;
  };
  struct SideEffectFn {
    SideEffectFn(const Latte_Nested &nested) : m_nested(nested) {}

    bool operator () () const;

   private:
    const Latte_Nested &m_nested;
  };
  struct GetOperatorFn {
    GetOperatorFn(const Latte_Nested &nested) : m_nested(nested) {}

    Latte_Operator *operator () () const;

   private:
    const Latte_Nested &m_nested;
  };

  Refcounter<Latte_Obj> m_nested;
  memo<bool, SelfEvalFn> m_self_eval;
  memo<bool, SideEffectFn> m_side_effect;
  memo<Latte_Operator *, GetOperatorFn> m_operator;

  friend SelfEvalFn;
  friend SideEffectFn;
  friend GetOperatorFn;
};

class Latte_WsNode : public Latte_Nested,
		     public Latte_Wstate {
 public:
  Latte_WsNode(Latte_Obj &, const Latte_Wstate &);
  Latte_WsNode(const Latte_WsNode &);

  const Latte_Wstate &wstate() const { return *this; }

  Latte_WsNode *as_wsnode() { return this; }
  const Latte_WsNode *as_wsnode() const { return this; }

  void visit(Latte_Visitor &);

  static Refcounter<Latte_Obj> wrap(Latte_Obj &, const Latte_Wstate &);

 protected:
  Refcounter<Latte_Obj> do_eval(Latte_Activation &);
};

class Latte_Boolean : virtual public Latte_Obj {
 public:
  Latte_Boolean(bool);
  Latte_Boolean(const Latte_Boolean &);

  Latte_Boolean *as_boolean() { return this; }
  const Latte_Boolean *as_boolean() const { return this; }

  bool bool_val() const { return m_val; }
  Latte_Number_t numeric_val() const { return (m_val ? 1 : 0); }

  void visit(Latte_Visitor &);

  void render(ostream &) const;

 private:
  bool m_val;
};

extern const Refcounter<Latte_Obj> &latte_false();
extern const Refcounter<Latte_Obj> &latte_true();

class Latte_Tangible : virtual public Latte_Obj,
		       public Latte_Wstate,
		       public Latte_FileLoc {
 public:
  Latte_Tangible(const Latte_Wstate &, const Latte_FileLoc &);
  Latte_Tangible(const Latte_Tangible &);

  Latte_Tangible       *as_tangible() 	    { return this; }
  const Latte_Tangible *as_tangible() const { return this; }

  Latte_Wstate &wstate() { return *this; }
  const Latte_Wstate &wstate() const { return *this; }
  void set_ws(const Latte_Wstate &other) { wstate() = other; }

  Latte_FileLoc &fileloc() { return *this; }
  const Latte_FileLoc &fileloc() const { return *this; }
  void set_fileloc(const Latte_FileLoc &other) { fileloc() = other; }
};

class Latte_Quote : public Latte_Tangible {
 public:
  enum Type {
    type_quote, type_quasiquote, type_unquote, type_unquote_splicing
  };

  Latte_Quote(const Latte_Wstate &,
              const Latte_FileLoc &,
              enum Type);
  Latte_Quote(const Latte_Quote &);

  enum Type type() const { return m_type; }

  void set_obj(const Refcounter<Latte_Obj> &);
  const Refcounter<Latte_Obj> &obj() const { return m_obj; }

  Latte_Quote *as_quote() { return this; }
  const Latte_Quote *as_quote() const { return this; }

  void visit(Latte_Visitor &);

  void render(ostream &) const;

  bool self_evaluating() const { return 0; }
  bool side_effects() const;

 protected:
  Refcounter<Latte_Obj> do_eval(Latte_Activation &);

 private:
  Refcounter<Latte_Obj> m_obj;
  enum Type m_type;
};

class Latte_Param : public Latte_Tangible,
		    public shstring {
 public:
  enum Type {
    type_rest, type_named
  };

  Latte_Param(const Latte_Wstate &,
	      const Latte_FileLoc &,
	      const shstring &,
	      enum Type);
  Latte_Param(const Latte_Param &other);

  const shstring &name() const { return *this; }

  bool is_named() const { return (m_type == type_named); }
  bool is_rest() const { return (m_type == type_rest); }

  Latte_Param *as_param() { return this; }
  const Latte_Param *as_param() const { return this; }

  void visit(Latte_Visitor &);

  void render(ostream &) const;

 private:
  Type m_type;
};

class Latte_Activation : public Refcounted {
 public:
  Latte_Activation();
  Latte_Activation(Latte_BindingEnv &);
  Latte_Activation(Latte_BindingEnv &, Latte_Activation &);

  const Refcounter<Latte_Obj> &lookup(unsigned long, unsigned long) const;
  const Refcounter<Latte_Obj> &lookup(const shstring &) const;

  Refcounter<Latte_Obj> &lookup_rw(unsigned long, unsigned long) const;
  Refcounter<Latte_Obj> &lookup_rw(const shstring &) const;

  unsigned long define(const shstring &, const Refcounter<Latte_Obj> &);

  const Refcounter<Latte_BindingEnv> &env() const { return m_env; }

  void freeze(bool val = 1) { m_frozen = val; }
  bool frozen() const { return m_frozen; }

  class ImpossibleDepth {
    // empty
  };

  class Frozen {
    // empty
  };

  void install_standard_definitions();

 private:
  Refcounter<Latte_Obj> &lookup_rw_aux(unsigned long,
                                       unsigned long,
                                       bool) const;
  Refcounter<Latte_Obj> &lookup_rw_aux(const shstring &, bool) const;

  mutable latte_vector< Refcounter<Latte_Obj> > m_objs;
  Refcounter<Latte_BindingEnv> m_env;
  Refcounter<Latte_Activation> m_outer;
  bool m_frozen;

  friend Latte_VarRef;
};

#ifdef SHDEQUE_IS_BEYOND_ME
typedef latte_deque< Refcounter<Latte_Obj> > Latte_ObjDeque;
#else // SHDEQUE_IS_BEYOND_ME
typedef shdeque< Refcounter<Latte_Obj> > Latte_ObjDeque;
#endif // SHDEQUE_IS_BEYOND_ME

class Latte_List : virtual public Latte_Obj,
                   public Latte_ObjDeque {
 public:
  Latte_List();
  Latte_List(const const_iterator &, const const_iterator &);
  Latte_List(const Latte_ObjDeque &);

  Latte_List 	       *as_list() 	    { return this; }
  const Latte_List     *as_list() const     { return this; }

  Latte_Operator *get_operator() { return m_operator.val(); }
  const Latte_Operator *get_operator() const { return m_operator.val(); }

  bool bool_val() const { return !empty(); }
  bool self_evaluating() const { return m_self_eval.val(); }

  void visit(Latte_Visitor &);

  void render(ostream &) const;

 protected:
  Refcounter<Latte_Obj> do_eval(Latte_Activation &);

 private:
  struct SelfEvalFn {
    SelfEvalFn(const Latte_List &list) : m_list(list) {}
    bool operator () () const;

   private:
    const Latte_List &m_list;
  };
  struct GetOperatorFn {
    GetOperatorFn(const Latte_List &list) : m_list(list) {}
    Latte_Operator *operator () () const;

   private:
    const Latte_List &m_list;
  };

  memo<bool, SelfEvalFn> m_self_eval;
  memo<Latte_Operator *, GetOperatorFn> m_operator;
};

class Latte_Group : public Latte_Tangible,
		    public Latte_List {
 public:
  Latte_Group(const Latte_Wstate &, const Latte_FileLoc &);
  Latte_Group(const Latte_Wstate &, const Latte_FileLoc &,
	      const Latte_List::const_iterator &,
	      const Latte_List::const_iterator &);
  Latte_Group(const Latte_Group &);

  Latte_Group       *as_group()       { return this; }
  const Latte_Group *as_group() const { return this; }

  bool bool_val() const { return !empty(); }

  bool self_evaluating() const { return m_self_eval.val(); }
  void visit(Latte_Visitor &);

  void render(ostream &) const;

 protected:
  Refcounter<Latte_Obj> do_eval(Latte_Activation &);

 private:
  struct SelfEvalFn {
    SelfEvalFn(const Latte_Group &group) : m_group(group) {}

    bool operator () () const;

   private:
    const Latte_Group &m_group;
  };

  memo<bool, SelfEvalFn> m_self_eval;
};

class Latte_Closure;

class Latte_Operator : virtual public Latte_Obj {
 public:
  Latte_Operator(bool eval = 1) : m_eval_args(eval) {}

  Latte_Operator       *as_operator() 	    { return this; }
  const Latte_Operator *as_operator() const { return this; }

  Latte_Operator       *get_operator()       { return this; }
  const Latte_Operator *get_operator() const { return this; }

  virtual const shstring &name() const = 0;

  bool eval_args() const { return m_eval_args; }
  void set_eval_args(bool e) { m_eval_args = e; }

  void visit(Latte_Visitor &);

  Refcounter<Latte_Obj> call_apply(const Latte_Wstate &,
				   const Latte_FileLoc &,
				   const Latte_List::const_iterator &,
				   const Latte_List::const_iterator &,
				   Latte_Activation &) const;

  virtual Latte_Closure *as_closure() { return 0; }
  virtual const Latte_Closure *as_closure() const { return 0; }

  class Latte_OpError : public Latte_Error {
   public:
    Latte_OpError(const Latte_Operator &op,
                  const Latte_FileLoc &loc) :
      Latte_Error(loc), m_opname(op.name()) {}

    const shstring &opname() const { return m_opname; }

   private:
    shstring m_opname;
  };

  class IllegalAssignment : public Latte_OpError {
   public:
    IllegalAssignment(const Latte_Operator &op,
                      const shstring &var,
                      const Latte_FileLoc &loc) :
      Latte_OpError(op, loc), m_varname(var) {}

    const shstring &varname() const { return m_varname; }

   private:
    shstring m_varname;
  };
  class InsufficientArgs : public Latte_OpError {
   public:
    InsufficientArgs(const Latte_Operator &op, const Latte_FileLoc &loc) :
      Latte_OpError(op, loc) {}
  };
  class BadType : public Latte_OpError {
   public:
    BadType(const Latte_Operator &op, long arg, const Latte_FileLoc &loc) :
      Latte_OpError(op, loc), m_argno(arg) {}

    long argno() const { return m_argno; }

   private:
    long m_argno;
  };
  class OutOfRange : public Latte_OpError {
   public:
    OutOfRange(const Latte_Operator &op, long arg, const Latte_FileLoc &loc) :
      Latte_OpError(op, loc), m_argno(arg) {}

    long argno() const { return m_argno; }

   private:
    long m_argno;
  };

 protected:
  virtual Refcounter<Latte_Obj> apply(const Latte_Wstate &,
				      const Latte_FileLoc &,
				      const Latte_List::const_iterator &,
				      const Latte_List::const_iterator &,
				      Latte_Activation &) const = 0;

 private:
  bool m_eval_args;
};

class Latte_Closure : public Latte_Operator {
 public:
  Latte_Closure(const Latte_List::const_iterator &,
		const Latte_List::const_iterator &,
		const Latte_List::const_iterator &,
		const Latte_List::const_iterator &,
		Latte_Activation &);
  Latte_Closure(const Latte_Closure &);

  void visit(Latte_Visitor &);

  const shstring &name() const;

  void set_macro(bool m) { m_macro = m; set_eval_args(!m); }

  Latte_Closure *as_closure() { return this; }
  const Latte_Closure *as_closure() const { return this; }

  class Useless : public Latte_OpError {
   public:
    Useless(const Latte_Closure &closure, const Latte_FileLoc &loc) :
      Latte_OpError(closure, loc) {}
  };

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &,
			      const Latte_FileLoc &,
			      const Latte_List::const_iterator &,
			      const Latte_List::const_iterator &,
			      Latte_Activation &) const;

 private:
  Refcounter<Latte_BindingEnv> m_env;
  Refcounter<Latte_Activation> m_activation;
  Refcounter<Latte_List> m_body;
  long m_rest_offset;		// the rest param is at (0,m_rest_offset)
				// in the activation (negative means no
				// rest param)
  long m_positional_params;	// it's not the rest param until this many
				// positionals have been consumed.
  bool m_macro;
};

class Latte_Assignment : public Latte_Tangible,
                         public Latte_Nested,
			 public shstring {
 public:
  Latte_Assignment(const Latte_Wstate &,
		   const Latte_FileLoc &,
		   const shstring &);
  Latte_Assignment(const Latte_Assignment &);

  const shstring &name() const { return *this; }

  Latte_Assignment 	 *as_assignment() 	{ return this; }
  const Latte_Assignment *as_assignment() const { return this; }

  Latte_Tangible *as_tangible() { return this; }
  const Latte_Tangible *as_tangible() const { return this; }

  void visit(Latte_Visitor &);

  void render(ostream &) const;

  bool self_evaluating() const { return 0; }

 protected:
  Refcounter<Latte_Obj> do_eval(Latte_Activation &);
};

class Latte_Str : public Latte_Tangible,
		  public shstring {
 public:
  Latte_Str(const Latte_Wstate &, const Latte_FileLoc &, const shstring &);
  Latte_Str(const Latte_Wstate &, const Latte_FileLoc &,
	    const Latte_Number_t &);
  Latte_Str(const Latte_Str &);

  shstring &str() { return *this; }
  const shstring &str() const { return *this; }

  Latte_Str 	      *as_str() 	  { return this; }
  const Latte_Str     *as_str() const 	  { return this; }

  Latte_Number_t numeric_val() const;

  void visit(Latte_Visitor &);

  void render(ostream &) const;

 private:
  mutable bool m_numval_known;
  mutable Latte_Number_t m_numval;
};

class Latte_VarRef : public Latte_Tangible,
		     public shstring {
 public:
  Latte_VarRef(const Latte_Wstate &, const Latte_FileLoc &, const shstring &);
  Latte_VarRef(const Latte_VarRef &);

  const shstring &name() const { return *this; }

  const Refcounter<Latte_Obj> &lookup(Latte_Activation &) const;
  Refcounter<Latte_Obj> &lookup_rw(Latte_Activation &) const;

  Latte_VarRef        *as_varref() 	  { return this; }
  const Latte_VarRef  *as_varref() const  { return this; }

  bool self_evaluating() const { return 0; }
  bool side_effects() const { return 0; }

  void visit(Latte_Visitor &);

  void render(ostream &) const;

  class Undefined : public Latte_Error {
   public:
    Undefined(const Latte_FileLoc &loc, const shstring &name) :
      Latte_Error(loc), m_name(name) {}

    const shstring &name() const { return m_name; }

   private:
    shstring m_name;
  };

  class Frozen : public Latte_Error {
   public:
    Frozen(const Latte_FileLoc &loc, const shstring &name) :
      Latte_Error(loc), m_name(name) {}

    const shstring &name() const { return m_name; }

   private:
    shstring m_name;
  };

 protected:
  Refcounter<Latte_Obj> do_eval(Latte_Activation &);

 private:
  Refcounter<Latte_Obj> &lookup_rw_aux(Latte_Activation &, bool) const;

  mutable unsigned long m_depth, m_offset;
  mutable bool m_depth_offset_known;
};

class Latte_Visitor {
 public:
  Latte_Visitor() : use_my_wstate(0) {}

  virtual ~Latte_Visitor() {}

  virtual void visit_assignment(Latte_Assignment &) {}
  virtual void visit_boolean(Latte_Boolean &) {}
  virtual void visit_closure(Latte_Closure &) {}
  virtual void visit_group(Latte_Group &);
  virtual void visit_list(Latte_List &);
  virtual void visit_nested(Latte_Nested &);
  virtual void visit_operator(Latte_Operator &) {}
  virtual void visit_param(Latte_Param &) {}
  virtual void visit_quote(Latte_Quote &);
  virtual void visit_varref(Latte_VarRef &) {}
  virtual void visit_wsnode(Latte_WsNode &);

  virtual void visit_str(Latte_Str &) = 0;

  const Latte_Wstate &suggest_wstate(const Latte_Wstate &);

 private:
  bool use_my_wstate;
  Latte_Wstate my_wstate;
};

class Latte_Stringify : public Latte_Visitor {
 public:
  Latte_Stringify(bool sp = 0) : m_space_separated(sp) {}

  void visit_str(Latte_Str &);

  const latte_string &str() const { return m_string; }

 private:
  bool m_space_separated;
  latte_string m_string;
};

class Latte_Listify : public Latte_Visitor {
 public:
  Latte_Listify() : m_list(new Latte_List) {}

  void visit_assignment(Latte_Assignment &);
  void visit_boolean(Latte_Boolean &);
  void visit_closure(Latte_Closure &);
  void visit_group(Latte_Group &);
  void visit_list(Latte_List &);
  void visit_nested(Latte_Nested &);
  void visit_operator(Latte_Operator &);
  void visit_param(Latte_Param &);
  void visit_varref(Latte_VarRef &);
  void visit_wsnode(Latte_WsNode &);
  void visit_str(Latte_Str &);

  const Latte_List &list() const { return *m_list; }
  Latte_List &release() const { return *m_list; }

 private:
  Refcounter<Latte_List> m_list;
};

class Latte_Reader : public latteFlexLexer {
 public:
  Latte_Reader(istream &, const shstring &);
  Latte_Reader(istream &, const shstring &, Latte_Activation &);

  int yylex();
  void process(Latte_Visitor &);

#if 0
  bool processed() const { return obj_stack.empty(); }
  // only call the following when processed() is false!
  const Latte_Obj &unprocessed() const { return *(obj_stack.top()); }
#endif

  void define_global(const shstring &, const Refcounter<Latte_Obj> &);

  Latte_Activation &global_activation() { return *m_global_activation; }

  class SyntaxError : public Latte_Error {
   public:
    SyntaxError(const Latte_FileLoc &loc) : Latte_Error(loc) {}
  };

  class NotIdentifier : public SyntaxError {
   public:
    NotIdentifier(const Latte_FileLoc &loc) : SyntaxError(loc) {}
  };

  class UnexpectedClose : public SyntaxError {
   public:
    UnexpectedClose(const Latte_FileLoc &loc) : SyntaxError(loc) {}
  };

  class IncompleteString : public SyntaxError {
   public:
    IncompleteString(const Latte_FileLoc &loc) : SyntaxError(loc) {}
  };

  class IncompleteGroup : public SyntaxError {
   public:
    IncompleteGroup(const Latte_FileLoc &loc) : SyntaxError(loc) {}
  };

  class IncompleteAssignment : public SyntaxError {
   public:
    IncompleteAssignment(const Latte_FileLoc &loc) : SyntaxError(loc) {}
  };

  class Incomplete : public SyntaxError {
   public:
    Incomplete(const Latte_FileLoc &loc) : SyntaxError(loc) {}
  };

  Latte_Wstate &ws() { return m_ws; }
  const Latte_Wstate &ws() const { return m_ws; }

  const shstring &filename() const { return m_filename; }

  const Refcounter<Latte_Activation> global_activation() const {
    return m_global_activation;
  }

 private:
  unsigned int m_column;
  shstring m_filename;
  Latte_Wstate m_ws;
  Refcounter<Latte_Activation> m_global_activation;
  unsigned int m_quoted_string_line;
};

class Latte_Lambda : public Latte_Operator {
 public:
  Latte_Lambda() : Latte_Operator(0) {}

  class NoParamList : public Latte_Error {
   public:
    NoParamList(const Latte_FileLoc &loc) : Latte_Error(loc) {}
  };

  const shstring &name() const;

 protected:
  Refcounter<Latte_Obj> apply(const Latte_Wstate &,
			      const Latte_FileLoc &,
			      const Latte_List::const_iterator &,
			      const Latte_List::const_iterator &,
			      Latte_Activation &) const;

};

typedef latte_deque<shstring> Latte_Path;

extern void latte_init(bool = 1);

extern Latte_Path &latte_path();
void latte_default_path();

extern Refcounter<Latte_Obj> latte_load_file(const shstring &,
					     Latte_Activation &);
extern Refcounter<Latte_Obj> latte_load_file(const shstring &,
					     Latte_Activation &,
                                             Latte_Visitor &);
extern Refcounter<Latte_Obj> latte_load_library(const shstring &,
						Latte_Activation &);

// From conf.cxx
extern const char latte_pkgdatadir[];
extern const char latte_version[];
extern const char latte_url[];

#endif // LATTE_H
