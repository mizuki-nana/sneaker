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
#include "logging/logger_base.h"

#include "logging/log_scheme.h"

#include <cstdio>


namespace sneaker {


namespace logging {


// -----------------------------------------------------------------------------

logger_base::logger_base(log_scheme* log_scheme)
  :
  m_log_scheme(log_scheme)
{
}

// -----------------------------------------------------------------------------

void
logger_base::write_impl(LogLevel log_lvl, char* buf, size_t buf_size,
  const char* format, va_list args)
{
  static const char* log_lvl_names[] {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
  };

  const char* log_lvl_name = log_lvl_names[static_cast<uint32_t>(log_lvl)];

  int n = snprintf(buf, buf_size, "[%s] ", log_lvl_name);

  if (n && static_cast<unsigned long>(n) < buf_size)
  {
    vsnprintf(buf + n, buf_size - static_cast<unsigned long>(n), format, args);

    m_log_scheme->write(buf);
  }
}

// -----------------------------------------------------------------------------


} /* end namespace logging */


} /* end namespace sneaker */
