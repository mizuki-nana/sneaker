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

#ifndef _RESERVATION_MAP_H_
#define _RESERVATION_MAP_H_

#include <map>
#include <stdlib.h>
#include "../libc/assert.h"
#include "../libc/hash.h"
#include "../libc/memory.h"
#include "../libc/uuid.h"


typedef hash_t token_t;


namespace sneaker {

namespace container {


template<class T>
class reservation_map {
public:
  reservation_map();
  ~reservation_map();

  size_t size();

  token_t reserve();

  bool member(token_t&) const;

  bool put(token_t&, T);

  bool get(token_t&, T*);

  bool unreserve(token_t&);

  void clear();

protected:
  void _reserve(token_t&);

  std::map<token_t, bool> * _name_book;
  std::map<token_t, T>    * _table_book;
};

template<class T>
sneaker::container::reservation_map<T>::reservation_map():
  _name_book(new std::map<token_t, bool>()),
  _table_book(new std::map<token_t, T>())
{
  ASSERT(_name_book);
  ASSERT(_table_book);
}

template<class T>
sneaker::container::reservation_map<T>::~reservation_map()
{
  DELETE(_name_book);
  DELETE(_table_book);
}

template<class T>
size_t
sneaker::container::reservation_map<T>::size()
{
  size_t size1 = _name_book->size();
  size_t size2 = _table_book->size();

  ASSERT(size1 == size2);

  return size1;
}

template<class T>
token_t
sneaker::container::reservation_map<T>::reserve()
{
  hash_t id = uuid_create_and_hash();

  this->_reserve(id);

  return (token_t)id;
}

template<class T>
void
sneaker::container::reservation_map<T>::_reserve(token_t& id)
{
  this->_name_book->insert(std::pair<token_t, bool>(id, true));
}

template<class T>
bool
sneaker::container::reservation_map<T>::member(token_t& id) const
{
  return this->_name_book->find(id) != this->_name_book->end();
}

template<class T>
bool
sneaker::container::reservation_map<T>::put(token_t& id, T value)
{
  if(!member(id)) {
    return false;
  }

  this->_table_book->insert(std::pair<token_t, T>(id, value));

  return true;
}

template<class T>
bool
sneaker::container::reservation_map<T>::get(token_t& id, T* ptr)
{
  if(!member(id)) {
    return false;
  }

  typename std::map<token_t, T>::iterator itr = this->_table_book->find(id);

  if(itr == this->_table_book->end()) {
    return false;
  }

  T value = (T)(itr->second);

  *ptr = value;

  return true;
}

template<class T>
bool
sneaker::container::reservation_map<T>::unreserve(token_t& id)
{
  if(!member(id)) {
    return false;
  }

  this->_name_book->erase(id);
  this->_table_book->erase(id);

  return true;
}

template<class T>
void
sneaker::container::reservation_map<T>::clear()
{
  this->_name_book->clear();
  this->_table_book->clear();
}


} /* namespace container */

} /* namespace sneaker */


#endif /* _RESERVATION_MAP_H_ */