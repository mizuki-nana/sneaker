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
 * `sneaker::container::assorted_value_map<K, ValueTypes...>` is an associative
 * container class where each set of multiple(zero or more) statically-typed
 * assorted values are associated to a single statically-typed key.
 *
 * The types of the keys and values are defined through templating. The type
 * of the key must be specified, followed by the assorted types of the values.
 *
 * Since this is an associative container, its interfaces and usage are based on
 * the ones of `std::map`.
 *
 * Example:
 *
 * // Define a map of fruits to their scores and descriptions
 * // (I'm being very subjective here).
 * sneaker::container::assorted_value_map<char*, int, char*> fruits;
 *
 * fruits.insert("Apple", 100, "Best thing ever <3");
 * fruits.insert("Bananna", 90, "Get ALL the Potassum you need!");
 * fruits.insert("Orange", 80, "sweet and sour, very citric...");
 *
 * size_t count = fruits.size();
 * std::cout << "There are %d fruits here." << count << std::endl;
 * std::cout << "My favorite is Apple: %s (score %d)" \
 *  << fruits.get<char*, 0>("Apple") << fruits.get<int, 1>("Apple") << std::endl;
 * std::cout << "The reason, you ask?" << std:endl;
 * std::cout << "It's because: %s." << fruits.get<char*, 2>("Apple") << std::endl;
 *
 */

#ifndef SNEAKER_ASSORTED_VALUE_MAP_H_
#define SNEAKER_ASSORTED_VALUE_MAP_H_

#include <boost/tuple/tuple.hpp>

#include <map>


namespace sneaker {


namespace container {


template<class K, class... ValueTypes>
class assorted_value_map {
public:
  using core_type = typename std::map<K, boost::tuple<ValueTypes... >>;

  using key_type                = typename core_type::key_type;
  using mapped_type             = typename core_type::mapped_type;
  using value_type              = typename core_type::value_type;
  using key_compare             = typename core_type::key_compare;
  using value_compare           = typename core_type::value_compare;
  using allocator_type          = typename core_type::allocator_type;
  using reference               = typename core_type::reference;
  using const_reference         = typename core_type::const_reference;
  using pointer                 = typename core_type::pointer;
  using const_pointer           = typename core_type::const_pointer;
  using iterator                = typename core_type::iterator;
  using const_iterator          = typename core_type::const_iterator;
  using reverse_iterator        = typename core_type::reverse_iterator;
  using const_reverse_iterator  = typename core_type::const_reverse_iterator;
  using difference_type         = typename core_type::difference_type;
  using size_type               = typename core_type::size_type;

  assorted_value_map();
  explicit assorted_value_map(const core_type&);

  assorted_value_map(const assorted_value_map<K, ValueTypes...>&);

  ~assorted_value_map();

  template<class Compare, class Alloc>
  static
  assorted_value_map<K, ValueTypes...> create() {
    return assorted_value_map<K, ValueTypes...>(
      core_type(Compare(), Alloc()));
  }

  template<class Compare, class Alloc>
  static
  assorted_value_map<K, ValueTypes...> create(
    const Compare& comparer, const Alloc& allocator)
  {
    return assorted_value_map<K, ValueTypes...>(
      core_type(comparer, allocator));
  }

  bool empty() const;

  size_type size() const;

  size_type max_size() const;

  void insert(K key, ValueTypes... values);

  void erase(iterator itr);
  size_type erase(const K& key);
  void erase(iterator first, iterator last);

  void swap(assorted_value_map<K, ValueTypes...>& other);

  void clear() noexcept;

  mapped_type& at(K key);
  const mapped_type& at(K key) const;

  template<class A, size_t Index>
  A get(K key);

  template<class A, size_t Index>
  const A& get(K key) const;

  mapped_type& operator[](const K& key);

  iterator begin();
  const_iterator begin() const;

  iterator end();
  const_iterator end() const;

  reverse_iterator rbegin();
  const_reverse_iterator rbegin() const;

  reverse_iterator rend();
  const_reverse_iterator rend() const;

  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;

  const_reverse_iterator crbegin() const noexcept;
  const_reverse_iterator crend() const noexcept;

