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

/*
`sneaker::container::assorted_value_map<K, ValueTypes...>` is an associative
container class where each set of multiple(zero or more) assorted
statically-typed values are associated to a single statically-typed key.

The types of the keys and values are defined through templating. The type
of the key must be specified, followed by the assorted types of the values.

Since this is an associative container, its interfaces and usage are very much
the same as the ones of `std::map`.

Example:

// Define a map of fruits to their scores and descriptions (I'm being very subjective here).
sneaker::container::assorted_value_map<char*, int, char*> fruits;

fruits.insert("Apple", 100, "Best thing ever <3");
fruits.insert("Bananna", 90, "Get ALL the Potassum you need!");
fruits.insert("Orange", 80, "sweet and sour, very citric...");

size_t count = fruits.size();
std::cout << "There are %d fruits here." << count << std::endl;
std::cout << "My favorite is Apple: %s (score %d)" << fruits.get<char*, 0>("Apple") \
    << fruits.get<int, 1>("Apple") << std::endl;
*/

#ifndef SNEAKER_ASSORTED_VALUE_MAP_H_
#define SNEAKER_ASSORTED_VALUE_MAP_H_

#include <map>
#include <boost/tuple/tuple.hpp>


namespace sneaker {


namespace container {


namespace tuple_nms = boost;


template<class K, class... ValueTypes>
class assorted_value_map {
public:
  assorted_value_map();
  ~assorted_value_map();

  /* definitions */
  using value_type              = typename tuple_nms::tuple<ValueTypes... >;
  using size_type               = typename std::map<K, value_type>::size_type;
  using iterator                = typename std::map<K, value_type>::iterator;
  using const_iterator          = typename std::map<K, value_type>::const_iterator;
  using reverse_iterator        = typename std::map<K, value_type>::reverse_iterator;
  using const_reverse_iterator  = typename std::map<K, value_type>::const_reverse_iterator;

  bool empty() const;

  size_type size() const;

  size_type max_size() const;

  void insert(K, ValueTypes...);

  void erase(iterator);
  size_type erase(const K&);
  void erase(iterator, iterator);

  void swap(assorted_value_map<K, ValueTypes...>&);

  void clear() noexcept;

        value_type& at(K);
  const value_type& at(K) const;

  template<class A, size_t Index>
  A get(K);

  template<class A, size_t Index>
  const A& get(K) const;

        value_type& operator[](K);
  const value_type& operator[](K) const;

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

  iterator find(K);
  const_iterator find(K) const;

protected:
  using core_type = typename std::map<K, value_type>;

