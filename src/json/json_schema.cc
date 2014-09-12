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
#include <cassert>
#include <cmath>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <boost/format.hpp>
#include "../../include/json/json_schema.h"


namespace sneaker {


namespace json {


/* Spec:
 * http://json-schema.org/latest/json-schema-core.html#anchor8
 */
enum json_type_enum {
  ARRAY,
  BOOLEAN,
  INTEGER,
  NUMBER,
  NUL,
  OBJECT,
  STRING
};


class json_primitive_type_validator {
public:
  virtual JSON::Type primitive_type() const = 0;

  virtual void validate(const JSON&, const JSON&) const = 0;
protected:
  void _validate_type(const JSON&) const;
};


class json_array_type_validator : public json_primitive_type_validator {
public:
  virtual JSON::Type primitive_type() const {
    return JSON::Type::ARRAY;
  }

  void validate(const JSON&, const JSON&) const;
private:
  void validate_additionalItems_and_items(const JSON::array&, const JSON::object&) const;
  void validate_maxItems(const JSON::array&, const JSON::object&) const;
  void validate_minItems(const JSON::array&, const JSON::object&) const;
  void validate_uniqueItems(const JSON::array&, const JSON::object&) const;
};

class json_boolean_type_validator : public json_primitive_type_validator {
public:
  virtual JSON::Type primitive_type() const {
    return JSON::Type::BOOL;
  }

  void validate(const JSON&, const JSON&) const;
};

class json_integer_type_validator : public json_primitive_type_validator {
public:
  virtual JSON::Type primitive_type() const {
    return JSON::Type::NUMBER;
  }

  void validate(const JSON&, const JSON&) const;
private:
  void validate_multipleOf(const int&, const JSON::object&) const;
  void validate_maximum_and_exclusiveMaximum(const int&, const JSON::object&) const;
  void validate_minimum_and_exclusiveMinimum(const int&, const JSON::object&) const;
};

class json_number_type_validator : public json_primitive_type_validator {
public:
  virtual JSON::Type primitive_type() const {
    return JSON::Type::NUMBER;
  }

  void validate(const JSON&, const JSON&) const;
private:
  void validate_multipleOf(const double&, const JSON::object&) const;
  void validate_maximum_and_exclusiveMaximum(const double&, const JSON::object&) const;
  void validate_minimum_and_exclusiveMinimum(const double&, const JSON::object&) const;
};

class json_null_type_validator : public json_primitive_type_validator {
public:
  virtual JSON::Type primitive_type() const {
    return JSON::Type::NUL;
  }

  void validate(const JSON&, const JSON&) const;
};

class json_object_type_validator : public json_primitive_type_validator {
public:
  virtual JSON::Type primitive_type() const {
    return JSON::Type::OBJECT;
  }

  void validate(const JSON&, const JSON&) const;
private:
  void validate_maxProperties(const JSON::object&, const JSON::object&) const;
  void validate_minProperties(const JSON::object&, const JSON::object&) const;
  void validate_required(const JSON::object&, const JSON::object&) const;
  void validate_properties(const JSON::object&, const JSON::object&) const;
  void validate_dependencies(const JSON::object&, const JSON::object&) const;
};

class json_string_type_validator : public json_primitive_type_validator {
public:
  virtual JSON::Type primitive_type() const {
    return JSON::Type::STRING;
  }

  void validate(const JSON&, const JSON&) const;
private:
  void validate_maxLength(const JSON::string&, const JSON::object&) const;
  void validate_minLength(const JSON::string&, const JSON::object&) const;
  void validate_pattern(const JSON::string&, const JSON::object&) const;
};


struct json_validator_wrapper {
  const json_primitive_type_validator* validator;
};


class json_validator_meta {
public:
  using map_type = std::unordered_map<std::string, json_validator_wrapper>;  

