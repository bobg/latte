/* latte-conf.h.  Generated automatically by configure.  */
/* latte-conf.h.in.  Generated automatically from configure.in by autoheader.  */
// -*- c++ -*-

#ifndef LATTE_CONF_H
# define LATTE_CONF_H


// Define to make Latte_Number_t be `double' instead of `long'
#define ENABLE_FLOATING_POINT 1

// Define to allow Latte to store whitespace as a sequence of zero or more
// newlines followed by a sequence of zero or more spaces, rather than
// preserving input whitespace faithfully
#define ENABLE_WHITESPACE_OPT 1

// Define if your C++ compiler understands `mutable'
#define HAVE_MUTABLE 1

// Define if deque<> has a ctor and an `insert' from a pair of const_iterators
#define HAVE_NOV96_DEQUE 1

// Define if stack<> takes container<T> as an argument instead of just T
#define HAVE_NOV96_STACK 1

// Define if string<> has a ctor from a pair of const_iterators
#define HAVE_TWO_ITERATOR_STRING_CTOR 1

// Define if STL has vector<>::resize()
#define HAVE_VECTOR_RESIZE 1

// Define if compiler chokes on shdeque.h
/* #undef SHDEQUE_IS_BEYOND_ME */

// Define if string<> has no erase() but has remove()
/* #undef USE_STRING_REMOVE */

/* Define if you have the <fstream> header file.  */
#define HAVE_FSTREAM 1

/* Define if you have the <fstream.h> header file.  */
#define HAVE_FSTREAM_H 1

/* Define if you have the <iosfwd> header file.  */
#define HAVE_IOSFWD 1

/* Define if you have the <iosfwd.h> header file.  */
/* #undef HAVE_IOSFWD_H */

/* Define if you have the <iostream> header file.  */
#define HAVE_IOSTREAM 1

/* Define if you have the <iostream.h> header file.  */
#define HAVE_IOSTREAM_H 1

/* Define if you have the <libintl.h> header file.  */
#define HAVE_LIBINTL_H 1

/* Define if you have the <string.h> header file.  */
#define HAVE_STRING_H 1

/* Define if you have the <wait.h> header file.  */
#define HAVE_WAIT_H 1

/* Name of package */
#define PACKAGE "latte"

/* Version number of package */
#define VERSION "2.1"


#endif // LATTE_CONF_H
