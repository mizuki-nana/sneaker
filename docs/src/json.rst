****
JSON
****

Interfaces for manipulating, validating and conversating data in JSON format.



JSON Serialization and Deserialization
======================================

JSON data object abstraction, and interfaces for JSON serialization and
deserialization.

This module provides a set of interfaces for interacting with raw JSON data,
and converting that data into data objects, and vice versa.

The top level function `sneaker::json::parse()` takes a JSON blob in string
format and returns the parsed JSON object, which is an instance of
`sneaker::json::JSON`.

Internally, instances of `sneaker::json::JSON` depends on several internal
types to encapsulate JSON data of various formats. For example,
`std::vector` is used to capture JSON arrays and `std::map` is used to
encapsulate JSON objects, and so on.

This module is largely based on some of the concepts and implementations
borrowed from the open source project "json11" from Dropbox, with minor
changes and fixes. Please refer to https://github.com/dropbox/json11
for more information.


Here is an example of parsing a JSON blob into its corresponding data object.

.. code-block:: cpp

  #include <cassert>
  #include <iostream>
  #include <string>
  #include <sneaker/json/json.h>

  using namespace sneaker::json;

  const std::string str = "{"
    "\"k1\": \"v1\","
    "\"k2\": -42,"
    "\"k3\": [\"a\", 123, true, false, null]"
  "}";

  auto json = sneaker::json::parse(str);

  assert(std::string("\"v1\"") == json["k1"].dump());
  assert(std::string("-42") == json["k2"].dump());
  assert(std::string("[\"a\", 123, true, false, null]") == json["k3"].dump());

  assert(std::string("v1") == json["k1"].string_value());
  assert(-42 == json["k2"].number_value());
  assert(std::string("a") == json["k3"][0].string_value());
  assert(123 == json["k3"][1].number_value());
  assert(true == json["k3"][2].bool_value());
  assert(false == json["k3"][3].bool_value());

  // Conversely, here is an example of serializing a JSON data object:

  JSON json = JSON::object {
    { "key1", "value1" },
    { "key2", 123.456 },
    { "key3", false },
    { "key4", JSON::array { 1, "a", true, nullptr } },
  };

  std::cout << json.dump() << std::endl;


Header file: `sneaker/json/json.h`


.. cpp:function:: sneaker::json::parse(const std::string& in)
---------------------------------------------------------------------------------------

  Top level function for parsing a JSON blob and returns the deserialized
  JSON data object.


.. cpp:class:: sneaker::json::invalid_json_error
------------------------------------------------

  Error thrown when parsing an invalid JSON blob.


