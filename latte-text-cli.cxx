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

#include <latte-text.h>
#include <latte-log.h>
#include <latte-fstream.h>
#include <getopt.h>
#include <cstring>
#include <unistd.h>

static void
report_backtrace(const Latte_Error &err)
{
  const latte_deque<Latte_FileLoc>::const_iterator end = err.backtrace_end();

  for (latte_deque<Latte_FileLoc>::const_iterator i = err.backtrace_begin();
       i != end;
       ++i)
    cerr << "  called from " << *i << endl;
}

main(int argc, char **argv)
{
  deque<latte_string> loads;
  bool no_default = 0;
  unsigned long log_flags = 0;
  const char *outfilename = 0;
  unsigned int wrapcol = 72;
  const char *sentence_end_chars = ".?!";
  const char *sentence_end_transparent_chars = ")}]\"";

  int option_index = 0;
  while (1) {
    static struct option options[] = {
      { "sentence-end", required_argument, 0, 'e' },
      { "sentence-end-transparent", required_argument, 0, 't' },
      { "wrap", required_argument, 0, 'w' },
      { "load", required_argument, 0, 'l' },
      { "no-default", no_argument, 0, 'n' },
      { "debug", optional_argument, 0, 'd' },
      { "version", no_argument, 0, 'v' },
      { "help", no_argument, 0, 'h' },
      { "output", required_argument, 0, 'o' },
      { 0, 0, 0, 0 }
    };
    static struct debug_val {
      const char *name;
      unsigned long val;
    } debug_vals[] = {
      { "eval", latte_log_EVAL },
      { "mem", latte_log_MEM },
      { 0, 0 }
    };

    int opt = getopt_long(argc, argv, "o:l:nd:vhw:t:e:",
                          options, &option_index);

    if (opt == -1)
      break;

    switch (opt) {
     case 'e':
      sentence_end_chars = optarg;
      break;

     case 't':
      sentence_end_transparent_chars = optarg;
      break;

     case 'w':
      wrapcol = atoi(optarg);
      break;

     case 'o':
      outfilename = optarg;
      break;

     case 'l':
      loads.push_back(latte_string(optarg));
      break;

     case 'n':
      no_default = 1;
      break;

     case 'd': {
       const char *p = optarg;
       do {
         const char *comma = strchr(p, ',');
         if (!comma)
           comma = p + strlen(p);
         for (const debug_val *dv = debug_vals; dv->name; ++dv) {
           if ((strlen(dv->name) == (comma - p))
               && !strncmp(dv->name, p, (comma - p)))
             log_flags |= dv->val;
         }
         p = comma;
       } while (*p++);
     }
     break;

     case 'v':
      cout << "This is latte-html version " << latte_version << endl;
      exit(0);
      break;                    // not reached

     case 'h':
     case '?':
      cout << "This is latte-text version " << latte_version << endl
           << "Copyright 1998,1999 Zanshin Inc." << endl;

      cout << "Usage: latte-html [options] FILE" << endl << endl;
      cout << "options are:" << endl << endl;
      cout << " -v, --version        Print latte-html version and exit"
           << endl;
      cout << " -h, --help           Print this help and exit"
           << endl;
      cout << " -l, --load=LIBRARY   Pre-load named LIBRARY"
           << endl;
      cout << " -n, --no-default     Do not load default libraries from "
           << latte_pkgdatadir
           << endl;
      cout << " -o, --output=FILE    Place output in FILE"
           << endl;
      cout << " -d, --debug=FLAG     Turn on FLAG debug flag"
           << endl;
      cout << "                      (flags are eval, mem)"
           << endl;

      cout << endl;
      cout << "This software is licensed solely for use under"
           << endl
           << "the terms of the Zanshin Public License, available at"
           << endl
           << latte_url
           << endl;

      exit(0);
      break;                    // not reached

     default:
      cerr << "latte-text: invalid argument (see `latte-text --help')" << endl;
      exit(1);
    }
  }

  const char *filename;
  istream *file_stream;

  if (optind >= argc) {
    filename = "[standard input]";
    file_stream = &cin;
  } else {
    filename = argv[optind];

    if (!strcmp(filename, "-")) {
      filename = "[standard input]";
      file_stream = &cin;
    } else {
      file_stream = new ifstream(filename);

      if (file_stream->fail()) {
        cerr << "Could not open " << filename << endl;
        exit(1);
      }
    }
  }

  ostream *out_file_stream = &cout;

  if (outfilename) {
    unlink(outfilename);        // ignore errors
    out_file_stream = new ofstream(outfilename);
    if (out_file_stream->fail()) {
      cerr << "Could not create " << outfilename << endl;
      exit(1);
    }
  }

  try {

    latte_text(no_default, log_flags,
               loads.begin(), loads.end(),
               wrapcol,
               sentence_end_chars, sentence_end_transparent_chars,
               filename, *file_stream, *out_file_stream);

  } catch (const Latte_Closure::Useless &err) {
    cerr << endl
         << "Useless subexpression in call to "
         << err.opname()
         << " at "
         << err.fileloc()
         << endl;
  } catch (const Latte_Operator::OutOfRange &err) {
    cerr << endl
         << "Argument "
         << (1 + err.argno())
         << " out of range in call to "
         << err.opname()
         << " at "
         << err.fileloc()
         << endl;
    report_backtrace(err);
    exit(1);
  } catch (const Latte_Operator::IllegalAssignment &err) {
    cerr << endl
         << "Illegal named parameter assignment ("
         << err.varname()
         << ") in call to function ("
         << err.opname()
         << ") at "
         << err.fileloc()
         << endl;
    report_backtrace(err);
    exit(1);
  } catch (const Latte_Operator::InsufficientArgs &err) {
    cerr << endl
         << "A function ("
         << err.opname()
         << ") was called with too few arguments at "
         << err.fileloc()
         << endl;
    report_backtrace(err);
    exit(1);
  } catch (const Latte_Operator::BadType &err) {
    cerr << endl
         << "Bad type in argument "
         << (1 + err.argno())
         << " in call to "
         << err.opname()
         << " at "
         << err.fileloc()
         << endl;
    report_backtrace(err);
    exit(1);
  } catch (const Latte_VarRef::Undefined &err) {
    cerr << endl << "Variable \"" << err.name() << "\" is undefined at ";
    cerr << err.fileloc() << endl;
    report_backtrace(err);
    exit(1);
  } catch (const Latte_Reader::NotIdentifier &err) {
    cerr << endl << "Not an identifier where expected at ";
    cerr << err.fileloc() << endl;
    report_backtrace(err);
    exit(1);
  } catch (const Latte_Reader::UnexpectedClose &err) {
    cerr << endl << "Unexpected close-brace at ";
    cerr << err.fileloc() << endl;
    report_backtrace(err);
    exit(1);
  } catch (const Latte_Reader::IncompleteString &err) {
    cerr << endl << "Incomplete string begins at ";
    cerr << err.fileloc() << endl;
    report_backtrace(err);
    exit(1);
  } catch (const Latte_Reader::IncompleteGroup &err) {
    cerr << endl << "Incomplete group begins at ";
    cerr << err.fileloc() << endl;
    report_backtrace(err);
    exit(1);
  } catch (const Latte_Reader::IncompleteAssignment &err) {
    cerr << endl << "Incomplete parameter assignment begins at ";
    cerr << err.fileloc() << endl;
    report_backtrace(err);
    exit(1);
  } catch (const Latte_Reader::Incomplete &err) {
    cerr << endl << "Processing incomplete at ";
    cerr << err.fileloc() << endl;
    report_backtrace(err);
    exit(1);
  } catch (const Latte_Reader::SyntaxError &err) {
    cerr << endl << "Some kind of syntax error at ";
    cerr << err.fileloc() << endl;
    report_backtrace(err);
    exit(1);
  } catch (const Latte_Lambda::NoParamList &err) {
    cerr << endl << "Attempt to define a closure with no parameter list at ";
    cerr << err.fileloc() << endl;
    report_backtrace(err);
    exit(1);
  } catch (const Latte_FileError &err) {
    cerr << endl << "File error ";
    if (!err.errfile().empty())
      cerr << "(file " << err.errfile() << ") ";
    cerr << "at ";
    cerr << err.fileloc() << endl;
    report_backtrace(err);
    exit(1);
  } catch (const Latte_SysError &err) {
    cerr << endl << "System error at ";
    cerr << err.fileloc() << endl;
    report_backtrace(err);
    exit(1);
  } catch (const Latte_Error &err) {
    cerr << endl << "Fatal error at ";
    cerr << err.fileloc() << endl;
    report_backtrace(err);
    exit(1);
  } catch (...) {
    cerr << endl << "Fatal error, location unknown" << endl;
    exit(1);
  };

  exit(0);
}
