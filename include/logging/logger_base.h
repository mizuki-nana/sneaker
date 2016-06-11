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
#ifndef SNEAKER_LOGGER_BASE_H_
#define SNEAKER_LOGGER_BASE_H_

#include "log_level.h"

#include <cstdarg>
#include <memory>


namespace sneaker {


namespace logging  {


// Forward declaration of `log_scheme`.
class log_scheme;


class logger_base
{
protected:
  explicit logger_base(log_scheme* log_scheme);

  template<size_t LINE_SIZE>
  void write_impl(LogLevel log_lvl, const char* format, va_list args)
  {
    char buf[LINE_SIZE] = {0};
    write_impl(log_lvl, buf, sizeof(buf), format, args);
  }

  void write_impl(LogLevel log_lvl, char* buf, size_t buf_size,
    const char* format, va_list args);

private:
  std::unique_ptr<log_scheme> m_log_scheme;
};


} /* end namespace logging */


} /* end namespace sneaker */


#endif /* SNEAKER_LOGGER_BASE_H_ */