  static const json_primitive_type_validator* get_validator(const std::string&);
private:
  static const map_type type_to_validator_map;
};

const json_validator_meta::map_type json_validator_meta::type_to_validator_map {
  {"array", {.validator=new json_array_type_validator()}},
  {"boolean", {.validator=new json_boolean_type_validator()}},
  {"integer", {.validator=new json_integer_type_validator()}},
  {"number", {.validator=new json_number_type_validator()}},
  {"null", {.validator=new json_null_type_validator()}},
  {"object", {.validator=new json_object_type_validator()}},
  {"string", {.validator=new json_string_type_validator()}},
};


} /* end namespace json */


} /* end namespace sneaker */


using boost::format;
using namespace sneaker::json;


const json_primitive_type_validator*
sneaker::json::json_validator_meta::get_validator(const std::string& type)
{
  if(type_to_validator_map.find(type) == type_to_validator_map.end()) {
    throw std::runtime_error(
      str(format("INVALID JSON PRIMITIVE TYPE MET: %s") % type)
    );
  }
  return type_to_validator_map.at(type).validator;    
}

void
sneaker::json::json_schema::validate(const JSON& data, const JSON& schema)
  throw(json_validation_error)
{
  const JSON::object& schema_object = schema.object_items();

  if(schema_object.find("allOf") != schema_object.end()) {
    sneaker::json::json_schema::validate_allOf(data, schema_object);
  } else if(schema_object.find("anyOf") != schema_object.end()) {
    sneaker::json::json_schema::validate_anyOf(data, schema_object);
  } else if(schema_object.find("oneOf") != schema_object.end()) {
    sneaker::json::json_schema::validate_oneOf(data, schema_object);
  } else if(schema_object.find("not") != schema_object.end()) {
    sneaker::json::json_schema::validate_not(data, schema_object);
  }

  if(schema_object.find("type") == schema_object.end()) {
    return;
  }

  const JSON::string& type = schema_object.at("type").string_value();
  std::string type_str = static_cast<std::string>(type);

  const json_primitive_type_validator* validator = json_validator_meta::get_validator(type);
  validator->validate(data, schema);
}

void
sneaker::json::json_schema::validate_allOf(
  const JSON& data, const JSON::object& schema_object) throw(json_validation_error)
{
  const JSON::array& schemas = schema_object.at("allOf").array_items(); 

  int valid_count = 0;
  for(auto itr = schemas.begin(); itr != schemas.end(); itr++) {
    const JSON& schema = static_cast<const JSON>(*itr);

    try {
      sneaker::json::json_schema::validate(data, schema);
      valid_count++;
    } catch (...) { // TODO: catch json_validation_error
      // Do nothing here.
    }
  }

  if(valid_count != schemas.size()) {
    throw json_validation_error(
      str(
        format("Object %s is not valid under all the sub-schemas") % data.dump()
      )
    );
  }
}

void
sneaker::json::json_schema::validate_anyOf(
  const JSON& data, const JSON::object& schema_object) throw(json_validation_error)
{
  const JSON::array& schemas = schema_object.at("anyOf").array_items(); 

  bool valid = false;
  for(auto itr = schemas.begin(); itr != schemas.end(); itr++) {
    const JSON& schema = static_cast<const JSON>(*itr);

    try {
      sneaker::json::json_schema::validate(data, schema);
      valid = true;
      break;
    } catch (...) { // TODO: catch json_validation_error
      // Do nothing here. 
    }
  }

  if(!valid) {
    throw json_validation_error(
      str(
        format(
          "Object %s is not valid under any sub-schemas"
        ) % data.dump()
      )
    );
  }
}

void
sneaker::json::json_schema::validate_oneOf(
  const JSON& data, const JSON::object& schema_object) throw(json_validation_error)
{
  const JSON::array& schemas = schema_object.at("oneOf").array_items(); 

  int valid_count = 0;
  for(auto itr = schemas.begin(); itr != schemas.end(); itr++) {
    const JSON& schema = static_cast<const JSON>(*itr);

    try {
      sneaker::json::json_schema::validate(data, schema);
      valid_count++;
    } catch (...) { // TODO: catch json_validation_error
      // Do nothing here.
    }
  }

  if(valid_count != 1) {
    throw json_validation_error(
      str(
        format(
          "Object %s is valid under less or more than one sub-schemas"
        ) % data.dump()
      )
    );
  }
} 

