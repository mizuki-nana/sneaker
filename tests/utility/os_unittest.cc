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

/* Unit tests for definitions defined in sneaker/utility/os.h */

#include "../../include/utility/os.h"

#include "../../include/testing/testing.h"

#include <iostream>


// -----------------------------------------------------------------------------

using namespace sneaker::utility;

// -----------------------------------------------------------------------------

class os_unittest : public ::testing::Test {};

// -----------------------------------------------------------------------------

TEST_F(os_unittest, TestGetProcessMemUsage)
{
  uint64_t vm_peak = 0;
  uint64_t vm_size = 0;
  uint64_t vm_hwm = 0;
  uint64_t vm_rss = 0;

  get_process_mem_usage(&vm_peak, &vm_size, &vm_hwm, &vm_rss);

  const std::string unit("kB");

  std::cout << "Memory usages from `get_process_mem_usage`:" << std::endl;
  std::cout << "VmPeak: " << vm_peak << " " << unit << std::endl;
  std::cout << "VmSize: " << vm_size << " " << unit << std::endl;
  std::cout << "VmHWM: " << vm_hwm << " " << unit << std::endl;
  std::cout << "VmRSS: " << vm_rss << " " << unit << std::endl;
}

// -----------------------------------------------------------------------------

TEST_F(os_unittest, TestGetProcessVmPeak)
{
  const uint64_t vm_peak = get_process_vm_peak();

  const std::string unit("kB");

  std::cout << "Memory usages from `get_process_vm_peak`:" << std::endl;
  std::cout << "VmPeak: " << vm_peak << " " << unit << std::endl;
}

// -----------------------------------------------------------------------------

TEST_F(os_unittest, TestGetProcessVmSize)
{
  const uint64_t vm_size = get_process_vm_size();

  const std::string unit("kB");

  std::cout << "Memory usages from `get_process_vm_size`:" << std::endl;
  std::cout << "VmSize: " << vm_size << " " << unit << std::endl;
}

// -----------------------------------------------------------------------------

TEST_F(os_unittest, TestGetProcessVmHWM)
{
  const uint64_t vm_hwm = get_process_vm_hwm();

  const std::string unit("kB");

  std::cout << "Memory usages from `get_process_vm_hwm`:" << std::endl;
  std::cout << "VmHWM: " << vm_hwm << " " << unit << std::endl;
}

// -----------------------------------------------------------------------------

TEST_F(os_unittest, TestGetProcessVmRSS)
{
  const uint64_t vm_rss = get_process_vm_rss();

  const std::string unit("kB");

  std::cout << "Memory usages from `get_process_vm_rss`:" << std::endl;
  std::cout << "VmRSS: " << vm_rss << " " << unit << std::endl;
}

// -----------------------------------------------------------------------------
