/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2014 Yanzheng Li

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

#include <stdio.h>
#include "../../include/testing/testing.h"
#include "../../include/threading/fixed_time_interval_daemon_service.h"


using namespace sneaker::threading;


void DummyHandler()
{
  printf("Dummy handler for daemon service running in the background thread...\n");
}

class FixedTimeIntervalDaemonServiceUnitTest : public ::testing::Test {};


TEST_F(FixedTimeIntervalDaemonServiceUnitTest, TestRunDaemonAsynchronously)
{
  fixed_time_interval_daemon_service daemon_service(2, DummyHandler);
  daemon_service.start();

  sleep(10);

  printf("10 seconds has passed...\n");

  return;
}

TEST_F(FixedTimeIntervalDaemonServiceUnitTest, TestRunDaemonSynchronously)
{
  fixed_time_interval_daemon_service daemon_service(2, DummyHandler, true, 5);
  daemon_service.start();

  printf("Dummy handler should ran 5 times...\n");

  return;
}