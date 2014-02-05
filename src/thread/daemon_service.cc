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

#include "../../include/thread/daemon_service.h"

DaemonService::DaemonService(bool wait_for_termination):
  _wait_for_termination(wait_for_termination)
{
  this->init();
}

void
DaemonService::init()
{
  pthread_attr_init(&_attr);
  pthread_create(&_thread_id, &_attr, handler, (void*)this);

  // if(_wait_for_termination) {
  //   void* res = NULL;
  //   pthread_join(_thread_id, &res);
  // }
}

DaemonService::~DaemonService()
{
  // Do nothing here.
}

void*
DaemonService::handler(void* instance)
{
  reinterpret_cast<DaemonService*>(instance)->handle();
  return NULL;
}