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

/* Unit test for `sneaker::threading::fixed_time_interval_daemon_service`
 * defined in include/threading/fixed_time_interval_daemon_service.h */

#include "../../include/threading/fixed_time_interval_daemon_service.h"

#include "../../include/testing/testing.h"

#include <iostream>


using namespace sneaker::threading;


class fixed_time_interval_daemon_service_unittest : public ::testing::Test {
public:
  static void DummyHandler(void*) {
    std::cout << '.';
  }
};


TEST_F(fixed_time_interval_daemon_service_unittest, TestRunDaemonAsynchronously)
{
  /* Tests that a daemon which runs on a background thread indefinitely,
   * and pokes the foreground threading at an interval of 20 milli-seconds.
   *
   * When the foreground thread finishes, a.k.a when the daemon goes out of
   * scope and gets destroyed, the background thread should terminate.
   */
  fixed_time_interval_daemon_service daemon_service(
    20, fixed_time_interval_daemon_service_unittest::DummyHandler);

  daemon_service.start();
}

TEST_F(fixed_time_interval_daemon_service_unittest, TestRunDaemonSynchronously)
{
  /* Tests that a daemon which runs on a background thread, and pokes the
   * foreground thread 5 times at an interval of 20 milli-seconds.
   *
   * The foreground has to wait for the background thread to terminate before
   * it can proceed.
   */
  fixed_time_interval_daemon_service daemon_service(
    20, fixed_time_interval_daemon_service_unittest::DummyHandler, true, 5);

  daemon_service.start();
  std::cout << '\n';
}
