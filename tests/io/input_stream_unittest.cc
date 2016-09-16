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

/* Unit test for definitions in sneaker/io/input_stream.h */

#include "io/input_stream.h"

#include "testing/testing.h"

#include <fstream>
#include <string>
#include <sstream>


// -----------------------------------------------------------------------------

class input_stream_unittest : public ::testing::Test
{
public:
  static const char* const FILEPATH;
  static const char* DATA;

protected:
  input_stream_unittest()
    :
    m_stream("tmp.txt")
  {
  }

  virtual void SetUp()
  {
    m_stream << DATA;
    m_stream.close();
  }

  virtual void TearDown()
  {
    remove("tmp.txt");
  }

  void test(std::unique_ptr<sneaker::io::input_stream>&& input_stream)
  {
    ASSERT_NE(nullptr, input_stream);

    const uint8_t* data = NULL;
    size_t len = 0;

    ASSERT_EQ(0, input_stream->bytes_read());

    bool res = input_stream->next(&data, &len); 

    ASSERT_EQ(true, res);
    ASSERT_EQ(3, len);
    ASSERT_EQ("Hel", std::string(data, data+len));
    ASSERT_EQ(3, input_stream->bytes_read());

    res = input_stream->next(&data, &len);

    ASSERT_EQ(true, res);
    ASSERT_EQ(3, len);
    ASSERT_EQ("lo ", std::string(data, data+len));
    ASSERT_EQ(6, input_stream->bytes_read());

    res = input_stream->next(&data, &len);

    ASSERT_EQ(true, res);
    ASSERT_EQ(3, len);
    ASSERT_EQ("wor", std::string(data, data+len));
    ASSERT_EQ(9, input_stream->bytes_read());

    res = input_stream->next(&data, &len);

    ASSERT_EQ(true, res);
    ASSERT_EQ(2, len);
    ASSERT_EQ("ld", std::string(data, data+len));
    ASSERT_EQ(11, input_stream->bytes_read());

    res = input_stream->next(&data, &len);

    ASSERT_EQ(false, res);
    ASSERT_EQ(2, len);
    ASSERT_EQ("ld", std::string(data, data+len));
    ASSERT_EQ(11, input_stream->bytes_read());
  }

  std::ofstream m_stream;
};

const char* const input_stream_unittest::FILEPATH = "tmp.txt";
const char* input_stream_unittest::DATA = "Hello world";

// -----------------------------------------------------------------------------

TEST_F(input_stream_unittest, Test_file_input_stream)
{
  auto input_stream = sneaker::io::file_input_stream(FILEPATH, 3);
  test(std::move(input_stream));
}

// -----------------------------------------------------------------------------

TEST_F(input_stream_unittest, Test_istream_input_stream)
{
  std::stringstream ss;
  ss << DATA;
  auto input_stream = sneaker::io::istream_input_stream(ss, 3);
  test(std::move(input_stream));
}

// -----------------------------------------------------------------------------

TEST_F(input_stream_unittest, Test_memory_input_stream)
{
  const char* data = DATA;
  auto input_stream = sneaker::io::memory_input_stream(
    reinterpret_cast<const uint8_t*>(data), strlen(data));

  ASSERT_NE(nullptr, input_stream);

  const uint8_t* data_read = NULL;
  size_t len = 0;

  bool res = input_stream->next(&data_read, &len);

  ASSERT_EQ(true, res);
  ASSERT_EQ(strlen(data), len);
  ASSERT_STREQ(data, reinterpret_cast<const char*>(data_read));
  ASSERT_EQ(strlen(data), input_stream->bytes_read());
}

// -----------------------------------------------------------------------------

class stream_reader_unittest : public input_stream_unittest {};

// -----------------------------------------------------------------------------

TEST_F(stream_reader_unittest, TestStreamReader)
{
  auto input_stream = sneaker::io::file_input_stream(FILEPATH, 3);
  sneaker::io::stream_reader reader(input_stream.get());

  ASSERT_EQ(true, reader.has_more());

  uint8_t byte = 0;
  bool res = reader.read(&byte);

  ASSERT_EQ(true, res);
  ASSERT_EQ('H', byte);

  uint8_t bytes[10] = {0};
  res = reader.read_bytes(reinterpret_cast<uint8_t*>(&bytes[0]), 4);

  ASSERT_EQ(true, res);
  ASSERT_STREQ("ello", reinterpret_cast<char*>(bytes));

  reader.skip_bytes(2);

  ASSERT_EQ(true, reader.has_more());

  uint8_t more_bytes[10] = {0};
  res = reader.read_bytes(more_bytes, 3);

  ASSERT_EQ(true, res);

  ASSERT_STREQ("orl", reinterpret_cast<char*>(more_bytes));

  res = reader.read(&byte);

  ASSERT_EQ(true, res);
  ASSERT_EQ('d', byte);

  ASSERT_EQ(false, reader.has_more());

  // At this point we have finished reading the stream.

  res = reader.has_more();
  ASSERT_EQ(false, res);

  res = reader.read(&byte);
  ASSERT_EQ(false, res);
  ASSERT_EQ('d', byte);

  uint8_t even_more_bytes[10] = {0};
  res = reader.read_bytes(even_more_bytes, 3);

  ASSERT_EQ(false, res);
  ASSERT_STREQ("", reinterpret_cast<char*>(even_more_bytes));
}

// -----------------------------------------------------------------------------
