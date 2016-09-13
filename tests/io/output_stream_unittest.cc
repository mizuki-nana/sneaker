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

/* Unit test for definitions in sneaker/io/output_stream.h */

#include "io/output_stream.h"

#include "testing/testing.h"

#include <sstream>


// -----------------------------------------------------------------------------

class output_stream_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(output_stream_unittest, Test_ostream_output_stream)
{
  std::stringstream ss;

  auto output_stream = sneaker::io::ostream_output_stream(ss, 3);
  ASSERT_NE(nullptr, output_stream);

  const uint8_t* data = NULL;
  size_t len = 0;

  bool res = output_stream->next(&data, &len);

  ASSERT_EQ(true, res);
  ASSERT_NE(nullptr, data);
  ASSERT_EQ(3, len);
  ASSERT_EQ(3, output_stream->bytes_written());

  res = output_stream->next(&data, &len);

  ASSERT_EQ(true, res);
  ASSERT_NE(nullptr, data);
  ASSERT_EQ(3, len);
  ASSERT_EQ(6, output_stream->bytes_written());

  res = output_stream->next(&data, &len);

  ASSERT_EQ(true, res);
  ASSERT_NE(nullptr, data);
  ASSERT_EQ(3, len);
  ASSERT_EQ(9, output_stream->bytes_written());

  res = output_stream->next(&data, &len);

  ASSERT_EQ(true, res);
  ASSERT_NE(nullptr, data);
  ASSERT_EQ(3, len);
  ASSERT_EQ(12, output_stream->bytes_written());
}

// -----------------------------------------------------------------------------