void
sneaker::json::json_schema::validate_not(
  const JSON& data, const JSON::object& schema_object) throw(json_validation_error)
{
  const JSON::object& schema = schema_object.at("not").object_items(); 

  bool valid = false;
  try {
    sneaker::json::json_schema::validate(data, schema);
    valid = true;
  } catch (...) { // TODO: catch json_validation_error
    // Do nothing here.
  }

  if(valid) {
    throw json_validation_error(
      str(
        format(
          "Object %s is valid under sub-schema %s, but it's not supposed to be"
        ) % data.dump() % JSON(schema).dump()
      )
    );
  }
} 

void
sneaker::json::json_schema::validate_enum(
  const JSON& data, const JSON::object& schema_object) throw(json_validation_error)
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor76
   *
   * TODO|SNEAKER-56: to be implemented.
   */
}

void
sneaker::json::json_schema::validate_definitions(
  const JSON& data, const JSON::object& schema_object) throw(json_validation_error)
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor94
   *
   * TODO|SNEAKER-56: to be implemented.
   */
}

void
sneaker::json::json_primitive_type_validator::_validate_type(const JSON& data) const
{
  if(data.type() != this->primitive_type()) {
    throw json_validation_error(
      str(format("Invalid type for %s") % data.dump())
    );
  }
}

/************************* json_array_type_validator **************************/

void
sneaker::json::json_array_type_validator::validate(
  const JSON& data, const JSON& schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor36
   */
  this->_validate_type(data);

  const JSON::array& json_array = data.array_items();
  const JSON::object& schema_object = const_cast<const JSON::object&>(schema.object_items());

  this->validate_additionalItems_and_items(json_array, schema_object);
  this->validate_maxItems(json_array, schema_object);
  this->validate_minItems(json_array, schema_object);
  this->validate_uniqueItems(json_array, schema_object);
}

void
sneaker::json::json_array_type_validator::validate_additionalItems_and_items(
  const JSON::array& json_array, const JSON::object& schema_object) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor37
   */
  if(schema_object.find("items") == schema_object.end()) {
    return;
  }

  const JSON& inner = schema_object.at("items");

  // Case if the value of `items` is an object.
  if(inner.is_object()) {
    sneaker::json::json_schema::validate(json_array, inner); 
    return;
  }

  // The value of `items` can only be an array at this point.
  assert(inner.is_array());

  const JSON::array& items = schema_object.at("items").array_items();

  if(schema_object.find("additionalItems") != schema_object.end()) {
    const JSON& inner = schema_object.at("additionalItems");

    if(inner.type() == JSON::Type::BOOL) {
      bool additional_items_boolean = inner.bool_value();

      if(!additional_items_boolean && json_array.size() > items.size()) {
        throw json_validation_error(
          str(
            format(
              "Array %s contains extra items (%d items allowed)"
            ) % JSON(json_array).dump() % items.size()
          )
        );
      }

    } else if(inner.type() == JSON::Type::OBJECT) {
      const JSON::object& additional_items_object = inner.object_items();

      if(additional_items_object.empty() && json_array.size() > items.size()) {
        throw json_validation_error(
          str(
            format(
              "Array %s contains extra items (%d items allowed)"
            ) % JSON(json_array).dump() % items.size()
          )
        );
      }
    }
  }

  for(int i = 0; i < json_array.size(); ++i) {
    if(i >= items.size()) {
      break;
    }

    const JSON& child = json_array.at(i);
    const JSON& child_schema = items.at(i);

    sneaker::json::json_schema::validate(child, child_schema);
  }
}

