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

/* Unit tests for definitions defined in sneaker/json/json_schema.h */

#include "json/json_schema.h"

#include "json/json.h"
#include "testing/testing.h"

#include <boost/format.hpp>

#include <cstring>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>


// -----------------------------------------------------------------------------

using boost::format;
using namespace sneaker::json;

// -----------------------------------------------------------------------------

class json_schema_unittest : public ::testing::Test {
protected:
  void validate_and_check_result(
    const JSON& data, const JSON& schema, const char* err=NULL)
  {
    if (err) {
      bool thrown = false;

      try {
        sneaker::json::json_schema::validate(data, schema);
      } catch (const sneaker::json::json_validation_error& e) {
        ASSERT_STREQ(err, e.what());
        thrown = true;
      }

      ASSERT_TRUE(thrown);
    } else {
      ASSERT_NO_THROW(sneaker::json::json_schema::validate(data, schema));
    }
  }
};

// -----------------------------------------------------------------------------

TEST_F(json_schema_unittest, TestEmptySchemaOnEmptyJSON)
{
  JSON data;
  JSON schema;

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

class json_schema_null_type_unittest : public json_schema_unittest {};

// -----------------------------------------------------------------------------

TEST_F(json_schema_null_type_unittest, TestValidationSuccessful)
{
  JSON data = sneaker::json::parse("[null]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"null\""
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

class json_schema_boolean_type_unittest : public json_schema_unittest {};

// -----------------------------------------------------------------------------

TEST_F(json_schema_boolean_type_unittest, TestValidationSuccessful)
{
  JSON data = sneaker::json::parse("[true]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"boolean\""
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

class json_schema_integer_type_unittest : public json_schema_unittest {};

// -----------------------------------------------------------------------------

TEST_F(json_schema_integer_type_unittest, TestValidationSuccessful)
{
  JSON data = sneaker::json::parse("[1]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"integer\""
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_integer_type_unittest, TestMultileOf)
{
  JSON data = sneaker::json::parse("[10]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"integer\","
          "\"multipleOf\": 5"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_integer_type_unittest, TestViolatingMultipleOf)
{
  JSON data = sneaker::json::parse("[10]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"integer\","
          "\"multipleOf\": 4"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Integer value 10 is not a multiple of 4"
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_integer_type_unittest, TestMaximum)
{
  JSON data = sneaker::json::parse("[10]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"integer\","
          "\"maximum\": 10"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_integer_type_unittest, TestViolatingMaximum)
{
  JSON data = sneaker::json::parse("[10]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"integer\","
          "\"maximum\": 9"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Integer value 10 exceeds the maximum value of 9"
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_integer_type_unittest, TestViolatingExclusiveMaximum)
{
  JSON data = sneaker::json::parse("[10]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"integer\","
          "\"maximum\": 10,"
          "\"exclusiveMaximum\": true"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Integer value 10 exceeds or equals to the maximum value of 10"
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_integer_type_unittest, TestMinimum)
{
  JSON data = sneaker::json::parse("[10]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"integer\","
          "\"minimum\": 10"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_integer_type_unittest, TestViolatingMinimum)
{
  JSON data = sneaker::json::parse("[10]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"integer\","
          "\"minimum\": 11"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Integer value 10 is less than the minimum value of 11"
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_integer_type_unittest, TestViolatingExclusiveMinimum)
{
  JSON data = sneaker::json::parse("[10]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"integer\","
          "\"minimum\": 10,"
          "\"exclusiveMinimum\": true"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Integer value 10 is less or equal to the minimum value of 10"
  );
}

// -----------------------------------------------------------------------------

class json_schema_string_type_unittest : public json_schema_unittest {};

// -----------------------------------------------------------------------------

TEST_F(json_schema_string_type_unittest, TestValidationSuccessful)
{
  JSON data = sneaker::json::parse("[\"Hello world\"]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"string\""
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_string_type_unittest, TestMaxLength)
{
  JSON data = sneaker::json::parse("[\"Hello world\"]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"string\","
          "\"maxLength\": 11"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_string_type_unittest, TestViolatingMaxLength)
{
  JSON data = sneaker::json::parse("[\"Hello world\"]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"string\","
          "\"maxLength\": 10"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "String Hello world exceeds maximum length of 10"
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_string_type_unittest, TestMinLength)
{
  JSON data = sneaker::json::parse("[\"Hello world\"]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"string\","
          "\"minLength\": 11"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_string_type_unittest, TestViolatingMinLength)
{
  JSON data = sneaker::json::parse("[\"Hello world\"]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"string\","
          "\"minLength\": 12"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "String \"Hello world\" does not meet the minimum length limit of 12"
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_string_type_unittest, TestPattern)
{
  JSON data = sneaker::json::parse("[\"subject\"]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"string\","
          "\"pattern\": \"(sub)(.*)\""
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_string_type_unittest, TestViolatingPatternMatching)
{
  JSON data = sneaker::json::parse("[\"Hello world\"]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"string\","
          "\"pattern\": \"(sub)(.*)\""
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "String \"Hello world\" does not match to regular expression (sub)(.*)"
  );
}

// -----------------------------------------------------------------------------

class json_schema_array_type_unittest : public json_schema_unittest {};

// -----------------------------------------------------------------------------

TEST_F(json_schema_array_type_unittest, TestValidationSuccessful)
{
  JSON data = sneaker::json::parse("[1, \"Hello world\", {}]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"integer\""
        "},"
        "{"
          "\"type\": \"string\""
        "},"
        "{"
          "\"type\": \"object\""
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_array_type_unittest, TestValidationFailed)
{
  JSON data = sneaker::json::parse("[1, \"Hello world\", {}]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"integer\""
        "},"
        "{"
          "\"type\": \"boolean\""
        "},"
        "{"
          "\"type\": \"object\""
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Invalid type for \"Hello world\""
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_array_type_unittest, TestValidationWithItemsBeingSchemaObject)
{
  JSON data = sneaker::json::parse("[1, 2, 3]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": {"
          "\"type\": \"integer\""
      "}"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_array_type_unittest, TestValidationFailedWithItemsBeingSchemaObject)
{
  JSON data = sneaker::json::parse("[1, 2, \"Hello world\"]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": {"
          "\"type\": \"integer\""
      "}"
    "}"
  );


  this->validate_and_check_result(
    data,
    schema,
    "Invalid type for \"Hello world\""
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_array_type_unittest, TestAdditionalItems)
{
  JSON data = sneaker::json::parse("[1, \"Hello world\", {}, false]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"integer\""
        "},"
        "{"
          "\"type\": \"string\""
        "},"
        "{"
          "\"type\": \"object\""
        "}"
      "],"
      "\"additionalItems\": true"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_array_type_unittest, TesViolatingAdditionalItems)
{
  JSON data = sneaker::json::parse("[1, \"Hello world\", {}, false]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"integer\""
        "},"
        "{"
          "\"type\": \"string\""
        "},"
        "{"
          "\"type\": \"object\""
        "}"
      "],"
      "\"additionalItems\": false"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Array [1, \"Hello world\", {}, false] contains extra items (3 items allowed)"
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_array_type_unittest, TestMaxItems)
{
  JSON data = sneaker::json::parse("[1, \"Hello world\", {}]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"integer\""
        "},"
        "{"
          "\"type\": \"string\""
        "},"
        "{"
          "\"type\": \"object\""
        "}"
      "],"
      "\"maxItems\": 3"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_array_type_unittest, TestViolatingMaxItems)
{
  JSON data = sneaker::json::parse("[1, \"Hello world\", {}]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"integer\""
        "},"
        "{"
          "\"type\": \"string\""
        "},"
        "{"
          "\"type\": \"object\""
        "}"
      "],"
      "\"maxItems\": 2"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Array [1, \"Hello world\", {}] exceeds the maximum number of items (2)"
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_array_type_unittest, TestMinItems)
{
  JSON data = sneaker::json::parse("[1, \"Hello world\", {}]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"integer\""
        "},"
        "{"
          "\"type\": \"string\""
        "},"
        "{"
          "\"type\": \"object\""
        "}"
      "],"
      "\"minItems\": 3"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_array_type_unittest, TestViolatingMinItems)
{
  JSON data = sneaker::json::parse("[1, \"Hello world\", {}]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"integer\""
        "},"
        "{"
          "\"type\": \"string\""
        "},"
        "{"
          "\"type\": \"object\""
        "}"
      "],"
      "\"minItems\": 4"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Array [1, \"Hello world\", {}] does not meet the minimum number of items (4)"
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_array_type_unittest, TestUniqueItems)
{
  JSON data = sneaker::json::parse("[1, \"Hello world\", {}]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"integer\""
        "},"
        "{"
          "\"type\": \"string\""
        "},"
        "{"
          "\"type\": \"object\""
        "}"
      "],"
      "\"uniqueItems\": true"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_array_type_unittest, TestViolatingUniqueItems)
{
  JSON data = sneaker::json::parse("[{}, {}, {}]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"object\""
        "},"
        "{"
          "\"type\": \"object\""
        "},"
        "{"
          "\"type\": \"object\""
        "}"
      "],"
      "\"uniqueItems\": true"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Array [{}, {}, {}] does not have unique items"
  );
}

// -----------------------------------------------------------------------------

class json_schema_object_type_unittest : public json_schema_unittest {};

// -----------------------------------------------------------------------------

TEST_F(json_schema_object_type_unittest, TestValidationSuccessful)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"name\": \"Tomiko Van\","
      "\"age\": 24,"
      "\"my favorite\": true"
    "}"
  );
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"name\": {"
          "\"type\": \"string\""
        "},"
        "\"age\": {"
          "\"type\": \"integer\""
        "},"
        "\"my favorite\": {"
          "\"type\": \"boolean\""
        "}"
      "}"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_object_type_unittest, TestValidationSuccessfulWithAdditionalProperties)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"name\": \"Tomiko Van\","
      "\"age\": 24,"
      "\"my favorite\": true,"
      "\"favorite color\": \"red\","
      "\"favorite food\": \"sushi\""
    "}"
  );
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"name\": {"
          "\"type\": \"string\""
        "},"
        "\"age\": {"
          "\"type\": \"integer\""
        "},"
        "\"my favorite\": {"
          "\"type\": \"boolean\""
        "}"
      "},"
      "\"additionalProperties\": true"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_object_type_unittest, TestValidationFailsWithAdditionalProperties)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"name\": \"Tomiko Van\","
      "\"age\": 24,"
      "\"my favorite\": true,"
      "\"favorite color\": \"red\","
      "\"favorite food\": \"sushi\""
    "}"
  );
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"name\": {"
          "\"type\": \"string\""
        "},"
        "\"age\": {"
          "\"type\": \"integer\""
        "},"
        "\"my favorite\": {"
          "\"type\": \"boolean\""
        "}"
      "},"
      "\"additionalProperties\": false"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Properties [favorite color, favorite food] are invalid in object {\"age\": 24, \"favorite color\": \"red\", \"favorite food\": \"sushi\", \"my favorite\": true, \"name\": \"Tomiko Van\"}"
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_object_type_unittest, TestValidationFailsWithAdditionalProperties2)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"name\": \"Tomiko Van\","
      "\"age\": 24,"
      "\"my favorite\": true,"
      "\"favorite color\": \"red\","
      "\"favorite food\": \"sushi\""
    "}"
  );
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"name\": {"
          "\"type\": \"string\""
        "},"
        "\"age\": {"
          "\"type\": \"integer\""
        "},"
        "\"my favorite\": {"
          "\"type\": \"boolean\""
        "},"
        "\"favorite season\": {"
          "\"type\": \"boolean\""
        "}"
      "},"
      "\"additionalProperties\": false"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Properties [favorite color, favorite food] are invalid in object {\"age\": 24, \"favorite color\": \"red\", \"favorite food\": \"sushi\", \"my favorite\": true, \"name\": \"Tomiko Van\"}"
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_object_type_unittest, TestValidationSuccessfulWithAdditionalPropertiesAndPatternProperties)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"name\": \"Tomiko Van\","
      "\"age\": 24,"
      "\"my favorite\": true,"
      "\"favorite color\": \"red\","
      "\"favorite number\": 8"
    "}"
  );
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"name\": {"
          "\"type\": \"string\""
        "},"
        "\"age\": {"
          "\"type\": \"integer\""
        "},"
        "\"my favorite\": {"
          "\"type\": \"boolean\""
        "}"
      "},"
      "\"additionalProperties\": false,"
      "\"patternProperties\": {"
        "\"(.*)(color)\": {"
          "\"type\": \"string\""
        "},"
        "\"(.*)(number)\": {"
          "\"type\": \"number\""
        "}"
      "}"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_object_type_unittest, TestValidationFailedWithInvalidField)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"name\": \"Tomiko Van\","
      "\"age\": 24,"
      "\"my favorite\": \"true\","
      "\"favorite color\": \"red\","
      "\"favorite number\": 8"
    "}"
  );
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"name\": {"
          "\"type\": \"string\""
        "},"
        "\"age\": {"
          "\"type\": \"integer\""
        "},"
        "\"my favorite\": {"
          "\"type\": \"boolean\""
        "}"
      "},"
      "\"additionalProperties\": false,"
      "\"patternProperties\": {"
        "\"(.*)(color)\": {"
          "\"type\": \"string\""
        "},"
        "\"(.*)(number)\": {"
          "\"type\": \"number\""
        "}"
      "}"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Invalid type for \"true\""
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_object_type_unittest, TestValidationFailedWithExtraProperty)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"name\": \"Tomiko Van\","
      "\"age\": 24,"
      "\"my favorite\": true,"
      "\"favorite color\": \"red\","
      "\"favorite number\": 8,"
      "\"favorite food\": \"sushi\""
    "}"
  );
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"name\": {"
          "\"type\": \"string\""
        "},"
        "\"age\": {"
          "\"type\": \"integer\""
        "},"
        "\"my favorite\": {"
          "\"type\": \"boolean\""
        "}"
      "},"
      "\"additionalProperties\": false,"
      "\"patternProperties\": {"
        "\"(.*)(color)\": {"
          "\"type\": \"string\""
        "},"
        "\"(.*)(number)\": {"
          "\"type\": \"number\""
        "}"
      "}"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Properties [favorite food] are invalid in object {\"age\": 24, \"favorite color\": \"red\", \"favorite food\": \"sushi\", \"favorite number\": 8, \"my favorite\": true, \"name\": \"Tomiko Van\"}"
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_object_type_unittest, TestValidationFailedWithInvalidRegexMatchedField)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"name\": \"Tomiko Van\","
      "\"age\": 24,"
      "\"my favorite\": true,"
      "\"favorite color\": \"red\","
      "\"favorite number\": \"eight\""
    "}"
  );
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"name\": {"
          "\"type\": \"string\""
        "},"
        "\"age\": {"
          "\"type\": \"integer\""
        "},"
        "\"my favorite\": {"
          "\"type\": \"boolean\""
        "}"
      "},"
      "\"additionalProperties\": false,"
      "\"patternProperties\": {"
        "\"(.*)(color)\": {"
          "\"type\": \"string\""
        "},"
        "\"(.*)(number)\": {"
          "\"type\": \"number\""
        "}"
      "}"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Invalid type for \"eight\""
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_object_type_unittest, TestMaxProperties)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"name\": \"Tomiko Van\","
      "\"age\": 24,"
      "\"my favorite\": true"
    "}"
  );
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"name\": {"
          "\"type\": \"string\""
        "},"
        "\"age\": {"
          "\"type\": \"integer\""
        "},"
        "\"my favorite\": {"
          "\"type\": \"boolean\""
        "}"
      "},"
      "\"maxProperties\": 3"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_object_type_unittest, TestViolatingMaxProperties)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"name\": \"Tomiko Van\","
      "\"age\": 24,"
      "\"my favorite\": true"
    "}"
  );
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"name\": {"
          "\"type\": \"string\""
        "},"
        "\"age\": {"
          "\"type\": \"integer\""
        "},"
        "\"my favorite\": {"
          "\"type\": \"boolean\""
        "}"
      "},"
      "\"maxProperties\": 2"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Object {\"age\": 24, \"my favorite\": true, \"name\": \"Tomiko Van\"} "
    "exceeds the maximum properties number of 2"
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_object_type_unittest, TestMinProperties)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"name\": \"Tomiko Van\","
      "\"age\": 24,"
      "\"my favorite\": true"
    "}"
  );
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"name\": {"
          "\"type\": \"string\""
        "},"
        "\"age\": {"
          "\"type\": \"integer\""
        "},"
        "\"my favorite\": {"
          "\"type\": \"boolean\""
        "}"
      "},"
      "\"minProperties\": 3"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_object_type_unittest, TestViolatingMinProperties)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"name\": \"Tomiko Van\","
      "\"age\": 24,"
      "\"my favorite\": true"
    "}"
  );
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"name\": {"
          "\"type\": \"string\""
        "},"
        "\"age\": {"
          "\"type\": \"integer\""
        "},"
        "\"my favorite\": {"
          "\"type\": \"boolean\""
        "}"
      "},"
      "\"minProperties\": 4"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Object {\"age\": 24, \"my favorite\": true, \"name\": \"Tomiko Van\"} "
    "does not meet the minimum properties number of 4"
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_object_type_unittest, TestRequired)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"name\": \"Tomiko Van\","
      "\"age\": 24,"
      "\"my favorite\": true"
    "}"
  );
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"name\": {"
          "\"type\": \"string\""
        "},"
        "\"age\": {"
          "\"type\": \"integer\""
        "},"
        "\"my favorite\": {"
          "\"type\": \"boolean\""
        "}"
      "},"
      "\"required\": ["
        "\"name\", \"age\""
      "]"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_object_type_unittest, TestMissingRequired)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"name\": \"Tomiko Van\","
      "\"age\": 24"
    "}"
  );
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"name\": {"
          "\"type\": \"string\""
        "},"
        "\"age\": {"
          "\"type\": \"integer\""
        "},"
        "\"my favorite\": {"
          "\"type\": \"boolean\""
        "}"
      "},"
      "\"required\": ["
        "\"name\", \"age\", \"my favorite\""
      "]"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Object {\"age\": 24, \"name\": \"Tomiko Van\"} "
    "does not have unique property \"my favorite\""
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_object_type_unittest, TestPropertyDependencies)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"name\": \"Tomiko Van\","
      "\"age\": 24"
    "}"
  );
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"name\": {"
          "\"type\": \"string\""
        "},"
        "\"age\": {"
          "\"type\": \"integer\""
        "}"
      "},"
      "\"dependencies\": {"
        "\"age\": ["
          "\"name\""
        "]"
      "}"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_object_type_unittest, TestMissingPropertyDependencies)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"age\": 24"
    "}"
  );
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"age\": {"
          "\"type\": \"integer\""
        "}"
      "},"
      "\"dependencies\": {"
        "\"age\": ["
          "\"name\""
        "]"
      "}"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Expected property name cannot be found in object {\"age\": 24}"
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_object_type_unittest, TestSchemaDependencies)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"name\": \"Tomiko Van\","
      "\"age\": 24"
    "}"
  );
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"age\": {"
          "\"type\": \"integer\""
        "}"
      "},"
      "\"dependencies\": {"
        "\"age\": {"
          "\"type\": \"object\","
          "\"properties\": {"
            "\"name\": {"
              "\"type\": \"string\""
            "}"
          "}"
        "}"
      "}"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_object_type_unittest, TestInvalidSchemaDependencies)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"name\": [\"Tomiko Van\"],"
      "\"age\": 24"
    "}"
  );
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"age\": {"
          "\"type\": \"integer\""
        "}"
      "},"
      "\"dependencies\": {"
        "\"age\": {"
          "\"type\": \"object\","
          "\"properties\": {"
            "\"name\": {"
              "\"type\": \"string\""
            "}"
          "}"
        "}"
      "}"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Invalid type for [\"Tomiko Van\"]"
  );
}

// -----------------------------------------------------------------------------

class json_schema_keyword_unittest : public json_schema_unittest {
protected:
  static const JSON data;
};

// -----------------------------------------------------------------------------

const JSON json_schema_keyword_unittest::data = sneaker::json::parse(
  "[\"Hello world\"]"
);

// -----------------------------------------------------------------------------

class json_schema_allOf_keyword_unittest : public json_schema_keyword_unittest {};

// -----------------------------------------------------------------------------

TEST_F(json_schema_allOf_keyword_unittest, TestValidationSuccessful)
{
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"allOf\": ["
            "{ \"type\": \"string\", \"maxLength\": 12 },"
            "{ \"type\": \"string\", \"minLength\": 8 }"
          "]"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(json_schema_keyword_unittest::data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_allOf_keyword_unittest, TestValidationFailed)
{
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"allOf\": ["
            "{ \"type\": \"string\", \"maxLength\": 100 },"
            "{ \"type\": \"string\", \"minLength\": 50 }"
          "]"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(
    json_schema_keyword_unittest::data,
    schema,
    "Object \"Hello world\" is not valid under all the sub-schemas"
  );
}

// -----------------------------------------------------------------------------

class json_schema_anyOf_keyword_unittest : public json_schema_keyword_unittest {};

// -----------------------------------------------------------------------------

TEST_F(json_schema_anyOf_keyword_unittest, TestValidationSuccessful)
{
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"anyOf\": ["
            "{ \"type\": \"number\" },"
            "{ \"type\": \"string\" },"
            "{ \"type\": \"array\" }"
          "]"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(json_schema_keyword_unittest::data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_anyOf_keyword_unittest, TestValidationFailed)
{
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"anyOf\": ["
            "{ \"type\": \"number\" },"
            "{ \"type\": \"object\" },"
            "{ \"type\": \"array\" }"
          "]"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(
    json_schema_keyword_unittest::data,
    schema,
    "Object \"Hello world\" is not valid under any sub-schemas"
  );
}

// -----------------------------------------------------------------------------

class json_schema_oneOf_keyword_unittest : public json_schema_keyword_unittest {};

// -----------------------------------------------------------------------------

TEST_F(json_schema_oneOf_keyword_unittest, TestValiationSuccessful)
{
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"oneOf\": ["
            "{ \"type\": \"string\" },"
            "{ \"type\": \"object\" },"
            "{ \"type\": \"array\" }"
          "]"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(json_schema_keyword_unittest::data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_oneOf_keyword_unittest, TestValiationFailedWithNoSubschemas)
{
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"oneOf\": []"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(
    json_schema_keyword_unittest::data,
    schema,
    "Object \"Hello world\" is invalid under less or more than one sub-schemas"
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_oneOf_keyword_unittest, TestValiationFailedWithNoMatchingSchemas)
{
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"oneOf\": ["
            "{ \"type\": \"number\" },"
            "{ \"type\": \"object\" },"
            "{ \"type\": \"array\" }"
          "]"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(
    json_schema_keyword_unittest::data,
    schema,
    "Object \"Hello world\" is invalid under less or more than one sub-schemas"
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_oneOf_keyword_unittest, TestValiationFailedWithAllMatchingSchemas)
{
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"oneOf\": ["
            "{ \"type\": \"string\" },"
            "{ \"type\": \"string\", \"maxLength\": 11 },"
            "{ \"type\": \"string\", \"minLength\": 11 }"
          "]"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(
    json_schema_keyword_unittest::data,
    schema,
    "Object \"Hello world\" is invalid under less or more than one sub-schemas"
  );
}

// -----------------------------------------------------------------------------

class json_schema_not_keyword_unittest : public json_schema_keyword_unittest {};

// -----------------------------------------------------------------------------

TEST_F(json_schema_not_keyword_unittest, TestValidationSuccessful)
{
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"not\": { \"type\": \"object\" }"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(json_schema_keyword_unittest::data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_not_keyword_unittest, TestValidationFailed)
{
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"not\": { \"type\": \"string\" }"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(
    json_schema_keyword_unittest::data,
    schema,
    "Object \"Hello world\" is valid under sub-schema {\"type\": \"string\"}, "
    "but it's not supposed to be"
  );
}

// -----------------------------------------------------------------------------

class json_schema_enum_keyword_unittest : public json_schema_keyword_unittest {};

// -----------------------------------------------------------------------------

TEST_F(json_schema_enum_keyword_unittest, TestValidationSuccessful)
{
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"string\","
          "\"enum\": ["
            "\"Sup world\","
            "\"Hello world\""
          "]"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(json_schema_keyword_unittest::data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_enum_keyword_unittest, TestValidationFailed)
{
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": ["
        "{"
          "\"type\": \"string\","
          "\"enum\": ["
            "\"Sup world\","
            "\"world hello\""
          "]"
        "}"
      "]"
    "}"
  );

  this->validate_and_check_result(
    json_schema_keyword_unittest::data,
    schema,
    "Object \"Hello world\" is invalid under the defined enum values"
  );
}

// -----------------------------------------------------------------------------

class json_schema_ref_unittest : public json_schema_unittest {
protected:
  static const JSON schema;
};

// -----------------------------------------------------------------------------

const JSON json_schema_ref_unittest::schema = sneaker::json::parse(
  "{"
    "\"type\": \"array\","
    "\"items\": {"
      "\"$ref\": \"#/definitions/positiveInteger\""
    "},"
    "\"definitions\": {"
      "\"positiveInteger\": {"
        "\"type\": \"integer\","
        "\"minimum\": 0,"
        "\"exclusiveMinimum\": true"
      "}"
    "}"
  "}"
);

// -----------------------------------------------------------------------------

TEST_F(json_schema_ref_unittest, TestValidationSuccessful)
{
  JSON data = sneaker::json::parse(
    "[1, 2, 3]"
  );

  this->validate_and_check_result(data, json_schema_ref_unittest::schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_ref_unittest, TestValidationFailedExpectedly)
{
  JSON data = sneaker::json::parse(
    "[1, 2, \"A\"]"
  );

  this->validate_and_check_result(
    data,
    json_schema_ref_unittest::schema,
    "Invalid type for \"A\""
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_ref_unittest, TestValidationFailsWithInvalidRefPath)
{
  JSON data = sneaker::json::parse(
    "[1, 2, 3]"
  );
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": {"
        "\"$ref\": \"#/definitions/helloWorld/positiveInteger\""
      "},"
      "\"definitions\": {"
        "\"positiveInteger\": {"
          "\"type\": \"integer\","
          "\"minimum\": 0,"
          "\"exclusiveMinimum\": true"
        "}"
      "}"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Invalid $ref path (sub path: helloWorld/positiveInteger)"
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_ref_unittest, TestRecursiveValidationSuccessful)
{
  JSON data = sneaker::json::parse(
    "["
        "{"
            "\"name\": \"Tomiko Van\","
            "\"inner\": {"
              "\"name\": \"Tomiko Van\""
            "}"
        "}"
    "]"
  );
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": {"
        "\"$ref\": \"#/definitions/tomiko\""
      "},"
      "\"definitions\": {"
        "\"tomiko\": {"
          "\"type\": \"object\","
          "\"properties\": {"
            "\"name\": {"
              "\"type\": \"string\""
            "},"
            "\"inner\": {"
              "\"$ref\": \"#/definitions/tomiko\""
            "}"
          "},"
          "\"required\": [\"name\"]"
        "}"
      "}"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_ref_unittest, TestRecursiveValidationFailed)
{
  JSON data = sneaker::json::parse(
    "["
        "{"
            "\"name\": \"Tomiko Van\","
            "\"inner\": {"
              "\"age\": \"25\""
            "}"
        "}"
    "]"
  );
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": {"
        "\"$ref\": \"#/definitions/tomiko\""
      "},"
      "\"definitions\": {"
        "\"tomiko\": {"
          "\"type\": \"object\","
          "\"properties\": {"
            "\"name\": {"
              "\"type\": \"string\""
            "},"
            "\"inner\": {"
              "\"$ref\": \"#/definitions/tomiko\""
            "}"
          "},"
          "\"required\": [\"name\"]"
        "}"
      "}"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Object {\"age\": \"25\"} does not have unique property \"name\""
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_ref_unittest, TestRecursiveDefinitionToTopLevel)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"inner\": {}"
    "}"
  );
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"inner\": {"
          "\"$ref\": \"#\""
        "}"
      "}"
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

class json_schema_semantic_format_unittest : public json_schema_unittest {};

// -----------------------------------------------------------------------------

class json_schema_datetime_format_unittest : public json_schema_semantic_format_unittest {
public:
  static const JSON schema;
};

// -----------------------------------------------------------------------------

const JSON json_schema_datetime_format_unittest::schema = sneaker::json::parse(
  "{"
    "\"type\": \"object\","
    "\"properties\": {"
      "\"data\": {"
        "\"type\": \"string\","
        "\"format\": \"date-time\""
      "}"
    "}"
  "}"
);

// -----------------------------------------------------------------------------

TEST_F(json_schema_datetime_format_unittest, TestValidDatetimeFormat)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"data\": \"2014-01-05T12:12:12\""
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_datetime_format_unittest, TestInvalidDatetimeFormat)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"data\": \"01/05/2014-12:12:12\""
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Invalid data in date-time format: \"01/05/2014-12:12:12\""
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_datetime_format_unittest, TestInvalidDataType)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"data\": []"
    "}"
  );

  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"data\": {"
          "\"type\": \"array\","
          "\"format\": \"date-time\""
        "}"
      "}"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Invalid semantic format \"date-time\" for data: []"
  );
}

// -----------------------------------------------------------------------------

class json_schema_email_format_unittest : public json_schema_semantic_format_unittest {
public:
  static const JSON schema;
};

// -----------------------------------------------------------------------------

const JSON json_schema_email_format_unittest::schema = sneaker::json::parse(
  "{"
    "\"type\": \"object\","
    "\"properties\": {"
      "\"data\": {"
        "\"type\": \"string\","
        "\"format\": \"email\""
      "}"
    "}"
  "}"
);

// -----------------------------------------------------------------------------

TEST_F(json_schema_email_format_unittest, TestValidEmailFormat)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"data\": \"yli@sneaker.org\""
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_email_format_unittest, TestInvalidEmailFormat)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"data\": \"Hello@@World.net\""
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Invalid data in email format: \"Hello@@World.net\""
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_email_format_unittest, TestInvalidDataType)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"data\": 123"
    "}"
  );

  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"data\": {"
          "\"type\": \"number\","
          "\"format\": \"email\""
        "}"
      "}"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Invalid semantic format \"email\" for data: 123"
  );
}

// -----------------------------------------------------------------------------

class json_schema_hostname_format_unittest : public json_schema_semantic_format_unittest {
public:
  static const JSON schema;
};

// -----------------------------------------------------------------------------

const JSON json_schema_hostname_format_unittest::schema = sneaker::json::parse(
  "{"
    "\"type\": \"object\","
    "\"properties\": {"
      "\"data\": {"
        "\"type\": \"string\","
        "\"format\": \"hostname\""
      "}"
    "}"
  "}"
);

// -----------------------------------------------------------------------------

TEST_F(json_schema_hostname_format_unittest, TestValidHostnameFormat)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"data\": \"hello-world.net\""
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_hostname_format_unittest, TestInvalidHostnameFormat)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"data\": \"mydomain@com\""
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Invalid data in hostname format: \"mydomain@com\""
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_hostname_format_unittest, TestInvalidDataType)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"data\": {}"
    "}"
  );

  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"data\": {"
          "\"type\": \"object\","
          "\"format\": \"hostname\""
        "}"
      "}"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Invalid semantic format \"hostname\" for data: {}"
  );
}

