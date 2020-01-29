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

#ifndef REFCOUNT_H
# define REFCOUNT_H

#ifndef CONFIGURE_IS_RUNNING
# include <latte-conf.h>
#endif // CONFIGURE_IS_RUNNING
#include <mutable.h>

template <class T>
class Refcounter {
 public:
  Refcounter() : val(0) {}
  explicit Refcounter(T *v) : val(v) { if (val) val->inc_refcount(); }
  Refcounter(const Refcounter<T> &other) : val(other.val) {
    if (val)
      val->inc_refcount();
  }

  ~Refcounter() { if (val) val->dec_refcount(); }

  Refcounter &operator = (const Refcounter<T> &other) {
    if (this != &other) {
      if (other.val)
	other.val->inc_refcount();
      if (val)
	val->dec_refcount();
      val = other.val;
    }
    return *this;
  }

  T *get() const { return val; }

  T &operator * () const { return *val; }
  T *operator -> () const { return val; }

 private:
  T *val;
};

class Refcounted {
 public:
  Refcounted() : refcount(0) {}

  // The dtor is virtual because this may be mixed into virtual classes
  virtual ~Refcounted() {}

  void inc_refcount() const { ++(mutate(refcount)); }
  void dec_refcount() const { if (!--(mutate(refcount))) delete this; }

 private:
  mutable unsigned long refcount;
};

#endif // REFCOUNT_H