void
sneaker::json::json_array_type_validator::validate_maxItems(
  const JSON::array& json_array, const JSON::object& schema_object) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor42
   */
  if(schema_object.find("maxItems") == schema_object.end()) {
    return;
  }

  int max_items = schema_object.at("maxItems").int_value();

  if(json_array.size() > max_items) {
    throw json_validation_error(
      str(
        format(
          "Array %s exceeds the maximum number of items (%d)"
        ) % JSON(json_array).dump() % max_items
      )
    );
  }
}

void
sneaker::json::json_array_type_validator::validate_minItems(
  const JSON::array& json_array, const JSON::object& schema_object) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor45
   */
  if(schema_object.find("minItems") == schema_object.end()) {
    return;
  }

  int min_items = schema_object.at("minItems").int_value();

  if(json_array.size() < min_items) {
    throw json_validation_error(
      str(
        format(
          "Array %s does not meet the minimum number of items (%d)"
        ) % JSON(json_array).dump() % min_items
      )
    );
  }
}

void
sneaker::json::json_array_type_validator::validate_uniqueItems(
  const JSON::array& json_array, const JSON::object& schema_object) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor49
   */
  if(schema_object.find("uniqueItems") == schema_object.end()) {
    return; 
  }

  if(json_array.empty()) {
    return;
  }

  bool unique_items = schema_object.at("uniqueItems").bool_value();

  std::set<JSON> elements;
  elements.insert(json_array.begin(), json_array.end());

  if(unique_items && !json_array.empty() && json_array.size() != elements.size()) {
    throw json_validation_error(
      str(
        format("Array %s does not have unique items") % JSON(json_array).dump()
      )
    );
  }
}

void
sneaker::json::json_boolean_type_validator::validate(
  const JSON& data, const JSON& schema) const
{
  this->_validate_type(data);
}

/************************ json_integer_type_validator *************************/

void
sneaker::json::json_integer_type_validator::validate(
  const JSON& data, const JSON& schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor13
   */
  this->_validate_type(data);

  int int_value = data.int_value();
  const JSON::object& schema_object = const_cast<const JSON::object&>(schema.object_items());

  this->validate_multipleOf(int_value, schema_object);
  this->validate_maximum_and_exclusiveMaximum(int_value, schema_object);
  this->validate_minimum_and_exclusiveMinimum(int_value, schema_object);
}

void
sneaker::json::json_integer_type_validator::validate_multipleOf(
  const int& int_value, const JSON::object& schema_object) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor14
   */
  if(schema_object.find("multipleOf") == schema_object.end()) {
    return;
  }

  int multiple_of_value = schema_object.at("multipleOf").int_value();

  int remainder = int_value % multiple_of_value;

  if(remainder != 0) {
    throw json_validation_error(
      str(
        format(
          "Integer value %d is not a multiple of %d"
        ) % int_value % multiple_of_value
      )
    );
  }
}

void
sneaker::json::json_integer_type_validator::validate_maximum_and_exclusiveMaximum(
  const int& int_value, const JSON::object& schema_object) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor17
   */
  if(schema_object.find("maximum") == schema_object.end()) {
    return; 
  }

  bool exclusiveMaximum = false;
  if(schema_object.find("exclusiveMaximum") != schema_object.end()) {
    exclusiveMaximum = schema_object.at("exclusiveMaximum").bool_value();
  }

  int maximum_value = schema_object.at("maximum").int_value();

  if(int_value > maximum_value) {
    throw json_validation_error(
      str(
        format(
          "Integer value %d exceeds the maximum value of %d"
        ) % int_value % maximum_value
      )
    );
  } else if(exclusiveMaximum && int_value == maximum_value) {
    throw json_validation_error(
      str(
        format(
          "Integer value %d exceeds or equals to the maximum value of %d"
        ) % int_value % maximum_value
      )
    );
  }
}

