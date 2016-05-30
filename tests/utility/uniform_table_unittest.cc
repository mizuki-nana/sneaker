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

/* Unit tests for definitions defined in sneaker/utility/uniform_table.h */

#include "utility/uniform_table.h"

#include "testing/testing.h"

#include <ostream>


// -----------------------------------------------------------------------------

class uniform_table_unittest : public ::testing::Test
{
protected:
  template<typename UniformTableType>
  void print(const UniformTableType& uniform_table) const
  {
    std::cout << uniform_table.str() << std::endl;
    std::cout << "(output above expected)" << std::endl;
  }
};

// -----------------------------------------------------------------------------

TEST_F(uniform_table_unittest, TestInitialization)
{
  sneaker::utility::uniform_table<10> uniform_table;
}

// -----------------------------------------------------------------------------

TEST_F(uniform_table_unittest, TestWithSingleColumn)
{
  sneaker::utility::uniform_table<10> uniform_table;
  uniform_table.write_separator();
  uniform_table.write("Ocean");
  uniform_table.write("Forest");
  uniform_table.write("Volcano");
  print(uniform_table);
}

// -----------------------------------------------------------------------------

TEST_F(uniform_table_unittest, TestWith2Columns)
{
  sneaker::utility::uniform_table<10, 10> uniform_table;
  uniform_table.write_separator();
  uniform_table.write("Ocean", "Blue");
  uniform_table.write("Forest", "Green");
  uniform_table.write("Volcano", "Red");
  print(uniform_table);
}

// -----------------------------------------------------------------------------

TEST_F(uniform_table_unittest, TestWith3Columns)
{
  sneaker::utility::uniform_table<2, 10, 10> uniform_table;
  uniform_table.write_separator();
  uniform_table.write(2, "Ocean", "Blue");
  uniform_table.write(3, "Forest", "Green");
  uniform_table.write(1, "Volcano", "Red");
  print(uniform_table);
}

// -----------------------------------------------------------------------------

TEST_F(uniform_table_unittest, TestWith3ColumnsWithLessColumnData)
{
  sneaker::utility::uniform_table<2, 10, 10> uniform_table;
  uniform_table.write_separator();
  uniform_table.write(2, "Ocean");
  uniform_table.write(3, "Forest");
  uniform_table.write(1, "Volcano");
  print(uniform_table);
}

// -----------------------------------------------------------------------------
