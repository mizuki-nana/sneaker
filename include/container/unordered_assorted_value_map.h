/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 Yanzheng Li

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

/*
 * `sneaker::container::unordered_assorted_value_map<K, ValueTypes...>` is an
 * associative container class where each set of multiple(zero or more) assorted
 * statically-typed values are associated to a single statically-typed key.
 *
 * The types of the keys and values are defined through templating. The type
 * of the key must be specified, followed by the assorted types of the values.
 *
 * Since this is an associative container, its interfaces and usage are very much
 * the same as the ones of `std::unordered_map`.
 *
 * Example:
 *
 * // Define a map of fruits to their scores and descriptions
 * // (I'm being very subjective here).
 * sneaker::container::unordered_assorted_value_map<char*, int, char*> fruits;
 *
 * fruits.insert("Apple", 100, "Best thing ever <3");
 * fruits.insert("Bananna", 90, "Get ALL the Potassum you need!");
 * fruits.insert("Orange", 80, "sweet and sour, very citric...");
 *
 * size_t count = fruits.size();
 * std::cout << "There are %d fruits here." << count << std::endl;
 * std::cout << "My favorite is Apple: %s (score %d)" << \
 *  fruits.get<char*, 0>("Apple") << fruits.get<int, 1>("Apple") << std::endl;
 * std::cout << "The reason, you ask?" << std:endl;
 * std::cout << "It's because: %s." << fruits.get<char*, 2>("Apple") << std::endl;
*/

#ifndef SNEAKER_UNORDERED_ASSORTED_VALUE_MAP_H_
#define SNEAKER_UNORDERED_ASSORTED_VALUE_MAP_H_

#include <boost/tuple/tuple.hpp>

#include <unordered_map>


namespace sneaker {


namespace container {


template<class K, class... ValueTypes>
class unordered_assorted_value_map {
public:
  using core_type = typename std::unordered_map<K, boost::tuple<ValueTypes... >>;

  using key_type              = typename core_type::key_type;
  using mapped_type           = typename core_type::mapped_type;
  using value_type            = typename core_type::value_type;
  using hasher                = typename core_type::hasher;
  using key_equal             = typename core_type::key_equal;
  using allocator_type        = typename core_type::allocator_type;
  using reference             = typename core_type::reference;
  using const_reference       = typename core_type::const_reference;
  using pointer               = typename core_type::pointer;
  using const_pointer         = typename core_type::const_pointer;
  using iterator              = typename core_type::iterator;
  using const_iterator        = typename core_type::const_iterator;
  using local_iterator        = typename core_type::local_iterator;
  using const_local_iterator  = typename core_type::const_local_iterator;
  using size_type             = typename core_type::size_type;
  using difference_type       = typename core_type::difference_type;

  explicit unordered_assorted_value_map();
  explicit unordered_assorted_value_map(core_type);

  unordered_assorted_value_map(
    const sneaker::container::unordered_assorted_value_map<K, ValueTypes...>&);

  ~unordered_assorted_value_map();

  template<size_type N, class Hash, class Pred, class Alloc>
  static
  sneaker::container::unordered_assorted_value_map<K, ValueTypes...> create() {
    return sneaker::container::unordered_assorted_value_map<K, ValueTypes...>(
      core_type(N, Hash(), Pred(), Alloc())
    );
  }

  template<size_type N, class Hash, class Pred, class Alloc>
  static
  sneaker::container::unordered_assorted_value_map<K, ValueTypes...> create(
    const Hash& hasher, const Pred& key_eq, const Alloc& allocator)
  {
    return sneaker::container::unordered_assorted_value_map<K, ValueTypes...>(
      core_type(N, hasher, key_eq, allocator)
    );
  }

  bool empty() const;

  size_type size() const;

  size_type max_size() const;

  void insert(K, ValueTypes...);

  void erase(iterator);
  size_type erase(const K&);
  void erase(iterator, iterator);

  void swap(unordered_assorted_value_map<K, ValueTypes...>&);

  void clear() noexcept;

  mapped_type& at(K);
  const mapped_type& at(K) const;

  template<class A, size_t Index>
  A get(K);

