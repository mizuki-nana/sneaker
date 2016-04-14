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

/* Unit test for definitions in sneaker/logging/logger.h */

#include "logging/logging.h"

#include "testing/testing.h"

#include <cstdio>
#include <fstream>
#include <string>


// -----------------------------------------------------------------------------

using namespace sneaker::logging;

// -----------------------------------------------------------------------------

class logger_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

class logger_simple_unittest : public logger_unittest {};

// -----------------------------------------------------------------------------

TEST_F(logger_simple_unittest, TestLogSimple1)
{
  logger<thread_safe_tag, exception_safe_tag> logger_(new stdout_log_scheme());
  logger_.write(LogLevel::LOG_LEVEL_DEBUG, "%s (%s line:%lu)",
    "Logging in TestLogSimple1 - thread safe and exception safe", __FILE__, __LINE__);
}

// -----------------------------------------------------------------------------

TEST_F(logger_simple_unittest, TestLogSimple2)
{
  logger<thread_unsafe_tag, exception_safe_tag> logger_(new stdout_log_scheme());
  logger_.write(LogLevel::LOG_LEVEL_INFO, "%s (%s line:%lu)",
    "Logging in TestLogSimple2 - not thread safe and exception safe", __FILE__, __LINE__);
}

// -----------------------------------------------------------------------------

TEST_F(logger_simple_unittest, TestLogSimple3)
{
  logger<thread_safe_tag, exception_unsafe_tag> logger_(new stdout_log_scheme());
  logger_.write(LogLevel::LOG_LEVEL_WARN, "%s (%s line:%lu)",
    "Logging in TestLogSimple3 - thread safe and not exception safe", __FILE__, __LINE__);
}

// -----------------------------------------------------------------------------

TEST_F(logger_simple_unittest, TestLogSimple4)
{
  logger<thread_unsafe_tag, exception_unsafe_tag> logger_(new stdout_log_scheme());
  logger_.write(LogLevel::LOG_LEVEL_ERROR, "%s (%s line:%lu)",
    "Logging in TestLogSimple4 - not thread safe and not exception safe", __FILE__, __LINE__);
}

// -----------------------------------------------------------------------------

TEST_F(logger_simple_unittest, TestLogSimple5)
{
  logger<thread_safe_tag, exception_safe_tag> logger_(new stdout_log_scheme());
  logger_.write<64>(LogLevel::LOG_LEVEL_DEBUG, "%s (%s line:%lu)",
    "Logging with 64 characters", __FILE__, __LINE__);
}

// -----------------------------------------------------------------------------

class file_logger_unittest : public logger_unittest {};

// -----------------------------------------------------------------------------

TEST_F(file_logger_unittest, TestLogWriteToFile)
{
  const char* filename = "./TestLogWriteToFile.txt";

  logger<thread_safe_tag, exception_safe_tag> logger_(new file_log_scheme(filename));

  logger_.write(LogLevel::LOG_LEVEL_DEBUG, "%s (%s line:%lu)",
    "Logging in TestLogWriteToFile - thread safe and exception safe", __FILE__, __LINE__);
  logger_.write(LogLevel::LOG_LEVEL_INFO, "%s (%s line:%lu)",
    "Logging in TestLogWriteToFile - not thread safe and exception safe", __FILE__, __LINE__);
  logger_.write(LogLevel::LOG_LEVEL_WARN, "%s (%s line:%lu)",
    "Logging in TestLogWriteToFile - thread safe and not exception safe", __FILE__, __LINE__);
  logger_.write(LogLevel::LOG_LEVEL_ERROR, "%s (%s line:%lu)",
    "Logging in TestLogWriteToFile - not thread safe and not exception safe", __FILE__, __LINE__);

  std::ifstream ifs(filename);
  ASSERT_TRUE(ifs.good());

  size_t num_lines = 0;
  std::string line;
  while (std::getline (ifs, line))
  {
    ++num_lines;
  }

  ASSERT_EQ(4, num_lines);

  remove(filename);
}

// -----------------------------------------------------------------------------

class logging_macros_unittest : public logger_unittest {};

// -----------------------------------------------------------------------------

TEST_F(logging_macros_unittest, Test_LOG_DEBUG)
{
  LOG_DEBUG("%s is %lu years old", "Sabor", 24);
}

// -----------------------------------------------------------------------------

TEST_F(logging_macros_unittest, Test_LOG_INFO)
{
  LOG_INFO("It's %lu, %lu years after %s was created", 2016, 2, "sneaker");
}

// -----------------------------------------------------------------------------

TEST_F(logging_macros_unittest, Test_LOG_WARN)
{
  LOG_WARN("Will we all be driving %s by %lu, only time will tell", "electric cars", 2020);
}

// -----------------------------------------------------------------------------

TEST_F(logging_macros_unittest, Test_LOG_ERROR)
{
  LOG_ERROR("I thought %lu + %lu = %lu, am I %s?", 1, 1, 3, "right");
}

// -----------------------------------------------------------------------------

TEST_F(logging_macros_unittest, Test_LOG_FATAL)
{
  LOG_FATAL("How many seasons does %s have? Answer: %lu", "The X Files", 9);
}

// -----------------------------------------------------------------------------

TEST_F(logging_macros_unittest, Test_LOG_DEBUG_DETAILED)
{
  LOG_DEBUG_DETAILED("%s is %lu years old", "Tohsaka", 22);
}

// -----------------------------------------------------------------------------

TEST_F(logging_macros_unittest, Test_LOG_INFO_DETAILED)
{
  LOG_INFO_DETAILED("It's %lu, %lu years after %s was created", 2016, 26, "Python");
}

// -----------------------------------------------------------------------------

TEST_F(logging_macros_unittest, Test_LOG_WARN_DETAILED)
{
  LOG_WARN_DETAILED("Will we all be driving %s by %lu, only time will tell", "warp prisms", 2100);
}

// -----------------------------------------------------------------------------

TEST_F(logging_macros_unittest, Test_LOG_ERROR_DETAILED)
{
  LOG_ERROR_DETAILED("I thought %lu + %lu = %lu, am I %s?", 1, 1, 2, "wrong");
}

// -----------------------------------------------------------------------------

TEST_F(logging_macros_unittest, Test_LOG_FATAL_DETAILED)
{
  LOG_FATAL_DETAILED("How many seasons does %s have? Answer: %lu", "Mad Men", 8);
}

// -----------------------------------------------------------------------------
