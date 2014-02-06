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

#ifndef _FIXED_TIME_INTERVAL_DAEMON_SERVICE_H_
#define _FIXED_TIME_INTERVAL_DAEMON_SERVICE_H_

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include "daemon_service.h"

typedef void(*ExternalHandler)(void);


namespace sneaker {

namespace threading {


class fixed_time_interval_daemon_service : public sneaker::threading::daemon_service {
public:
  fixed_time_interval_daemon_service(size_t, ExternalHandler);
  ~fixed_time_interval_daemon_service();

  size_t interval() const;

protected:
  virtual void handle();

  static void tick_handler(
    const boost::system::error_code&,
    boost::asio::deadline_timer* t,
    fixed_time_interval_daemon_service*
  );

  void invoke_external_handler();

private:
  ExternalHandler _external_handler;
  size_t _interval;
};


} /* threading */

} /* sneaker */


#endif /* _FIXED_TIME_INTERVAL_DAEMON_SERVICE_H_ */