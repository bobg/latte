/* -*- indented-text -*- */

/* Copyright 1999 Zanshin Inc.                       <http://www.zanshin.com/>
 *
 * The contents of this file are subject to the Zanshin Public License Version
 * 1.0 (the "License"); you may not use this file except in compliance with the
 * License.  You should have received a copy of the License with Latte; see
 * the file COPYING.  You may also obtain a copy of the License at
 * <http://www.zanshin.com/ZPL.html>.
 * 
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 * 
 * The Original Code is Latte.
 * 
 * The Initial Developer of the Original Code is Zanshin, Inc.
 */

%pure_parser

%{

#include <latte.h>

struct Tangible {
  Tangible(const Latte_Wstate &w, const Latte_FileLoc &l) : ws(w), loc(l) {}

  Latte_Wstate ws;
  Latte_FileLoc loc;
};

%}

%union {
  Latte_Obj *expr;
  Latte_Group *group;
  shstring *str;
  Latte_Assignment *assignment;
  Latte_Quote *quote;
  struct Tangible *tangible;
}

%{

struct latteparse_control {
  Latte_Reader *lexer;
  Latte_Visitor *visitor;
  Latte_Wstate ws;
  Latte_FileLoc loc;
};

extern int lattelex(YYSTYPE *, void *);
extern void latteerror(const char *);

#define YYPARSE_PARAM param
#define YYLEX_PARAM param
#define PARAM ((latteparse_control *) param)

%}

%type <expr> expr
%type <group> exprs

%start top

%token <str> PARAM_ASSIGN
%token <str> PARAM_NAME
%token <str> REST_PARAM
%token <str> STR
%token <str> VAR_REF
%token QUASIQUOTE
%token QUOTE
%token UNQUOTE
%token UNQUOTE_SPLICING

%%

top: top expr {
       Refcounter<Latte_Obj> obj($2);
       Refcounter<Latte_Obj> val(obj->eval(PARAM->lexer->global_activation()));

       val->visit(*(PARAM->visitor));
     } |
     ;

expr: VAR_REF
        {
          $$ = new Latte_VarRef(PARAM->ws, PARAM->loc, *$1);
        }
      | PARAM_NAME
        {
          $$ = new Latte_Param(PARAM->ws, PARAM->loc, *$1,
                               Latte_Param::type_named);
        }
      | PARAM_ASSIGN
          {
            $<assignment>$ = new Latte_Assignment(PARAM->ws, PARAM->loc, *$1);
          }
        expr
          {
            $<assignment>2->set_nested_obj(Refcounter<Latte_Obj>($3));
            $$ = $<assignment>2;
          }
      | REST_PARAM
          {
            $$ = new Latte_Param(PARAM->ws, PARAM->loc, *$1,
                                 Latte_Param::type_rest);
          }
      | QUOTE
          {
            $<quote>$ = new Latte_Quote(PARAM->ws, PARAM->loc,
                                        Latte_Quote::type_quote);
          }
        expr
          {
            $<quote>2->set_obj(Refcounter<Latte_Obj>($3));
            $$ = $<quote>2;
          }
      | QUASIQUOTE
          {
            $<quote>$ = new Latte_Quote(PARAM->ws, PARAM->loc,
                                        Latte_Quote::type_quasiquote);
          }
        expr
          {
            $<quote>2->set_obj(Refcounter<Latte_Obj>($3));
            $$ = $<quote>2;
          }
      | UNQUOTE
          {
            $<quote>$ = new Latte_Quote(PARAM->ws, PARAM->loc,
                                        Latte_Quote::type_unquote);
          }
        expr
          {
            $<quote>2->set_obj(Refcounter<Latte_Obj>($3));
            $$ = $<quote>2;
          }
      | UNQUOTE_SPLICING
          {
            $<quote>$ = new Latte_Quote(PARAM->ws, PARAM->loc,
                                        Latte_Quote::type_unquote_splicing);
          }
        expr
          {
            $<quote>2->set_obj(Refcounter<Latte_Obj>($3));
            $$ = $<quote>2;
          }
      | '{'
          {
            $<tangible>$ = new Tangible(PARAM->ws, PARAM->loc);
          }
        exprs '}'
          {
            $3->set_ws($<tangible>2->ws);
            $3->set_fileloc($<tangible>2->loc);
            delete $<tangible>2;
            $$ = $3;
          }
      | STR
          {
            $$ = new Latte_Str(PARAM->ws, PARAM->loc, *$1);
          }
      | error
          {
            throw Latte_Reader::SyntaxError(PARAM->loc);
          };

exprs: exprs expr {
         $1->push_back(Refcounter<Latte_Obj>($2));
         $$ = $1;
       } | {
         $$ = new Latte_Group(PARAM->ws, PARAM->loc);
       }
       ;

%%

int
lattelex(YYSTYPE *lvalp, void *param)
{
  int result = PARAM->lexer->yylex();

  PARAM->ws = PARAM->lexer->ws().transfer();
  PARAM->loc = Latte_FileLoc(PARAM->lexer->filename(),
                             PARAM->lexer->lineno());

  switch (result) {
   case PARAM_ASSIGN:
    lvalp->str = new shstring(PARAM->lexer->YYText() + 1,
                              PARAM->lexer->YYLeng() - 2);
    break;

   case PARAM_NAME:
    lvalp->str = new shstring(PARAM->lexer->YYText() + 2,
                              PARAM->lexer->YYLeng() - 2);
    break;

   case REST_PARAM:
    lvalp->str = new shstring(PARAM->lexer->YYText() + 2,
                              PARAM->lexer->YYLeng() - 2);
    break;

   case STR: {
    string result(PARAM->lexer->YYText(), PARAM->lexer->YYLeng());
    size_t p = 0;

    while (p < result.size()) {
      if (result[p] == '\\')
        result.erase(p, 1);
      ++p;
    }

    lvalp->str = new shstring(result);
    break;
   }

   case VAR_REF:
    lvalp->str = new shstring(PARAM->lexer->YYText() + 1,
                              PARAM->lexer->YYLeng() - 1);
    break;
  }

  return result;
}

void
Latte_Reader::process(Latte_Visitor &visitor)
{
  latteparse_control control;

  control.lexer = this;
  control.visitor = &visitor;

  try {
    if (latteparse((void *) &control))
      throw Incomplete(Latte_FileLoc(m_filename, lineno()));
  } catch(Latte_Error err) {
    err.set_fileloc(Latte_FileLoc(m_filename, lineno()));
    throw;
  };
}

void
latteerror(const char *s)
{
}