// -----------------------------------------------------------------------------

class json_schema_ipv4_format_unittest : public json_schema_semantic_format_unittest {
public:
  static const JSON schema;
};

// -----------------------------------------------------------------------------

const JSON json_schema_ipv4_format_unittest::schema = sneaker::json::parse(
  "{"
    "\"type\": \"object\","
    "\"properties\": {"
      "\"data\": {"
        "\"type\": \"string\","
        "\"format\": \"ipv4\""
      "}"
    "}"
  "}"
);

// -----------------------------------------------------------------------------

TEST_F(json_schema_ipv4_format_unittest, TestValidIpv4Format)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"data\": \"172.16.254.1\""
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_ipv4_format_unittest, TestInvalidIpv4Format)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"data\": \"2001:db8:0:1234:0:567:8:1\""
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Invalid data in ipv4 format: \"2001:db8:0:1234:0:567:8:1\""
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_ipv4_format_unittest, TestInvalidDataType)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"data\": true"
    "}"
  );

  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"data\": {"
          "\"type\": \"boolean\","
          "\"format\": \"ipv4\""
        "}"
      "}"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Invalid semantic format \"ipv4\" for data: true"
  );
}

// -----------------------------------------------------------------------------

class json_schema_ipv6_format_unittest : public json_schema_semantic_format_unittest {
public:
  static const JSON schema;
};

