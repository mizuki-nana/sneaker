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

/**
 * `sneaker::utility::uniform_table` is a utility class for managing formatting
 * and printing data into a uniform table. This class is templatized by the
 * sizes of the columns that form the uniform table.
 *
 * Example:
 *
 * sneaker::utility::uniform_table<2, 10, 10> uniform_table;
 * uniform_table.write_separator();
 * uniform_table.write(2, "Ocean", "Blue");
 * uniform_table.write(3, "Forest", "Green");
 * uniform_table.write(1, "Volcano", "Red");
 * std::cout << uniform_table.str();
 *
 * will give the following output:
 *
 * | -- | ---------- | ---------- |
 * |  2 |      Ocean |       Blue |
 * |  3 |     Forest |      Green |
 * |  1 |    Volcano |        Red |
 *
 */

#ifndef SNEAKER_UNIFORM_TABLE_H_
#define SNEAKER_UNIFORM_TABLE_H_

#include <iomanip>
#include <ostream>
#include <sstream>


namespace sneaker {


namespace utility {


template<size_t... ColumnSizes>
class uniform_table
{
private:
  template<size_t ColumnSize, size_t... RestColumnSizes>
  struct uniform_table_proxy
  {
    enum { column_size = ColumnSize };
    typedef uniform_table_proxy<RestColumnSizes...> next_type;

    size_t size() const { return column_size; }
    next_type next() const { return next_type(); }
  };

  template<size_t ColumnSize>
  struct uniform_table_proxy<ColumnSize>
  {
    enum { column_size = ColumnSize };
    size_t size() const { return column_size; }
  };

public:
  std::ostream stream() const
  {
    return m_ss;
  }

  std::string str() const
  {
    return m_ss.str();
  }

  template<typename... Args>
  void write(Args... args)
  {
    static_assert(sizeof...(Args) <= sizeof...(ColumnSizes), "Extra arguments supplied");

    m_ss << "| ";

    typedef uniform_table_proxy<ColumnSizes...> ProxyType;
    write_impl(ProxyType(), args...);

    m_ss << std::endl;
  }

  void write_separator()
  {
    m_ss << "| ";

    typedef uniform_table_proxy<ColumnSizes...> ProxyType;
    write_separator_impl(ProxyType(), ColumnSizes...);

    m_ss << std::endl;
  }

private:
  template<typename ProxyType, typename T, typename... Args>
  void write_impl(ProxyType proxy, T value, Args... args)
  {
    write_impl(proxy, value);
    write_impl(proxy.next(), args...);
  }

  template<typename ProxyType, typename T>
  void write_impl(ProxyType proxy, T value)
  {
    m_ss << std::setfill(' ') << std::setw(static_cast<int>(proxy.size())) << value << " | ";
  }

  template<typename ProxyType, typename T, typename... Args>
  void write_separator_impl(ProxyType proxy, T value, Args... args)
  {
    write_separator_impl(proxy, value);
    write_separator_impl(proxy.next(), args...);
  }

  template<typename ProxyType, typename T>
  void write_separator_impl(ProxyType proxy, T value)
  {
    m_ss << std::setfill('-') << std::setw(static_cast<int>(proxy.size())) << '-' << " | ";
  }

  std::stringstream m_ss;
};


} /* end namespace utility */


} /* end namespace sneaker */


#endif /* SNEAKER_UNIFORM_TABLE_H_ */
