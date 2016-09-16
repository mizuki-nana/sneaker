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
#ifndef SNEAKER_OUTPUT_STREAM_H_
#define SNEAKER_OUTPUT_STREAM_H_

#include <cstdint>
#include <memory>
#include <iosfwd>


namespace sneaker {
namespace io {

// -----------------------------------------------------------------------------

class output_stream
{
public:
  output_stream() {}

  virtual ~output_stream();

  /**
   * Marks this class non-copyable.
   */
  output_stream(output_stream&) = delete;
  output_stream& operator=(output_stream&) = delete;

  /**
   * Returns a buffer that can be written into.
   * On successful return, `data` has the pointer to the buffer
   * and `len` has the number of bytes available at data.
   */
  virtual bool next(uint8_t** data, size_t* len) = 0; 

  /**
   * Returns the number of bytes written so far into this stream.
   * The whole buffer returned by `next()` is assumed to be written.
   */
  virtual size_t bytes_written() const = 0;

  /**
   * "Returns" back to the stream some of the buffer obtained
   * from in the last call to next().
   */
  virtual void backup(size_t len) = 0;

  /**
   * Flushes any data remaining in the buffer to the stream's underlying
   * store, if any.
   */
  virtual void flush() = 0;
};

// -----------------------------------------------------------------------------

/**
 * Returns a new instance of `output_stream` whose contents are to be written
 * to a file, in chunks of given buffer size.
 *
 * If there is a file with the given name, it is truncated and overwritten.
 * If there is no file with the given name, it is created.
 */
std::unique_ptr<output_stream> file_output_stream(const char* filename,
  size_t buffer_size);

// -----------------------------------------------------------------------------

/**
 * Returns a new instance `output_stream` whose contents are to be written to
 * the specified std::ostream.
 */
std::unique_ptr<output_stream> ostream_output_stream(std::ostream& stream,
  size_t buffer_size);

// -----------------------------------------------------------------------------

class stream_writer
{
public:
  explicit stream_writer(output_stream*);

  /**
   * Writes a single byte to the stream.
   */
  bool write(uint8_t c);

  /**
   * Writes the specified number of bytes to the stream.
   */
  bool write_bytes(const uint8_t* blob, size_t n);

  /**
   * Backs up upto the currently written data and flushes the underlying stream.
   */
  void flush();

private:

  /**
   * Attempts to get more space to write to. Returns true if more storage are
   * available, false otherwise.
   */
  bool more();

  output_stream* m_stream;
  uint8_t* m_next;
  uint8_t* m_end;
};

// -----------------------------------------------------------------------------

} /* end namespace io */
} /* end namespace sneaker */

#endif /* SNEAKER_OUTPUT_STREAM_H_ */
