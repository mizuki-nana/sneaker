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

/* Unit tests for definitions defined in sneaker/utility/cmdline_program.h */

#include "utility/cmdline_program.h"
#include "utility/util.numeric.h"

#include "testing/testing.h"

#include <boost/format.hpp>

#include <array>
#include <cstdint>
#include <limits>
#include <string>
#include <vector>


// -----------------------------------------------------------------------------

class cmdline_program_unittest : public ::testing::Test
{
public:
  class dummy_cmdline_program : public sneaker::utility::cmdline_program
  {
    public:
      dummy_cmdline_program()
        :
        sneaker::utility::cmdline_program("Dummy testing cmdline program"),
        m_input(),
        m_output(),
        m_uint32(0),
        m_uint64(0),
        m_float(0.0f),
        m_boolean(false),
        m_expected_input("--"),
        m_expected_output("--"),
        m_expected_uint32(std::numeric_limits<uint32_t>::max()),
        m_expected_float(std::numeric_limits<float>::max()),
        m_expected_boolean(true),
        m_result()
      {
        add_positional_parameter("input", 1);
        add_string_parameter("input", "a dummy input file", &m_input);
        add_string_parameter("output", "a dummy output file", &m_output);
        add_uint32_parameter("uint32", "a dummy number", &m_uint32);
        add_uint64_parameter("uint64", "a dummy number", &m_uint64);
        add_float_parameter("float", "a dummy floating point number", &m_float);
        add_boolean_parameter("boolean", "a dummy boolean", &m_boolean);
      }

      void set_expected_input(const std::string& expected_input)
      {
        m_expected_input = expected_input;
      }

      void set_expected_output(const std::string& expected_output)
      {
        m_expected_output = expected_output;
      }

      void set_expected_uint32(uint32_t expected_uint32)
      {
        m_expected_uint32 = expected_uint32;
      }

      void set_expected_uint64(uint64_t expected_uint64)
      {
        m_expected_uint64 = expected_uint64;
      }

      void set_expected_float(float expected_float)
      {
        m_expected_float = expected_float;
      }

      void set_expected_boolean(bool expected_boolean)
      {
        m_expected_boolean = expected_boolean;
      }

      std::string result() const
      {
        return m_result;
      }

    private:
      /** Overrides `cmdline_program::do_run()` */
      virtual int do_run()
      {
        m_result.assign(str(boost::format("%lu - %.2f - %d") % m_uint32 % m_float % m_boolean));
        return !!m_result.empty();
      }

      /** Overrides `cmdline_program::check_parameters()` */
      bool check_parameters() const
      {
        return (
          m_expected_input == m_input &&
          m_expected_output == m_output &&
          m_expected_uint32 == m_uint32 &&
          m_expected_uint64 == m_uint64 &&
          sneaker::utility::floats_equal(m_expected_float, m_float) &&
          m_expected_boolean == m_boolean
        );
      }

      std::string m_input;
      std::string m_output;
      uint32_t m_uint32;
      uint64_t m_uint64;
      float m_float;
      bool m_boolean;

      std::string m_expected_input;
      std::string m_expected_output;
      uint32_t m_expected_uint32;
      uint64_t m_expected_uint64;
      float m_expected_float;
      bool m_expected_boolean;

      std::string m_result;
  };
};

// -----------------------------------------------------------------------------

TEST_F(cmdline_program_unittest, TestHelp)
{
  dummy_cmdline_program dummy_program;

  const int argc = 2;
  std::array<char*, argc> argv {{
    (char*)"dummy_program",
    (char*)"--help"
  }};

  const int res = dummy_program.run(argc, argv.data());

  ASSERT_EQ(0, res);
}

// -----------------------------------------------------------------------------

TEST_F(cmdline_program_unittest, TestRunWithInvalidArgument)
{
  dummy_cmdline_program dummy_program;

  const int argc = 3;
  std::array<char*, argc> argv {{
    (char*)"dummy_program",
    (char*)"--invalid-arg",
    (char*)"32",
  }};

  const int res = dummy_program.run(argc, argv.data());

  ASSERT_EQ(-1, res);
}

// -----------------------------------------------------------------------------

TEST_F(cmdline_program_unittest, TestRunWithInvalidArgumentValue)
{
  dummy_cmdline_program dummy_program;

  const int argc = 3;
  std::array<char*, argc> argv {{
    (char*)"dummy_program",
    (char*)"--uint32",
    (char*)"Not an integer",
  }};

  const int res = dummy_program.run(argc, argv.data());

  ASSERT_EQ(-1, res);
}

// -----------------------------------------------------------------------------

TEST_F(cmdline_program_unittest, TestRun)
{
  dummy_cmdline_program dummy_program;

  const std::string expected_input("some input");
  const std::string expected_output("some output");
  const uint32_t expected_uint32 = 32;
  const uint64_t expected_uint64 = 64;
  const float expected_float = 3.14f;
  const bool expected_boolean = true;

  dummy_program.set_expected_input(expected_input);
  dummy_program.set_expected_output(expected_output);
  dummy_program.set_expected_uint32(expected_uint32);
  dummy_program.set_expected_uint64(expected_uint64);
  dummy_program.set_expected_float(expected_float);
  dummy_program.set_expected_boolean(expected_boolean);

  const int argc = 11;
  std::array<char*, argc> argv {{
    (char*)"dummy_program",
    (char*)expected_input.c_str(),
    (char*)"--output",
    (char*)expected_output.c_str(),
    (char*)"--uint32",
    (char*)"32",
    (char*)"--uint64",
    (char*)"64",
    (char*)"--float",
    (char*)"3.14",
    (char*)"--boolean",
  }};

  const int res = dummy_program.run(argc, argv.data());

  ASSERT_EQ(0, res);

  const std::string expected_result("32 - 3.14 - 1");
  const std::string actual_result(dummy_program.result());

  ASSERT_EQ(expected_result, actual_result);
}

// -----------------------------------------------------------------------------

TEST_F(cmdline_program_unittest, TestRunWithInvalidParameter)
{
  dummy_cmdline_program dummy_program;

  const std::string expected_input("some input");
  const std::string expected_output("some output");
  const uint32_t expected_uint32 = 32;
  const uint64_t expected_uint64 = 64;
  const float expected_float = 3.14f;
  const bool expected_boolean = true;

  dummy_program.set_expected_input(expected_input);
  dummy_program.set_expected_output(expected_output);
  dummy_program.set_expected_uint32(expected_uint32);
  dummy_program.set_expected_uint64(expected_uint64);
  dummy_program.set_expected_float(expected_float);
  dummy_program.set_expected_boolean(expected_boolean);

  const int argc = 10;
  std::array<char*, argc> argv {{
    (char*)"dummy_program",
    (char*)"Hello world",
    (char*)"--output",
    (char*)"Hello world",
    (char*)"--uint32",
    (char*)"0",
    (char*)"--uint64",
    (char*)"0",
    (char*)"--float",
    (char*)"0.0",
  }};

  const int res = dummy_program.run(argc, argv.data());

  ASSERT_EQ(-1, res);
}

// -----------------------------------------------------------------------------
