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

/* Unit tests for definitions defined in sneaker/json/json.h */

#include <cstring>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>
#include "../../include/testing/testing.h"
#include "../../include/json/json.h"
#include "../../include/json/json_parser.h"


using namespace sneaker::json;


class json_unittest : public ::testing::Test {};


TEST_F(json_unittest, TestEmptyInitialization)
{
  JSON json;

  ASSERT_EQ(std::string("null"), json.dump());
}

TEST_F(json_unittest, TestInitializationByList)
{
  std::list<int> l1 { 1, 2, 3 };
  std::vector<int> l2 { 1, 2, 3 };
  std::set<int> l3 { 1, 2, 3 };

  JSON j1(l1);
  JSON j2(l2);
  JSON j3(l3);

  ASSERT_EQ(j1, j2);
  ASSERT_EQ(j2, j3);
  ASSERT_EQ(j3, j1);
}

TEST_F(json_unittest, TestInitializationByMap)
{
  std::map<std::string, std::string> m1 { { "k1", "v1" }, { "k2", "v2" } };
  std::unordered_map<std::string, std::string> m2 { { "k1", "v1" }, { "k2", "v2" } };

  JSON j1(m1);
  JSON j2(m2);

  ASSERT_EQ(j1, j2);
}


class json_parse_unittest : public json_unittest {};


TEST_F(json_parse_unittest, TestParseSimpleJSON)
{
  const std::string str = "{"
    "\"k1\": \"v1\","
    "\"k2\": -42,"
    "\"k3\": [\"a\", 123, true, false, null]"
  "}";

  auto json = sneaker::json::parse(str);

  ASSERT_EQ(std::string("\"v1\""), json["k1"].dump());
  ASSERT_EQ(std::string("-42"), json["k2"].dump());
  ASSERT_EQ(std::string("[\"a\", 123, true, false, null]"), json["k3"].dump());

  ASSERT_EQ("v1", json["k1"].string_value());
  ASSERT_EQ(-42, json["k2"].number_value());
  ASSERT_EQ("a", json["k3"][0].string_value());
  ASSERT_EQ(123, json["k3"][1].number_value());
  ASSERT_EQ(true, json["k3"][2].bool_value());
  ASSERT_EQ(false, json["k3"][3].bool_value());
}

TEST_F(json_parse_unittest, TestParseUnicodeEscape)
{
  const std::string str =
    R"([ "blah\ud83d\udca9blah\ud83dblah\udca9blah\u0000blah\u1234" ])";

  const char utf8[] = "blah" "\xf0\x9f\x92\xa9" "blah" "\xed\xa0\xbd" "blah"
                      "\xed\xb2\xa9" "blah" "\0" "blah" "\xe1\x88\xb4";

  auto json = sneaker::json::parse(str);

  ASSERT_EQ((sizeof utf8) - 1, json[0].string_value().size());
  ASSERT_EQ(0, memcmp(json[0].string_value().data(), utf8, sizeof(utf8)));
}

TEST_F(json_parse_unittest, TestParseNonAsciiCharacters)
{
  // This test current fails.
  /*
  const std::string json_str = "[ありがとう]";
  auto json = sneaker::json::parse(json_str);

  ASSERT_EQ(json_str, json.dump());
  */
}


class json_parse_failure_unittest : public json_parse_unittest {
protected:
  void parse_and_assert_error(const std::string& invalid_json, const std::string& expected_err) {
    // High level parsing through `sneaker::json::parse()`.
    ASSERT_ANY_THROW(
      sneaker::json::parse(invalid_json);
    );

    // Because `sneaker::json::parse()` throws an exception on
    // invalid JSON, and the fact that we cannot use `ASSERT_THROW` due to
    // SNEAKER-53, the workaround to check the specific error message on
    // different JSON blobs is to use `sneaker::json::json_parser`.
    std::string err;
    sneaker::json::json_parser parser { invalid_json, 0, err, false };

    parser.parse_json();

    ASSERT_EQ(true, parser.failed);
    ASSERT_EQ(expected_err, parser.err);
  }
};


TEST_F(json_parse_failure_unittest, TestParseJsonWithoutOuterListOrObjectLayer)
{
  parse_and_assert_error("This is an invalid JSON.", "Expected value, got 'T' (84)");
}

TEST_F(json_parse_failure_unittest, TestParseJsonWithInvalidBooleanValue)
{
  parse_and_assert_error("[TRUE]", "Expected value, got 'T' (84)");
}

TEST_F(json_parse_failure_unittest, TestParseJsonWithMultipleAdjacentJsonObjects)
{
  parse_and_assert_error("{}{}{}", "Unexpected trailing '{' (123)");
}

TEST_F(json_parse_failure_unittest, TestParseJsonWithMismatchingBrackets)
{
  parse_and_assert_error("[{]]", "Expected '\"' in object, got ']' (93)");
}

TEST_F(json_parse_failure_unittest, TestParseJsonWithMissingColon)
{
  parse_and_assert_error("{\"hello\" \"world\"}", "Expected ':' in object, got '\"' (34)");
}

TEST_F(json_parse_failure_unittest, TestParseJsonWithTrailingWhitespaces)
{
  parse_and_assert_error("{[1,2,3]}    ", "Expected '\"' in object, got '[' (91)");
}

TEST_F(json_parse_failure_unittest, TestParseJsonWithMissingQuote)
{
  parse_and_assert_error("{\"hello: \"world\"}", "Expected ':' in object, got 'w' (119)");
}

