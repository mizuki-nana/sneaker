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
#ifndef SNEAKER_LRU_CACHE_H_
#define SNEAKER_LRU_CACHE_H_

#include <boost/bimap.hpp>
#include <boost/bimap/list_of.hpp>
#include <boost/bimap/unordered_set_of.hpp>


namespace sneaker {
namespace cache {

template<typename K, typename V, size_t N>
class lru_cache
{
public:
  typedef K key_type;
  typedef V value_type;

  lru_cache()
    :
    m_container()
  {
  }

  bool empty() const
  {
    return m_container.empty();
  }

  bool full() const
  {
    return m_container.size() == N;
  }

  size_t size() const
  {
    return m_container.size();
  }

  bool find(key_type key) const
  {
    return m_container.left.find(key) != m_container.left.end();
  }

  bool get(const key_type& key, value_type& res)
  {
    const typename container_type::left_iterator it =
      m_container.left.find(key);

    if (it != m_container.left.end())
    {
      m_container.right.relocate(m_container.right.end(),
        m_container.project_right(it));

      res = it->second;

      return true;
    }

    return false;
  }

  void next_erasure_pair(key_type** key_ptr, value_type** value_ptr)
  {
    if (full())
    {
      auto remove_itr = m_container.right.begin();
      *key_ptr = const_cast<key_type*>(&(*remove_itr).get_left());
      *value_ptr = &(*remove_itr).get_right();
    }
  }

  void insert(key_type key, const value_type& value)
  {
    if (full())
    {
      auto remove_itr = m_container.right.begin();
      m_container.right.erase(remove_itr);
    }

    m_container.insert(typename container_type::value_type(key, value));
  }

  bool erase(key_type key)
  {
    return m_container.left.erase(key);
  }

  void clear()
  {
    m_container.clear();
  }

private:
  typedef boost::bimaps::bimap<boost::bimaps::unordered_set_of<key_type>,
    boost::bimaps::list_of<value_type>> container_type;

  container_type m_container;
};

} /* end namespace cache */
} /* end namespace sneaker */


#endif /* SNEAKER_LRU_CACHE_H_ */
