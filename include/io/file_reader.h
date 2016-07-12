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

/* Thin file reader abstraction */

#ifndef SNEAKER_FILE_READER_H_
#define SNEAKER_FILE_READER_H_

#include <string>
#include <vector>


namespace sneaker {


namespace io {


class file_reader
{
public:
  file_reader();
  explicit file_reader(const char* path);
  explicit file_reader(const std::string& path);

  const char* file_path() const;
  void set_path(const char* path);

  bool read(char** p, size_t* size_read=NULL) const;
  bool read(std::vector<char>* buf, size_t* size_read=NULL) const;

protected:
  std::string m_path;
};


} /* end namespace io */


} /* end namespace sneaker */


#endif /* SNEAKER_FILE_READER_H_ */
