/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 Yanzheng Li

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

#include "../../include/io/file_reader.h"

#include "../../include/testing/testing.h"

#include <cstdio>
#include <fstream>
#include <iostream>


class file_reader_unittest : public ::testing::Test {
public:
  virtual void SetUp() {
    m_temp_file.open(m_file_path);
    m_temp_file << m_content;
    m_temp_file.close();
  }

  virtual void TearDown() {
    remove(m_file_path);
  }

  const char* m_file_path = "./__temp__.txt";
  const char* m_content = "Hello world!!!\n";
  std::ofstream m_temp_file;
};


TEST_F(file_reader_unittest, TestInitialization)
{
  sneaker::io::file_reader reader;
  const char* actual_file_path = reader.file_path();

  ASSERT_EQ(0, strlen(actual_file_path));
}

TEST_F(file_reader_unittest, TestInitializationWithFilePath)
{
  sneaker::io::file_reader reader(m_file_path);
  const char* actual_file_path = reader.file_path();

  ASSERT_STREQ(m_file_path, actual_file_path);
}

TEST_F(file_reader_unittest, TestSetPath)
{
  sneaker::io::file_reader reader(m_file_path);
  const char* actual_file_path = reader.file_path();

  ASSERT_STREQ(m_file_path, actual_file_path);

  const char* new_path = "./hello_world.txt";

  reader.set_path(new_path);

  actual_file_path = reader.file_path();

  ASSERT_STREQ(new_path, actual_file_path);
}

TEST_F(file_reader_unittest, TestReadFile)
{
  sneaker::io::file_reader reader(m_file_path);

  char* content_read = nullptr;

  bool res = reader.read_file(&content_read);

  ASSERT_EQ(true, res);
  ASSERT_NE(nullptr, content_read);
  ASSERT_STREQ(m_content, content_read);
}

TEST_F(file_reader_unittest, TestReadFileUnsuccessful)
{
  sneaker::io::file_reader reader("!@#$^&*((");
  char* content_read = nullptr;

  bool res = reader.read_file(&content_read);

  ASSERT_EQ(false, res);
  ASSERT_EQ(nullptr, content_read);
}
