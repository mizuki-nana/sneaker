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
#ifndef SNEAKER_CACHE_INTERFACE_H_
#define SNEAKER_CACHE_INTERFACE_H_

#include <cstdlib>


namespace sneaker {


namespace cache {


template<class CacheScheme, class OnInsert, class OnErase>
class cache_interface
{
public:
  typedef typename CacheScheme::key_type key_type;
  typedef typename CacheScheme::value_type value_type;

  cache_interface(OnInsert on_insert, OnErase on_erase)
    :
    m_on_insert(on_insert),
    m_on_erase(on_erase),
    m_scheme(m_on_insert, m_on_erase)
  {
  }

  bool empty() const
  {
    return m_scheme.empty();
  }

  size_t size() const
  {
    return m_scheme.size();
  }

  bool find(key_type key) const
  {
    return m_scheme.find(key);
  }

  bool get(key_type key, value_type& value)
  {
    return m_scheme.get(key, value);
  }

  void insert(key_type key, const value_type& value)
  {
    m_scheme.insert(key, value);
  }

  bool erase(key_type key)
  {
    return m_scheme.erase(key);
  }

  void clear()
  {
    m_scheme.clear();
  }

private:
  OnInsert m_on_insert;
  OnErase m_on_erase;
  typename CacheScheme::template impl<OnInsert, OnErase> m_scheme;
};


} /* end namespace cache */


} /* end namespace sneaker */


#endif /* SNEAKER_CACHE_INTERFACE_H_ */
