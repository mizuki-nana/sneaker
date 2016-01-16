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

#include "../../include/utility/cmdline_program.h"

#include "../../include/testing/testing.h"

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
        m_number(0),
        m_float(0.0f),
        m_boolean(false),
        m_expected_input("--"),
        m_expected_output("--"),
        m_expected_number(std::numeric_limits<uint32_t>::max()),
        m_expected_float(std::numeric_limits<float>::max()),
        m_expected_boolean(true)
      {
        add_positional_parameter("input", 1);
        add_string_parameter("input", "a dummy input file", &m_input);
        add_string_parameter("output", "a dummy output file", &m_output);
        add_uint32_parameter("number", "a dummy number", &m_number);
        add_float_parameter("float", "a dummy floating point number", &m_float);
        add_boolean_parameter("boolean", "a dummy boolean", &m_boolean);
      }

      virtual int do_run()
      {
        return 0;
      }

      virtual bool check_parameters() const
      {
        return (
          m_expected_input == m_input &&
          m_expected_output == m_output &&
          m_expected_number == m_number &&
          m_expected_float == m_float &&
          m_expected_boolean == m_boolean
        );
      }

      void set_expected_input(const std::string& expected_input)
      {
        m_expected_input = expected_input;
      }

      void set_expected_output(const std::string& expected_output)
      {
        m_expected_output = expected_output;
      }

      void set_expected_number(const uint32_t expected_number)
      {
        m_expected_number = expected_number;
      }

      void set_expected_float(const float expected_float)
      {
        m_expected_float = expected_float;
      }

      void set_expected_boolean(const bool expected_boolean)
      {
        m_expected_boolean = expected_boolean;
      }

    private:
      std::string m_input;
      std::string m_output;
      uint32_t m_number;
      float m_float;
      bool m_boolean;

      std::string m_expected_input;
      std::string m_expected_output;
      uint32_t m_expected_number;
      float m_expected_float;
      bool m_expected_boolean;
  };
};

// -----------------------------------------------------------------------------

TEST_F(cmdline_program_unittest, TestRun)
{
  dummy_cmdline_program dummy_program;

  const std::string expected_input = "some input";
  const std::string expected_output = "some output";
  const uint32_t expected_number = 32;
  const float expected_float = 3.14;
  const bool expected_boolean = true;

  dummy_program.set_expected_input(expected_input);
  dummy_program.set_expected_output(expected_output);
  dummy_program.set_expected_number(expected_number);
  dummy_program.set_expected_float(expected_float);
  dummy_program.set_expected_boolean(expected_boolean);

  int argc = 9;
  char** argv = nullptr;

  argv = (char* [])
  {
    (char*)"dummy_program",
    (char*)expected_input.c_str(),
    (char*)"--output",
    (char*)expected_output.c_str(),
    (char*)"--number",
    (char*)"32",
    (char*)"--float",
    (char*)"3.14",
    (char*)"--boolean",
  };

  int res = dummy_program.run(argc, argv);

  ASSERT_EQ(0, res);
}

// -----------------------------------------------------------------------------
