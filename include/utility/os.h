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
#ifndef SNEAKER_UTILITY_OS_H_
#define SNEAKER_UTILITY_OS_H_

#include <cstdint>


namespace sneaker {
namespace utility {

// -----------------------------------------------------------------------------

/**
 * Get the memory usages of the current process, in number of kilo bytes.
 *
 * Supported platforms:
 *  - Linux
 *  - OS X
 *
 * vm_peak: Peak virtual memory size (currently not supported on OS X).
 * vm_size: Virtual memory size.
 * vm_hwm: Peak resident set size ("high water mark") (currently not supported on OS X).
 * vm_rss: Resident set size.
 *
 * References:
 *  - http://man7.org/linux/man-pages/man5/proc.5.html
 *      Search for: /proc/[pid]/status
 *  - http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
 *  	Scroll down to the "Mac OS X" section.
 */
void
get_process_mem_usage(uint64_t* vm_peak, uint64_t* vm_size,
  uint64_t* vm_hwm, uint64_t* vm_rss);

// -----------------------------------------------------------------------------

/** Get the peak virtual memory size of the current process, in kB. */
uint64_t get_process_vm_peak();

// -----------------------------------------------------------------------------

/** Get the virtual memory size of the current process, in kB. */
uint64_t get_process_vm_size();

// -----------------------------------------------------------------------------

/** Get the peak resident set size of the current process, in kB. */
uint64_t get_process_vm_hwm();

// -----------------------------------------------------------------------------

/** Get the resident set size of the current process, in kB. */
uint64_t get_process_vm_rss();

// -----------------------------------------------------------------------------

} /* end namespace utility */
} /* end namespace sneaker */


#endif /* SNEAKER_UTILITY_OS_H_ */
