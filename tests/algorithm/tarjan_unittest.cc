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

/* Unit test for definitions in sneaker/algorithm/tarjan.h */

#include <list>
#include "../../include/testing/testing.h"
#include "../../include/algorithm/tarjan.h"


using namespace sneaker::algorithm;


class tarjan_unittest : public ::testing::Test {
public:
  void detect_cycle_and_assert_results(
    std::list<tarjan<int>::vertex*>& vertices,
    int expected_components,
    int expected_independent_components,
    int expected_cycles
  )
  {
    tarjan<int> algo;
    auto components = algo.get_components(vertices);

    auto actual_components = components.size();
    auto actual_independent_components = components.independent_components().size();
    auto actual_cycles = components.cycles().size();

    ASSERT_EQ(expected_components, actual_components);
    ASSERT_EQ(expected_independent_components, actual_independent_components);
    ASSERT_EQ(expected_cycles, actual_cycles);

    ASSERT_EQ(actual_components, actual_independent_components + actual_cycles);
  }
};

TEST_F(tarjan_unittest, TestEmptyGraph)
{
  /*
   * Tests the following object graph:
   *
   * ()
   */
  std::list<tarjan<int>::vertex*> vertices;

  this->detect_cycle_and_assert_results(vertices, 0, 0, 0);
}

TEST_F(tarjan_unittest, TestSingleVertex)
{
  /* Tests the following object graph:
   *
   * (1)
   */
  std::list<tarjan<int>::vertex*> vertices;
  tarjan<int>::vertex v1(1);

  vertices.push_back(&v1);

  this->detect_cycle_and_assert_results(vertices, 1, 1, 0);
}

TEST_F(tarjan_unittest, TestSingleEdge)
{
  /* Tests the following object graph:
   *
   * (1) -> (2)
   */
  std::list<tarjan<int>::vertex*> vertices;

  tarjan<int>::vertex v1(1);
  tarjan<int>::vertex v2(2);

  v1.dependencies().push_back(&v2);

  vertices.push_back(&v1);
  vertices.push_back(&v2);

  this->detect_cycle_and_assert_results(vertices, 2, 2, 0);
}

TEST_F(tarjan_unittest, TestLinearEdges)
{
  /* Tests the following object graph:
   *
   * (1) -> (2) -> (3)
   */
  std::list<tarjan<int>::vertex*> vertices;

  tarjan<int>::vertex v1(1);
  tarjan<int>::vertex v2(2);
  tarjan<int>::vertex v3(3);

  v1.dependencies().push_back(&v2);
  v2.dependencies().push_back(&v3);

  vertices.push_back(&v1);
  vertices.push_back(&v2);
  vertices.push_back(&v3);

  this->detect_cycle_and_assert_results(vertices, 3, 3, 0);
}

TEST_F(tarjan_unittest, TestSimpleCycle)
{
  /* Tests the following object graph:
   *
   * (1) -> (2)
   *  ^      |
   *  |______|
   */
  std::list<tarjan<int>::vertex*> vertices;

  tarjan<int>::vertex v1(1);
  tarjan<int>::vertex v2(2);

  v1.dependencies().push_back(&v2);
  v2.dependencies().push_back(&v1);

  vertices.push_back(&v1);
  vertices.push_back(&v2);

  this->detect_cycle_and_assert_results(vertices, 1, 0, 1);
}

TEST_F(tarjan_unittest, TestSimpleCycle2)
{
  /* Tests the following object graph:
   *
   * (1) -> (2) -> (3)
   *  ^             |
   *  |_____________|
   */
  std::list<tarjan<int>::vertex*> vertices;

  tarjan<int>::vertex v1(1);
  tarjan<int>::vertex v2(2);
  tarjan<int>::vertex v3(3);

  v1.dependencies().push_back(&v2);
  v2.dependencies().push_back(&v3);
  v3.dependencies().push_back(&v1);

  vertices.push_back(&v1);
  vertices.push_back(&v2);
  vertices.push_back(&v3);

  this->detect_cycle_and_assert_results(vertices, 1, 0, 1);
}

