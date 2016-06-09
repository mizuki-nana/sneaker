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
#ifndef SNEAKER_DECORATORS_H_
#define SNEAKER_DECORATORS_H_

#include "function.h"

#include <cassert>
#include <cstdint>


namespace sneaker {


namespace functional {


// -----------------------------------------------------------------------------

template<class R, class... Args>
class retry: public sneaker::functional::function<R, Args...> {
public:
  template<class Functor>
  retry(Functor func)
    :
    sneaker::functional::function<R, Args...>(func)
  {
  }

  template<class ExceptionType, uint32_t N>
  R operator() (Args... args) const {
    for (size_t i = 0; i < N; ++i) {
      try {
        return this->m_func(args...);
      } catch(const ExceptionType&) {
        continue;
      } catch(const std::exception& exc) {
        throw exc;
      }
    }

    return this->m_func(args...);
  }
};

// -----------------------------------------------------------------------------


} /* end namespace functional */


} /* end namespace sneaker */


#endif /* SNEAKER_DECORATORS_H_ */
