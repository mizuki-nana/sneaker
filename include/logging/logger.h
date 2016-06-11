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
#ifndef SNEAKER_LOGGER_H_
#define SNEAKER_LOGGER_H_

#include "exception_safety_tag.h"
#include "logger_base.h"
#include "log_level.h"
#include "log_scheme.h"
#include "thread_safety_tag.h"

#include <cstdarg>
#include <mutex>


namespace sneaker {


namespace logging {


// -----------------------------------------------------------------------------

template<typename thread_safety_tag, typename exception_safety_tag>
class logger : protected logger_base
{
public:
  explicit logger(log_scheme* log_scheme);

  template<size_t LINE_SIZE=1024>
  void write(LogLevel log_lvl, const char* format, ...);
};

// -----------------------------------------------------------------------------

template<typename thread_safety_tag, typename exception_safety_tag>
logger<thread_safety_tag, exception_safety_tag>::logger(log_scheme* log_scheme)
  :
  logger_base(log_scheme)
{
}

// -----------------------------------------------------------------------------

template<>
template<size_t LINE_SIZE>
void
logger<thread_unsafe_tag, exception_safe_tag>::write(LogLevel log_lvl,
  const char* format, ...)
{
  try
  {
    va_list args;
    va_start(args, format);
    write_impl<LINE_SIZE>(log_lvl, format, args);
    va_end(args);
  }
  catch (...)
  {
  }
}

// -----------------------------------------------------------------------------

template<>
template<size_t LINE_SIZE>
void
logger<thread_unsafe_tag, exception_unsafe_tag>::write(LogLevel log_lvl,
  const char* format, ...)
{
  va_list args;
  va_start(args, format);
  write_impl<LINE_SIZE>(log_lvl, format, args);
  va_end(args);
}

// -----------------------------------------------------------------------------

template<typename exception_safety_tag>
class logger<thread_safe_tag, exception_safety_tag> : protected logger_base
{
public:
  explicit logger(log_scheme* log_scheme)
    :
    logger_base(log_scheme),
    m_mtx()
  {
  }

  template<size_t LINE_SIZE=1024>
  void write(LogLevel log_lvl, const char* format, ...);

private:
  std::mutex m_mtx;
};

// -----------------------------------------------------------------------------

template<>
template<size_t LINE_SIZE>
void
logger<thread_safe_tag, exception_safe_tag>::write(LogLevel log_lvl,
  const char* format, ...)
{
  std::lock_guard<std::mutex> lock(m_mtx);

  try
  {
    va_list args;
    va_start(args, format);
    write_impl<LINE_SIZE>(log_lvl, format, args);
    va_end(args);
  }
  catch (...)
  {
  }
}

// -----------------------------------------------------------------------------

template<>
template<size_t LINE_SIZE>
void
logger<thread_safe_tag, exception_unsafe_tag>::write(LogLevel log_lvl,
  const char* format, ...)
{
  std::lock_guard<std::mutex> lock(m_mtx);

  va_list args;
  va_start(args, format);
  write_impl<LINE_SIZE>(log_lvl, format, args);
  va_end(args);
}

// -----------------------------------------------------------------------------


} /* end namespace logging */


} /* end namespace sneaker */


#endif /* SNEAKER_LOGGER_H_ */