  template<class A, size_t Index>
  const A& get(K) const;

  mapped_type& operator[](K);
  const mapped_type& operator[](K) const;

  iterator begin();
  const_iterator begin() const;

  iterator end();
  const_iterator end() const;

  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;

  iterator find(K);
  const_iterator find(K) const;

  float load_factor() const noexcept;

  float max_load_factor() const noexcept;
  void max_load_factor(float);

  void rehash(size_type);
  void reserve(size_type);

  hasher hash_function() const;
  key_equal key_eq() const;
  allocator_type get_allocator() const noexcept;

protected:
  core_type m_core;
};

// -----------------------------------------------------------------------------

namespace {

template<class K, class... ValueTypes>
using _MyType = typename sneaker::container::unordered_assorted_value_map<K, ValueTypes...>;

} /* end namespace */

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::unordered_assorted_value_map():
  m_core(core_type())
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::unordered_assorted_value_map(core_type core):
  m_core(core)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::unordered_assorted_value_map(
  const sneaker::container::unordered_assorted_value_map<K, ValueTypes...>& other
):
  m_core(other.m_core)
{
  // Do nothing here.
}

template<class K, class... ValueTypes>
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::~unordered_assorted_value_map()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
bool
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::empty() const
{
  return m_core.empty();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::size_type
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::size() const
{
  return m_core.size();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::size_type
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::max_size() const
{
  return m_core.max_size();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
void
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::insert(
  K key,
  ValueTypes... values
)
{
  m_core.insert( value_type(key, mapped_type(values...)) );
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
void
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::erase(iterator itr)
{
  m_core.erase(itr);
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::size_type
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::erase(const K& key)
{
  return m_core.erase(key);
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
void
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::erase(
  iterator first, iterator last)
{
  m_core.erase(first, last);
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
void
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::swap(
  sneaker::container::unordered_assorted_value_map<K, ValueTypes...>& other)
{
  m_core.swap(other.m_core);
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
void
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::clear() noexcept
{
  m_core.clear();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::mapped_type&
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::at(K key)
{
  return static_cast<mapped_type&>(m_core.at(key));
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
const typename _MyType<K, ValueTypes...>::mapped_type&
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::at(K key) const
{
  return static_cast<const mapped_type&>(m_core.at(key));
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
template<class A, size_t Index>
A
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::get(K key)
{
  return boost::get<Index>(at(key));
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
template<class A, size_t Index>
const A&
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::get(K key) const
{
  return boost::get<Index>(at(key));
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::mapped_type&
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::operator[](K key)
{
  return at(key);
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
const typename _MyType<K, ValueTypes...>::mapped_type&
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::operator[](K key) const
{
  return at(key);
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::iterator
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::begin()
{
  return m_core.begin();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::const_iterator
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::begin() const
{
  return m_core.begin();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::iterator
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::end()
{
  return m_core.end();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::const_iterator
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::end() const
{
  return m_core.end();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::const_iterator
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::cbegin() const noexcept
{
  return m_core.cbegin();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::const_iterator
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::cend() const noexcept
{
  return m_core.cend();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::iterator
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::find(K key)
{
  return m_core.find(key);
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::const_iterator
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::find(K key) const
{
  return m_core.find(key);
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
float
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::load_factor() const noexcept
{
  return m_core.load_factor();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
float
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::max_load_factor() const noexcept
{
  return m_core.max_load_factor();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
void
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::max_load_factor(float z)
{
  return m_core.max_load_factor(z);
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
void
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::rehash(size_type n)
{
  return m_core.rehash(n);
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
void
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::reserve(size_type n)
{
  return m_core.reserve(n);
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::hasher
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::hash_function() const
{
  return m_core.hash_function();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::key_equal
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::key_eq() const
{
  return m_core.key_eq();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::allocator_type
sneaker::container::unordered_assorted_value_map<K, ValueTypes...>::get_allocator() const noexcept
{
  return m_core.get_allocator();
}

// -----------------------------------------------------------------------------


} /* end namespace container */


} /* end namespace sneaker */


#endif /* SNEAKER_UNORDERED_ASSORTED_VALUE_MAP_H_ */
