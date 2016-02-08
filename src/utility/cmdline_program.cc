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
#include "../../include/utility/cmdline_program.h"

#include <boost/program_options.hpp>

#include <cstdint>
#include <exception>
#include <iostream>
#include <string>


namespace sneaker {


namespace utility {


// -----------------------------------------------------------------------------

cmdline_program::cmdline_program(const char* description)
  :
  m_description(description)
{
  m_options.add_options()("help", "Print a help message and exit");
}

// -----------------------------------------------------------------------------

int
cmdline_program::do_run()
{
  return 0;
}

// -----------------------------------------------------------------------------

bool
cmdline_program::check_parameters() const
{
  return true;
}

// -----------------------------------------------------------------------------

void
cmdline_program::add_string_parameter(
  const char* name, const char* description, std::string* res)
{
  m_options.add_options()
    (name, boost::program_options::value<std::string>(res), description);
}

// -----------------------------------------------------------------------------

void
cmdline_program::add_uint64_parameter(
  const char* name, const char* description, uint64_t* res)
{
  m_options.add_options()
    (name, boost::program_options::value<uint64_t>(res), description);
}

// -----------------------------------------------------------------------------

void
cmdline_program::add_uint32_parameter(
  const char* name, const char* description, uint32_t* res)
{
  m_options.add_options()
    (name, boost::program_options::value<uint32_t>(res), description);
}

// -----------------------------------------------------------------------------

void
cmdline_program::add_float_parameter(
  const char* name, const char* description, float* res)
{
  m_options.add_options()
    (name, boost::program_options::value<float>(res), description);
}

// -----------------------------------------------------------------------------

void
cmdline_program::add_positional_parameter(
  const char* name, int n)
{
  m_positionals.add(name, n);
}

// -----------------------------------------------------------------------------

void
cmdline_program::add_boolean_parameter(
  const char* name, const char* description, bool* res)
{
  m_options.add_options()(name, description);
  m_boolean_params.push_back(boolean_option(name, res));
}

// -----------------------------------------------------------------------------

bool
cmdline_program::option_provided(const char* name) const
{
  return m_opts_map.count(name) > 0;
}

// -----------------------------------------------------------------------------

int
cmdline_program::run(int argc, char** argv)
{
  try
  {
    // Parse command-line parameters.
    boost::program_options::store(
      boost::program_options::command_line_parser(
        argc, argv).options(m_options).positional(m_positionals).run(),
      m_opts_map
    );
    boost::program_options::notify(m_opts_map);

    // Check if help is requested.
    if (option_provided("help"))
    {
      std::cout << m_description << std::endl;
      std::cout << "Usage: " << argv[0] << " [options]" << std::endl;
      std::cout << "Options: " << std::endl;
      std::cout << m_options;
      return 0;
    }

    // Set boolean parameters.
    for (auto i = 0; i < m_boolean_params.size(); ++i)
    {
      *m_boolean_params[i].res = option_provided(m_boolean_params[i].name);
    }

    // Check the parameters.
    if (!check_parameters())
    {
      std::cerr << "Invalid arguments. Run with --help for usage." << std::endl;
      return -1;
    }

    return do_run();
  }
  catch (const boost::program_options::error& ex)
  {
    std::cerr << "Error in parsing options: " << ex.what() << std::endl;
    return -1;
  }
  catch (const std::exception& ex)
  {
    std::cerr << "Exception: " << ex.what() << std::endl;
  }
  catch (...)
  {
    std::cerr << "Unknown exception" << std::endl;
    return -1;
  }

  return -1;
}

// -----------------------------------------------------------------------------


} /* end namespace utility */


} /* end namespace sneaker */