// -----------------------------------------------------------------------------

const JSON json_schema_ipv6_format_unittest::schema = sneaker::json::parse(
  "{"
    "\"type\": \"object\","
    "\"properties\": {"
      "\"data\": {"
        "\"type\": \"string\","
        "\"format\": \"ipv6\""
      "}"
    "}"
  "}"
);

// -----------------------------------------------------------------------------

TEST_F(json_schema_ipv6_format_unittest, TestValidIpv6Format)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"data\": \"2001:db8:0:1234:0:567:8:1\""
    "}"
  );

  this->validate_and_check_result(data, schema);
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_ipv6_format_unittest, TestInvalidIpv6Format)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"data\": \"172.16.254.1\""
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Invalid data in ipv6 format: \"172.16.254.1\""
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_ipv6_format_unittest, TestInvalidDataType)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"data\": false"
    "}"
  );

  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"data\": {"
          "\"type\": \"boolean\","
          "\"format\": \"ipv6\""
        "}"
      "}"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Invalid semantic format \"ipv6\" for data: false"
  );
}

// -----------------------------------------------------------------------------

class json_schema_uri_format_unittest : public json_schema_semantic_format_unittest {
public:
  static const JSON schema;
  static const std::list<std::string> VALID_URIs;
};

// -----------------------------------------------------------------------------

