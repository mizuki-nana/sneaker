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
#include "../../include/utility/os.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>


namespace sneaker {


namespace utility {


// -----------------------------------------------------------------------------

template<typename StreamType, typename ValueType>
void
parse(const std::string& line, const std::string& key, ValueType* value)
{
  StreamType stream(line.substr(key.size()));
  stream >> *value;
}

// -----------------------------------------------------------------------------

void
get_process_mem_usage(uint64_t* vm_peak, uint64_t* vm_size,
  uint64_t* vm_hwm, uint64_t* vm_rss)
{
#ifdef __linux__
  static const std::string VM_PEAK_KEY("VmPeak:");
  static const std::string VM_SIZE_KEY("VmSize:");
  static const std::string VM_HWM_KEY("VmHWM:");
  static const std::string VM_RSS_KEY("VmRSS:");

  std::ifstream stat_stream("/proc/self/status", std::ios::in);

  std::string line;

  while (std::getline(stat_stream, line))
  {
    if (vm_peak && line.compare(0, VM_PEAK_KEY.size(), VM_PEAK_KEY) == 0)
    {
      parse<std::stringstream, uint64_t>(line, VM_PEAK_KEY, vm_peak);
    }
    else if (vm_size && line.compare(0, VM_SIZE_KEY.size(), VM_SIZE_KEY) == 0)
    {
      parse<std::stringstream, uint64_t>(line, VM_SIZE_KEY, vm_size);
    }
    else if (vm_hwm && line.compare(0, VM_HWM_KEY.size(), VM_HWM_KEY) == 0)
    {
      parse<std::stringstream, uint64_t>(line, VM_HWM_KEY, vm_hwm);
    }
    else if (vm_rss && line.compare(0, VM_RSS_KEY.size(), VM_RSS_KEY) == 0)
    {
      parse<std::stringstream, uint64_t>(line, VM_RSS_KEY, vm_rss);
    }
  }

  stat_stream.close();
#endif
}

// -----------------------------------------------------------------------------

uint64_t get_process_vm_peak()
{
  uint64_t res = 0;
  get_process_mem_usage(&res, NULL, NULL, NULL);
  return res;
}

// -----------------------------------------------------------------------------

uint64_t get_process_vm_size()
{
  uint64_t res = 0;
  get_process_mem_usage(NULL, &res, NULL, NULL);
  return res;
}

// -----------------------------------------------------------------------------

uint64_t get_process_vm_hwm()
{
  uint64_t res = 0;
  get_process_mem_usage(NULL, NULL, &res, NULL);
  return res;
}

// -----------------------------------------------------------------------------

uint64_t get_process_vm_rss()
{
  uint64_t res = 0;
  get_process_mem_usage(NULL, NULL, NULL, &res);
  return res;
}

// -----------------------------------------------------------------------------


} /* end namespace utility */


} /* end namespace sneaker */
