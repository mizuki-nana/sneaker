#ifndef SNEAKER_DECORATORS_H_
#define SNEAKER_DECORATORS_H_


#include <cassert>
#include "function.h"


namespace sneaker {


namespace functional {


template<class R, class... Args>
class retry: public sneaker::functional::function<R, Args...> {
public:
  template<class Functor>
  retry(Functor func) : sneaker::functional::function<R, Args...>(func) {}

  template<class ExceptionType, uint32_t N>
  R operator() (Args... args) const {
    for(int i = 0; i < N; ++i) {
      try {
        return this->_func(args...);
      } catch(const ExceptionType&) {
        continue;
      } catch(const std::exception& exc) {
        throw exc;
      }
    }

    return this->_func(args...);
  }
};


} /* end namespace functional */


} /* end namespace sneaker */


#endif /* SNEAKER_DECORATORS_H_ */
