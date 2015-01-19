/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 Yanzheng Li

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
#include "../../include/io/file_reader.h"

#include <cassert>
#include <fstream>
#include <sstream>


sneaker::io::file_reader::file_reader()
{
  // Do nothing here.
}

sneaker::io::file_reader::file_reader(
  const char* path
) : m_path(path)
{
  assert(this->file_path());
}

const char*
sneaker::io::file_reader::file_path() const
{
  return this->m_path.c_str();
}

void
sneaker::io::file_reader::set_path(const char* path)
{
  assert(path);
  this->m_path = path;
}

bool
sneaker::io::file_reader::read_file(char** p) const
{
  if (!this->file_path()) {
    return  false;
  }

  std::ifstream file(this->file_path());

  if (file.fail()) {
    return false;
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  std::string str = buffer.str();
  *p = (char*)str.c_str();

  return true;
}
