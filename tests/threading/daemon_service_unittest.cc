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

/* Unit test for `sneaker::threading::daemon_service`
 * defined in include/threading/daemon_service.h */

#include "../../include/threading/daemon_service.h"

#include "../../include/testing/testing.h"


class dummy_daemon_service : public sneaker::threading::daemon_service {
public:
  dummy_daemon_service(int num, bool wait_for_termination=false):
    daemon_service(wait_for_termination),
    _num(num)
  {
  }

  virtual void handle() {
    this->_num += 1;
  }

  int num() {
    return this->_num;
  }

private:
  int _num;
};


class daemon_service_unittest : public ::testing::Test {};


TEST_F(daemon_service_unittest, TestRunDaemonAsynchronously)
{
  dummy_daemon_service dummy_daemon(5);
  dummy_daemon.start();

  // sleep for one second in main thread to wait for daemon thread to process.
  sleep(1);

  ASSERT_EQ(5 + 1, dummy_daemon.num());
}

TEST_F(daemon_service_unittest, TestRunDaemonSynchronously)
{
  dummy_daemon_service dummy_daemon(5, true);
  dummy_daemon.start();

  ASSERT_EQ(5 + 1, dummy_daemon.num());
}
