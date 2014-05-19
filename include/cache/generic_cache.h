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

#ifndef SNEAKER_GENERIC_CACHE_H_
#define SNEAKER_GENERIC_CACHE_H_

#include <algorithm>
#include <cassert>
#include <map>


namespace sneaker {


namespace cache {


template<class K, class T, class CreateHandler, class DestroyHandler>
class generic_cache {
public:
  explicit generic_cache(CreateHandler, DestroyHandler);

  ~generic_cache();

  bool empty() const;

  size_t size() const;

  bool member(K) const;

  bool get(K, T*);

  bool put(K, bool=false);

  bool erase(K);

  void clear();

private:
  T _find(K) const;
  bool _erase(K);

  void _check_invariance() const;

  CreateHandler _create_handler;
  DestroyHandler _destroy_handler;
  std::map<K, T> _map;
};

template<class K, class T, class CreateHandler, class DestroyHandler>
sneaker::cache::generic_cache<K, T, CreateHandler, DestroyHandler>::generic_cache(
  CreateHandler create_handler,
  DestroyHandler destroy_handler
):
  _create_handler(create_handler),
  _destroy_handler(destroy_handler),
  _map(std::map<K, T>())
{
  this->_check_invariance();
}

template<class K, class T, class CreateHandler, class DestroyHandler>
sneaker::cache::generic_cache<K, T, CreateHandler, DestroyHandler>::~generic_cache()
{
  this->clear();
}

template<class K, class T, class CreateHandler, class DestroyHandler>
void
sneaker::cache::generic_cache<K, T, CreateHandler, DestroyHandler>::_check_invariance() const
{
  assert(this->_create_handler);
  assert(this->_destroy_handler);
}

template<class K, class T, class CreateHandler, class DestroyHandler>
bool
sneaker::cache::generic_cache<K, T, CreateHandler, DestroyHandler>::empty() const
{
  return this->_map.empty();
}

template<class K, class T, class CreateHandler, class DestroyHandler>
size_t
sneaker::cache::generic_cache<K, T, CreateHandler, DestroyHandler>::size() const
{
  return this->_map.size();
}

template<class K, class T, class CreateHandler, class DestroyHandler>
bool
sneaker::cache::generic_cache<K, T, CreateHandler, DestroyHandler>::member(K key) const
{
  return this->_map.find(key) != this->_map.end();
}

template<class K, class T, class CreateHandler, class DestroyHandler>
bool
sneaker::cache::generic_cache<K, T, CreateHandler, DestroyHandler>::get(K key, T* ptr)
{
  assert(ptr);

  if(member(key)) {
    *ptr = _find(key);
    return true;
  }

  bool res = this->put(key, true);

  if(!res) {
    return false;
  }

  T obj = _find(key);
  *ptr = obj;

  return true;
}

template<class K, class T, class CreateHandler, class DestroyHandler>
bool
sneaker::cache::generic_cache<K, T, CreateHandler, DestroyHandler>::put(K key, bool forceUpdate)
{
  if(member(key)) {
    if(!forceUpdate) {
      return false;
    } else {
      _map.erase(key);
    }
  }

  T ptr = nullptr;

  bool res = _create_handler(key, &ptr);

  if(ptr == nullptr || !res) {
    return false;
  }

  _map.insert(std::pair<K, T>(key, ptr));

  return true;
}

template<class K, class T, class CreateHandler, class DestroyHandler>
bool
sneaker::cache::generic_cache<K, T, CreateHandler, DestroyHandler>::erase(K key)
{
  bool res = this->_erase(key);

  if(!res) {
    return res;
  }

  this->_map.erase(key);

  return true;
}

template<class K, class T, class CreateHandler, class DestroyHandler>
void
sneaker::cache::generic_cache<K, T, CreateHandler, DestroyHandler>::clear()
{
  std::for_each(
    this->_map.begin(),
    this->_map.end(),
    [this](const std::pair<K, T>& pair) {
      K key = pair.first;
      this->_erase(key);
    }
  );

  this->_map.clear();
}

template<class K, class T, class CreateHandler, class DestroyHandler>
T
sneaker::cache::generic_cache<K, T, CreateHandler, DestroyHandler>::_find(K key) const
{
  typename std::map<K, T>::const_iterator itr = _map.find(key);
  return (T)(itr->second);
}

template<class K, class T, class CreateHandler, class DestroyHandler>
bool
sneaker::cache::generic_cache<K, T, CreateHandler, DestroyHandler>::_erase(K key)
{
  if(!this->member(key)) {
    return false;
  }

  T ptr = nullptr;

  this->get(key, &ptr);

  if(!ptr) {
    return false;
  }

  bool res = _destroy_handler(key, &ptr);

  return res;
}


} /* end namespace cache */


} /* end namespace sneaker */


#endif /* SNEAKER_GENERIC_CACHE_H_ */