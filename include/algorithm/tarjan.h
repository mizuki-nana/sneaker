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
 * This class implements Tarjan's Strong Connected Graph Algorithm, which is an
 * algorithm that finds the strongly connected components in a directed graph.
 * More information can be found at:
 *
 * http://en.wikipedia.org/wiki/Tarjan's_strongly_connected_components_algorithm
 * http://en.algoritmy.net/article/44220/Tarjans-algorithm
 *
 * The class is a templated class that takes the type of the encapsulating
 * value in the vertices. The method `get_components` takes a list of vertices,
 * assuming that the vertices form the graph in mind. It returns an instance
 * of `strongly_connected_component_list`, which has information on the
 * independent components as well as cycles in the entire graph.
 *
 * NOTE: An instance of the class can only perform cycle detection once.
 *       Running the method on the same instance more than once will result
 *       in inaccurate results.
 *
 * Example:
 *
 *  Tests the following object graph:
 *
 *  (1) -> (2) -> (3)
 *   ^             |
 *   |_____________|
 *
 *  using namespace sneaker::algorithm;
 *
 *  std::list<tarjan<int>::vertex*> vertices;
 *
 *  tarjan<int>::vertex v1(1);
 *  tarjan<int>::vertex v2(2);
 *  tarjan<int>::vertex v3(3);
 *
 *  v1.dependencies().push_back(&v2);
 *  v2.dependencies().push_back(&v3);
 *  v3.dependencies().push_back(&v1);
 *
 *  vertices.push_back(&v1);
 *  vertices.push_back(&v2);
 *  vertices.push_back(&v3);
 *
 *  tarjan<int> algo;
 *  auto components = algo.detect_cycle(vertices);
 *
 *  // This graph has one component, which is an independent component, thus
 *  // no cycles detected.
 *  assert(1 == components.size());
 *  assert(0 == components.independent_components().size());
 *  assert(1 == components.cycles().size());
 */

#ifndef SNEAKER_ALGORITHM_TARJAN_H_
#define SNEAKER_ALGORITHM_TARJAN_H_

#include <algorithm>
#include <list>
#include <stack>


namespace sneaker {


namespace algorithm {


template<class T>
class tarjan {
public:

  using Enumerable = typename std::list<T>;

  class vertex {
    public:
      explicit vertex(): _index(-1), _lowlink(0) {};

      explicit vertex(T value): _value(value), _index(-1), _lowlink(0) {};

      using _Enumerable = typename std::list<vertex*>;

      using iterator = typename _Enumerable::iterator;

      bool operator==(const vertex& other) {
        return this->value() == other.value() &&
          this->index() == other.index() &&
          this->lowlink() == other.lowlink();
      }

      bool operator!=(const vertex& other) {
        return !(*this == other);
      }

      iterator begin() {
        return _dependencies.begin();
      }

      iterator end() {
        return _dependencies.end();
      }

      int index() const {
        return _index;
      }

      int lowlink() const {
        return _lowlink;
      }

      T value() const {
        return _value;
      }

      void set_index(int index) {
        _index = index;
      }

      void set_lowlink(int lowlink) {
        _lowlink = lowlink;
      }

      std::list<vertex*>& dependencies() {
        return _dependencies;
      }

    private:
      T _value;
      int _index;
      int _lowlink;
      _Enumerable _dependencies;
  }; /* end class vertex */

  class strongly_connected_component_list {
    public:
      explicit strongly_connected_component_list():
        _collection(std::list<Enumerable>())
      {}

      void add(Enumerable& scc) {
        _collection.push_back(scc);
      }

      int size() const {
        return _collection.size();
      }

      std::list<Enumerable> independent_components() const {
        std::list<Enumerable> components;

        std::for_each(
          _collection.begin(),
          _collection.end(),
          [&components](Enumerable vertices) {
            if(vertices.size() <= 1) {
              components.push_back(vertices);
            }
          }
        );

        return components;
      }

      std::list<Enumerable> cycles() const {
        std::list<Enumerable> components;

        std::for_each(
          _collection.begin(),
          _collection.end(),
          [&components](Enumerable vertices) {
            if(vertices.size() > 1) {
              components.push_back(vertices);
            }
          }
        );

        return components;
      }

    private:
      std::list<Enumerable> _collection;
  }; /* end class strongly_connected_component_list */

  explicit tarjan():
    _index(0),
    _stack(std::list<vertex*>()),
    _components(strongly_connected_component_list())
  {};

  strongly_connected_component_list get_components(std::list<vertex*>&);

private:
  void _strong_connect(vertex*);

  int _index;
  std::list<vertex*> _stack;
  strongly_connected_component_list _components;
};


template<class T>
using _MyType = typename sneaker::algorithm::tarjan<T>;


template<class T>
typename _MyType<T>::strongly_connected_component_list
sneaker::algorithm::tarjan<T>::get_components(std::list<vertex*>& graph)
{
  for(auto itr = graph.begin(); itr != graph.end(); ++itr) {
    vertex* vtx = static_cast<vertex*>(*itr);
    if(vtx->index() < 0) {
      this->_strong_connect(vtx);
    }
    *itr = vtx;
  };

  return _components;
}

template<class T>
void
sneaker::algorithm::tarjan<T>::_strong_connect(vertex* vtx)
{
  vtx->set_index(this->_index);
  vtx->set_lowlink(this->_index);

  _index++;

  _stack.push_back(vtx);

  for(auto itr = vtx->begin(); itr != vtx->end(); ++itr) {
    vertex* w = static_cast<vertex*>(*itr);

    if(w->index() < 0) {
      this->_strong_connect(w);
      vtx->set_lowlink(std::min(vtx->lowlink(), w->lowlink()));
    } else {
      auto itr = std::find_if(
        _stack.begin(),
        _stack.end(),
        [&w](vertex* vtx_) {
          return *vtx_ == *w;
        }
      );

      if(itr != _stack.end()) {
        vtx->set_lowlink(std::min(vtx->lowlink(), w->index()));
      }
    }

    *itr = w;
  };

  if(vtx->lowlink() == vtx->index()) {
    Enumerable scc;
    vertex *w = nullptr;

    do {
      w = _stack.back();
      scc.push_back(w->value());
      _stack.pop_back();
    } while(*vtx != *w);

    _components.add(scc);
  }
}


} /* end namespace algorithm */


} /* end namespace sneaker */


#endif /* SNEAKER_ALGORITHM_TARJAN_H_ */
