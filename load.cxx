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

#include <latte.h>
#include <latte-fstream.h>
#include <unistd.h>

class LoadVisitor : public Latte_Visitor {
 public:
  void visit_str(Latte_Str &) {}
};

Refcounter<Latte_Obj>
latte_load_file(const shstring &filename,
                Latte_Activation &activation)
{
  LoadVisitor visitor;
  return latte_load_file(filename, activation, visitor);
}

Refcounter<Latte_Obj>
latte_load_file(const shstring &filename,
                Latte_Activation &activation,
                Latte_Visitor &visitor)
{
  ifstream in(filename.str().c_str());

  if (in.fail())
    throw Latte_FileError(filename.str());

  Latte_Reader reader(in, filename, activation);
  reader.process(visitor);
  in.close();

  return latte_true();
}

static Latte_Path path;

Latte_Path &
latte_path()
{
  return path;
}

void
latte_default_path()
{
  path.erase(path.begin(), path.end());

  const char *envpath = getenv("LATTE_PATH");

  if (envpath) {
    do {
      const char *colon = strchr(envpath, ':');
      if (colon) {
	path.push_back(shstring(envpath, colon - envpath));
	envpath = colon + 1;
      } else {
	path.push_back(envpath);
	envpath = 0;
      }
    } while (envpath);
  } else {

#if !defined(WIN32) && !defined(macintosh)
    path.push_back(latte_pkgdatadir);
    path.push_back(".");
#endif // !WIN32 && !macintosh

  }
}

#undef sep

#ifdef macintosh
# define sep ":"
# define ext ".latte"
#endif // macintosh

#ifdef WIN32
# define sep "\\"
# define ext ".lat"
#endif // WIN32

#ifndef sep
# define sep "/"
# define ext ".latte"
#endif // sep

Refcounter<Latte_Obj>
latte_load_library(const shstring &libname, Latte_Activation &activation)
{
  if (path.empty()) {
    latte_string filename = libname.str();

    if (!access(filename.c_str(), F_OK))
      return latte_load_file(filename, activation);

    filename += ext;

    if (!access(filename.c_str(), F_OK))
      return latte_load_file(filename, activation);
  } else {
    for (Latte_Path::const_iterator i = path.begin(); i != path.end(); ++i) {
      latte_string filename = (*i).str();
      filename += sep;
      filename += libname.str();

      if (!access(filename.c_str(), F_OK))
        return latte_load_file(filename, activation);

      filename += ext;

      if (!access(filename.c_str(), F_OK))
        return latte_load_file(filename, activation);
    }

    latte_string filename = libname.str();

    if (!access(filename.c_str(), F_OK))
      return latte_load_file(filename, activation);

    filename += ext;

    if (!access(filename.c_str(), F_OK))
      return latte_load_file(filename, activation);
  }

  throw Latte_FileError(libname.str());
}
