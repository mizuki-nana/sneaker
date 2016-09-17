/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2016 Yanzheng Li

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************************/
#include "io/tmp_file.h"
#include "libc/utils.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <unistd.h>


namespace sneaker {

namespace {
template<typename T, template<class> class Pred>
T
retry(T(*func)(), const Pred<T>& pred)
{
  T res;
  do {
    res = func();
  } while (!pred(res));
  return res;
}

template<typename T>
struct FilePathPred
{
  bool operator()(T) const;
};

template<>
struct FilePathPred<const char*>
{
  bool operator()(const char* filepath) const
  {
    // http://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c-cross-platform
    return access(filepath, F_OK | W_OK) == -1;
  }
};

const char*
try_get_tmp_file_path()
{
  static const char* base = "/tmp/";
  char buf[1000] = {0};
  snprintf(buf, sizeof(buf), "%s%s", base, generate_text(8, 16));
  return strdup(buf);
}

const char*
try_get_persistent_tmp_file_path()
{
#ifdef __APPLE__
  static const char* base = getenv("TMPDIR");
#else
  // http://superuser.com/questions/332610/where-is-the-temporary-directory-in-linux
  static const char* base = "/var/tmp/";
#endif
  char buf[1000] = {0};
  snprintf(buf, sizeof(buf), "%s%s", base, generate_text(8, 16));
  return strdup(buf);
}

} /* end anonymous namespace */

namespace io {

// -----------------------------------------------------------------------------

const char*
get_tmp_file_path()
{
  static const FilePathPred<const char*> pred{};
  return retry<const char*>(try_get_tmp_file_path, pred);
}

// -----------------------------------------------------------------------------

const char*
get_persistent_tmp_file_path()
{
  static const FilePathPred<const char*> pred{};
  return retry<const char*>(try_get_persistent_tmp_file_path, pred);
}

// -----------------------------------------------------------------------------

} /* end namespace io */
} /* end namespace sneaker */
