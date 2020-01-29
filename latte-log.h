// -*- c++ -*-

// Copyright 1997-1999 Zanshin Inc.                  <http://www.zanshin.com/>

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

#ifndef LATTE_LOG_H
# define LATTE_LOG_H

#include <latte.h>
#include <iostream.h>

enum LatteLogFlags {
  latte_log_EVAL =  1 << 0,
  latte_log_MEM =   1 << 1
};

extern unsigned long latte_log_flags;

class DevNull : public ostream {
  class DevNullBuf : public streambuf {
   public:
    DevNullBuf();

    int overflow(int);

   private:
    char buf[64];		// arbitrary
  };

 public:
  DevNull() : ostream(0) { ostream::init(&buf); }

 private:
  DevNullBuf buf;
};

extern ostream &LatteLog(unsigned long = ~((unsigned long) 0));
extern ostream &LatteLog(unsigned long, const Latte_FileLoc &);

#endif // LATTE_LOG_H
