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

// Define `BOOST_SYSTEM_NO_DEPRECATED` to not including deprecated
// definitions during compilation.
#define BOOST_SYSTEM_NO_DEPRECATED

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "../../include/thread/fixed_time_interval_daemon_service.h"
#include "../../include/libc/assert.h"

FixedTimeIntervalDaemonService::FixedTimeIntervalDaemonService(
  size_t interval,
  ExternalHandler external_handler
):
  _interval(interval),
  _external_handler(external_handler),
  DaemonService(false)
{
  ASSERT(_interval);
  ASSERT(_external_handler);
}

FixedTimeIntervalDaemonService::~FixedTimeIntervalDaemonService()
{
  // Do nothing here.
}

size_t
FixedTimeIntervalDaemonService::interval() const
{
  return _interval;
}

void
FixedTimeIntervalDaemonService::handle()
{
  boost::asio::io_service io;
  boost::asio::deadline_timer t(io, boost::posix_time::seconds(_interval));

  t.async_wait(
    boost::bind(tick_handler, boost::asio::placeholders::error, &t, this)
  );

  io.run();
}

void
FixedTimeIntervalDaemonService::invoke_external_handler()
{
  this->_external_handler();
}

void
FixedTimeIntervalDaemonService::tick_handler(
  const boost::system::error_code& e,
  boost::asio::deadline_timer* t,
  FixedTimeIntervalDaemonService* daemon_service
)
{
  t->expires_at(
    t->expires_at() + boost::posix_time::seconds(daemon_service->interval())
  );

  t->async_wait(
    boost::bind(
      tick_handler, boost::asio::placeholders::error, t, daemon_service)
  );

  daemon_service->invoke_external_handler();
}