.. cpp:class:: sneaker::json::JSON
----------------------------------

  .. cpp:type:: JSON::Type
    :noindex:

    The type of the JSON object. Values are `NUL`, `NUMBER`, `BOOL`, `STRING`,
    `ARRAY` and `OBJECT`.

  .. cpp:type:: JSON::string
    :noindex:

    The underflying JSON string type.

  .. cpp:type:: JSON::array
    :noindex:

    The underlying JSON array type.

  .. cpp:type:: JSON::object
    :noindex:

    The underlying JSON object type.

  .. cpp:function:: JSON()
    :noindex:

    Default constructor.

  .. cpp:function:: JSON(null)
    :noindex:

    Constructor.

  .. cpp:function:: JSON(double)
    :noindex:

    Constructor.

  .. cpp:function:: JSON(int)
    :noindex:

    Constructor.

  .. cpp::function:: JSON(bool)
    :noindex:

    Constructor.

  .. cpp:function:: JSON(const string&)
    :noindex:

    Constructor.

  .. cpp:function:: JSON(string&&)
    :noindex:

    Constructor.

  .. cpp:function:: JSON(const char*)
    :noindex:

    Constructor.

  .. cpp:function:: JSON(const array&)
    :noindex:

    Constructor.

  .. cpp:function:: JSON(array&&)
    :noindex:

    Constructor.

  .. cpp:function:: JSON(const object&)
    :noindex:

    Constructor.

  .. cpp:function:: JSON(object&&)
    :noindex:

    Constructor.

  .. cpp:function:: template<class T, class = decltype(&T::to_json)>
                    JSON(const T& t)
    :noindex:

    Implicit constructor: anything with a to_json() function.

  .. cpp:function:: template<class M, typename std::enable_if<
                      std::is_constructible<std::string, typename M::key_type>::value &&
                      std::is_constructible<JSON, typename M::mapped_type>::value, int>::type = 0
                    >
                    JSON(const M& m)
    :noindex:

    Implicit constructor: map-like objects (std::map, std::unordered_map, etc).

  .. cpp:function:: template<class V, typename std::enable_if<
                      std::is_constructible<JSON, typename V::value_type>::value, int>::type = 0
                    >
                    JSON(const V& v)
    :noindex:

    Implicit constructor: vector-like objects (std::list, std::vector, std::set, etc).

  .. cpp:function:: Type type() const
    :noindex:

    Gets the type of the JSON object.

  .. cpp:function:: bool is_null()
    :noindex:

    Determines if this instance represents a JSON null value.

  .. cpp:function:: bool is_number()
    :noindex:

    Determines if this instance represents a JSON numeric value.

  .. cpp:function:: bool is_bool()
    :noindex:

    Determines if this instance represents a JSON boolean value.

  .. cpp:function:: bool is_string()
    :noindex:

    Determines if this instance represents a JSON string value.

  .. cpp:function:: bool is_array()
    :noindex:

    Determines if this instance represents a JSON array value.

  .. cpp:function:: bool is_object()
    :noindex:

    Determines if this instance represents a JSON object value.

  .. cpp:function:: double number_value() const
    :noindex:

    Gets the encapsulating floating numeric value of this JSON object.

  .. cpp:function:: int int_value() const
    :noindex:

    Gets the encapsulating integer numeric value of this JSON object.

  .. cpp:function:: bool bool_value() const
    :noindex:

    Gets the encapsulating boolean value of this JSON object.

  .. cpp:function:: const string& string_value() const
    :noindex:

    Gets the encapsulating string value of this JSON object.

  .. cpp:function:: const array& array_items() const
    :noindex:

    Gets the encapsulating array value of this JSON object.

  .. cpp:function:: const object& object_items() const
    :noindex:

    Gets the encapsulating object value of this JSON object.

  .. cpp:function:: const JSON& operator[](size_t i) const
    :noindex:

    JSON array type element accessor.

  .. cpp:function:: const JSON& operator[](const std::string& key) const
    :noindex:

    JSON object type element accessor.

  .. cpp:function:: bool operator==(const JSON& other) const
    :noindex:

    Equality operator.

  .. cpp:function:: bool operator<(const JSON& other) const
    :noindex:

    Less Than equality operator.

  .. cpp:function:: bool operator!=(const JSON& other) const
    :noindex:

    Inequality operator.

  .. cpp:function:: bool operator<=(const JSON& other) const
    :noindex:

    Less Than or Equal equality operator.

  .. cpp:function:: bool operator>(const JSON& other) const
    :noindex:

    Greater Than equality operator.

  .. cpp:function:: bool operator>=(const JSON& other) const
    :noindex:

    Greater Than or Equal equality operator.

  .. cpp:function:: void dump(std::string& out) const
    :noindex:

    Serializes the JSON data object and dumps the result into the provided
    string.

  .. cpp:function:: std::string dump() const
    :noindex:

    Serializes the JSON data object and returns the result string.


JSON Schema Validation
======================


Interface for validating JSON blobs using JSON schemas.


The validation mechanisms are implemented based on the JSON Schema Validation
specification. More information can be found at:

  http://json-schema.org/documentation.html

The implementation is strictly based on the latest specification found at
http://json-schema.org/latest/json-schema-validation.html. All features
specified in the specification are supported.


Example:

.. code-block:: cpp

  #include <string>
  #include <sneaker/json/json.h>
  #include <sneaker/json/json_schema.h>

  using namespace sneaker::json;

  const std::string json_str = "{"
    "\"name\": \"Tomiko Van\","
    "\"age\": 28,"
    "\"interests\": ["
      "music",
      "swimming,"
      "reading"
    "],"
    "\"married\": false,"
    "\"languages\": {"
      "\"Japanese\": \"fluent\","
      "\"Chinese\": \"beginner\","
      "\"English\": \"fluent\""
    "}"
  "}";

  const std::string schema_str = "{"
    "\"type\": \"object\","
    "\"properties\": {"
      "\"name\": {"
        "\"type\": \"string\","
        "\"maxLength\": 50"
      "},"
      "\"age\": {"
        "\"type\": \"number\","
        "\"minimum\": 0,"
        "\"maximum\": 120"
      "},"
      "\"married\": {"
        "\"type\": \"boolean\""
      "},"
      "\"interests\": {"
        "\"type\": \"array\","
        "\"uniqueItems\": ["
          "\"music\","
          "\"dancing\","
          "\"swimming\","
          "\"reading\""
        "]"
      "},"
      "\"languages\": {"
        "\"type\": \"object\""
      "}"
    "}"
  "}";

  JSON json = sneaker::json::parse(json_str);
  JSON schema = sneaker::json::parse(schema_str);

  sneaker::json::json_schema::validate(json, schema);


.. cpp:class:: sneaker::json::json_validation_error
---------------------------------------------------

  Error thrown when parsing an JSON schema validation fails.


.. cpp:function:: sneaker::json::json_schema::validate(const JSON&, const JSON&)
--------------------------------------------------------------------------------

  Interface for validating a JSON blob with a specified JSON schema. The first
  argument is the JSON blob to be validated, and the second argument is the
  JSON schema. The JSON schema passed in must be valid, as no validation
  is performed on the schema object itself, and an invalid schema will cause
  undefined behaviors during validation.

  If validation is successful, nothing happens. Otherwise an instance of
  `json_validation_error` is thrown.
