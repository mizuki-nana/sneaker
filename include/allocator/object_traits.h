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
#ifndef SNEAKER_OBJECT_TRAITS_H_
#define SNEAKER_OBJECT_TRAITS_H_

#include <utility>


namespace sneaker {


namespace allocator {


template<typename T>
class object_traits {
public:
  template<typename U>
  struct rebind {
    typedef object_traits<U> other;
  };

  inline explicit object_traits();

  template<typename U>
  inline explicit object_traits(object_traits<U> const&);

  inline T* address(T& r);
  inline T const* address(T const&r);

  inline void construct(T* p, const T& t);

  template<class U, class... Args>
  inline void construct(U* p, Args&&... args);

  inline void destroy(T* p);
};

// -----------------------------------------------------------------------------

template<typename T>
object_traits<T>::object_traits()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<typename T>
template<typename U>
object_traits<T>::object_traits(object_traits<U> const&)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<typename T>
T* object_traits<T>::address(T& r)
{
  return &r;
}

// -----------------------------------------------------------------------------

template<typename T>
T const* object_traits<T>::address(T const& r)
{
  return &r;
}

// -----------------------------------------------------------------------------

template<typename T>
void object_traits<T>::construct(T* p, const T& t)
{
  ::new ((void*)p) T (t);
}

// -----------------------------------------------------------------------------

template<typename T>
template<class U, class... Args>
void object_traits<T>::construct(U* p, Args&&... args)
{
  ::new ((void*)p) U (std::forward<Args>(args)...);
}

// -----------------------------------------------------------------------------

template<typename T>
void object_traits<T>::destroy(T* p)
{
  p->~T();
}

// -----------------------------------------------------------------------------


} /* end namespace allocator */


} /* end namespace sneaker */


#endif /* SNEAKER_OBJECT_TRAITS_H_ */