void
sneaker::json::json_integer_type_validator::validate_minimum_and_exclusiveMinimum(
  const int& int_value, const JSON::object& schema_object) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor21
   */
  if(schema_object.find("minimum") == schema_object.end()) {
    return;
  }

  bool exclusiveMinimum = false;
  if(schema_object.find("exclusiveMinimum") != schema_object.end()) {
    exclusiveMinimum = schema_object.at("exclusiveMinimum").bool_value();
  }

  int minimum_value = schema_object.at("minimum").int_value();

  if(int_value < minimum_value) {
    throw json_validation_error(
      str(
        format(
          "Integer value %d is less than the minimum value of %d"
        ) % int_value % minimum_value
      )
    );
  } else if(exclusiveMinimum && int_value == minimum_value) {
    throw json_validation_error(
      str(
        format(
          "Integer value %d is less or equal to the minimum value of %d"
        ) % int_value % minimum_value
      )
    );
  }
}

/************************* json_number_type_validator *************************/

void
sneaker::json::json_number_type_validator::validate(
  const JSON& data, const JSON& schema) const
{
  this->_validate_type(data);

  double number_value = data.number_value();
  const JSON::object& schema_object = const_cast<const JSON::object&>(schema.object_items());

  this->validate_multipleOf(number_value, schema_object);
  this->validate_maximum_and_exclusiveMaximum(number_value, schema_object);
  this->validate_minimum_and_exclusiveMinimum(number_value, schema_object);
}

void
sneaker::json::json_number_type_validator::validate_multipleOf(
  const double& number_value, const JSON::object& schema_object) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor14
   */
  if(schema_object.find("multipleOf") == schema_object.end()) {
    return;
  }

  double multiple_of_value = schema_object.at("multipleOf").number_value();

  double remainder = fmod(number_value, multiple_of_value);

  if(remainder != 0.0) {
    throw json_validation_error(
      str(
        format(
          "Number value %f is not a multiple of %f"
        ) % number_value % multiple_of_value
      )
    );
  }
}

void
sneaker::json::json_number_type_validator::validate_maximum_and_exclusiveMaximum(
  const double& number_value, const JSON::object& schema_object) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor17
   */
  if(schema_object.find("maximum") == schema_object.end()) {
    return; 
  }

  bool exclusiveMaximum = false;
  if(schema_object.find("exclusiveMaximum") != schema_object.end()) {
    exclusiveMaximum = schema_object.at("exclusiveMaximum").bool_value();
  }

  double maximum_value = schema_object.at("maximum").number_value();

  if(number_value > maximum_value) {
    throw json_validation_error(
      str(
        format(
          "Number value %f exceeds the maximum value of %f"
        ) % number_value % maximum_value
      )
    );
  } else if(exclusiveMaximum && number_value == maximum_value) {
    throw json_validation_error(
      str(
        format(
          "Number value %f exceeds or equals to the maximum value of %f"
        ) % number_value % maximum_value
      )
    );
  }
}

void
sneaker::json::json_number_type_validator::validate_minimum_and_exclusiveMinimum(
  const double& number_value, const JSON::object& schema_object) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor21
   */
  if(schema_object.find("minimum") == schema_object.end()) {
    return;
  }

  bool exclusiveMinimum = false;
  if(schema_object.find("exclusiveMinimum") != schema_object.end()) {
    exclusiveMinimum = schema_object.at("exclusiveMinimum").bool_value();
  }

  double minimum_value = schema_object.at("minimum").number_value();

  if(number_value < minimum_value) {
    throw json_validation_error(
      str(
        format(
          "Number value %f is less than the minimum value of %f"
        ) % number_value % minimum_value
      )
    );
  } else if(exclusiveMinimum && number_value == minimum_value) {
    throw json_validation_error(
      str(
        format(
          "Number value %f is less or equal to the minimum value of %f"
        ) % number_value % minimum_value
      )
    );
  }
}

/************************** json_null_type_validator **************************/

