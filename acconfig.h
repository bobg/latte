// -*- c++ -*-

#ifndef LATTE_CONF_H
# define LATTE_CONF_H

@TOP@

// Define to make Latte_Number_t be `double' instead of `long'
#undef ENABLE_FLOATING_POINT

// Define to allow Latte to store whitespace as a sequence of zero or more
// newlines followed by a sequence of zero or more spaces, rather than
// preserving input whitespace faithfully
#undef ENABLE_WHITESPACE_OPT

// Define if your C++ compiler understands `mutable'
#undef HAVE_MUTABLE

// Define if deque<> has a ctor and an `insert' from a pair of const_iterators
#undef HAVE_NOV96_DEQUE

// Define if stack<> takes container<T> as an argument instead of just T
#undef HAVE_NOV96_STACK

// Define if string<> has a ctor from a pair of const_iterators
#undef HAVE_TWO_ITERATOR_STRING_CTOR

// Define if STL has vector<>::resize()
#undef HAVE_VECTOR_RESIZE

// Define if compiler chokes on shdeque.h
#undef SHDEQUE_IS_BEYOND_ME

// Define if string<> has no erase() but has remove()
#undef USE_STRING_REMOVE

// Automake makes me put these here
#undef PACKAGE
#undef VERSION

@BOTTOM@

#endif // LATTE_CONF_H
