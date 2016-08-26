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
 * Interfaces for JSON serialization and deserialization.
 *
 * This module provides a set of interfaces for interacting with raw JSON data,
 * and converting that data into data objects, and vice versa.
 *
 * The top level function `sneaker::json::parse()` takes a JSON blob in string
 * format and returns the parsed JSON object, which is an instance of
 * `sneaker::json::JSON`.
 *
 * Internally, instances of `sneaker::json::JSON` depends on several internal
 * types to encapsulate JSON data of various formats. For example,
 * `std::vector` is used to capture JSON arrays and `std::map` is used to
 * encapsulate JSON objects, and so on.
 *
 * This module is largely based on some of the concepts and implementations
 * borrowed from the open source project "json11" from Dropbox, with minor
 * changes and fixes. Please refer to
 *
 *  https://github.com/dropbox/json11
 *
 * for more information.
 *
 * Here is an example of parsing a JSON blob into its corresponding data object.
 *
 *  #include <cassert>
 *  #include <iostream>
 *  #include <string>
 *  #include <sneaker/json/json.h>
 *
 *  using namespace sneaker::json;
 *
 *  const std::string str = "{"
 *    "\"k1\": \"v1\","
 *    "\"k2\": -42,"
 *    "\"k3\": [\"a\", 123, true, false, null]"
 *  "}";
 *
 *  auto json = sneaker::json::parse(str);
 *
 *  assert(std::string("\"v1\"") == json["k1"].dump());
 *  assert(std::string("-42") == json["k2"].dump());
 *  assert(std::string("[\"a\", 123, true, false, null]") == json["k3"].dump());
 *
 *  assert(std::string("v1") == json["k1"].string_value());
 *  assert(-42 == json["k2"].number_value());
 *  assert(std::string("a") == json["k3"][0].string_value());
 *  assert(123 == json["k3"][1].number_value());
 *  assert(true == json["k3"][2].bool_value());
 *  assert(false == json["k3"][3].bool_value());
 *
 *
 * Conversely, here is an example of serializing a JSON data object:
 *
 *  JSON json = JSON::object {
 *    { "key1", "value1" },
 *    { "key2", 123.456 },
 *    { "key3", false },
 *    { "key4", JSON::array { 1, "a", true, nullptr } },
 *  };
 *
 *  std::cout << json.dump() << std::endl;
 */

#ifndef SNEAKER_JSON_H_
#define SNEAKER_JSON_H_

#include <cstdint>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>


namespace sneaker {
namespace json {

// -----------------------------------------------------------------------------

// Forward declaration of `sneaker::json::json_value`.
class json_value;

// -----------------------------------------------------------------------------

typedef std::invalid_argument invalid_json_error;

// -----------------------------------------------------------------------------

class JSON {
public:
  enum Type {
    NUL,
    NUMBER,
    BOOL,
    STRING,
    ARRAY,
    OBJECT
  };

  typedef std::string string;
  typedef std::vector<JSON> array;
  typedef std::map<JSON::string, JSON> object;
  typedef std::nullptr_t null;

  JSON();
  JSON(null);
  JSON(int);
  JSON(double);
  JSON(bool);
  JSON(const string&);
  JSON(string&&);
  JSON(const char *);
  JSON(const array&);
  JSON(array&&);
  JSON(const object&);
  JSON(object&&);

  /* Implicit constructor: anything with a to_json() function. */
  template<class T, class = decltype(&T::to_json)>
  JSON(const T& t) : JSON(t.to_json()) {}

  /* Implicit constructor: map-like objects (std::map, std::unordered_map, etc). */
  template<class M, typename std::enable_if<
    std::is_constructible<std::string, typename M::key_type>::value &&
    std::is_constructible<JSON, typename M::mapped_type>::value, int64_t>::type = 0
  >
  JSON(const M& m) : JSON(object(m.begin(), m.end())) {}

  /* Implicit constructor: vector-like objects (std::list, std::vector, std::set, etc). */
  template<class V, typename std::enable_if<
    std::is_constructible<JSON, typename V::value_type>::value, int64_t>::type = 0
  >
  JSON(const V& v) : JSON(array(v.begin(), v.end())) {}

  /* This prevents JSON(some_pointer) from accidentally producing a bool.
   * Use JSON(bool(some_pointer)) if that behavior is desired. */
  JSON(void*) = delete;

  Type type() const;

  bool is_null()   const { return type() == NUL; }
  bool is_number() const { return type() == NUMBER; }
  bool is_bool()   const { return type() == BOOL; }
  bool is_string() const { return type() == STRING; }
  bool is_array()  const { return type() == ARRAY; }
  bool is_object() const { return type() == OBJECT; }

  double number_value() const;
  int64_t int_value() const;
  bool bool_value() const;
  const string& string_value() const;
  const array& array_items() const;
  const object& object_items() const;
  const JSON& operator[](size_t i) const;
  const JSON& operator[](const std::string& key) const;

  bool operator==(const JSON& other) const;
  bool operator<(const JSON& other) const;
  bool operator!=(const JSON& rhs) const;
  bool operator<=(const JSON& rhs) const;
  bool operator>(const JSON& rhs) const;
  bool operator>=(const JSON& rhs) const;

  void dump(std::string& out) const;

  std::string dump() const;

  static JSON from_int64(int64_t);

private:
  JSON(int64_t, char);

  std::shared_ptr<json_value> m_ptr;
};

// -----------------------------------------------------------------------------

JSON parse(const std::string& in);

// -----------------------------------------------------------------------------

} /* end namespace json */
} /* end namespace sneaker */


#endif /* SNEAKER_JSON_H_ */
