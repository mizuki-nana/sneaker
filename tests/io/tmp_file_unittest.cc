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

/* Unit tests for definitions in sneaker/io/tmp_file.h */

#include "io/tmp_file.h"

#include "testing/testing.h"

#include <cstdio>

#include <unistd.h>


class tmp_file_unittest : public ::testing::Test
{
protected:
  void test(const char* msg, const char*(*func)()) const
  {
    int i = 5;
    while (i--)
    {
      const char* filepath = func();
      ASSERT_EQ(-1, access(filepath, F_OK | W_OK));
      printf("%s: %s\n", msg, filepath);
    }
  }
};

// -----------------------------------------------------------------------------

TEST_F(tmp_file_unittest, Test_get_tmp_file_path)
{
  test("Generate tmp file path", sneaker::io::get_tmp_file_path);
}

// -----------------------------------------------------------------------------

TEST_F(tmp_file_unittest, Test_get_persistent_tmp_file_path)
{
  test("Generate persistent tmp file path",
    sneaker::io::get_persistent_tmp_file_path);
}

// -----------------------------------------------------------------------------

