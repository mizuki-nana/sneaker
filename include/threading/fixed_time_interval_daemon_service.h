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
#ifndef SNEAKER_FIXED_TIME_INTERVAL_DAEMON_SERVICE_H_
#define SNEAKER_FIXED_TIME_INTERVAL_DAEMON_SERVICE_H_

#include <cstdint>
#include <limits.h>
#include <memory>


typedef void(*ExternalHandler)(void*);


namespace sneaker {


namespace threading {


class fixed_time_interval_daemon_service
{
public:
  fixed_time_interval_daemon_service(uint32_t, ExternalHandler,
    bool=false, int32_t=INT_MAX);

  virtual ~fixed_time_interval_daemon_service();

  size_t interval() const;

  virtual bool start();

private:
  class impl;

  std::unique_ptr<impl> m_impl;
};


} /* end namespace threading */


} /* end namespace sneaker */


#endif /* SNEAKER_FIXED_TIME_INTERVAL_DAEMON_SERVICE_H_ */
