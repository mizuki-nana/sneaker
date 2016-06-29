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

/* Unit tests for definitions defined in sneaker/utility/io.h */

#include "utility/io.h"

#include "testing/testing.h"

#include <cstdint>
#include <string>


// -----------------------------------------------------------------------------

class io_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(io_unittest, TestHexToBytes)
{
  const std::string hex("deadbeef10203040b00b1e50");

  std::string bytes_str;

  sneaker::utility::hex_to_bytes(hex, &bytes_str);

  ASSERT_FALSE(bytes_str.empty());

  std::string actual_hex;
  sneaker::utility::bytes_to_hex(bytes_str, &actual_hex);

  ASSERT_FALSE(actual_hex.empty());

  ASSERT_EQ(hex, actual_hex);
}

// -----------------------------------------------------------------------------

TEST_F(io_unittest, TestBytesToHex)
{
  const std::string hex_str("deadbeef10203040b00b1e50");

  std::string bytes_str;
  sneaker::utility::hex_to_bytes(hex_str, &bytes_str);

  {
    std::string hex;
    sneaker::utility::bytes_to_hex(bytes_str, &hex);

    ASSERT_FALSE(hex.empty());

    std::string actual_bytes_str;
    sneaker::utility::hex_to_bytes(hex, &actual_bytes_str);

    ASSERT_EQ(bytes_str, actual_bytes_str);
  }
}

// -----------------------------------------------------------------------------