void
sneaker::json::json_null_type_validator::validate(
  const JSON& data, const JSON& schema) const
{
  this->_validate_type(data);
}

/************************* json_object_type_validator *************************/

void
sneaker::json::json_object_type_validator::validate(
  const JSON& data, const JSON& schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor53
   */
  this->_validate_type(data);

  const JSON::object& object_value = data.object_items();
  const JSON::object& schema_object = schema.object_items();

  this->validate_maxProperties(object_value, schema_object);
  this->validate_minProperties(object_value, schema_object);
  this->validate_required(object_value, schema_object);
  this->validate_properties(object_value, schema_object);
  this->validate_dependencies(object_value, schema_object);
}

void
sneaker::json::json_object_type_validator::validate_maxProperties(
  const JSON::object& object_value, const JSON::object& schema_object) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor54
   */
  if(schema_object.find("maxProperties") == schema_object.end()) {
    return;
  }

  int max_properties = schema_object.at("maxProperties").int_value();

  if(object_value.size() > max_properties) {
    throw json_validation_error(
      str(
        format(
          "Object %s exceeds the maximum properties number of %d"
        ) % JSON(object_value).dump() % max_properties
      )
    );
  }
}

void
sneaker::json::json_object_type_validator::validate_minProperties(
  const JSON::object& object_value, const JSON::object& schema_object) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor57
   */
  if(schema_object.find("minProperties") == schema_object.end()) {
    return;
  }

  int min_properties = schema_object.at("minProperties").int_value();

  if(object_value.size() < min_properties) {
    throw json_validation_error(
      str(
        format(
          "Object %s does not meet the minimum properties number of %d"
        ) % JSON(object_value).dump() % min_properties
      )
    );
  }
}

void
sneaker::json::json_object_type_validator::validate_required(
  const JSON::object& object_value, const JSON::object& schema_object) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor61
   */
  if(schema_object.find("required") == schema_object.end()) {
    return;
  }

  const JSON::array& json_array = schema_object.at("required").array_items();

  for(auto itr = json_array.begin(); itr != json_array.end(); itr++) {
    const JSON& array_value = static_cast<JSON>(*itr);
    const JSON::string& unique = array_value.string_value();

    std::string unique_value = static_cast<std::string>(unique);

    if(object_value.find(unique_value) == object_value.end()) {
      throw json_validation_error(
        str(
          format(
            "Object %s does not have unique property %s"
          ) % JSON(object_value).dump() % unique_value
        )
      );
    }
  }
}

