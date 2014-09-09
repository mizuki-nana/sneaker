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

/* Unit tests for definitions defined in sneaker/json/json_schema.h */

#include <cstring>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>
#include "../../include/testing/testing.h"
#include "../../include/json/json.h"
#include "../../include/json/json_schema.h"


using namespace sneaker::json;


class json_schema_unittest : public ::testing::Test {
protected:
  void validate_and_check_result(
    const JSON& data, const JSON& schema, const char* err=NULL
  ) {
    // TODO: once SNEAKER-53 is resolved, we can actually assert that `err`
    // is the expected error message.
    if(err) {
      ASSERT_ANY_THROW(sneaker::json::json_schema::validate(data, schema));
    } else {
      ASSERT_NO_THROW(sneaker::json::json_schema::validate(data, schema));
    }
  }
};


TEST_F(json_schema_unittest, TestEmptySchemaOnEmptyJSON)
{
  JSON data;
  JSON schema;

  this->validate_and_check_result(data, schema);
}


class json_schema_null_type_unittest : public json_schema_unittest {};


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


class json_schema_boolean_type_unittest : public json_schema_unittest {};


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


class json_schema_integer_type_unittest : public json_schema_unittest {};


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

  this->validate_and_check_result(data, schema, "");
}

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

  this->validate_and_check_result(data, schema, "");
}

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

  this->validate_and_check_result(data, schema, "");
}

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

  this->validate_and_check_result(data, schema, "");
}

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

  this->validate_and_check_result(data, schema, "");
}


class json_schema_string_type_unittest : public json_schema_unittest {};


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

  this->validate_and_check_result(data, schema, "");
}

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

  this->validate_and_check_result(data, schema, "");
}

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

  this->validate_and_check_result(data, schema, "");
}


class json_schema_array_type_unittest : public json_schema_unittest {};


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

  this->validate_and_check_result(data, schema, ""); 
}

TEST_F(json_schema_array_type_unittest, TestValidationWithItemsBeingSchemaObject)
{
  JSON data = sneaker::json::parse("[1, \"Hello world\", {}]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": {"
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
    "}"
  );

  this->validate_and_check_result(data, schema); 
}

TEST_F(json_schema_array_type_unittest, TestValidationFailedWithItemsBeingSchemaObject)
{
  JSON data = sneaker::json::parse("[1, \"Hello world\", {}]");
  JSON schema = sneaker::json::parse(
    "{"
      "\"type\": \"array\","
      "\"items\": {"
        "\"type\": \"array\","
        "\"items\": ["
          "{"
            "\"type\": \"integer\""
          "},"
          "{"
            "\"type\": \"number\""
          "},"
          "{"
            "\"type\": \"object\""
          "}"
        "]"
      "}"
    "}"
  );

  this->validate_and_check_result(data, schema, ""); 
}

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

  this->validate_and_check_result(data, schema, ""); 
}

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

  this->validate_and_check_result(data, schema, ""); 
}

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

  this->validate_and_check_result(data, schema, ""); 
}

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

  this->validate_and_check_result(data, schema, ""); 
}


class json_schema_object_type_unittest : public json_schema_unittest {};


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

  this->validate_and_check_result(data, schema, "");
}

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

  this->validate_and_check_result(data, schema, "");
}

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

  this->validate_and_check_result(data, schema, "");
}

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

  this->validate_and_check_result(data, schema, "");
}

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

  this->validate_and_check_result(data, schema, "");
}

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

  this->validate_and_check_result(data, schema, "");
}


class json_schema_keyword_unittest : public json_schema_unittest {
protected:
  static const JSON data;
};

const JSON json_schema_keyword_unittest::data = sneaker::json::parse(
  "[\"Hello world\"]"
);


class json_schema_allOf_keyword_unittest : public json_schema_keyword_unittest {};


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

  this->validate_and_check_result(json_schema_keyword_unittest::data, schema, "");
}


class json_schema_anyOf_keyword_unittest : public json_schema_keyword_unittest {};


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

  this->validate_and_check_result(json_schema_keyword_unittest::data, schema, "");
}


class json_schema_oneOf_keyword_unittest : public json_schema_keyword_unittest {};


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

  this->validate_and_check_result(json_schema_keyword_unittest::data, schema, "");
}

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

  this->validate_and_check_result(json_schema_keyword_unittest::data, schema, "");
}

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

  this->validate_and_check_result(json_schema_keyword_unittest::data, schema, "");
}


class json_schema_not_keyword_unittest : public json_schema_keyword_unittest {};


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

  this->validate_and_check_result(json_schema_keyword_unittest::data, schema, "");
}