  core_type _core;
};


template<class K, class... ValueTypes>
using _MyType = typename sneaker::container::assorted_value_map<K, ValueTypes...>;


template<class K, class... ValueTypes>
sneaker::container::assorted_value_map<K, ValueTypes...>::assorted_value_map():
  _core(core_type())
{
}

template<class K, class... ValueTypes>
sneaker::container::assorted_value_map<K, ValueTypes...>::~assorted_value_map()
{
  // Do nothing.
}

template<class K, class... ValueTypes>
bool
sneaker::container::assorted_value_map<K, ValueTypes...>::empty() const
{
  return _core.empty();
}

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::size_type
sneaker::container::assorted_value_map<K, ValueTypes...>::size() const
{
  return _core.size();
}

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::size_type
sneaker::container::assorted_value_map<K, ValueTypes...>::max_size() const
{
  return _core.max_size();
}

template<class K, class... ValueTypes>
void
sneaker::container::assorted_value_map<K, ValueTypes...>::insert(
  K key,
  ValueTypes... values
)
{
  _core.insert(
    std::pair<K, value_type>(
      key,
      value_type(values...)
    )
  );
}

template<class K, class... ValueTypes>
void
sneaker::container::assorted_value_map<K, ValueTypes...>::erase(iterator itr)
{
  _core.erase(itr);
}

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::size_type
sneaker::container::assorted_value_map<K, ValueTypes...>::erase(const K& key)
{
  return _core.erase(key);
}

template<class K, class... ValueTypes>
void
sneaker::container::assorted_value_map<K, ValueTypes...>::erase(iterator first, iterator last)
{
  _core.erase(first, last);
}

template<class K, class... ValueTypes>
void
sneaker::container::assorted_value_map<K, ValueTypes...>::swap(
  sneaker::container::assorted_value_map<K, ValueTypes...>& other
)
{
  _core.swap(other._core);
}

template<class K, class... ValueTypes>
void
sneaker::container::assorted_value_map<K, ValueTypes...>::clear() noexcept
{
  _core.clear();
}

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::value_type&
sneaker::container::assorted_value_map<K, ValueTypes...>::at(K key)
{
  return static_cast<value_type&>(_core.at(key));
}

template<class K, class... ValueTypes>
const typename _MyType<K, ValueTypes...>::value_type&
sneaker::container::assorted_value_map<K, ValueTypes...>::at(K key) const
{
  return static_cast<const value_type&>(_core.at(key));
}

template<class K, class... ValueTypes>
template<class A, size_t Index>
A
sneaker::container::assorted_value_map<K, ValueTypes...>::get(K key)
{
  return boost::get<Index>(at(key));
}

template<class K, class... ValueTypes>
template<class A, size_t Index>
const A&
sneaker::container::assorted_value_map<K, ValueTypes...>::get(K key) const
{
  return boost::get<Index>(at(key));
}

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::value_type&
sneaker::container::assorted_value_map<K, ValueTypes...>::operator[](K key)
{
  return at(key);
}

template<class K, class... ValueTypes>
const typename _MyType<K, ValueTypes...>::value_type&
sneaker::container::assorted_value_map<K, ValueTypes...>::operator[](K key) const
{
  return at(key);
}

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::iterator
sneaker::container::assorted_value_map<K, ValueTypes...>::begin()
{
  return _core.begin();
}

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::const_iterator
sneaker::container::assorted_value_map<K, ValueTypes...>::begin() const
{
  return _core.begin();
}

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::iterator
sneaker::container::assorted_value_map<K, ValueTypes...>::end()
{
  return _core.end();
}

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::const_iterator
sneaker::container::assorted_value_map<K, ValueTypes...>::end() const
{
  return _core.end();
}

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::reverse_iterator
sneaker::container::assorted_value_map<K, ValueTypes...>::rbegin()
{
  return _core.rbegin();
}

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::const_reverse_iterator
sneaker::container::assorted_value_map<K, ValueTypes...>::rbegin() const
{
  return _core.rbegin();
}

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::reverse_iterator
sneaker::container::assorted_value_map<K, ValueTypes...>::rend()
{
  return _core.rend();
}

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::const_reverse_iterator
sneaker::container::assorted_value_map<K, ValueTypes...>::rend() const
{
  return _core.rend();
}

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::const_iterator
sneaker::container::assorted_value_map<K, ValueTypes...>::cbegin() const noexcept
{
  return _core.cbegin();
}

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::const_iterator
sneaker::container::assorted_value_map<K, ValueTypes...>::cend() const noexcept
{
  return _core.cend();
}

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::const_reverse_iterator
sneaker::container::assorted_value_map<K, ValueTypes...>::crbegin() const noexcept
{
  return _core.crbegin();
}

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::const_reverse_iterator
sneaker::container::assorted_value_map<K, ValueTypes...>::crend() const noexcept
{
  return _core.crend();
}

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::iterator
sneaker::container::assorted_value_map<K, ValueTypes...>::find(K key)
{
  return _core.find(key);
}

template<class K, class... ValueTypes>
typename _MyType<K, ValueTypes...>::const_iterator
sneaker::container::assorted_value_map<K, ValueTypes...>::find(K key) const
{
  return _core.find(key);
}


} /* end namespace container */


} /* end namespace sneaker */


#endif /* SNEAKER_ASSORTED_VALUE_MAP_H_ */
