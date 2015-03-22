/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 Yanzheng Li

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
#include "../../include/utility/stack_trace.h"

#include <cxxabi.h>
#include <execinfo.h>
#include <ostream>
#include <string>


void
sneaker::utility::stack_trace::print_stack_trace(
  std::ostream& stream, unsigned int max_frames)
{
  unsigned int frames_num = max_frames + 1;

  // Array for storing stack trace data.
  void* addrlist[frames_num];

  // Retrieve current stack addresses.
  int addrlen = backtrace(addrlist, frames_num);

  if (!addrlen)
  {
    return;
  }

  // Resolve addresses into strings containing "filename(function+address)"
  // Note: This array must be freed.
  char** symbollist = backtrace_symbols(addrlist, addrlen);

  if (!symbollist)
  {
    return;
  }

  // Allocate string which will be filled with the demangled function name.
  size_t func_name_size = 256;
  std::string funcname;
  funcname.reserve(func_name_size);

  // Iterate over the returned symbol lines.
  // Skip the first line, it is the address of this function.
  for (int i = 1; i < addrlen; ++i)
  {
    char *begin_name = 0;
    char *begin_offset = 0;
    char *end_offset = 0;

    // Find parentheses and +address offset surrounding the mangled name:
    // ./module(function+0x16c) [0x8185ca7]
    for (char *p = symbollist[i]; *p; ++p)
    {
      if (*p == '(')
      {
        begin_name = p;
      }
      else if (*p == '+')
      {
        begin_offset = p;
      }
      else if (*p == ')' && begin_offset)
      {
        end_offset = p;
        break;
      }
    }

    if (begin_name && begin_offset && end_offset
        && begin_name < begin_offset)
    {
      *begin_name++ = '\0';
      *begin_offset++ = '\0';
      *end_offset = '\0';

      // Mangled name is now in [begin_name, begin_offset)
      // caller offset in [begin_name, end_offset).
      // Now apply __ctx_demangle()
      int status;
      char* output_buffer = NULL;
      char* res = abi::__cxa_demangle(begin_name, output_buffer, &func_name_size, &status);

      if (status == 0 && res)
      {
        funcname.assign(res);
        stream << symbollist[i] << " : " << funcname << '+' << begin_name << std::endl;
        free(res);
      }
      else
      {
        // Demangling failed. Output function name as a C function with no arguments.
        stream << symbollist[i] << " : " << begin_name << "()" << "+" << begin_offset << std::endl;
      }
    }
    else
    {
      // Could not parse the line.
      stream << symbollist[i] << std::endl;
    }
  }

  free(symbollist);
}
