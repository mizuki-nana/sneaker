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

/* This module provides an interface for validating JSON blobs using JSON
 * schemas.
 *
 * The validation mechanisms are implemented based on the JSON Schema Validation
 * specification. More information can be found at:
 *
 *  http://json-schema.org/documentation.html
 *
 * The current implementation does not support all the features mentioned in
 * the spec, although quite exhaustive. More features will be supported in
 * future versions.
 *
 * The implementation is strictly based on the latest specification found at
 * http://json-schema.org/latest/json-schema-validation.html. In the current
 * version, the following sections in the spec are not supported:
 *
 * 5.5.7 Validation keywords for any instance type - definitions
 * 6. Metadata keywords
 * 7. Semantic validation with "format"
 *
 *
 * Example:
 *
 *  #include <string>
 *  #include <sneaker/json/json.h>
 *  #include <sneaker/json/json_schema.h>
 *
 *  using namespace sneaker::json;
 *
 *  const std::string json_str = "{"
 *    "\"name\": \"Tomiko Van\","
 *    "\"age\": 28,"
 *    "\"interests\": ["
 *      "music",
 *      "swimming,"
 *      "reading"
 *    "],"
 *    "\"married\": false,"
 *    "\"languages\": {"
 *      "\"Japanese\": \"fluent\","
 *      "\"Chinese\": \"beginner\","
 *      "\"English\": \"fluent\""
 *    "}"
 *  "}";
 *
 *  const std::string schema_str = "{"
 *    "\"type\": \"object\","
 *    "\"properties\": {"
 *      "\"name\": {"
 *        "\"type\": \"string\","
 *        "\"maxLength\": 50"
 *      "},"
 *      "\"age\": {"
 *        "\"type\": \"number\","
 *        "\"minimum\": 0,"
 *        "\"maximum\": 120"
 *      "},"
 *      "\"married\": {"
 *        "\"type\": \"boolean\""
 *      "},"
 *      "\"interests\": {"
 *        "\"type\": \"array\","
 *        "\"uniqueItems\": ["
 *          "\"music\","
 *          "\"dancing\","
 *          "\"swimming\","
 *          "\"reading\""
 *        "]"
 *      "},"
 *      "\"languages\": {"
 *        "\"type\": \"object\""
 *      "}"
 *    "}"
 *  "}";
 *
 *  JSON json = sneaker::json::parse(json_str);
 *  JSON schema = sneaker::json::parse(schema_str);
 *
 *  sneaker::json::json_schema::validate(json, schema);
 */

#ifndef SNEAKER_JSON_SCHEMA_H_
#define SNEAKER_JSON_SCHEMA_H_

#include <stdexcept>
#include "json.h"


using namespace sneaker::json;


namespace sneaker {


namespace json {


class json_validation_error : public std::invalid_argument {
public:
  explicit json_validation_error(const std::string& what_arg):
    std::invalid_argument(what_arg) {}
  explicit json_validation_error(const char* what_arg):
    std::invalid_argument(what_arg) {}
};


class json_schema {
public:
  static void validate(const JSON&, const JSON&) throw(json_validation_error);
private:
  static void validate_allOf(const JSON&, const JSON::object&) throw(json_validation_error);
  static void validate_anyOf(const JSON&, const JSON::object&) throw(json_validation_error);
  static void validate_oneOf(const JSON&, const JSON::object&) throw(json_validation_error);
  static void validate_not(const JSON&, const JSON::object&) throw(json_validation_error);
  static void validate_enum(const JSON&, const JSON::object&) throw(json_validation_error);
  static void validate_definitions(const JSON&, const JSON::object&) throw(json_validation_error);
};


} /* end namespace json */


} /* end namespace sneaker */


#endif /* SNEAKER_JSON_SCHEMA_H_ */
