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

#ifndef SNEAKER_RESERVATION_MAP_H_
#define SNEAKER_RESERVATION_MAP_H_

#include <stdlib.h>
#include "assorted_value_map.h"
#include "../libc/hash.h"
#include "../libc/memory.h"
#include "../libc/uuid.h"


namespace sneaker {


namespace container {


template<class T>
class reservation_map {
public:
  typedef hash_t token_t;

  reservation_map();
  ~reservation_map();

  using core_type = typename sneaker::container::assorted_value_map<token_t, bool, T>;

  size_t size();

  token_t reserve();

  bool member(token_t) const;

  bool put(token_t, T);

  bool get(token_t, T*);

  bool unreserve(token_t);

  void clear();

protected:
  void _reserve(token_t);

  core_type _map;
};

template<class T>
sneaker::container::reservation_map<T>::reservation_map():
  _map(core_type())
{
  // Do nothing here.
}

template<class T>
sneaker::container::reservation_map<T>::~reservation_map()
{
  // Do nothing here.
}

template<class T>
size_t
sneaker::container::reservation_map<T>::size()
{
  return _map.size();
}

template<class T>
typename sneaker::container::reservation_map<T>::token_t
sneaker::container::reservation_map<T>::reserve()
{
  hash_t id = uuid_create_and_hash();

  this->_reserve(id);

  return (token_t)id;
}

template<class T>
void
sneaker::container::reservation_map<T>::_reserve(
  sneaker::container::reservation_map<T>::token_t id)
{
  _map.insert(id, true, 0);
}

template<class T>
bool
sneaker::container::reservation_map<T>::member(
  sneaker::container::reservation_map<T>::token_t id) const
{
  typename core_type::const_iterator itr = _map.find(id);
  if(itr == _map.end()) {
    return false;
  }
  return _map.template get<bool, 0>(id);
}

template<class T>
bool
sneaker::container::reservation_map<T>::put(
  sneaker::container::reservation_map<T>::token_t id, T value)
{
  if(!member(id)) {
    return false;
  }

  typename core_type::iterator itr = this->_map.find(id);

  if(itr == this->_map.end()) {
    return false;
  } else {
    this->_map.erase(itr);
  }
  _map.insert(id, true, value);

  return true;
}

template<class T>
bool
sneaker::container::reservation_map<T>::get(
  sneaker::container::reservation_map<T>::token_t id, T* ptr)
{
  if(!member(id)) {
    return false;
  }

  typename core_type::iterator itr = this->_map.find(id);

  if(itr == this->_map.end()) {
    return false;
  }

  T value = _map.template get<T, 1>(id);

  *ptr = value;

  return true;
}

template<class T>
bool
sneaker::container::reservation_map<T>::unreserve(
  sneaker::container::reservation_map<T>::token_t id)
{
  if(!member(id)) {
    return false;
  }

  this->_map.erase(id);

  return true;
}

template<class T>
void
sneaker::container::reservation_map<T>::clear()
{
  this->_map.clear();
}


} /* end namespace container */


} /* end namespace sneaker */


#endif /* SNEAKER_RESERVATION_MAP_H_ */