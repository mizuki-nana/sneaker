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
#ifndef SNEAKER_CONTEXT_MANAGER_H_
#define SNEAKER_CONTEXT_MANAGER_H_

#include <vector>


namespace sneaker {
namespace context {

class context_manager {
public:
  virtual ~context_manager() {}

protected:
  virtual void __enter__() = 0;
  virtual void __exit__() = 0;

  template<class F, class... Args>
  friend void scoped_context(context_manager* mngr, F func, Args... args);

  template<class F, class... Args>
  friend void nested_context(
    const std::vector<sneaker::context::context_manager*>& mngrs, F func, Args... args);
};

} /* end namespace context */
} /* end namespace sneaker */


#endif /* SNEAKER_CONTEXT_MANAGER_H_ */
