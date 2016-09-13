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
#include "io/output_stream.h"

#include <cstdio>
#include <ios>
#include <iostream>
#include <vector>

#include <unistd.h>
#include <fcntl.h>


namespace sneaker {

namespace {

// -----------------------------------------------------------------------------

class output_buffer_writter
{
public:
  virtual ~output_buffer_writter() {}
  virtual void write(const uint8_t* blob, size_t len) = 0;
};

// -----------------------------------------------------------------------------

class file_output_buffer_writter : public output_buffer_writter
{
public:
  file_output_buffer_writter(const char* filename)
    :
    m_fd(open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644))
  {
    if (m_fd < 0)
    {
      throw std::ios_base::failure("Cannot open file");
    }
  }

  ~file_output_buffer_writter()
  {
    close(m_fd);
  }

  virtual void write(const uint8_t* blob, size_t len)
  {
    if (::write(m_fd, blob, len) < 0)
    {
      throw std::ios_base::failure("Cannot write file");
    }
  }

private:  
  const int m_fd;
};

// -----------------------------------------------------------------------------

class ostream_output_buffer_writter : public output_buffer_writter
{
public:
  ostream_output_buffer_writter(std::ostream& stream)
    :
    m_stream(stream)
  {
  }

  virtual void write(const uint8_t* blob, size_t len)
  {
    m_stream.write(reinterpret_cast<const char*>(blob),
      static_cast<std::streamsize>(len));
  }

private:
  std::ostream& m_stream;
};

// -----------------------------------------------------------------------------

class output_stream_buffer_writter : public io::output_stream
{
public:
  output_stream_buffer_writter(std::unique_ptr<output_buffer_writter>&& output,
    size_t buffer_size)
    :
    m_buffer_size(buffer_size),
    m_buffer(m_buffer_size),
    m_output(std::move(output)),
    m_next(m_buffer.data()),
    m_available_bytes(m_buffer_size),
    m_bytes_written(0)
  {
  }

  ~output_stream_buffer_writter()
  {
  }

  virtual bool next(const uint8_t** data, size_t* len)
  {
    // Invaiant: m_bytes_written == byteswritten + m_buffer_size - m_available_bytes;

    if (m_available_bytes == 0)
    {
      flush();
    }

    *data = m_next;
    *len = m_available_bytes;
    m_next += m_available_bytes;
    m_bytes_written += m_available_bytes;
    m_available_bytes = 0;

    return true;
  }

  virtual size_t bytes_written() const
  {
    return m_bytes_written;
  }

  virtual void flush()
  {
    m_output->write(m_buffer.data(), m_buffer_size - m_available_bytes);
    m_next = m_buffer.data();
    m_available_bytes = m_buffer_size;
  }

private:
  size_t m_buffer_size;
  std::vector<uint8_t> m_buffer;
  std::unique_ptr<output_buffer_writter> m_output;
  uint8_t* m_next;
  size_t m_available_bytes;
  size_t m_bytes_written;
};

// -----------------------------------------------------------------------------

} /* end anonymous namespace */

namespace io {

// -----------------------------------------------------------------------------

output_stream::~output_stream()
{
  // Do nothing.
}

// -----------------------------------------------------------------------------

std::unique_ptr<output_stream> file_output_stream(const char* filename,
  size_t buffer_size)
{
  std::unique_ptr<output_buffer_writter> output(
    new file_output_buffer_writter(filename));

  return std::unique_ptr<output_stream>(
    new output_stream_buffer_writter(std::move(output), buffer_size));
}

// -----------------------------------------------------------------------------

std::unique_ptr<output_stream>
ostream_output_stream(std::ostream& stream, size_t buffer_size)
{
  std::unique_ptr<output_buffer_writter> output(
    new ostream_output_buffer_writter(stream));

  return std::unique_ptr<output_stream>(
    new output_stream_buffer_writter(std::move(output), buffer_size));
}

// -----------------------------------------------------------------------------

} /* end namespace io */
} /* end namespace sneaker */
