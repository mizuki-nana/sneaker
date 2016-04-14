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
#ifndef SNEAKER_LOGGING_H_
#define SNEAKER_LOGGING_H_

#include "exception_safety_tag.h"
#include "logger.h"
#include "log_level.h"
#include "log_scheme.impl.h"
#include "thread_safety_tag.h"
#include "defaults.h"

#include <ctime>
#include <sstream>


// -----------------------------------------------------------------------------

#define LOG(lvl, file, line, format, ...)                 \
{                                                         \
  std::stringstream ss;                                   \
  ss << format << " (%s line %d)";                        \
  sneaker::logging::default_logger()->write(              \
    lvl, ss.str().c_str(), ##__VA_ARGS__, file, line);    \
}

// -----------------------------------------------------------------------------

#define LOG_DEBUG(format, ...) LOG(sneaker::logging::LogLevel::LOG_LEVEL_DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...)  LOG(sneaker::logging::LogLevel::LOG_LEVEL_INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...)  LOG(sneaker::logging::LogLevel::LOG_LEVEL_WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) LOG(sneaker::logging::LogLevel::LOG_LEVEL_ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_FATAL(format, ...) LOG(sneaker::logging::LogLevel::LOG_LEVEL_FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)

// -----------------------------------------------------------------------------

#define LOG_DETAILED(lvl, file, line, format, ...)        \
{                                                         \
  time_t rawtime;                                         \
  time(&rawtime);                                         \
  std::stringstream ss;                                   \
  ss << ctime(&rawtime) << format << " (%s line %d)";     \
  sneaker::logging::default_logger()->write(              \
    lvl, ss.str().c_str(), ##__VA_ARGS__, file, line);    \
}

// -----------------------------------------------------------------------------

#define LOG_DEBUG_DETAILED(format, ...) LOG_DETAILED(sneaker::logging::LogLevel::LOG_LEVEL_DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_INFO_DETAILED(format, ...)  LOG_DETAILED(sneaker::logging::LogLevel::LOG_LEVEL_INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_WARN_DETAILED(format, ...)  LOG_DETAILED(sneaker::logging::LogLevel::LOG_LEVEL_WARN, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_ERROR_DETAILED(format, ...) LOG_DETAILED(sneaker::logging::LogLevel::LOG_LEVEL_ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_FATAL_DETAILED(format, ...) LOG_DETAILED(sneaker::logging::LogLevel::LOG_LEVEL_FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)

// -----------------------------------------------------------------------------


#endif /* SNEAKER_LOGGING_H_ */
