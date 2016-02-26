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
#ifndef SNEAKER_CMDLINE_PROGRAM_H_
#define SNEAKER_CMDLINE_PROGRAM_H_

#include <boost/program_options.hpp>

#include <cstdint>
#include <string>


namespace sneaker {


namespace utility {


class cmdline_program
{
public:
  int run(int argc, char** argv);

protected:
  explicit cmdline_program(const char*);

  virtual ~cmdline_program();

  void add_string_parameter(const char*, const char*, std::string*);

  void add_uint32_parameter(const char*, const char*, uint32_t*);

  void add_uint64_parameter(const char*, const char*, uint64_t*);

  void add_float_parameter(const char*, const char*, float*);

  void add_positional_parameter(const char*, int);

  void add_boolean_parameter(const char*, const char*, bool*);

  template<typename T>
  void add_array_parameter(const char* name, const char* desc, std::vector<T>* res)
  {
    m_options.add_options()
      (name, boost::program_options::value<std::vector<T> >(res), desc);
  }

  bool option_provided(const char*) const;

private:
  /**
   * Runs the program, and returns the return value of the program.
   */
  virtual int do_run() = 0;

  /**
   * Checks the parameters passed to the program, and returns a boolean
   * indicating if the parameters are valid.
   */
  virtual bool check_parameters() const = 0;

private:
  typedef struct boolean_option
  {
    boolean_option(const char* name_, bool* res_)
      :
      name(name_),
      res(res_)
    {
    }

    const char* name;
    bool* res;
  } boolean_option;

private:
  const std::string m_description;
  boost::program_options::options_description m_options;
  boost::program_options::positional_options_description m_positionals;
  std::vector<boolean_option> m_boolean_params;
  boost::program_options::variables_map m_opts_map;
};


} /* end namespace utility */


} /* end namespace sneaker */


#endif /* SNEAKER_CMDLINE_PROGRAM_H_ */
