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


#ifndef SNEAKER_SCOPED_CONTEXT_H_
#define SNEAKER_SCOPED_CONTEXT_H_

#include <algorithm>
#include <cassert>
#include <vector>
#include "context_manager.h"


namespace sneaker {


namespace context {


template<class F, class... Args>
void scoped_context(
  sneaker::context::context_manager* mngr, F func, Args... args)
{
  assert(mngr);

  mngr->__enter__();

  try {
    func(mngr, args...);
  } catch (...) {}

  mngr->__exit__();
}


template<class F, class... Args>
void nested_context(
  std::vector<sneaker::context::context_manager*> mngrs, F func, Args... args)
{
  std::for_each(
    mngrs.begin(),
    mngrs.end(),
    [](context_manager *mngr) {
      mngr->__enter__();
    }
  );

  try {
    func(mngrs, args...);
  } catch (...) {}

  std::for_each(
    mngrs.rbegin(),
    mngrs.rend(),
    [](context_manager *mngr) {
      mngr->__exit__();
    }
  );
}


} /* end namespace context */


} /* end namespace sneaker */


#endif /* SNEAKER_SCOPED_CONTEXT_H_ */
