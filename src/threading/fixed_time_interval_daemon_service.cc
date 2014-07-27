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

#include <cassert>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "../../include/threading/fixed_time_interval_daemon_service.h"


sneaker::threading::fixed_time_interval_daemon_service::fixed_time_interval_daemon_service(
  size_t interval,
  ExternalHandler external_handler,
  bool wait_for_termination,
  ssize_t max_iterations
):
  sneaker::threading::daemon_service(wait_for_termination),
  _external_handler(external_handler),
  _interval(interval),
  _max_iterations(max_iterations),
  _iteration_count(0)
{
  assert(_interval >= 0);
  assert(_external_handler);
}

sneaker::threading::fixed_time_interval_daemon_service::~fixed_time_interval_daemon_service()
{
  // Do nothing here.
}

size_t
sneaker::threading::fixed_time_interval_daemon_service::interval() const
{
  return _interval;
}

void
sneaker::threading::fixed_time_interval_daemon_service::handle()
{
  boost::asio::io_service io;
  boost::asio::deadline_timer t(io, boost::posix_time::seconds(_interval));

  t.async_wait(
    boost::bind(tick_handler, boost::asio::placeholders::error, &t, this)
  );

  io.run();
}

void
sneaker::threading::fixed_time_interval_daemon_service::invoke_external_handler()
{
  this->_external_handler(this);
  this->increment_iteration_count();
}

void
sneaker::threading::fixed_time_interval_daemon_service::increment_iteration_count()
{
  this->_iteration_count++;
}

bool
sneaker::threading::fixed_time_interval_daemon_service::can_continue()
{
  return this->_iteration_count < this->_max_iterations || this->_max_iterations == -1;
}

void
sneaker::threading::fixed_time_interval_daemon_service::tick_handler(
  const boost::system::error_code& e,
  boost::asio::deadline_timer* t,
  sneaker::threading::fixed_time_interval_daemon_service* daemon_service
)
{
  if(!daemon_service->can_continue()) {
    return;
  }

  t->expires_at(
    t->expires_at() + boost::posix_time::seconds(daemon_service->interval())
  );

  t->async_wait(
    boost::bind(
      tick_handler, boost::asio::placeholders::error, t, daemon_service)
  );

  daemon_service->invoke_external_handler();
}
