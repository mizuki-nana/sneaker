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
#ifndef SNEAKER_INPUT_STREAM_H_
#define SNEAKER_INPUT_STREAM_H_

#include <cstdint>
#include <memory>
#include <iosfwd>


namespace sneaker {
namespace io {

// -----------------------------------------------------------------------------

class input_stream
{
public:
  input_stream() {}

  virtual ~input_stream();

  /**
   * Marks this class non-copyable.
   */
  input_stream(input_stream&) = delete;
  input_stream& operator=(input_stream&) = delete;

  /**
   * Reads data from the stream.
   *
   * Returns true if some data is successfully read, false if no more data is
   * available or an error has occurred.
   */
  virtual bool next(const uint8_t** data, size_t* len) = 0; 

  /**
   * Skips a specified number of bytes.
   */
  virtual void skip(size_t len) = 0;

  /**
   * Returns the number of bytes read from this stream so far.
   */
  virtual size_t bytes_read() const = 0;
};

// -----------------------------------------------------------------------------

/**
 * Returns a new instance of `input_stream` whose contents come from the
 * specified file. Data is read in chunks of given buffer size.
 */
std::unique_ptr<input_stream> file_input_stream(const char* filename,
  size_t buffer_size);

// -----------------------------------------------------------------------------

/**
 * Returns a new instance of `input_stream` whose contents come from the given
 * `std::istream`. The `std::istream` object should outlive the returned result.
 */
std::unique_ptr<input_stream> istream_input_stream(std::istream& stream,
  size_t buffer_size);

// -----------------------------------------------------------------------------

/**
 * Returns a new instance of `input_stream` whose data comes from the specified
 * byte array.
 */
std::unique_ptr<input_stream> memory_input_stream(const uint8_t* data,
  size_t len);

// -----------------------------------------------------------------------------

class stream_reader
{
public:
  explicit stream_reader(input_stream*);

  /**
   * Read one byte from the underlying stream. Returns true if the read is
   * successful, false otherwise.
   */
  bool read(uint8_t*);

  /**
   * Reads the given number of bytes from the underlying stream.
   * Returns true if there are enough bytes to read, false otherwise.
   */
  bool read_bytes(uint8_t* blob, size_t n);

  /**
   * Skips the given number of bytes.
   */
  void skip_bytes(size_t n);

  /**
   * Returns true if and only if the end of stream is not reached.
   */
  bool has_more();

private:

  /**
   * Get as many bytes from the underlying stream as possible in a single
   * chunk.
   *
   * Returns true if some data could be obtained. False is no more
   * data is available on the stream.
   */
  bool fill();

  input_stream* m_stream;
  const uint8_t* m_next;
  const uint8_t* m_end;
};

// -----------------------------------------------------------------------------

} /* end namespace io */
} /* end namespace sneaker */

#endif /* SNEAKER_INPUT_STREAM_H_ */
