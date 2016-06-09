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
#ifndef SNEAKER_STANDARD_ALLOC_POLICY_H_
#define SNEAKER_STANDARD_ALLOC_POLICY_H_

#include <limits>
#include <memory>
#include <type_traits>


#if defined(__clang__) and __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wdeprecated"
#endif


namespace sneaker {


namespace allocator {


template<typename T>
class standard_alloc_policy {
public:
  typedef T                 value_type;
  typedef value_type*       pointer;
  typedef const value_type* const_pointer;
  typedef value_type&       reference;
  typedef const value_type& const_reference;
  typedef std::size_t       size_type;
  typedef std::ptrdiff_t    difference_type;
  typedef std::true_type    propagate_on_container_move_assignment;

  template<typename U>
  struct rebind {
    typedef standard_alloc_policy<U> other;
  };

  standard_alloc_policy();

  standard_alloc_policy(const standard_alloc_policy&);

  template<typename U>
  explicit standard_alloc_policy(const standard_alloc_policy<U>&);

  inline pointer allocate(size_type cnt, typename std::allocator<void>::const_pointer=0);
  inline void deallocate(pointer p, size_type);

  inline size_type max_size() const;
};

// -----------------------------------------------------------------------------

template<typename T>
standard_alloc_policy<T>::standard_alloc_policy()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<typename T>
standard_alloc_policy<T>::standard_alloc_policy(const standard_alloc_policy&)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<typename T>
template<typename U>
standard_alloc_policy<T>::standard_alloc_policy(const standard_alloc_policy<U>&)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<typename T>
typename standard_alloc_policy<T>::pointer
standard_alloc_policy<T>::allocate(
  size_type n, typename std::allocator<void>::const_pointer)
{
  return reinterpret_cast<pointer>(::operator new(n * sizeof(T)));
}

// -----------------------------------------------------------------------------

template<typename T>
void standard_alloc_policy<T>::deallocate(pointer p, size_type)
{
  ::operator delete(p);
}

// -----------------------------------------------------------------------------

template<typename T>
typename standard_alloc_policy<T>::size_type
standard_alloc_policy<T>::max_size() const
{
  return std::numeric_limits<size_type>::max();
}

// -----------------------------------------------------------------------------

/* Equality operators. */
template<typename T, typename T2>
inline bool operator==(const standard_alloc_policy<T>&,
  const standard_alloc_policy<T2>&)
{
  return true;
}

// -----------------------------------------------------------------------------

template<typename T, typename T2>
inline bool operator!=(const standard_alloc_policy<T>& lhs,
  const standard_alloc_policy<T2>& rhs)
{
  return !(operator==(lhs, rhs));
}

// -----------------------------------------------------------------------------

template<typename T, typename other_allocator>
inline bool operator==(const standard_alloc_policy<T>&, const other_allocator&)
{
  return false;
}

// -----------------------------------------------------------------------------

template<typename T, typename other_allocator>
inline bool operator!=(const standard_alloc_policy<T>& lhs,
  const other_allocator& rhs)
{
  return !(operator==(lhs, rhs));
}

// -----------------------------------------------------------------------------


} /* end namespace allocator */


} /* end namespace sneaker */


#endif /* SNEAKER_STANDARD_ALLOC_POLICY_H_ */


#if defined(__clang__) and __clang__
  #pragma clang diagnostic pop
#endif
