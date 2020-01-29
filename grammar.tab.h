typedef union {
  Latte_Obj *expr;
  Latte_Group *group;
  shstring *str;
  Latte_Assignment *assignment;
  Latte_Quote *quote;
  struct Tangible *tangible;
} YYSTYPE;
#define	PARAM_ASSIGN	257
#define	PARAM_NAME	258
#define	REST_PARAM	259
#define	STR	260
#define	VAR_REF	261
#define	QUASIQUOTE	262
#define	QUOTE	263
#define	UNQUOTE	264
#define	UNQUOTE_SPLICING	265