TEST_F(tarjan_unittest, TestTwoIsolatedCycles)
{
  /* Tests the following object graph:
   *
   * (1) -> (2) -> (3)  (4) -> (5) -> (6)
   *  ^             |    ^             |
   *  |_____________|    |_____________|
   */
  std::list<tarjan<int>::vertex*> vertices;

  tarjan<int>::vertex v1(1);
  tarjan<int>::vertex v2(2);
  tarjan<int>::vertex v3(3);

  v1.dependencies().push_back(&v2);
  v2.dependencies().push_back(&v3);
  v3.dependencies().push_back(&v1);

  vertices.push_back(&v1);
  vertices.push_back(&v2);
  vertices.push_back(&v3);

  tarjan<int>::vertex v4(4);
  tarjan<int>::vertex v5(5);
  tarjan<int>::vertex v6(6);

  v4.dependencies().push_back(&v5);
  v5.dependencies().push_back(&v6);
  v6.dependencies().push_back(&v4);

  vertices.push_back(&v4);
  vertices.push_back(&v5);
  vertices.push_back(&v6);

  this->detect_cycle_and_assert_results(vertices, 2, 0, 2);
}

TEST_F(tarjan_unittest, TestCycleWithInwardStub)
{
  /* Tests the following object graph:
   *
   * (1) -> (2) -> (3) <- (4)
   *  ^             |
   *  |_____________|
   */
  std::list<tarjan<int>::vertex*> vertices;

  tarjan<int>::vertex v1(1);
  tarjan<int>::vertex v2(2);
  tarjan<int>::vertex v3(3);
  tarjan<int>::vertex v4(4);

  v1.dependencies().push_back(&v2);
  v2.dependencies().push_back(&v3);
  v3.dependencies().push_back(&v1);
  v4.dependencies().push_back(&v3);

  vertices.push_back(&v1);
  vertices.push_back(&v2);
  vertices.push_back(&v3);
  vertices.push_back(&v4);

  this->detect_cycle_and_assert_results(vertices, 2, 1, 1);
}

TEST_F(tarjan_unittest, TestCycleWithOutwardStub)
{
  /* Tests the following object graph:
   *
   * (1) -> (2) -> (3) -> (4)
   *  ^             |
   *  |_____________|
   */
  std::list<tarjan<int>::vertex*> vertices;

  tarjan<int>::vertex v1(1);
  tarjan<int>::vertex v2(2);
  tarjan<int>::vertex v3(3);
  tarjan<int>::vertex v4(4);

  v1.dependencies().push_back(&v2);
  v2.dependencies().push_back(&v3);
  v3.dependencies().push_back(&v1);
  v3.dependencies().push_back(&v4);

  vertices.push_back(&v1);
  vertices.push_back(&v2);
  vertices.push_back(&v3);
  vertices.push_back(&v4);

  this->detect_cycle_and_assert_results(vertices, 2, 1, 1);
}

TEST_F(tarjan_unittest, TestNestedCycles)
{
  /* Tests the following object graph:
   *
   * (1) -> (2) -> (3) -> (4)
   *  ^      ^      |      |
   *  |      |--<---|      |
   *  |                    |
   *  |<------- (5) <------|
   */
  std::list<tarjan<int>::vertex*> vertices;

  tarjan<int>::vertex v1(1);
  tarjan<int>::vertex v2(2);
  tarjan<int>::vertex v3(3);
  tarjan<int>::vertex v4(4);
  tarjan<int>::vertex v5(5);

  v1.dependencies().push_back(&v2);
  v2.dependencies().push_back(&v3);
  v3.dependencies().push_back(&v4);
  v4.dependencies().push_back(&v5);
  v5.dependencies().push_back(&v1);

  v3.dependencies().push_back(&v2);

  vertices.push_back(&v1);
  vertices.push_back(&v2);
  vertices.push_back(&v3);
  vertices.push_back(&v4);
  vertices.push_back(&v5);

  this->detect_cycle_and_assert_results(vertices, 1, 0, 1);
}

TEST_F(tarjan_unittest, TestAdjacentCycles)
{
  /* Tests the following object graph:
   *
   * (1) -> (2) -> (3) -> (4) -> (5)
   *  ^            |^             |
   *  |____________||_____________|
   */
  std::list<tarjan<int>::vertex*> vertices;

  tarjan<int>::vertex v1(1);
  tarjan<int>::vertex v2(2);
  tarjan<int>::vertex v3(3);
  tarjan<int>::vertex v4(4);
  tarjan<int>::vertex v5(5);

  v1.dependencies().push_back(&v2);
  v2.dependencies().push_back(&v3);
  v3.dependencies().push_back(&v1);
  v3.dependencies().push_back(&v4);
  v4.dependencies().push_back(&v5);
  v5.dependencies().push_back(&v3);

  vertices.push_back(&v1);
  vertices.push_back(&v2);
  vertices.push_back(&v3);
  vertices.push_back(&v4);
  vertices.push_back(&v5);

  this->detect_cycle_and_assert_results(vertices, 1, 0, 1);
}
