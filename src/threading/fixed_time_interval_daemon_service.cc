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
#include "threading/fixed_time_interval_daemon_service.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/system/error_code.hpp>

#include <cassert>


namespace sneaker {


namespace threading {


// -----------------------------------------------------------------------------

fixed_time_interval_daemon_service::fixed_time_interval_daemon_service(
  uint32_t interval,
  ExternalHandler external_handler,
  bool wait_for_termination,
  int32_t max_iterations
):
  daemon_service(wait_for_termination),
  m_external_handler(external_handler),
  m_interval(interval),
  m_max_iterations(max_iterations),
  m_iteration_count(0),
  m_io_service(),
  m_timer(m_io_service, boost::posix_time::milliseconds(m_interval))
{
  assert(m_interval >= 0);
  assert(m_external_handler);
}

// -----------------------------------------------------------------------------

fixed_time_interval_daemon_service::~fixed_time_interval_daemon_service()
{
  stop();
}

// -----------------------------------------------------------------------------

size_t
fixed_time_interval_daemon_service::interval() const
{
  return m_interval;
}

// -----------------------------------------------------------------------------

void
fixed_time_interval_daemon_service::handle()
{
  m_timer.async_wait(
    boost::bind(tick_handler, boost::asio::placeholders::error, &m_timer, this)
  );

  m_io_service.run();
}

// -----------------------------------------------------------------------------

void
fixed_time_interval_daemon_service::invoke_external_handler()
{
  this->m_external_handler(this);
  this->increment_iteration_count();
}

// -----------------------------------------------------------------------------

void
fixed_time_interval_daemon_service::increment_iteration_count()
{
  this->m_iteration_count++;
}

// -----------------------------------------------------------------------------

bool
fixed_time_interval_daemon_service::can_continue() const
{
  return m_max_iterations == -1 ||
    (static_cast<int32_t>(m_iteration_count) < m_max_iterations);
}

// -----------------------------------------------------------------------------

void
fixed_time_interval_daemon_service::stop()
{
  using namespace boost::system;
  auto err = errc::make_error_code(errc::operation_canceled);
  m_timer.cancel(err);
  m_timer.wait();

  m_io_service.stop();
}

// -----------------------------------------------------------------------------

void
fixed_time_interval_daemon_service::tick_handler(
  const boost::system::error_code& err,
  boost::asio::deadline_timer* t,
  fixed_time_interval_daemon_service* daemon_service
)
{
  if (err.value() != boost::system::errc::success)
  {
    return;
  }

  if (!daemon_service->can_continue()) {
    return;
  }

  t->expires_from_now(
    boost::posix_time::milliseconds(
      static_cast<boost::int64_t>(daemon_service->interval()))
  );

  t->async_wait(
    boost::bind(
      tick_handler, boost::asio::placeholders::error, t, daemon_service)
  );

  daemon_service->invoke_external_handler();
}

// -----------------------------------------------------------------------------

void
fixed_time_interval_daemon_service::dummy_tick_handler(
  const boost::system::error_code& /* e */,
  boost::asio::deadline_timer* /* t */,
  long)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------


} /* end namespace threading */


} /* end namespace sneaker */
