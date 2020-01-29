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

#ifndef SHSTRING_H
# define SHSTRING_H

#ifndef CONFIGURE_IS_RUNNING
# include <latte-conf.h>
#endif // CONFIGURE_IS_RUNNING

#include <latte-string.h>
#include <functional>
#include <iostream.h>

#include <refcount.h>

class shstring {
 public:
  typedef latte_string::iterator iterator;
  typedef latte_string::const_iterator const_iterator;

  shstring();
  shstring(const char *);
  shstring(const char *, size_t);
  shstring(const string &);
  shstring(const shstring &);
  shstring(const const_iterator &, const const_iterator &);

  ~shstring();

  shstring &operator = (const shstring &);
  shstring &operator = (const char *);

  bool null() const { return !rep.get(); }
  bool empty() const { return null() || rep->empty(); }

  bool operator < (const shstring &) const;
  bool operator == (const shstring &other) const;

  iterator begin() { return rep->begin(); }
  const_iterator begin() const { return rep->begin(); }

  iterator end() { return rep->end(); }
  const_iterator end() const { return rep->end(); }

  size_t size() const { return (rep.get() ? rep->size() : 0); }

  // don't call when null() is true!!
  const latte_string &str() const { return rep->str(); }

 private:
  class Rep : public Refcounted, public latte_string {
   public:
    Rep(const char *);
    Rep(const char *, size_t);
    Rep(const string &);
    Rep(const string::const_iterator &,
	const string::const_iterator &);

    latte_string &str() { return *this; }
    const latte_string &str() const { return *this; }

   private:
    friend ostream &operator << (ostream &, const shstring &);
    friend class shstring;
  };

  Refcounter<Rep> rep;

  friend ostream &operator << (ostream &, const shstring &);
};

extern ostream &
operator << (ostream &, const shstring &);

#endif // SHSTRING_H