const JSON json_schema_uri_format_unittest::schema = sneaker::json::parse(
  "{"
    "\"type\": \"object\","
    "\"properties\": {"
      "\"data\": {"
        "\"type\": \"string\","
        "\"format\": \"uri\""
      "}"
    "}"
  "}"
);

// -----------------------------------------------------------------------------

const std::list<std::string> json_schema_uri_format_unittest::VALID_URIs {
  // Typical examples
  "http://www.ietf.org/rfc/rfc2396.txt",
  "ftp://ftp.is.co.za/rfc/rfc1808.txt",
  "ldap://[2001:db8::7]/c=GB?objectClass?one",
  "mailto:John.Doe@example.com",
  "news:comp.infosystems.www.servers.unix",
  "tel:+1-816-555-1212",
  "telnet://192.0.2.16:80/",
  "urn:oasis:names:specification:docbook:dtd:xml:4.1.2",
  // Normal examples
  "g:h",
  "http://a/b/c/g",
  "http://g",
  "http://a/b/c/d;p?y",
  "http://a/b/c/g?y",
  "http://a/b/c/d;p?q#s",
  "http://a/b/c/g#s",
  "http://a/b/c/g?y#s",
  "http://a/b/c/;x",
  "http://a/b/c/g;x",
  "http://a/b/c/g;x?y#s",
  "http://a/b/c/d;p?q",
  "http://a/b/c/",
  "http://a/b/",
  // Abnormal examples
  "http://a/b/c/g.",
  "http://a/b/c/.g",
  "http://a/b/c/g..",
  "http://a/b/c/..g",
  "http://a/b/c/g;x=1/y",
  "http://a/b/c/g?y/./x",
  "http://a/b/c/g?y/../x",
  "http://a/b/c/g#s/./x",
  "http://a/b/c/g#s/../x",
  "http:g",
};

// -----------------------------------------------------------------------------

TEST_F(json_schema_uri_format_unittest, TestValidURIFormat)
{
  const std::string data_shell = "{"
    "\"data\": \"%s\""
  "}";

  for (auto itr = VALID_URIs.cbegin(); itr != VALID_URIs.cend(); ++itr) {
    std::string uri = static_cast<std::string>(*itr);

    JSON data = sneaker::json::parse(
        str(format(data_shell) % uri)
    );

    this->validate_and_check_result(data, schema);
  }
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_uri_format_unittest, TestInvalidURIFormat)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"data\": \"hello world\""
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Invalid data in uri format: \"hello world\""
  );
}

// -----------------------------------------------------------------------------

TEST_F(json_schema_uri_format_unittest, TestInvalidDataType)
{
  JSON data = sneaker::json::parse(
    "{"
      "\"data\": {}"
    "}"
  );

  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"object\","
      "\"properties\": {"
        "\"data\": {"
          "\"type\": \"object\","
          "\"format\": \"uri\""
        "}"
      "}"
    "}"
  );

  this->validate_and_check_result(
    data,
    schema,
    "Invalid semantic format \"uri\" for data: {}"
  );
}

// -----------------------------------------------------------------------------
