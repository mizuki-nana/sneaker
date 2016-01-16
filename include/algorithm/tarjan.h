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

/* This class implements Tarjan's Strong Connected Graph Algorithm, which is an
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
 **/

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
      explicit vertex(): m_index(-1), m_lowlink(0) {};

      explicit vertex(T value): m_value(value), m_index(-1), m_lowlink(0) {};

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
        return m_dependencies.begin();
      }

      iterator end() {
        return m_dependencies.end();
      }

      int index() const {
        return m_index;
      }

      int lowlink() const {
        return m_lowlink;
      }

      T value() const {
        return m_value;
      }

      void set_index(int index) {
        m_index = index;
      }

      void set_lowlink(int lowlink) {
        m_lowlink = lowlink;
      }

      std::list<vertex*>& dependencies() {
        return m_dependencies;
      }

    private:
      T m_value;
      int m_index;
      int m_lowlink;
      _Enumerable m_dependencies;
  }; /* end class vertex */


  class strongly_connected_component_list {
    public:
      explicit strongly_connected_component_list():
        m_collection(std::list<Enumerable>())
      {
      }

      void add(Enumerable& scc) {
        m_collection.push_back(scc);
      }

      int size() const {
        return m_collection.size();
      }

      std::list<Enumerable> independent_components() const {
        std::list<Enumerable> components;

        std::for_each(
          m_collection.begin(),
          m_collection.end(),
          [&components](Enumerable vertices) {
            if (vertices.size() <= 1) {
              components.push_back(vertices);
            }
          }
        );

        return components;
      }

      std::list<Enumerable> cycles() const {
        std::list<Enumerable> components;

        std::for_each(
          m_collection.begin(),
          m_collection.end(),
          [&components](Enumerable vertices) {
            if (vertices.size() > 1) {
              components.push_back(vertices);
            }
          }
        );

        return components;
      }

    private:
      std::list<Enumerable> m_collection;
  }; /* end class strongly_connected_component_list */

  explicit tarjan() :
    m_index(0),
    m_stack(std::list<vertex*>()),
    m_components(strongly_connected_component_list())
  {
  }

  strongly_connected_component_list get_components(std::list<vertex*>&);

private:
  void strong_connect(vertex*);

  int m_index;
  std::list<vertex*> m_stack;
  strongly_connected_component_list m_components;
};

// -----------------------------------------------------------------------------

template<class T>
using _MyType = typename sneaker::algorithm::tarjan<T>;

// -----------------------------------------------------------------------------

template<class T>
typename _MyType<T>::strongly_connected_component_list
sneaker::algorithm::tarjan<T>::get_components(std::list<vertex*>& graph)
{
  for (auto itr = graph.begin(); itr != graph.end(); ++itr) {
    vertex* vtx = static_cast<vertex*>(*itr);
    if (vtx->index() < 0) {
      this->strong_connect(vtx);
    }
    *itr = vtx;
  };

  return m_components;
}

// -----------------------------------------------------------------------------

template<class T>
void
sneaker::algorithm::tarjan<T>::strong_connect(vertex* vtx)
{
  vtx->set_index(this->m_index);
  vtx->set_lowlink(this->m_index);

  m_index++;

  m_stack.push_back(vtx);

  for (auto itr = vtx->begin(); itr != vtx->end(); ++itr) {
    vertex* w = static_cast<vertex*>(*itr);

    if (w->index() < 0) {
      this->strong_connect(w);
      vtx->set_lowlink(std::min(vtx->lowlink(), w->lowlink()));
    } else {
      auto itr = std::find_if (
        m_stack.begin(),
        m_stack.end(),
        [&w](vertex* vtx_) {
          return *vtx_ == *w;
        }
      );

      if (itr != m_stack.end()) {
        vtx->set_lowlink(std::min(vtx->lowlink(), w->index()));
      }
    }

    *itr = w;
  };

  if (vtx->lowlink() == vtx->index()) {
    Enumerable scc;
    vertex *w = nullptr;

    do {
      w = m_stack.back();
      scc.push_back(w->value());
      m_stack.pop_back();
    } while (*vtx != *w);

    m_components.add(scc);
  }
}

// -----------------------------------------------------------------------------


} /* end namespace algorithm */


} /* end namespace sneaker */


#endif /* SNEAKER_ALGORITHM_TARJAN_H_ */
