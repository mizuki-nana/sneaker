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
#include "logging/log_scheme.impl.h"

#include <iostream>
#include <fstream>


namespace sneaker {


namespace logging {


// -----------------------------------------------------------------------------

log_scheme::~log_scheme()
{
}

// -----------------------------------------------------------------------------

stream_log_scheme::stream_log_scheme()
  :
  m_ost(NULL)
{
}

// -----------------------------------------------------------------------------

stream_log_scheme::stream_log_scheme(std::ostream* ost)
  :
  m_ost(ost)
{
}

// -----------------------------------------------------------------------------

/* virtual */
stream_log_scheme::~stream_log_scheme()
{
}

// -----------------------------------------------------------------------------

void
stream_log_scheme::write(const char* msg)
{
  if (m_ost && m_ost->good())
  {
    *m_ost << msg << std::endl;
  }
}

// -----------------------------------------------------------------------------

stdout_log_scheme::stdout_log_scheme()
  :
  stream_log_scheme(&std::cout)
{
}

// -----------------------------------------------------------------------------

stdout_log_scheme::~stdout_log_scheme()
{
}

// -----------------------------------------------------------------------------

stderr_log_scheme::stderr_log_scheme()
  :
  stream_log_scheme(&std::cerr)
{
}

// -----------------------------------------------------------------------------

stderr_log_scheme::~stderr_log_scheme()
{
}

// -----------------------------------------------------------------------------

file_log_scheme::file_log_scheme(const char* filename)
  :
  stream_log_scheme(new std::ofstream(filename))
{
}

// -----------------------------------------------------------------------------

file_log_scheme::~file_log_scheme()
{
  if (m_ost)
  {
    auto ost = dynamic_cast<std::ofstream*>(m_ost);
    if (ost)
    {
      ost->close();
    }

    free(m_ost);
  }
}

// -----------------------------------------------------------------------------


} /* end namespace logging */


} /* end namespace sneaker */
