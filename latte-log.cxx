// Copyright 1997-1998 Zanshin Inc.                  <http://www.zanshin.com/>

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

#include <latte-log.h>

ostream &
actual_latte_log()
{
  return cerr;
}

unsigned long latte_log_flags = 0; // all off

// throw away the argument char, and overwrite the old buf
int
DevNull::DevNullBuf::overflow(int)
{
  setp(buf, buf + sizeof(buf));
  return 0;
}

static DevNull *dev_null = new DevNull;

DevNull::DevNullBuf::DevNullBuf()
{
  setp(buf, buf + sizeof(buf));
}

ostream &
LatteLog(unsigned long flags)
{
  if (flags & latte_log_flags)
    return actual_latte_log();
  return *dev_null;
}

ostream &
LatteLog(unsigned long flags, const Latte_FileLoc &loc)
{
  return (LatteLog(flags) << loc << ": ");
}