TEST_F(json_parse_failure_unittest, TestParseJsonWithExtraQuote)
{
  parse_and_assert_error("{\"hello\": \"world\"\"}", "Expected ',' in object, got '\"' (34)");
}

TEST_F(json_parse_failure_unittest, TestParseJsonWithExtraCommaInArray)
{
  parse_and_assert_error("[1, 2, 3,]", "Expected value, got ']' (93)");
}

TEST_F(json_parse_failure_unittest, TestParseJsonWithInvalidNumber)
{
  parse_and_assert_error("[0.-00000]", "At least one digit required in fractional part");
}

TEST_F(json_parse_failure_unittest, TestParseJsonWithLeadingZeros)
{
  parse_and_assert_error("[00.01]", "Leading 0s not permitted in numbers"); 
}

TEST_F(json_parse_failure_unittest, TestParseJsonWithNoDigitInFraction)
{
  parse_and_assert_error("[0.]", "At least one digit required in fractional part");
}

TEST_F(json_parse_failure_unittest, TestParseJsonWithInvalidExponent)
{
  parse_and_assert_error("[0.0E]", "At least one digit required in exponent");
}

TEST_F(json_parse_failure_unittest, TestParseJsonWithInvalidEscapedString)
{
  parse_and_assert_error("[\"\\\"]", "Unexpected end of input in string");
}


class json_serialization_unittest : public json_unittest {
public:
  class Point {
    public:
      int x;
      int y;
      Point (int x, int y) : x(x), y(y) {}
      JSON to_json() const { return JSON::array { x, y }; }
  };

  class ComplexPoint : public Point {
    public:
      ComplexPoint(int x, int y) : Point(x, y) {}
      JSON to_json() const { return JSON::object { {"re", x}, {"im", y} }; }
  };
};


TEST_F(json_serialization_unittest, TestSimpleNumberSerialization)
{
  sneaker::json::JSON json(68);

  ASSERT_EQ(68, json.number_value());
  ASSERT_EQ(false, json.bool_value());
  ASSERT_EQ(std::string(""), json.string_value());
  ASSERT_EQ(std::string("68"), json.dump());
}

TEST_F(json_serialization_unittest, TestSimpleBooleanSerialization)
{
  sneaker::json::JSON json(true);

  ASSERT_EQ(0, json.number_value());
  ASSERT_EQ(true, json.bool_value());
  ASSERT_EQ(std::string(""), json.string_value());
  ASSERT_EQ(std::string("true"), json.dump());
}

TEST_F(json_serialization_unittest, TestSimpleNullptrSerialization)
{
  sneaker::json::JSON json = sneaker::json::JSON::null();

  ASSERT_EQ(0, json.number_value());
  ASSERT_EQ(false, json.bool_value());
  ASSERT_EQ(std::string(""), json.string_value());
  ASSERT_EQ(std::string("null"), json.dump());
}

TEST_F(json_serialization_unittest, TestSimpleStringSerialization)
{
  sneaker::json::JSON json("Hello world");

  ASSERT_EQ(0, json.number_value());
  ASSERT_EQ(false, json.bool_value());
  ASSERT_EQ(std::string("Hello world"), json.string_value());
  ASSERT_EQ(std::string("\"Hello world\""), json.dump());
}

TEST_F(json_serialization_unittest, TestSimpleArraySerialization)
{
  sneaker::json::JSON json = sneaker::json::JSON::array();

  ASSERT_EQ(0, json.number_value());
  ASSERT_EQ(false, json.bool_value());
  ASSERT_EQ(std::string(""), json.string_value());
  ASSERT_EQ(std::string("[]"), json.dump());
}

TEST_F(json_serialization_unittest, TestSimpleObjectSerialization)
{
  sneaker::json::JSON json = sneaker::json::JSON::object();

  ASSERT_EQ(0, json.number_value());
  ASSERT_EQ(false, json.bool_value());
  ASSERT_EQ(std::string(""), json.string_value());
  ASSERT_EQ(std::string("{}"), json.dump());
}

TEST_F(json_serialization_unittest, TestComplexSerialization)
{
  sneaker::json::JSON json = sneaker::json::JSON::object {
    { "key1", "value1" },
    { "key2", 123.456 },
    { "key3", false },
    { "key4", sneaker::json::JSON::array { 1, "a", true, nullptr } },
  };

  const std::string serialized = "{"
    "\"key1\": \"value1\", "
    "\"key2\": 123.456, "
    "\"key3\": false, "
    "\"key4\": [1, \"a\", true, null]"
  "}";

  ASSERT_EQ(serialized, json.dump());
}

TEST_F(json_serialization_unittest, TestClassObjectToJsonArraySerialization)
{
  std::vector<Point> points = { { 1, 2 }, { 10, 20 }, { 100, 200 } };
  
  std::string expected_json = "[[1, 2], [10, 20], [100, 200]]";
  std::string actual_json = JSON(points).dump();

  ASSERT_EQ(expected_json, actual_json);
}

TEST_F(json_serialization_unittest, TestClassObjectToJsonObjectSerialization)
{
  std::vector<ComplexPoint> points = { { 1, 2 }, { 10, 20 }, { 100, 200 } };

  std::string expected_json = "[{\"im\": 2, \"re\": 1}, {\"im\": 20, \"re\": 10}, {\"im\": 200, \"re\": 100}]";
  std::string actual_json = JSON(points).dump();

  ASSERT_EQ(expected_json, actual_json);
}
