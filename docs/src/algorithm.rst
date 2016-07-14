**********
Algorithms
**********

Implementations of advanced algorithms that are handy for solving
frequently encountered problems.


Tarjan's Strongly Connected Graph Algorithm
===========================================

Abstraction that implements Tarjan's Strongly Connected Graph Algorithm.

Header file: `sneaker/algorithm/tarjan.h`

.. cpp:class:: sneaker::algorithm::tarjan<T>
--------------------------------------------

  This class is a templated class that takes a type of the encapsulating
  value in the vertices. The method `get_components()` takes a list of vertices,
  assuming that the vertices form the graph in mind. It returns an instance
  of `strongly_connected_component_list`, which has information on the
  independent components as well as cycles in the entire graph.

  NOTE: An instance of the class can only perform cycle detection once.
  Running the method on the same instance more than once will result
  in inaccurate results.

  .. code-block:: cpp

    #include <sneaker/algorithm/tarjan.h>

    /* Tests the following object graph:
     * (1) -> (2) -> (3)
     * ^             |
     * |_____________|
     */

    using namespace sneaker::algorithm;

    std::vector<tarjan<int>::vertex*> vertices;

    tarjan<int>::vertex v1(1);
    tarjan<int>::vertex v2(2);
    tarjan<int>::vertex v3(3);

    v1.dependencies().push_back(&v2);
    v2.dependencies().push_back(&v3);
    v3.dependencies().push_back(&v1);

    vertices.push_back(&v1);
    vertices.push_back(&v2);
    vertices.push_back(&v3);

    tarjan<int> algo;
    auto components = algo.detect_cycle(vertices);

    // This graph has one component, which is an independent component,
    // thus no cycles detected.
    assert(1 == components.size());
    assert(0 == components.independent_components().size());
    assert(1 == components.cycles().size());

  .. cpp:type:: typedef std::vector<T> Enumerable
    :noindex:

    Type represents each strongly connected component in the graph.

  .. cpp:type:: typedef std::vector<vertex*> VerticesSet
    :noindex:

    Type of a set of vertices in the graph.

  .. cpp:class:: sneaker::algorithm::tarjan<T>::vertex
  ----------------------------------------------------

    Type represents a vertex in a graph.

    .. cpp:function:: vertex()
      :noindex:

      Constructor.

    .. cpp:function:: explicit vertext(T)
      :noindex:

      Constructor that takes an instance of the encapsulating type.

    .. cpp:type:: iterator
      :noindex:

      The iterator for the list of neighbor vertices.

    .. cpp:function:: bool operator==(const vertex&)
      :noindex:

      Equality operator.

    .. cpp:function:: bool operator!=(const vertex&)
      :noindex:

      Inequality operator.

    .. cpp:function:: iterator begin()
      :noindex:

      Returns an iterator that points to the beginning of the
      neighbor vertices.

    .. cpp:function:: iterator end()
      :noindex:

      Returns an iterator that points to the end of the
      neighbor vertices.

    .. cpp:function:: int index() const
      :noindex:

      Returns the index value associated with this vertex.

    .. cpp:function:: int lowlink() const
      :noindex:

      Returns the low link value associated with this vertex.

    .. cpp:function:: T value() const
      :noindex:

      Returns a copy of the encapsulating value.

    .. cpp:function:: void set_index(int)
      :noindex:

      Sets the index value of this vertex.

    .. cpp:function:: void set_lowlink(int)
      :noindex:

      Sets the low link value of this vertex.

    .. cpp:function:: VerticesSet& dependencies()
      :noindex:

      Returns the list of neighbor vertices.

  .. cpp:class:: sneaker::algorithm::tarjan<T>::strongly_connected_component_list
  -------------------------------------------------------------------------------

    Represents a set of strongly connected components in a directed graph.

    .. cpp:function:: strongly_connected_component_list()
      :noindex:

      Constructor.

    .. cpp:function:: void add(const Enumerable&)
      :noindex:

      Adds a strongly connected component list to the collection.

    .. cpp:function:: size_t size() const
      :noindex:

      Returns the number of strongly connected components in the graph.

    .. cpp:function:: std::vector<Enumerable> independent_components() const
      :noindex:

      Gets the set of independent components in the graph.

    .. cpp:function:: std::vector<Enumerable> cycles() const
      :noindex:

      Gets the set of cycles in the graph.

  .. cpp:function:: tarjan()
    :noindex:

    Constructor.

  .. cpp:function:: strongly_connected_component_list get_components(const VerticesSet&)
    :noindex:

    Given a set of vertices in a graph, returns a set of connected components
    in the graph.
