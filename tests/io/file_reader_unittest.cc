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
#include <cstring>
#include <fstream>
#include <string>


// -----------------------------------------------------------------------------

namespace {

const static std::string CONTENT("Hello world!!!");

} /* end anonymous namespace */

// -----------------------------------------------------------------------------

class file_reader_unittest : public ::testing::Test {
public:
  file_reader_unittest()
    :
    ::testing::Test(),
    m_file_path()
  {
    char buf[20] = {0};
    sprintf(buf, "__temp__%d.txt", rand() % 99);
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

TEST_F(file_reader_unittest, TestInitializationWithFilePath)
{
  sneaker::io::file_reader reader(m_file_path);
  const char* actual_file_path = reader.file_path();

  ASSERT_STREQ(m_file_path.c_str(), actual_file_path);
}

// -----------------------------------------------------------------------------

TEST_F(file_reader_unittest, TestSetPath)
{
  sneaker::io::file_reader reader(m_file_path);
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
  sneaker::io::file_reader reader(m_file_path);

  char* content_read = nullptr;
  size_t size_read = 0;

  bool res = reader.read(&content_read, &size_read);

  ASSERT_EQ(true, res);
  ASSERT_NE(nullptr, content_read);

  ASSERT_EQ(CONTENT.size(), size_read);
  ASSERT_EQ(strlen(CONTENT.c_str()), strlen(content_read));
  ASSERT_STREQ(CONTENT.c_str(), content_read);
}

// -----------------------------------------------------------------------------

TEST_F(file_reader_unittest, TestReadFileUnsuccessful)
{
  sneaker::io::file_reader reader("!@#$^&*((");
  char* content_read = nullptr;

  bool res = reader.read(&content_read);

  ASSERT_EQ(false, res);
  ASSERT_EQ(nullptr, content_read);
}

// -----------------------------------------------------------------------------

class file_reader_binary_unittest : public file_reader_unittest
{
public:
  file_reader_binary_unittest()
    :
    file_reader_unittest(),
    m_data(reinterpret_cast<long long>(&CONTENT))
  {
  }

  virtual void SetUp() {
    std::ofstream ofs(m_file_path, std::ios::binary);
    if (ofs.good())
    {
      ofs << m_data;
    }
    ofs.close();
  }

  virtual void TearDown() {
    remove(m_file_path.c_str());
  }

  long long m_data;
};

// -----------------------------------------------------------------------------

TEST_F(file_reader_binary_unittest, TestReadBinaryContent)
{
  sneaker::io::file_reader reader(m_file_path);

  char* content_read = nullptr;
  size_t size_read = 0;

  const bool res = reader.read(&content_read, &size_read);

  ASSERT_EQ(true, res);
  ASSERT_NE(nullptr, content_read);

  std::stringstream expected_output_stream;
  expected_output_stream << m_data;

  std::stringstream actual_output_stream;
  actual_output_stream.write(content_read, static_cast<std::streamsize>(size_read));

  ASSERT_EQ(expected_output_stream.str().size(), actual_output_stream.str().size());

  ASSERT_EQ(expected_output_stream.str(), actual_output_stream.str());
}

// -----------------------------------------------------------------------------

TEST_F(file_reader_binary_unittest, TestReadBinaryContent2)
{
  sneaker::io::file_reader reader(m_file_path);

  std::vector<char> content_read;
  size_t size_read = 0;

  const bool res = reader.read(&content_read, &size_read);

  ASSERT_EQ(true, res);
  ASSERT_EQ(false, content_read.empty());
}

// -----------------------------------------------------------------------------
