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
#include "io/input_stream.h"

#include <cstdio>
#include <ios>
#include <iostream>
#include <vector>

#include <unistd.h>
#include <fcntl.h>


namespace sneaker {

namespace {

// -----------------------------------------------------------------------------

class input_buffer_reader
{
public:
  virtual ~input_buffer_reader() { }
  virtual void seek(size_t len) = 0;
  virtual bool read(uint8_t* b, size_t to_read, size_t& actual) = 0;
};

// -----------------------------------------------------------------------------

class file_input_buffer_reader : public input_buffer_reader
{
public:
  file_input_buffer_reader(const char* filename)
    :
    m_fd(open(filename, O_RDONLY))
  {
    if (m_fd < 0)
    {
      throw std::ios_base::failure("Cannot open file");
    }
  }

  ~file_input_buffer_reader()
  {
    close(m_fd);
  }

  void seek(size_t len)
  {
    off_t r = lseek(m_fd, static_cast<off_t>(len), SEEK_CUR);
    if (r == static_cast<off_t>(-1))
    {
      throw std::ios_base::failure("Cannot skip file");
    }
  }

  bool read(uint8_t* blob, size_t to_read, size_t& actual)
  {
    auto n = ::read(m_fd, blob, to_read);
    if (n > 0)
    {
      actual = static_cast<size_t>(n);
      return true;
    }
    return false;
  }

protected:
  const int m_fd;
};

// -----------------------------------------------------------------------------

class istream_input_buffer_reader : public input_buffer_reader
{
public: 
  istream_input_buffer_reader(std::istream& stream)
    :
    m_stream(stream)
  {
  }

  void seek(size_t len)
  {
    if (! m_stream.seekg(static_cast<off_t>(len), std::ios_base::cur))
    {
      throw std::ios_base::failure("Cannot skip file");
    }
  }

  bool read(uint8_t* blob, size_t to_read, size_t& actual)
  {
    m_stream.read(reinterpret_cast<char*>(blob),
      static_cast<std::streamsize>(to_read));

    if (m_stream.bad())
    {
      return false;
    }

    actual = static_cast<size_t>(m_stream.gcount());
    return (! m_stream.eof() || actual != 0);
  }

private:
  std::istream& m_stream;
};

// -----------------------------------------------------------------------------

class input_stream_buffer_reader : public io::input_stream
{
public:
  input_stream_buffer_reader(std::unique_ptr<input_buffer_reader>&& in,
    size_t buffer_size)
    :
    io::input_stream(),
    m_buffer_size(buffer_size),
    m_buffer(buffer_size),
    m_in(std::move(in)),
    m_byte_count(0),
    m_next(m_buffer.data()),
    m_available(0)
  {
  }

  virtual ~input_stream_buffer_reader()
  {
  }

  bool next(const uint8_t** data, size_t *size)
  {
    if (m_available == 0 && !fill())
    {
      return false;
    }

    *data = m_next;
    *size = m_available;
    m_next += m_available;
    m_byte_count += m_available;
    m_available = 0;

    return true;
  }

  void skip(size_t len)
  {
    while (len > 0)
    {
      if (m_available == 0)
      {
          m_in->seek(len);
          m_byte_count += len;
          return;
      }

      size_t n = std::min(m_available, len);
      m_available -= n;
      m_next += n;
      len -= n;
      m_byte_count += n;
    }
  }

  size_t bytes_read() const
  {
    return m_byte_count;
  }

  bool fill()
  {
    size_t n = 0;
    if (m_in->read(m_buffer.data(), m_buffer_size, n))
    {
      m_next = m_buffer.data();
      m_available = n;
      return true;
    }
    return false;
  }

private:
  const size_t m_buffer_size;
  std::vector<uint8_t> m_buffer;
  std::unique_ptr<input_buffer_reader> m_in;
  size_t m_byte_count;
  uint8_t* m_next;
  size_t m_available;
};

// -----------------------------------------------------------------------------

class memory_input_stream_reader : public io::input_stream
{
public:
  memory_input_stream_reader(const uint8_t *data, size_t len)
    :
    m_data(data),
    m_total_size(len),
    m_bytes_read(0)
  {
  }

  bool next(const uint8_t** data, size_t* len)
  {
    if (m_bytes_read == m_total_size)
    {
      return false;
    }
    *data = &m_data[m_bytes_read];
    *len = m_total_size - m_bytes_read;
    m_bytes_read = m_total_size;
    return true;
  }

  void skip(size_t len)
  {
    if (len > (m_total_size - m_bytes_read))
    {
      len = m_total_size - m_bytes_read;
    }

    m_bytes_read += len;
  }

  size_t bytes_read() const
  {
    return m_bytes_read;
  }

private:
  const uint8_t* const m_data;
  const size_t m_total_size;
  size_t m_bytes_read;
};

// -----------------------------------------------------------------------------

} /* end anonymous namespace */

namespace io {

// -----------------------------------------------------------------------------

input_stream::~input_stream()
{
  // Do nothing.
}

// -----------------------------------------------------------------------------

std::unique_ptr<input_stream>
file_input_stream(const char* filename, size_t buffer_size)
{
  std::unique_ptr<input_buffer_reader> input(
    new file_input_buffer_reader(filename));

  return std::unique_ptr<input_stream>(
    new input_stream_buffer_reader(std::move(input), buffer_size));
}

// -----------------------------------------------------------------------------

std::unique_ptr<input_stream>
istream_input_stream(std::istream& stream, size_t buffer_size)
{
  std::unique_ptr<input_buffer_reader> input(
    new istream_input_buffer_reader(stream));

  return std::unique_ptr<input_stream>(
    new input_stream_buffer_reader(std::move(input), buffer_size));
}

// -----------------------------------------------------------------------------

std::unique_ptr<input_stream>
memory_input_stream(const uint8_t* data, size_t len)
{
  return std::unique_ptr<input_stream>(
    new memory_input_stream_reader(data, len)); 
}

// -----------------------------------------------------------------------------

} /* end namespace io */
} /* end namespace sneaker */
