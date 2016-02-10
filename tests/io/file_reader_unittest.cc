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

/* Unit test for `sneaker::io::file_reader` defined in sneaker/io/file_reader.h */

#include "io/file_reader.h"

#include "testing/testing.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>


// -----------------------------------------------------------------------------

namespace {

const static std::string CONTENT("Hello world!!!\n");

} /* end anonymous namespace */

// -----------------------------------------------------------------------------

class file_reader_unittest : public ::testing::Test {
public:
  file_reader_unittest()
    :
    ::testing::Test(),
    m_file_path()
  {
    char buf[10] = {0};
    sprintf(buf, "__temp__%d.txt", rand() % 10);
    m_file_path.assign(buf);
  }

  virtual void SetUp() {
    std::ofstream ofs(m_file_path);
    if (ofs.good())
    {
      ofs << CONTENT;
    }
    ofs.close();
  }

  virtual void TearDown() {
    remove(m_file_path.c_str());
  }

  std::string m_file_path;
};

// -----------------------------------------------------------------------------

TEST_F(file_reader_unittest, TestInitialization)
{
  sneaker::io::file_reader reader;
  const char* actual_file_path = reader.file_path();

  ASSERT_EQ(0, strlen(actual_file_path));
}

// -----------------------------------------------------------------------------

TEST_F(file_reader_unittest, TestInitializationWithFilePath)
{
  sneaker::io::file_reader reader(m_file_path.c_str());
  const char* actual_file_path = reader.file_path();

  ASSERT_STREQ(m_file_path.c_str(), actual_file_path);
}

// -----------------------------------------------------------------------------

TEST_F(file_reader_unittest, TestSetPath)
{
  sneaker::io::file_reader reader(m_file_path.c_str());
  const char* actual_file_path = reader.file_path();

  ASSERT_STREQ(m_file_path.c_str(), actual_file_path);

  const char* new_path = "./hello_world.txt";

  reader.set_path(new_path);

  actual_file_path = reader.file_path();

  ASSERT_STREQ(new_path, actual_file_path);
}

// -----------------------------------------------------------------------------

TEST_F(file_reader_unittest, TestReadFile)
{
  sneaker::io::file_reader reader(m_file_path.c_str());

  char* content_read = nullptr;

  bool res = reader.read_file(&content_read);

  ASSERT_EQ(true, res);
  ASSERT_NE(nullptr, content_read);
  ASSERT_STREQ(CONTENT.c_str(), content_read);
}

// -----------------------------------------------------------------------------

TEST_F(file_reader_unittest, TestReadFileUnsuccessful)
{
  sneaker::io::file_reader reader("!@#$^&*((");
  char* content_read = nullptr;

  bool res = reader.read_file(&content_read);

  ASSERT_EQ(false, res);
  ASSERT_EQ(nullptr, content_read);
}

// -----------------------------------------------------------------------------
