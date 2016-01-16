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
#ifndef SNEAKER_FUNCTION_H_
#define SNEAKER_FUNCTION_H_

#include <cassert>
#include <functional>
#include <pthread.h>


namespace sneaker {


namespace functional {


template<class R, class... Args>
class function {
public:

  typedef R(*_F)(Args...);

  using implicit_type = typename function::_F;
  typedef R return_type;

  function(_F func);

  template<class Functor>
  function(Functor func);

  const function<R, Args...>& operator=(_F);

  R operator() (Args... args) const;

  operator implicit_type();

  void invoke_async(Args... args);

protected:
  _F m_func;

  static void* handler(void* p);
};

// -----------------------------------------------------------------------------

template<class R, class... Args>
function<R, Args...>::function(_F func):
  m_func(func)
{
  assert(m_func);
}

// -----------------------------------------------------------------------------

template<class R, class... Args>
template<class Functor>
function<R, Args...>::function(Functor func)
{
  assert(func);
  m_func = func;
}

// -----------------------------------------------------------------------------

template<class R, class... Args>
const function<R, Args...>&
function<R, Args...>::operator=(_F func)
{
  assert(func);
  m_func = func;
  return *this;
}

// -----------------------------------------------------------------------------

template<class R, class... Args>
R
function<R, Args...>::operator() (Args... args) const
{
  return m_func(args...);
}

// -----------------------------------------------------------------------------

template<class R, class... Args>
function<R, Args...>::operator implicit_type()
{
  return m_func;
}

// -----------------------------------------------------------------------------

template<class R, class... Args>
void
function<R, Args...>::invoke_async(Args... args)
{
  auto f = std::bind(m_func, args...);
  std::function<void()> *wrapper = new std::function<void()>(f);

  pthread_t thread_id;
  pthread_create(&thread_id, NULL, handler, (void*)(wrapper));
}

// -----------------------------------------------------------------------------

template<class R, class... Args>
void*
function<R, Args...>::handler(void* p)
{
  std::function<void()> *f = reinterpret_cast<std::function<void()>*>(p);
  (*f)();
  delete f;
  return NULL;
}

// -----------------------------------------------------------------------------

using call = typename sneaker::functional::function<void>;

// -----------------------------------------------------------------------------

template<class... Args>
using action = typename sneaker::functional::function<void, Args...>;

// -----------------------------------------------------------------------------

template<class... Args>
using predicate = typename sneaker::functional::function<bool, Args...>;

// -----------------------------------------------------------------------------


} /* end namespace functional */


} /* end namespace sneaker */


#endif /* SNEAKER_FUNCTION_H_ */
