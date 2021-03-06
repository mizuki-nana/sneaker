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
#include "threading/daemon_service.h"


namespace sneaker {


namespace threading {


// -----------------------------------------------------------------------------

daemon_service::daemon_service(bool wait_for_termination)
  :
  m_wait_for_termination(wait_for_termination)
{
  this->init();
}

// -----------------------------------------------------------------------------

void
daemon_service::init()
{
  pthread_attr_init(&m_attr);
  pthread_attr_setdetachstate(&m_attr, PTHREAD_CREATE_JOINABLE);
}

// -----------------------------------------------------------------------------

daemon_service::~daemon_service()
{
  pthread_attr_destroy(&m_attr);
}

// -----------------------------------------------------------------------------

bool
daemon_service::start()
{
  int created = pthread_create(&m_thread_id, &m_attr, handler, reinterpret_cast<void*>(this));

  if (m_wait_for_termination) {
    void* res = NULL;
    created = pthread_join(m_thread_id, &res);
  }

  return created == 0;
}

// -----------------------------------------------------------------------------

void*
daemon_service::handler(void* instance)
{
  reinterpret_cast<daemon_service*>(instance)->handle();
  return NULL;
}

// -----------------------------------------------------------------------------


} /* end namespace threading */


} /* end namespace sneaker */