  iterator find(K key);
  const_iterator find(K key) const;

protected:
  core_type m_core;
};


// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
assorted_value_map<K, ValueTypes...>::assorted_value_map()
  :
  m_core(core_type())
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
assorted_value_map<K, ValueTypes...>::assorted_value_map(const core_type& core)
  :
  m_core(core)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
assorted_value_map<K, ValueTypes...>::assorted_value_map(
  const assorted_value_map<K, ValueTypes...>& other)
  :
  m_core(other.m_core)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
assorted_value_map<K, ValueTypes...>::~assorted_value_map()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
bool
assorted_value_map<K, ValueTypes...>::empty() const
{
  return m_core.empty();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename assorted_value_map<K, ValueTypes...>::size_type
assorted_value_map<K, ValueTypes...>::size() const
{
  return m_core.size();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename assorted_value_map<K, ValueTypes...>::size_type
assorted_value_map<K, ValueTypes...>::max_size() const
{
  return m_core.max_size();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
void
assorted_value_map<K, ValueTypes...>::insert(K key, ValueTypes... values)
{
  m_core.insert( value_type(key, mapped_type(values...)) );
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
void
assorted_value_map<K, ValueTypes...>::erase(iterator itr)
{
  m_core.erase(itr);
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename assorted_value_map<K, ValueTypes...>::size_type
assorted_value_map<K, ValueTypes...>::erase(const K& key)
{
  return m_core.erase(key);
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
void
assorted_value_map<K, ValueTypes...>::erase(
  iterator first, iterator last)
{
  m_core.erase(first, last);
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
void
assorted_value_map<K, ValueTypes...>::swap(
  assorted_value_map<K, ValueTypes...>& other)
{
  m_core.swap(other.m_core);
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
void
assorted_value_map<K, ValueTypes...>::clear() noexcept
{
  m_core.clear();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename assorted_value_map<K, ValueTypes...>::mapped_type&
assorted_value_map<K, ValueTypes...>::at(K key)
{
  return static_cast<mapped_type&>(m_core.at(key));
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
const typename assorted_value_map<K, ValueTypes...>::mapped_type&
assorted_value_map<K, ValueTypes...>::at(K key) const
{
  return static_cast<const mapped_type&>(m_core.at(key));
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
template<class A, size_t Index>
A
assorted_value_map<K, ValueTypes...>::get(K key)
{
  return boost::get<Index>(at(key));
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
template<class A, size_t Index>
const A&
assorted_value_map<K, ValueTypes...>::get(K key) const
{
  return boost::get<Index>(at(key));
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename assorted_value_map<K, ValueTypes...>::mapped_type&
assorted_value_map<K, ValueTypes...>::operator[](const K& key)
{
  return m_core[key];
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename assorted_value_map<K, ValueTypes...>::iterator
assorted_value_map<K, ValueTypes...>::begin()
{
  return m_core.begin();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename assorted_value_map<K, ValueTypes...>::const_iterator
assorted_value_map<K, ValueTypes...>::begin() const
{
  return m_core.begin();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename assorted_value_map<K, ValueTypes...>::iterator
assorted_value_map<K, ValueTypes...>::end()
{
  return m_core.end();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename assorted_value_map<K, ValueTypes...>::const_iterator
assorted_value_map<K, ValueTypes...>::end() const
{
  return m_core.end();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename assorted_value_map<K, ValueTypes...>::reverse_iterator
assorted_value_map<K, ValueTypes...>::rbegin()
{
  return m_core.rbegin();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename assorted_value_map<K, ValueTypes...>::const_reverse_iterator
assorted_value_map<K, ValueTypes...>::rbegin() const
{
  return m_core.rbegin();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename assorted_value_map<K, ValueTypes...>::reverse_iterator
assorted_value_map<K, ValueTypes...>::rend()
{
  return m_core.rend();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename assorted_value_map<K, ValueTypes...>::const_reverse_iterator
assorted_value_map<K, ValueTypes...>::rend() const
{
  return m_core.rend();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename assorted_value_map<K, ValueTypes...>::const_iterator
assorted_value_map<K, ValueTypes...>::cbegin() const noexcept
{
  return m_core.cbegin();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename assorted_value_map<K, ValueTypes...>::const_iterator
assorted_value_map<K, ValueTypes...>::cend() const noexcept
{
  return m_core.cend();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename assorted_value_map<K, ValueTypes...>::const_reverse_iterator
assorted_value_map<K, ValueTypes...>::crbegin() const noexcept
{
  return m_core.crbegin();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename assorted_value_map<K, ValueTypes...>::const_reverse_iterator
assorted_value_map<K, ValueTypes...>::crend() const noexcept
{
  return m_core.crend();
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename assorted_value_map<K, ValueTypes...>::iterator
assorted_value_map<K, ValueTypes...>::find(K key)
{
  return m_core.find(key);
}

// -----------------------------------------------------------------------------

template<class K, class... ValueTypes>
typename assorted_value_map<K, ValueTypes...>::const_iterator
assorted_value_map<K, ValueTypes...>::find(K key) const
{
  return m_core.find(key);
}

// -----------------------------------------------------------------------------


} /* end namespace container */


} /* end namespace sneaker */


#endif /* SNEAKER_ASSORTED_VALUE_MAP_H_ */
