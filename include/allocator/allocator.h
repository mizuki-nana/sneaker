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
#ifndef SNEAKER_ALLOCATOR_H_
#define SNEAKER_ALLOCATOR_H_

#include "alloc_policy.h"
#include "object_traits.h"


#if defined(__clang__) and __clang__
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wdeprecated"
#endif


namespace sneaker {
namespace allocator {

template<typename T, typename Policy=standard_alloc_policy<T>, typename Traits=object_traits<T>>
class allocator: public Policy, public Traits {
private:
  typedef Policy _Policy;
  typedef Traits _Traits;

public:
  using value_type      = typename _Policy::value_type;
  using pointer         = typename _Policy::pointer;
  using const_pointer   = typename _Policy::const_pointer;
  using reference       = typename _Policy::reference;
  using const_reference = typename _Policy::const_reference;
  using size_type       = typename _Policy::size_type;
  using difference_type = typename _Policy::difference_type;

  template<typename U>
  struct rebind {
    typedef allocator<U, typename _Policy::template rebind<U>::other,
      typename _Traits::template rebind<U>::other > other;
  };

  allocator();

  allocator(const allocator& rhs);

  template<typename U>
  allocator(const allocator<U>&);

  template <typename U, typename P, typename T2>
  allocator(const allocator<U, P, T2>& rhs);
};

// -----------------------------------------------------------------------------

template<typename T, typename Policy, typename Traits>
allocator<T, Policy, Traits>::allocator()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<typename T, typename Policy, typename Traits>
allocator<T, Policy, Traits>::allocator(const allocator& rhs)
  :
  Policy(rhs),
  Traits(rhs)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<typename T, typename Policy, typename Traits>
template<typename U>
allocator<T, Policy, Traits>::allocator(const allocator<U>&)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<typename T, typename Policy, typename Traits>
template <typename U, typename P, typename T2>
allocator<T, Policy, Traits>::allocator(const allocator<U, P, T2>& rhs)
  :
  Policy(rhs),
  Traits(rhs)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

/* Equality operators. */
template<typename T, typename P, typename Tr>
inline bool operator==(const allocator<T, P, Tr>& lhs,
  const allocator<T, P, Tr>& rhs)
{
  return operator==(static_cast<P>(lhs), static_cast<P>(rhs));
}

// -----------------------------------------------------------------------------

template<typename T, typename P, typename Tr, typename T2, typename P2, typename Tr2>
inline bool operator==(const allocator<T, P, Tr>& lhs,
  const allocator<T2, P2, Tr2>& rhs)
{
  return operator==(static_cast<P>(lhs), static_cast<P2>(rhs));
}

// -----------------------------------------------------------------------------

template<typename T, typename P, typename Tr, typename other_allocator>
inline bool operator==(const allocator<T, P, Tr>& lhs,
  const other_allocator& rhs)
{
  return operator==(static_cast<P>(lhs), rhs);
}

// -----------------------------------------------------------------------------

template<typename T, typename P, typename Tr>
inline bool operator!=(const allocator<T, P, Tr>& lhs, allocator<T, P, Tr> const& rhs)
{
  return !operator==(lhs, rhs);
}

// -----------------------------------------------------------------------------

template<typename T, typename P, typename Tr, typename T2, typename P2, typename Tr2>
inline bool operator!=(allocator<T, P, Tr> const& lhs,
  const allocator<T2, P2, Tr2>& rhs)
{
  return !operator==(lhs, rhs);
}

// -----------------------------------------------------------------------------

template<typename T, typename P, typename Tr, typename other_allocator>
inline bool operator!=(const allocator<T, P, Tr>& lhs,
  const other_allocator& rhs)
{
  return !operator==(lhs, rhs);
}

// -----------------------------------------------------------------------------

} /* end namespace allocator */
} /* end namespace sneaker */


#endif /* SNEAKER_ALLOCATOR_H_ */


#if defined(__clang__) and __clang__
  #pragma clang diagnostic pop
#endif
