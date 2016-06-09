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

/**
 * Wrapper class around `std::atomic<T>` with a templated maximum value.
 */

#ifndef SNEAKER_ATOMIC_H_
#define SNEAKER_ATOMIC_H_

#include <algorithm>
#include <atomic>
#include <type_traits>


namespace sneaker {


namespace threading {


template<class T, T UPPER_LIMIT>
class atomic
{
public:
  atomic();
  atomic(T);

  atomic(const atomic<T, UPPER_LIMIT>&);

  atomic<T, UPPER_LIMIT>& operator=(const T& value);

  template<class U, U UPPER_LIMIT_2>
  atomic& operator=(const atomic<U, UPPER_LIMIT_2>&);

  atomic<T, UPPER_LIMIT>& operator++();

  atomic<T, UPPER_LIMIT> operator++(int);

  T value() const;

  operator T() const;

private:
  std::atomic<T> m_value;
};

// -----------------------------------------------------------------------------

template<class T, T UPPER_LIMIT>
atomic<T, UPPER_LIMIT>::atomic()
  :
  m_value(0)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<class T, T UPPER_LIMIT>
atomic<T, UPPER_LIMIT>::atomic(T value)
  :
  m_value(value)
{
  if (m_value > UPPER_LIMIT)
  {
    m_value = 0;
  }
}

// -----------------------------------------------------------------------------

template<class T, T UPPER_LIMIT>
atomic<T, UPPER_LIMIT>::atomic(const atomic<T, UPPER_LIMIT>& other)
{
  m_value = other.m_value.load();
}

// -----------------------------------------------------------------------------

template<class T, T UPPER_LIMIT>
atomic<T, UPPER_LIMIT>&
atomic<T, UPPER_LIMIT>::operator=(const T& value)
{
  m_value = value;
  return *this;
}

// -----------------------------------------------------------------------------

template<class T, T UPPER_LIMIT>
template<class U, U UPPER_LIMIT_2>
atomic<T, UPPER_LIMIT>&
atomic<T, UPPER_LIMIT>::operator=(const atomic<U, UPPER_LIMIT_2>& other)
{
  m_value = static_cast<T>(other.value());

  if (m_value > UPPER_LIMIT)
  {
    m_value = 0;
  }

  return *this;
}

// -----------------------------------------------------------------------------

template<class T, T UPPER_LIMIT>
atomic<T, UPPER_LIMIT>&
atomic<T, UPPER_LIMIT>::operator++()
{
  ++m_value;

  if (m_value > UPPER_LIMIT)
  {
    m_value = 0;
  }

  return *this;
}

// -----------------------------------------------------------------------------

template<class T, T UPPER_LIMIT>
atomic<T, UPPER_LIMIT>
atomic<T, UPPER_LIMIT>::operator++(int)
{
  auto copy(*this);

  this->operator++();

  return copy;
}

// -----------------------------------------------------------------------------

template<class T, T UPPER_LIMIT>
T
atomic<T, UPPER_LIMIT>::value() const
{
  return m_value.load();
}

// -----------------------------------------------------------------------------

template<class T, T UPPER_LIMIT>
atomic<T, UPPER_LIMIT>::operator T() const
{
  return value();
}

// -----------------------------------------------------------------------------

template<class T, T UPPER_LIMIT, class U, U UPPER_LIMIT_2>
bool
operator==(const atomic<T, UPPER_LIMIT> lhs,
  const atomic<U, UPPER_LIMIT_2> rhs)
{
  return std::is_same<T, U>::value && UPPER_LIMIT == UPPER_LIMIT_2 &&
    lhs.value() == rhs.value();
}

// -----------------------------------------------------------------------------

template<class T, T UPPER_LIMIT, class U, U UPPER_LIMIT_2>
bool
operator!=(const atomic<T, UPPER_LIMIT>& lhs,
  const atomic<U, UPPER_LIMIT_2>& rhs)
{
  return !operator==(lhs, rhs);
}

// -----------------------------------------------------------------------------


} /* end namespace threading */


} /* end namespace sneaker */


#endif /* SNEAKER_ATOMIC_H_ */
