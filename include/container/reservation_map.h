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
#ifndef SNEAKER_RESERVATION_MAP_H_
#define SNEAKER_RESERVATION_MAP_H_

#include <boost/uuid/random_generator.hpp>

#include <map>
#include <set>
#include <stdlib.h>


namespace sneaker {


namespace container {


template<class T>
class reservation_map {
public:
  typedef boost::uuids::uuid token_t;
  typedef boost::uuids::random_generator generator_type;

  reservation_map();
  ~reservation_map();

  size_t size() const;

  token_t reserve();

  bool member(token_t id) const;

  bool put(token_t id, T value);

  bool get(token_t id, T* ptr);

  bool unreserve(token_t id);

  void clear();

protected:
  void reserve(token_t id);

  using token_set_type = typename std::set<token_t>;
  using map_type = typename std::map<token_t, T>;

  token_set_type m_tokens;
  map_type m_map;
  generator_type m_token_generator;
};

// -----------------------------------------------------------------------------

template<class T>
reservation_map<T>::reservation_map()
  :
  m_tokens(),
  m_map(),
  m_token_generator()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<class T>
reservation_map<T>::~reservation_map()
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

template<class T>
size_t
reservation_map<T>::size() const
{
  return m_map.size();
}

// -----------------------------------------------------------------------------

template<class T>
typename reservation_map<T>::token_t
reservation_map<T>::reserve()
{
  token_t id = m_token_generator();

  reserve(id);

  return static_cast<token_t>(id);
}

// -----------------------------------------------------------------------------

template<class T>
void
reservation_map<T>::reserve(reservation_map<T>::token_t id)
{
  m_tokens.insert(id);
}

// -----------------------------------------------------------------------------

template<class T>
bool
reservation_map<T>::member(reservation_map<T>::token_t id) const
{
  typename token_set_type::const_iterator itr = m_tokens.find(id);
  return itr != m_tokens.cend();
}

// -----------------------------------------------------------------------------

template<class T>
bool
reservation_map<T>::put(reservation_map<T>::token_t id, T value)
{
  if (!member(id)) {
    return false;
  }

  typename token_set_type::iterator itr = m_tokens.find(id);

  if (itr == m_tokens.end()) {
    return false;
  }

  m_map.insert(std::pair<token_t, T>(id, value));

  return true;
}

// -----------------------------------------------------------------------------

template<class T>
bool
reservation_map<T>::get(reservation_map<T>::token_t id, T* ptr)
{
  if (!member(id)) {
    return false;
  }

  typename map_type::iterator itr = m_map.find(id);

  if (itr == m_map.cend()) {
    return false;
  }

  T value = m_map.at(id);

  *ptr = value;

  return true;
}

// -----------------------------------------------------------------------------

template<class T>
bool
reservation_map<T>::unreserve(reservation_map<T>::token_t id)
{
  if (!member(id)) {
    return false;
  }

  m_tokens.erase(id);
  m_map.erase(id);

  return true;
}

// -----------------------------------------------------------------------------

template<class T>
void
reservation_map<T>::clear()
{
  m_tokens.clear();
  m_map.clear();
}

// -----------------------------------------------------------------------------


} /* end namespace container */


} /* end namespace sneaker */


#endif /* SNEAKER_RESERVATION_MAP_H_ */