void
sneaker::json::json_object_type_validator::validate_properties(
  const JSON::object& object_value, const JSON::object& schema_object) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor64
   */
  if(schema_object.find("properties") == schema_object.end()) {
    return;
  }

  const JSON::object& properties = schema_object.at("properties").object_items();

  bool hasAdditionalProperties = false;
  bool hasPatternProperties = false;
  bool do_normal_validation = true;

  if(schema_object.find("additionalProperties") != schema_object.end()) {
    hasAdditionalProperties = true;  
  } else if(schema_object.find("patternProperties") != schema_object.end()) {
    hasPatternProperties = true;
  }

  if(hasAdditionalProperties) {
    const JSON& inner = schema_object.at("additionalProperties");
    if(inner.is_bool() && inner.bool_value() == false) {
      do_normal_validation = false;
    }
  }

  if(do_normal_validation) {
    for(auto itr = properties.begin(); itr != properties.end(); itr++) {
      std::string property = static_cast<std::string>(itr->first);

      if(object_value.find(property) == object_value.end()) {
        throw json_validation_error(
          str(
            format(
              "Property %s not found in object %s"
            ) % property % JSON(object_value).dump()
          )
        );
      }

      const JSON& child = object_value.at(property);
      const JSON& property_schema = static_cast<JSON>(itr->second);

      sneaker::json::json_schema::validate(child, property_schema);
    }
  } else {
    const JSON::object& pattern_properties = schema_object.at(
      "patternProperties"
    ).object_items();

    std::set<std::string> properties_set;

    for(auto itr = object_value.begin(); itr != object_value.end(); itr++) {
      std::string property = static_cast<std::string>(itr->first);
      properties_set.insert(property);
    }

    for(auto itr = properties.begin(); itr != properties.end(); itr++) {
      std::string property = static_cast<std::string>(itr->first);

      const JSON& child = object_value.at(property);
      const JSON& property_schema = static_cast<JSON>(itr->second);

      sneaker::json::json_schema::validate(child, property_schema);

      properties_set.erase(property);
    }

    for(auto itr = pattern_properties.begin(); itr != pattern_properties.end(); itr++) {
      std::string property_pattern = static_cast<std::string>(itr->first);
      std::regex property_pattern_regex(property_pattern);
      
      for(auto itr_ = properties_set.begin(); itr_ != properties_set.end(); itr_++) {
        std::string property = static_cast<std::string>(*itr_);

        std::smatch sm;
        std::regex_match(property, sm, property_pattern_regex);

        if(sm.empty()) {
          continue;
        }

        const JSON& child = object_value.at(property);
        const JSON& property_schema = static_cast<JSON>(itr->second);    

        sneaker::json::json_schema::validate(child, property_schema);
        properties_set.erase(property);
      }
    }

    if(!properties_set.empty()) {
      throw json_validation_error(
        str(
          format(
            "There are %d properties cannot be validated in object %s"
          ) % properties_set.size() % JSON(object_value).dump()
        )
      );
    }
  }
}

void
sneaker::json::json_object_type_validator::validate_dependencies(
  const JSON::object& object_value, const JSON::object& schema_object) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor70
   */
  // TODO|SNEAKER-56: to be implemented...
}

/************************* json_string_type_validator *************************/

void
sneaker::json::json_string_type_validator::validate(
  const JSON& data, const JSON& schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor25
   */
  this->_validate_type(data);

  const JSON::string& string_value = data.string_value();
  const JSON::object& schema_object = schema.object_items();

  this->validate_maxLength(string_value, schema_object);
  this->validate_minLength(string_value, schema_object);
  this->validate_pattern(string_value, schema_object);
}

void
sneaker::json::json_string_type_validator::validate_maxLength(
  const JSON::string& string_value, const JSON::object& schema_object) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor26
   */
  if(schema_object.find("maxLength") == schema_object.end()) {
    return;
  }

  int max_length = schema_object.at("maxLength").int_value();

  if(string_value.size() > max_length) {
    throw json_validation_error(
      str(
        format(
          "String %s exceeds maximum length of %d"
        ) % string_value % max_length
      )
    );
  }
}

void
sneaker::json::json_string_type_validator::validate_minLength(
  const JSON::string& string_value, const JSON::object& schema_object) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor29
   */
  if(schema_object.find("minLength") == schema_object.end()) {
    return;
  }

  int min_length = schema_object.at("minLength").int_value();

  if(string_value.size() < min_length) {
    throw json_validation_error(
      str(
        format(
          "String %s does not meet the minimum length limit of %d"
        ) % string_value % min_length
      )
    );
  }
}

void
sneaker::json::json_string_type_validator::validate_pattern(
  const JSON::string& string_value, const JSON::object& schema_object) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor33
   */
  if(schema_object.find("pattern") == schema_object.end()) {
    return;
  }

  const JSON::string& pattern_value = schema_object.at("pattern").string_value();

  std::smatch sm;
  std::string str_to_match = static_cast<std::string>(string_value);
  std::string pattern_str = static_cast<std::string>(pattern_value);
  std::regex regex(pattern_str, std::regex::ECMAScript);

  std::regex_match(str_to_match, sm, regex);

  if(sm.empty()) {
    throw json_validation_error(
      str(
        format(
          "String %s does not match to regular expression %s"
        ) % string_value % pattern_str
      )
    );
  }
}