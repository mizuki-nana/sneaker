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

#ifndef SNEAKER_ATOMIC_INCREMENTOR_H_
#define SNEAKER_ATOMIC_INCREMENTOR_H_

#include <atomic>
#include <stdexcept>


namespace sneaker {


namespace atomic {


template<class T, T UPPER_LIMIT>
class atomic_incrementor {
public:
  atomic_incrementor();
  atomic_incrementor(T);

  atomic_incrementor(const atomic_incrementor<T, UPPER_LIMIT>&);

  atomic_incrementor<T, UPPER_LIMIT>& operator=(const T&);

  atomic_incrementor<T, UPPER_LIMIT>& operator++()
    throw(std::overflow_error, std::underflow_error);

  operator T() const;

  bool operator==(const T&) const;
  bool operator!=(const T&) const;

protected:
  std::atomic<T> _value;
};


template<class T, T UPPER_LIMIT>
using _MyType = typename sneaker::atomic::atomic_incrementor<T, UPPER_LIMIT>;


template<class T, T UPPER_LIMIT>
sneaker::atomic::atomic_incrementor<T, UPPER_LIMIT>::atomic_incrementor():
  _value(0)
{
  // Do nothing here.
}

template<class T, T UPPER_LIMIT>
sneaker::atomic::atomic_incrementor<T, UPPER_LIMIT>::atomic_incrementor(T value):
  _value(value)
{
  // Do nothing here.
}

template<class T, T UPPER_LIMIT>
sneaker::atomic::atomic_incrementor<T, UPPER_LIMIT>::atomic_incrementor(
  const atomic_incrementor<T, UPPER_LIMIT>& other)
{
  _value = static_cast<T>(other._value);
}

template<class T, T UPPER_LIMIT>
_MyType<T, UPPER_LIMIT>&
sneaker::atomic::atomic_incrementor<T, UPPER_LIMIT>::operator=(const T& value)
{
  _value = value;
  return *this;
}

template<class T, T UPPER_LIMIT>
_MyType<T, UPPER_LIMIT>&
sneaker::atomic::atomic_incrementor<T, UPPER_LIMIT>::operator++()
  throw(std::overflow_error, std::underflow_error)
{
  T current_value = _value;
  _value++;

  if (_value > UPPER_LIMIT) {
    throw std::overflow_error("Overflow error occurred during increment");
  } else if (_value < current_value) {
    throw std::underflow_error("Underflow error occurred during increment");
  }

  return *this;
}

template<class T, T UPPER_LIMIT>
sneaker::atomic::atomic_incrementor<T, UPPER_LIMIT>::operator T() const
{
  return this->_value.load();    
}

template<class T, T UPPER_LIMIT>
bool
sneaker::atomic::atomic_incrementor<T, UPPER_LIMIT>::operator==(const T& other) const
{
  return this->_value == other;
}

template<class T, T UPPER_LIMIT>
bool
sneaker::atomic::atomic_incrementor<T, UPPER_LIMIT>::operator!=(const T& other) const
{
  return !(*this == other);
}


} /* end namespace atomic */


} /* end namespace sneaker */


#endif /* SNEAKER_ATOMIC_INCREMENTOR_H_ */