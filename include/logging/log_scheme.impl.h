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
#ifndef SNEAKER_LOG_SCHEME_IMPL_H_
#define SNEAKER_LOG_SCHEME_IMPL_H_

#include "log_scheme.h"

#include <iostream>


namespace sneaker {


namespace logging {


// -----------------------------------------------------------------------------

class stream_log_scheme : public log_scheme
{
public:
  virtual ~stream_log_scheme();

  virtual void write(const char* msg);

protected:
  stream_log_scheme();
  explicit stream_log_scheme(std::ostream* ost);

  std::ostream* m_ost;
};

// -----------------------------------------------------------------------------

class stdout_log_scheme : public stream_log_scheme
{
public:
  stdout_log_scheme();
  virtual ~stdout_log_scheme();
};

// -----------------------------------------------------------------------------

class stderr_log_scheme : public stream_log_scheme
{
public:
  stderr_log_scheme();
  virtual ~stderr_log_scheme();
};

// -----------------------------------------------------------------------------

class file_log_scheme : public stream_log_scheme
{
public:
  file_log_scheme(const char* filename);
  virtual ~file_log_scheme();
};

// -----------------------------------------------------------------------------


} /* end namespace logging */


} /* end namespace sneaker */


#endif /* SNEAKER_LOG_SCHEME_IMPL_H_ */
