// -*- c++ -*-

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

#ifndef LATTE_SHDEQUE_H
# define LATTE_SHDEQUE_H

#include <latte-deque.h>
#include <refcount.h>

template <class T>
class shdeque {
 private:
  class rep : public Refcounted,
              public latte_deque<T> {
   private:
    typedef latte_deque<T> super;

   public:
    class const_iterator;

    class iterator : public super::iterator {
     public:
      iterator(rep &r, const super::iterator &i) :
        super::iterator(i), m_rep(&r) {}
      iterator(const iterator &other) :
        super::iterator(other), m_rep(other.m_rep) {}

      iterator &operator = (const iterator &other) {
        if (this != &other) {
          m_rep = other.m_rep;
          static_cast<super::iterator &>(*this) = other;
        }
        return *this;
      }

      iterator operator + (long n) const {
        return iterator(*m_rep,
                        (static_cast<super::iterator &>(*this) + n));
      }

      bool operator == (const iterator &other) const {
        return ((this == &other)
                || ((m_rep == other.m_rep)
                    && (static_cast<const super::iterator &>(*this) ==
                        static_cast<const super::iterator &>(other))));
      }

     private:
      const rep *m_rep;         // rep is incomplete here, so unfortunately
                                // this cannot be Refcounter< rep >
      friend const_iterator;
      friend shdeque<T>;
    };

    class const_iterator : public super::const_iterator {
     public:
      const_iterator(const rep &r, const super::const_iterator &i) :
        super::const_iterator(i), m_rep(&r) {}
      const_iterator(const iterator &i) :
        super::const_iterator(i), m_rep(i.m_rep) {}
      const_iterator(const const_iterator &other) :
        super::const_iterator(other), m_rep(other.m_rep) {}

      const_iterator &operator = (const const_iterator &other) {
        if (this != &other) {
          m_rep = other.m_rep;
          static_cast<super::const_iterator &>(*this) = other;
        }
        return *this;
      }

      const_iterator operator + (long n) const {
        return const_iterator(*m_rep,
                     (static_cast<const super::const_iterator &>(*this) + n));
      }

      bool operator == (const const_iterator &other) const {
        return ((this == &other)
                || ((m_rep == other.m_rep)
                    && (static_cast<const super::const_iterator &>(*this) ==
                        static_cast<const super::const_iterator &>(other))));
      }

     private:
      const rep *m_rep;         // rep is incomplete here, so unfortunately
                                // this cannot be Refcounter< rep >
      friend iterator;
      friend shdeque<T>;
    };

    rep() {}
    rep(const iterator &a, const iterator &b) : super(a, b) {}
    rep(const rep &other) : super(other.begin(), other.end()) {}

    iterator begin() { return iterator(*this, super::begin()); }
    iterator end() { return iterator(*this, super::end()); }
    const_iterator begin() const { return const_iterator(*this,
                                                         super::begin()); }
    const_iterator end() const { return const_iterator(*this,
                                                       super::end()); }
  };

 public:
  typedef rep::iterator iterator;
  typedef rep::const_iterator const_iterator;

 private:
  Refcounter<const rep> m_rep;  // this...
  const_iterator m_begin, m_end; // ...must precede this

  void adjust() {
    m_begin = m_rep->begin();
    m_end = m_rep->end();
  }

 public:
  shdeque() : m_rep(new rep), m_begin(m_rep->begin()), m_end(m_rep->end()) {}
  shdeque(const latte_deque<T>::const_iterator &a,
          const latte_deque<T>::const_iterator &b) :
    m_rep(new rep(a, b)),
    m_begin(*m_rep, m_rep->begin()),
    m_end(*m_rep, m_rep->end()) {}
  shdeque(const const_iterator &a, const const_iterator &b) :
    m_rep(a.m_rep), m_begin(*m_rep, a), m_end(*m_rep, b) {}
  shdeque(const shdeque<T> &other) :
    m_rep(other.m_rep), m_begin(other.m_begin), m_end(other.m_end) {}

  size_t size() const { return m_end - m_begin; }
  bool empty() const { return (m_begin == m_end); }

  // iterator begin() { return m_rep->begin(); }
  const_iterator begin() const { return m_begin; }
  // iterator end() { return m_rep->end(); }
  const_iterator end() const { return m_end; }

  // T &front() { return m_rep->front(); }
  const T &front() const { return *m_begin; }
  // T &back() { return m_rep->back(); }
  const T &back() const { const_iterator i = m_end; return *--i; }

  // T &operator [] (long n) { return *(m_begin + n); }
  const T &operator [] (long n) const { return *(m_begin + n); }

  void push_front(const T &t) {
    const_cast<rep &>(*m_rep).push_front(t);
    adjust();
  }
  void push_back(const T &t) {
    const_cast<rep &>(*m_rep).push_back(t);
    adjust();
  }

  void pop_front() {
    const_cast<rep &>(*m_rep).pop_front();
    adjust();
  }
  void pop_back() {
    const_cast<rep &>(*m_rep).pop_back();
    adjust();
  }

  void append(const latte_deque<T>::const_iterator &a,
              const latte_deque<T>::const_iterator &b) {
    const_cast<rep &>(*m_rep).append(a, b);
    adjust();
  }
};

#endif // LATTE_SHDEQUE_H
