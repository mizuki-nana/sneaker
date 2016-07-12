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
#include "io/file_reader.h"

#include <cassert>
#include <cstring>
#include <fstream>


namespace sneaker {


namespace io {


namespace {

// -----------------------------------------------------------------------------

bool read_file(const std::string& file_path, std::ifstream& file)
{
  file.open(file_path, std::ios::binary);

  if (file.fail()) {
    return false;
  } else if (!file.good()) {
    return false;
  }

  return true;
}

// -----------------------------------------------------------------------------

} /* anonymous namespace */

// -----------------------------------------------------------------------------

file_reader::file_reader()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

file_reader::file_reader(const char* path)
  :
  m_path(path)
{
  assert(file_path());
}

// -----------------------------------------------------------------------------

file_reader::file_reader(const std::string& path)
  :
  m_path(path)
{
}

// -----------------------------------------------------------------------------

const char*
file_reader::file_path() const
{
  return m_path.c_str();
}

// -----------------------------------------------------------------------------

void
file_reader::set_path(const char* path)
{
  assert(path);
  m_path = path;
}

// -----------------------------------------------------------------------------

bool
file_reader::read(char** p, size_t* size_read) const
{
  assert(p);

  std::ifstream file;

  if (!read_file(file_path(), file))
  {
    return false;
  }

  std::vector<char> buffer((std::istreambuf_iterator<char>(file)),
    std::istreambuf_iterator<char>());

  file.close();

  void* ptr = reinterpret_cast<void*>(*p);

  if (! (ptr) ) {
    ptr = malloc(buffer.size() * sizeof(char));
    memset(ptr, 0, buffer.size());
  }

  if (! (ptr) ) {
    return false;
  }

  memcpy(ptr, buffer.data(), buffer.size());

  *p = reinterpret_cast<char*>(ptr);

  if (! (*p) ) {
    return false;
  }

  if (size_read) {
    *size_read = buffer.size();
  }

  return true;
}

// -----------------------------------------------------------------------------

bool
file_reader::read(std::vector<char>* buf, size_t* size_read) const
{
  std::ifstream file;

  if (!read_file(file_path(), file))
  {
    return false;
  }

  std::vector<char> buffer((std::istreambuf_iterator<char>(file)),
    std::istreambuf_iterator<char>());

  file.close();

  if (size_read) {
    *size_read = buffer.size();
  }

  *buf = std::move(buffer);

  return true;
}

// -----------------------------------------------------------------------------


} /* end namespace io */


} /* end namespace sneaker */
