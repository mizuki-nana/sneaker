/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2015 Yanzheng Li

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
#include "../../include/json/json_schema.h"

#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <boost/regex.hpp>
#include <boost/system/error_code.hpp>

#include <cassert>
#include <cmath>
#include <ctime>
#include <list>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>


namespace sneaker {


namespace json {


namespace json_schema_internal {


// -----------------------------------------------------------------------------

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

// -----------------------------------------------------------------------------

class json_primitive_type_validator {
public:
  virtual JSON::Type primitive_type() const = 0;

  virtual void validate(const JSON&, const JSON&, const JSON::object&) const = 0;
protected:
  void validate(const JSON&, const JSON&) const;

  void validate_type(const JSON&) const;
  void validate_semantic_format(const JSON&, const JSON&) const;
};

// -----------------------------------------------------------------------------

class json_array_type_validator : public json_primitive_type_validator {
public:
  virtual JSON::Type primitive_type() const {
    return JSON::Type::ARRAY;
  }

  void validate(const JSON&, const JSON&, const JSON::object&) const;

private:
  void validate_additionalItems_and_items(
    const JSON::array&, const JSON::object&, const JSON::object&) const;

  void validate_maxItems(
    const JSON::array&, const JSON::object&, const JSON::object&) const;

  void validate_minItems(
    const JSON::array&, const JSON::object&, const JSON::object&) const;

  void validate_uniqueItems(
    const JSON::array&, const JSON::object&, const JSON::object&) const;
};

// -----------------------------------------------------------------------------

class json_boolean_type_validator : public json_primitive_type_validator {
public:
  virtual JSON::Type primitive_type() const {
    return JSON::Type::BOOL;
  }

  void validate(const JSON&, const JSON&, const JSON::object&) const;
};

// -----------------------------------------------------------------------------

class json_integer_type_validator : public json_primitive_type_validator {
public:
  virtual JSON::Type primitive_type() const {
    return JSON::Type::NUMBER;
  }

  void validate(const JSON&, const JSON&, const JSON::object&) const;

private:
  void validate_multipleOf(
    const int&, const JSON::object&, const JSON::object&) const;

  void validate_maximum_and_exclusiveMaximum(
    const int&, const JSON::object&, const JSON::object&) const;

  void validate_minimum_and_exclusiveMinimum(
    const int&, const JSON::object&, const JSON::object&) const;
};

// -----------------------------------------------------------------------------

class json_number_type_validator : public json_primitive_type_validator {
public:
  virtual JSON::Type primitive_type() const {
    return JSON::Type::NUMBER;
  }

  void validate(const JSON&, const JSON&, const JSON::object&) const;

private:
  void validate_multipleOf(
    const double&, const JSON::object&, const JSON::object&) const;

  void validate_maximum_and_exclusiveMaximum(
    const double&, const JSON::object&, const JSON::object&) const;

  void validate_minimum_and_exclusiveMinimum(
    const double&, const JSON::object&, const JSON::object&) const;
};

// -----------------------------------------------------------------------------

class json_null_type_validator : public json_primitive_type_validator {
public:
  virtual JSON::Type primitive_type() const {
    return JSON::Type::NUL;
  }

  void validate(const JSON&, const JSON&, const JSON::object&) const;
};

// -----------------------------------------------------------------------------

class json_object_type_validator : public json_primitive_type_validator {
public:
  virtual JSON::Type primitive_type() const {
    return JSON::Type::OBJECT;
  }

  void validate(const JSON&, const JSON&, const JSON::object&) const;

private:
  void validate_maxProperties(
    const JSON::object&, const JSON::object&, const JSON::object&) const;

  void validate_minProperties(
    const JSON::object&, const JSON::object&, const JSON::object&) const;

  void validate_required(
    const JSON::object&, const JSON::object&, const JSON::object&) const;

  void validate_properties(
    const JSON::object&, const JSON::object&, const JSON::object&) const;

  void validate_dependencies(
    const JSON::object&, const JSON::object&, const JSON::object&) const;
};

// -----------------------------------------------------------------------------

class json_string_type_validator : public json_primitive_type_validator {
public:
  virtual JSON::Type primitive_type() const {
    return JSON::Type::STRING;
  }

  void validate(const JSON&, const JSON&, const JSON::object&) const;

private:
  void validate_maxLength(
    const JSON::string&, const JSON::object&, const JSON::object&) const;

  void validate_minLength(
    const JSON::string&, const JSON::object&, const JSON::object&) const;

  void validate_pattern(
    const JSON::string&, const JSON::object&, const JSON::object&) const;
};

// -----------------------------------------------------------------------------

struct json_validator_wrapper {
  const json_primitive_type_validator* validator;
};

// -----------------------------------------------------------------------------

class json_validator_meta {
public:
  using map_type = std::unordered_map<std::string, json_validator_wrapper>;

  static const json_primitive_type_validator* get_validator(const std::string&);

private:
  static const map_type type_to_validator_map;
};

// -----------------------------------------------------------------------------

const json_validator_meta::map_type json_validator_meta::type_to_validator_map {
  { "array",    { .validator=new json_array_type_validator()   } },
  { "boolean",  { .validator=new json_boolean_type_validator() } },
  { "integer",  { .validator=new json_integer_type_validator() } },
  { "number",   { .validator=new json_number_type_validator()  } },
  { "null",     { .validator=new json_null_type_validator()    } },
  { "object",   { .validator=new json_object_type_validator()  } },
  { "string",   { .validator=new json_string_type_validator()  } },
};

// -----------------------------------------------------------------------------

class json_schema_keyword_validator {
public:
  virtual void validate(
    const JSON&, const JSON::object&, const JSON::object&) const = 0;
};

// -----------------------------------------------------------------------------

class json_schema_allOf_keyword_validator : public json_schema_keyword_validator {
public:
  virtual void validate(
    const JSON&, const JSON::object&, const JSON::object&) const throw(json_validation_error);
};


class json_schema_anyOf_keyword_validator : public json_schema_keyword_validator {
public:
  virtual void validate(
    const JSON&, const JSON::object&, const JSON::object&) const throw(json_validation_error);
};


class json_schema_oneOf_keyword_validator : public json_schema_keyword_validator {
public:
  virtual void validate(
    const JSON&, const JSON::object&, const JSON::object&) const throw(json_validation_error);
};


class json_schema_not_keyword_validator : public json_schema_keyword_validator {
public:
  virtual void validate(
    const JSON&, const JSON::object&, const JSON::object&) const throw(json_validation_error);
};


class json_schema_enum_keyword_validator : public json_schema_keyword_validator {
public:
  virtual void validate(
    const JSON&, const JSON::object&, const JSON::object&) const throw(json_validation_error);
};


struct json_schema_keyword_validator_wrapper {
  const json_schema_keyword_validator* validator;
};


class json_schema_keyword_validator_meta {
public:
  using map_type = std::unordered_map<std::string, json_schema_keyword_validator_wrapper>;

  static const map_type keyword_to_validator_map;
};


const json_schema_keyword_validator_meta::map_type json_schema_keyword_validator_meta::keyword_to_validator_map {
  { "allOf",  { .validator=new json_schema_allOf_keyword_validator() } },
  { "anyOf",  { .validator=new json_schema_anyOf_keyword_validator() } },
  { "oneOf",  { .validator=new json_schema_oneOf_keyword_validator() } },
  { "not",    { .validator=new json_schema_not_keyword_validator()   } },
  { "enum",   { .validator=new json_schema_enum_keyword_validator()  } },
};

// -----------------------------------------------------------------------------

class json_schema_ref_validator {
public:
  void validate(const JSON&, const JSON&, const JSON::object&) const;
};

// -----------------------------------------------------------------------------

class json_schema_semantic_format_validator {
public:
  virtual JSON::Type target_type() const = 0;
  virtual void validate(const JSON&, const JSON&) const = 0;
};

// -----------------------------------------------------------------------------

class json_schema_datetime_format_validator : public json_schema_semantic_format_validator {
public:
  virtual JSON::Type target_type() const {
    return JSON::Type::STRING;
  }

  void validate(const JSON&, const JSON&) const throw(json_validation_error);
};

// -----------------------------------------------------------------------------

class json_schema_email_format_validator : public json_schema_semantic_format_validator {
public:
  virtual JSON::Type target_type() const {
    return JSON::Type::STRING;
  }

  void validate(const JSON&, const JSON&) const throw(json_validation_error);
};

// -----------------------------------------------------------------------------

class json_schema_hostname_format_validator : public json_schema_semantic_format_validator {
public:
  virtual JSON::Type target_type() const {
    return JSON::Type::STRING;
  }

  void validate(const JSON&, const JSON&) const throw(json_validation_error);
};

// -----------------------------------------------------------------------------

class json_schema_ipv4_format_validator : public json_schema_semantic_format_validator {
public:
  virtual JSON::Type target_type() const {
    return JSON::Type::STRING;
  }

  void validate(const JSON&, const JSON&) const throw(json_validation_error);
};

// -----------------------------------------------------------------------------

class json_schema_ipv6_format_validator : public json_schema_semantic_format_validator {
public:
  virtual JSON::Type target_type() const {
    return JSON::Type::STRING;
  }

  void validate(const JSON&, const JSON&) const throw(json_validation_error);
};

// -----------------------------------------------------------------------------

class json_schema_uri_format_validator : public json_schema_semantic_format_validator {
public:
  virtual JSON::Type target_type() const {
    return JSON::Type::STRING;
  }

  void validate(const JSON&, const JSON&) const throw(json_validation_error);
};

// -----------------------------------------------------------------------------

struct json_schema_semantic_format_validator_wrapper {
  const json_schema_semantic_format_validator* validator;
};

// -----------------------------------------------------------------------------

class json_schema_semantic_format_validator_meta {
public:
  using map_type = std::unordered_map<std::string, json_schema_semantic_format_validator_wrapper>;

  static const map_type format_to_validator_map;
};

// -----------------------------------------------------------------------------

const json_schema_semantic_format_validator_meta::map_type json_schema_semantic_format_validator_meta::format_to_validator_map {
  { "date-time",  { .validator=new json_schema_datetime_format_validator() } },
  { "email",      { .validator=new json_schema_email_format_validator()    } },
  { "hostname",   { .validator=new json_schema_hostname_format_validator() } },
  { "ipv4",       { .validator=new json_schema_ipv4_format_validator()     } },
  { "ipv6",       { .validator=new json_schema_ipv6_format_validator()     } },
  { "uri",        { .validator=new json_schema_uri_format_validator()      } },
};

// -----------------------------------------------------------------------------

void validate(const JSON&, const JSON&, const JSON::object&) throw(json_validation_error);

// -----------------------------------------------------------------------------

JSON::object _get_ref(JSON::object, std::list<std::string>&) throw(json_validation_error);

// -----------------------------------------------------------------------------

JSON::object get_ref(JSON::object, JSON::string) throw(json_validation_error);

// -----------------------------------------------------------------------------


} /* end namespace json_schema_internal */


} /* end namespace json */


} /* end namespace sneaker */


using boost::format;
using namespace sneaker::json;

// -----------------------------------------------------------------------------

const std::string REF_PATH_DELIMITER = "/";

// -----------------------------------------------------------------------------

const sneaker::json::json_schema_internal::json_primitive_type_validator*
sneaker::json::json_schema_internal::json_validator_meta::get_validator(
  const std::string& type)
{
  if (type_to_validator_map.find(type) == type_to_validator_map.end()) {
    throw std::runtime_error(
      str(format("INVALID JSON PRIMITIVE TYPE MET: %s") % type)
    );
  }
  return type_to_validator_map.at(type).validator;
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema::validate(
  const JSON& data, const JSON& schema) throw(json_validation_error)
{
  const JSON::object original_schema = schema.object_items();

  sneaker::json::json_schema_internal::validate(
    data, schema.object_items(), original_schema);
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::validate(
  const JSON& data,
  const JSON& schema,
  const JSON::object& original_schema) throw(json_validation_error)
{
  const JSON::object& schema_object = schema.object_items();

  auto keyword_to_validator_map = json_schema_keyword_validator_meta::keyword_to_validator_map;

  for (auto itr = keyword_to_validator_map.begin(); itr != keyword_to_validator_map.end(); itr++) {
    std::string keyword = itr->first;
    auto validator_wrapper = itr->second;

    if (schema_object.find(keyword) != schema_object.end()) {
      auto validator = validator_wrapper.validator;
      validator->validate(data, schema_object, original_schema);
    }
  }

  if (schema_object.find("type") != schema_object.end()) {
    const JSON::string& type = schema_object.at("type").string_value();

    std::string type_str = static_cast<std::string>(type);

    const json_primitive_type_validator* validator = \
      json_validator_meta::get_validator(type);

    validator->validate(data, schema, original_schema);

  } else if (schema_object.find("$ref") != schema_object.end()) {
    sneaker::json::json_schema_internal::json_schema_ref_validator().validate(
      data, schema, original_schema);
  }
}

// -----------------------------------------------------------------------------

JSON::object
sneaker::json::json_schema_internal::_get_ref(
  JSON::object schema_object, std::list<std::string>& sections) throw(json_validation_error)
{
  if (sections.empty()) {
    return schema_object;
  }

  std::string first = sections.front();

  if (first == "#") {
    sections.pop_front();
    return sneaker::json::json_schema_internal::_get_ref(schema_object, sections);
  }

  if (schema_object.find(first) == schema_object.end()) {
    throw json_validation_error(
      str(
        format(
          "Invalid $ref path (sub path: %s)"
        ) % boost::algorithm::join(sections, REF_PATH_DELIMITER)
      )
    );
  }

  sections.pop_front();

  JSON::object inner_schema = schema_object.at(first).object_items();

  return sneaker::json::json_schema_internal::_get_ref(inner_schema, sections);
}

// -----------------------------------------------------------------------------

JSON::object
sneaker::json::json_schema_internal::get_ref(
  JSON::object schema_object, JSON::string ref_path) throw(json_validation_error)
{
  std::string raw_ref_path = static_cast<std::string>(ref_path);
  std::list<std::string> sections;
  boost::split(sections, raw_ref_path, boost::is_any_of(REF_PATH_DELIMITER));

  return sneaker::json::json_schema_internal::_get_ref(schema_object, sections);
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_schema_allOf_keyword_validator::validate(
  const JSON& data,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const throw(json_validation_error)
{
  const JSON::array& schemas = schema_object.at("allOf").array_items();

  JSON::array::size_type valid_count = 0;
  for (auto itr = schemas.begin(); itr != schemas.end(); itr++) {
    const JSON& schema = static_cast<const JSON>(*itr);

    try {
      sneaker::json::json_schema_internal::validate(data, schema, original_schema);
      valid_count++;
    } catch (const sneaker::json::json_validation_error&) {
      // Do nothing here.
    }
  }

  if (valid_count != schemas.size()) {
    throw json_validation_error(
      str(
        format("Object %s is not valid under all the sub-schemas") % data.dump()
      )
    );
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_schema_anyOf_keyword_validator::validate(
  const JSON& data,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const throw(json_validation_error)
{
  const JSON::array& schemas = schema_object.at("anyOf").array_items();

  bool valid = false;
  for (auto itr = schemas.begin(); itr != schemas.end(); itr++) {
    const JSON& schema = static_cast<const JSON>(*itr);

    try {
      sneaker::json::json_schema_internal::validate(data, schema, original_schema);
      valid = true;
      break;
    } catch (sneaker::json::json_validation_error&) {
      // Do nothing here.
    }
  }

  if (!valid) {
    throw json_validation_error(
      str(
        format(
          "Object %s is not valid under any sub-schemas"
        ) % data.dump()
      )
    );
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_schema_oneOf_keyword_validator::validate(
  const JSON& data,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const throw(json_validation_error)
{
  const JSON::array& schemas = schema_object.at("oneOf").array_items();

  int valid_count = 0;
  for (auto itr = schemas.begin(); itr != schemas.end(); itr++) {
    const JSON& schema = static_cast<const JSON>(*itr);

    try {
      sneaker::json::json_schema_internal::validate(data, schema, original_schema);
      valid_count++;
    } catch (sneaker::json::json_validation_error&) {
      // Do nothing here.
    }
  }

  if (valid_count != 1) {
    throw json_validation_error(
      str(
        format(
          "Object %s is invalid under less or more than one sub-schemas"
        ) % data.dump()
      )
    );
  }
} 

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_schema_not_keyword_validator::validate(
  const JSON& data,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const throw(json_validation_error)
{
  const JSON::object& schema = schema_object.at("not").object_items();

  bool valid = false;
  try {
    sneaker::json::json_schema_internal::validate(data, schema, original_schema);
    valid = true;
  } catch (sneaker::json::json_validation_error&) {
    // Do nothing here.
  }

  if (valid) {
    throw json_validation_error(
      str(
        format(
          "Object %s is valid under sub-schema %s, but it's not supposed to be"
        ) % data.dump() % JSON(schema).dump()
      )
    );
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_schema_enum_keyword_validator::validate(
  const JSON& data,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const throw(json_validation_error)
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor76
   */
  const JSON::array& enum_array = schema_object.at("enum").array_items();

  bool valid = false;
  for (auto itr = enum_array.begin(); itr != enum_array.end(); itr++) {
    const JSON& enum_value = static_cast<const JSON>(*itr);

    if (data == enum_value) {
      valid = true;
      break;
    }
  }

  if (!valid) {
    throw json_validation_error(
      str(
        format(
          "Object %s is invalid under the defined enum values"
        ) % data.dump()
      )
    );
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_schema_ref_validator::validate(
  const JSON& data, const JSON& schema, const JSON::object& original_schema) const
{
  const JSON::string& ref_path = schema.object_items().at("$ref").string_value();

  JSON::object original_schema_ = original_schema;
  JSON::string ref_path_ = ref_path;

  JSON::object ref_schema = sneaker::json::json_schema_internal::get_ref(
    original_schema_, ref_path_);

  sneaker::json::json_schema_internal::validate(data, ref_schema, original_schema);
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_schema_datetime_format_validator::validate(
  const JSON& data, const JSON& schema) const throw(json_validation_error)
{
  /*
   * The value of the `date-time` format must be the Internet Date/Time Format.
   *
   * Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor108
   *
   * Internet Date/Time Format (RFC 3339):
   * http://tools.ietf.org/html/rfc3339#section-5.6
   */
  const JSON::string& format_value = data.string_value();
  const std::string datetime_str = static_cast<std::string>(format_value);

  auto validate = [](const std::string& datetime_str) -> bool {
    const char* format = "%Y-%m-%dT%H:%M:%S";
    struct tm tm;
    char* res = strptime(datetime_str.c_str(), format, &tm);
    return res != NULL;
  };

  bool valid = validate(datetime_str);

  if (!valid) {
    throw json_validation_error(
      str(
        format(
          "Invalid data in date-time format: %s"
        ) % data.dump()
      )
    );
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_schema_email_format_validator::validate(
  const JSON& data, const JSON& schema) const throw(json_validation_error)
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor111
   *
   * Internet Message Format, Addr-Spec Specification (RFC 5322):
   * http://tools.ietf.org/html/rfc5322#section-3.4.1
   */
  const JSON::string& format_value = data.string_value();
  const std::string email_str = static_cast<std::string>(format_value);

  /* Email address validation implementation from:
   * http://www.oreillynet.com/network/excerpt/spcookbook_chap03/index3.html
   */
  auto validate = [](const std::string& email_str) -> bool {
    const char* address = email_str.c_str();
    int count = 0;
    const char *c, *domain;
    static const char *rfc822_specials = "()<>@,;:\\\"[]";

    /* first we validate the name portion (name@domain) */
    for (c = address;  *c;  c++) {
      if (*c == '\"' && (c == address || *(c - 1) == '.' || *(c - 1) == '\"')) {
        while (*++c) {
          if (*c == '\"') {
            break;
          }

          if (*c == '\\' && (*++c == ' ')) {
            continue;
          }

          if (*c <= ' ' || *c >= 127) {
            return 0;
          }
        }

        if (!*c++) {
          return 0;
        }

        if (*c == '@') {
          break;
        }

        if (*c != '.') {
          return 0;
        }

        continue;
      } /* end of if (...) */

      if (*c == '@') {
        break;
      }

      if (*c <= ' ' || *c >= 127) {
        return 0;
      }

      if (strchr(rfc822_specials, *c)) {
        return 0;
      }
    } /* end of for-loop */

    if (c == address || *(c - 1) == '.') {
      return 0;
    }

    /* next we validate the domain portion (name@domain) */
    if (!*(domain = ++c)) {
      return 0;
    }

    do {
      if (*c == '.') {
        if (c == domain || *(c - 1) == '.') {
          return 0;
        }
        count++;
      }

      if (*c <= ' ' || *c >= 127) {
        return 0;
      }

      if (strchr(rfc822_specials, *c)) {
        return 0;
      }
    } while (*++c);

    return (count >= 1);
  };

  bool valid = validate(email_str);

  if (!valid) {
    throw json_validation_error(
      str(
        format(
          "Invalid data in email format: %s"
        ) % data.dump()
      )
    );
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_schema_hostname_format_validator::validate(
  const JSON& data, const JSON& schema) const throw(json_validation_error)
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor114
   *
   * DOMAIN NAMES - CONCEPTS AND FACILITIES (RFC 1034):
   * http://tools.ietf.org/html/rfc1034
   */
  const JSON::string& format_value = data.string_value();
  const std::string hostname = static_cast<std::string>(format_value);

  auto validate = [](const std::string& hostname) -> bool {
    struct addrinfo *result;
    int error;
    error = getaddrinfo(hostname.c_str(), NULL, NULL, &result);
    return error == 0;
  };

  bool valid = validate(hostname);

  if (!valid) {
    throw json_validation_error(
      str(
        format(
          "Invalid data in hostname format: %s"
        ) % data.dump()
      )
    );
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_schema_ipv4_format_validator::validate(
  const JSON& data, const JSON& schema) const throw(json_validation_error)
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor117
   *
   * Binary Labels in the Domain Name System (RFC 2673):
   * http://tools.ietf.org/html/rfc2673
   */
  const JSON::string& format_value = data.string_value();
  const std::string ipv4_address = static_cast<std::string>(format_value);

  auto validate = [](const std::string& ipv4_address) -> bool {
    boost::system::error_code ec;
    auto parsed_address = boost::asio::ip::address::from_string(ipv4_address, ec);
    return bool(ec == 0 && parsed_address.is_v4());
  };

  bool valid = validate(ipv4_address);

  if (!valid) {
    throw json_validation_error(
      str(
        format(
          "Invalid data in ipv4 format: %s"
        ) % data.dump()
      )
    );
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_schema_ipv6_format_validator::validate(
  const JSON& data, const JSON& schema) const throw(json_validation_error)
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor120
   *
   * Binary Labels in the Domain Name System (RFC 2673):
   * http://tools.ietf.org/html/rfc2673
   */
  const JSON::string& format_value = data.string_value();
  const std::string ipv4_address = static_cast<std::string>(format_value);

  auto validate = [](const std::string& ipv4_address) -> bool {
    boost::system::error_code ec;
    auto parsed_address = boost::asio::ip::address::from_string(ipv4_address, ec);
    return bool(ec == 0 && parsed_address.is_v6());
  };

  bool valid = validate(ipv4_address);

  if (!valid) {
    throw json_validation_error(
      str(
        format(
          "Invalid data in ipv6 format: %s"
        ) % data.dump()
      )
    );
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_schema_uri_format_validator::validate(
  const JSON& data, const JSON& schema) const throw(json_validation_error)
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor123
   *
   * Uniform Resource Identifier (URI): Generic Syntax (RFC 3986):
   * http://tools.ietf.org/html/rfc3986
   *
   * URI regex referenced from:
   * http://jmrware.com/articles/2009/uri_regexp/URI_regex.html
   */
  const JSON::string& format_value = data.string_value();
  const std::string uri = static_cast<std::string>(format_value);

  auto validate = [](const std::string& uri) -> bool {
    const std::string re_str = "^(?:[A-Za-z][A-Za-z0-9+\\-.]*:(?:\\/\\/(?:(?:[A-Za-z0-9\\-._~!$&'()*+,;=:]|%[0-9A-Fa-f]{2})*@)?(?:\\[(?:(?:(?:(?:[0-9A-Fa-f]{1,4}:){6}|::(?:[0-9A-Fa-f]{1,4}:){5}|(?:[0-9A-Fa-f]{1,4})?::(?:[0-9A-Fa-f]{1,4}:){4}|(?:(?:[0-9A-Fa-f]{1,4}:){0,1}[0-9A-Fa-f]{1,4})?::(?:[0-9A-Fa-f]{1,4}:){3}|(?:(?:[0-9A-Fa-f]{1,4}:){0,2}[0-9A-Fa-f]{1,4})?::(?:[0-9A-Fa-f]{1,4}:){2}|(?:(?:[0-9A-Fa-f]{1,4}:){0,3}[0-9A-Fa-f]{1,4})?::[0-9A-Fa-f]{1,4}:|(?:(?:[0-9A-Fa-f]{1,4}:){0,4}[0-9A-Fa-f]{1,4})?::)(?:[0-9A-Fa-f]{1,4}:[0-9A-Fa-f]{1,4}|(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?))|(?:(?:[0-9A-Fa-f]{1,4}:){0,5}[0-9A-Fa-f]{1,4})?::[0-9A-Fa-f]{1,4}|(?:(?:[0-9A-Fa-f]{1,4}:){0,6}[0-9A-Fa-f]{1,4})?::)|[Vv][0-9A-Fa-f]+\\.[A-Za-z0-9\\-._~!$&'()*+,;=:]+)\\]|(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)|(?:[A-Za-z0-9\\-._~!$&'()*+,;=]|%[0-9A-Fa-f]{2})*)(?::[0-9]*)?(?:\\/(?:[A-Za-z0-9\\-._~!$&'()*+,;=:@]|%[0-9A-Fa-f]{2})*)*|\\/(?:(?:[A-Za-z0-9\\-._~!$&'()*+,;=:@]|%[0-9A-Fa-f]{2})+(?:\\/(?:[A-Za-z0-9\\-._~!$&'()*+,;=:@]|%[0-9A-Fa-f]{2})*)*)?|(?:[A-Za-z0-9\\-._~!$&'()*+,;=:@]|%[0-9A-Fa-f]{2})+(?:\\/(?:[A-Za-z0-9\\-._~!$&'()*+,;=:@]|%[0-9A-Fa-f]{2})*)*|)(?:\\?(?:[A-Za-z0-9\\-._~!$&'()*+,;=:@\\/?]|%[0-9A-Fa-f]{2})*)?(?:\\#(?:[A-Za-z0-9\\-._~!$&'()*+,;=:@\\/?]|%[0-9A-Fa-f]{2})*)?|(?:\\/\\/(?:(?:[A-Za-z0-9\\-._~!$&'()*+,;=:]|%[0-9A-Fa-f]{2})*@)?(?:\\[(?:(?:(?:(?:[0-9A-Fa-f]{1,4}:){6}|::(?:[0-9A-Fa-f]{1,4}:){5}|(?:[0-9A-Fa-f]{1,4})?::(?:[0-9A-Fa-f]{1,4}:){4}|(?:(?:[0-9A-Fa-f]{1,4}:){0,1}[0-9A-Fa-f]{1,4})?::(?:[0-9A-Fa-f]{1,4}:){3}|(?:(?:[0-9A-Fa-f]{1,4}:){0,2}[0-9A-Fa-f]{1,4})?::(?:[0-9A-Fa-f]{1,4}:){2}|(?:(?:[0-9A-Fa-f]{1,4}:){0,3}[0-9A-Fa-f]{1,4})?::[0-9A-Fa-f]{1,4}:|(?:(?:[0-9A-Fa-f]{1,4}:){0,4}[0-9A-Fa-f]{1,4})?::)(?:[0-9A-Fa-f]{1,4}:[0-9A-Fa-f]{1,4}|(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?))|(?:(?:[0-9A-Fa-f]{1,4}:){0,5}[0-9A-Fa-f]{1,4})?::[0-9A-Fa-f]{1,4}|(?:(?:[0-9A-Fa-f]{1,4}:){0,6}[0-9A-Fa-f]{1,4})?::)|[Vv][0-9A-Fa-f]+\\.[A-Za-z0-9\\-._~!$&'()*+,;=:]+)\\]|(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)|(?:[A-Za-z0-9\\-._~!$&'()*+,;=]|%[0-9A-Fa-f]{2})*)(?::[0-9]*)?(?:\\/(?:[A-Za-z0-9\\-._~!$&'()*+,;=:@]|%[0-9A-Fa-f]{2})*)*|\\/(?:(?:[A-Za-z0-9\\-._~!$&'()*+,;=:@]|%[0-9A-Fa-f]{2})+(?:\\/(?:[A-Za-z0-9\\-._~!$&'()*+,;=:@]|%[0-9A-Fa-f]{2})*)*)?|(?:[A-Za-z0-9\\-._~!$&'()*+,;=@]|%[0-9A-Fa-f]{2})+(?:\\/(?:[A-Za-z0-9\\-._~!$&'()*+,;=:@]|%[0-9A-Fa-f]{2})*)*|)(?:\\?(?:[A-Za-z0-9\\-._~!$&'()*+,;=:@\\/?]|%[0-9A-Fa-f]{2})*)?(?:\\#(?:[A-Za-z0-9\\-._~!$&'()*+,;=:@\\/?]|%[0-9A-Fa-f]{2})*)?)$";
    boost::regex re(re_str);
    return boost::regex_match(uri, re);
  };

  bool valid = validate(uri);
  if (!valid) {
    throw json_validation_error(
      str(
        format(
          "Invalid data in uri format: %s"
        ) % data.dump()
      )
    );
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema::validate_definitions(
  const JSON& data,
  const JSON::object& schema_object,
  const JSON::object& original_schema) throw(json_validation_error)
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor94
   *
   * Do nothing here. This is handled by $ref.
   */
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_primitive_type_validator::validate(
  const JSON& data, const JSON& schema) const
{
  this->validate_type(data);
  this->validate_semantic_format(data, schema);
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_primitive_type_validator::validate_type(
  const JSON& data) const
{
  if (data.type() != this->primitive_type()) {
    throw json_validation_error(
      str(format("Invalid type for %s") % data.dump())
    );
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_primitive_type_validator::validate_semantic_format(
  const JSON& data, const JSON& schema) const
{
  const JSON::object& schema_object = schema.object_items();
  if (schema_object.find("format") != schema_object.end()) {
    const JSON::string& format_value = schema_object.at("format").string_value();
    std::string format_str = static_cast<std::string>(format_value);

    auto itr = json_schema_semantic_format_validator_meta::format_to_validator_map.find(format_str);

    if (itr == json_schema_semantic_format_validator_meta::format_to_validator_map.end()) {
      throw std::runtime_error(
        str(format("INVALID SEMANTIC FORMAT MET: %s") % format_str)
      );
    }

    auto validator_wrapper = itr->second;
    auto validator = validator_wrapper.validator;

    if (validator->target_type() != data.type()) {
      throw json_validation_error(
        str(
          format(
            "Invalid semantic format \"%s\" for data: %s"
          ) % format_str % data.dump()
        )
      );
    }

    validator->validate(data, schema);
  }
}

// -----------------------------------------------------------------------------

/************************* json_array_type_validator **************************/

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_array_type_validator::validate(
  const JSON& data, const JSON& schema, const JSON::object& original_schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor36
   */
  json_primitive_type_validator::validate(data, schema);

  const JSON::array& json_array = data.array_items();
  const JSON::object& schema_object = const_cast<const JSON::object&>(schema.object_items());

  this->validate_additionalItems_and_items(json_array, schema_object, original_schema);
  this->validate_maxItems(json_array, schema_object, original_schema);
  this->validate_minItems(json_array, schema_object, original_schema);
  this->validate_uniqueItems(json_array, schema_object, original_schema);
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_array_type_validator::validate_additionalItems_and_items(
  const JSON::array& json_array,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor37
   */
  if (schema_object.find("items") == schema_object.end()) {
    return;
  }

  const JSON& inner = schema_object.at("items");

  // Case if the value of `items` is an object.
  if (inner.is_object()) {
    for (JSON::array::size_type i = 0; i < json_array.size(); ++i) {
      const JSON& child = json_array.at(i);
      sneaker::json::json_schema_internal::validate(child, inner, original_schema); 
    }
    return;
  }

  // The value of `items` can only be an array at this point.
  assert(inner.is_array());

  const JSON::array& items = schema_object.at("items").array_items();

  if (schema_object.find("additionalItems") != schema_object.end()) {
    const JSON& inner = schema_object.at("additionalItems");

    if (inner.type() == JSON::Type::BOOL) {
      bool additional_items_boolean = inner.bool_value();

      if (!additional_items_boolean && json_array.size() > items.size()) {
        throw json_validation_error(
          str(
            format(
              "Array %s contains extra items (%d items allowed)"
            ) % JSON(json_array).dump() % items.size()
          )
        );
      }

    } else if (inner.type() == JSON::Type::OBJECT) {
      const JSON::object& additional_items_object = inner.object_items();

      if (additional_items_object.empty() && json_array.size() > items.size()) {
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

  for (JSON::array::size_type i = 0; i < json_array.size(); ++i) {
    if (i >= items.size()) {
      break;
    }

    const JSON& child = json_array.at(i);
    const JSON& child_schema = items.at(i);

    sneaker::json::json_schema_internal::validate(child, child_schema, original_schema);
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_array_type_validator::validate_maxItems(
  const JSON::array& json_array,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor42
   */
  if (schema_object.find("maxItems") == schema_object.end()) {
    return;
  }

  int max_items = schema_object.at("maxItems").int_value();

  if (json_array.size() > static_cast<JSON::array::size_type>(max_items)) {
    throw json_validation_error(
      str(
        format(
          "Array %s exceeds the maximum number of items (%d)"
        ) % JSON(json_array).dump() % max_items
      )
    );
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_array_type_validator::validate_minItems(
  const JSON::array& json_array,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor45
   */
  if (schema_object.find("minItems") == schema_object.end()) {
    return;
  }

  int min_items = schema_object.at("minItems").int_value();

  if (json_array.size() < static_cast<JSON::array::size_type>(min_items)) {
    throw json_validation_error(
      str(
        format(
          "Array %s does not meet the minimum number of items (%d)"
        ) % JSON(json_array).dump() % min_items
      )
    );
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_array_type_validator::validate_uniqueItems(
  const JSON::array& json_array,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor49
   */
  if (schema_object.find("uniqueItems") == schema_object.end()) {
    return;
  }

  if (json_array.empty()) {
    return;
  }

  bool unique_items = schema_object.at("uniqueItems").bool_value();

  std::set<JSON> elements;
  elements.insert(json_array.begin(), json_array.end());

  if (unique_items && !json_array.empty() && json_array.size() != elements.size()) {
    throw json_validation_error(
      str(
        format("Array %s does not have unique items") % JSON(json_array).dump()
      )
    );
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_boolean_type_validator::validate(
  const JSON& data, const JSON& schema, const JSON::object& original_schema) const
{
  json_primitive_type_validator::validate(data, schema);
}

// -----------------------------------------------------------------------------

/************************ json_integer_type_validator *************************/

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_integer_type_validator::validate(
  const JSON& data, const JSON& schema, const JSON::object& original_schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor13
   */
  json_primitive_type_validator::validate(data, schema);

  int int_value = data.int_value();
  const JSON::object& schema_object = const_cast<const JSON::object&>(schema.object_items());

  this->validate_multipleOf(int_value, schema_object, original_schema);
  this->validate_maximum_and_exclusiveMaximum(int_value, schema_object, original_schema);
  this->validate_minimum_and_exclusiveMinimum(int_value, schema_object, original_schema);
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_integer_type_validator::validate_multipleOf(
  const int& int_value,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor14
   */
  if (schema_object.find("multipleOf") == schema_object.end()) {
    return;
  }

  int multiple_of_value = schema_object.at("multipleOf").int_value();

  int remainder = int_value % multiple_of_value;

  if (remainder != 0) {
    throw json_validation_error(
      str(
        format(
          "Integer value %d is not a multiple of %d"
        ) % int_value % multiple_of_value
      )
    );
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_integer_type_validator::validate_maximum_and_exclusiveMaximum(
  const int& int_value,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor17
   */
  if (schema_object.find("maximum") == schema_object.end()) {
    return;
  }

  bool exclusiveMaximum = false;
  if (schema_object.find("exclusiveMaximum") != schema_object.end()) {
    exclusiveMaximum = schema_object.at("exclusiveMaximum").bool_value();
  }

  int maximum_value = schema_object.at("maximum").int_value();

  if (int_value > maximum_value) {
    throw json_validation_error(
      str(
        format(
          "Integer value %d exceeds the maximum value of %d"
        ) % int_value % maximum_value
      )
    );
  } else if (exclusiveMaximum && int_value == maximum_value) {
    throw json_validation_error(
      str(
        format(
          "Integer value %d exceeds or equals to the maximum value of %d"
        ) % int_value % maximum_value
      )
    );
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_integer_type_validator::validate_minimum_and_exclusiveMinimum(
  const int& int_value,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor21
   */
  if (schema_object.find("minimum") == schema_object.end()) {
    return;
  }

  bool exclusiveMinimum = false;
  if (schema_object.find("exclusiveMinimum") != schema_object.end()) {
    exclusiveMinimum = schema_object.at("exclusiveMinimum").bool_value();
  }

  int minimum_value = schema_object.at("minimum").int_value();

  if (int_value < minimum_value) {
    throw json_validation_error(
      str(
        format(
          "Integer value %d is less than the minimum value of %d"
        ) % int_value % minimum_value
      )
    );
  } else if (exclusiveMinimum && int_value == minimum_value) {
    throw json_validation_error(
      str(
        format(
          "Integer value %d is less or equal to the minimum value of %d"
        ) % int_value % minimum_value
      )
    );
  }
}

// -----------------------------------------------------------------------------

/************************* json_number_type_validator *************************/

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_number_type_validator::validate(
  const JSON& data, const JSON& schema, const JSON::object& original_schema) const
{
  json_primitive_type_validator::validate(data, schema);

  double number_value = data.number_value();
  const JSON::object& schema_object = const_cast<const JSON::object&>(schema.object_items());

  this->validate_multipleOf(number_value, schema_object, original_schema);
  this->validate_maximum_and_exclusiveMaximum(number_value, schema_object, original_schema);
  this->validate_minimum_and_exclusiveMinimum(number_value, schema_object, original_schema);
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_number_type_validator::validate_multipleOf(
  const double& number_value,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor14
   */
  if (schema_object.find("multipleOf") == schema_object.end()) {
    return;
  }

  double multiple_of_value = schema_object.at("multipleOf").number_value();

  double remainder = fmod(number_value, multiple_of_value);

  if (remainder != 0.0) {
    throw json_validation_error(
      str(
        format(
          "Number value %f is not a multiple of %f"
        ) % number_value % multiple_of_value
      )
    );
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_number_type_validator::validate_maximum_and_exclusiveMaximum(
  const double& number_value,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor17
   */
  if (schema_object.find("maximum") == schema_object.end()) {
    return;
  }

  bool exclusiveMaximum = false;
  if (schema_object.find("exclusiveMaximum") != schema_object.end()) {
    exclusiveMaximum = schema_object.at("exclusiveMaximum").bool_value();
  }

  double maximum_value = schema_object.at("maximum").number_value();

  if (number_value > maximum_value) {
    throw json_validation_error(
      str(
        format(
          "Number value %f exceeds the maximum value of %f"
        ) % number_value % maximum_value
      )
    );
  } else if (exclusiveMaximum && number_value == maximum_value) {
    throw json_validation_error(
      str(
        format(
          "Number value %f exceeds or equals to the maximum value of %f"
        ) % number_value % maximum_value
      )
    );
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_number_type_validator::validate_minimum_and_exclusiveMinimum(
  const double& number_value,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor21
   */
  if (schema_object.find("minimum") == schema_object.end()) {
    return;
  }

  bool exclusiveMinimum = false;
  if (schema_object.find("exclusiveMinimum") != schema_object.end()) {
    exclusiveMinimum = schema_object.at("exclusiveMinimum").bool_value();
  }

  double minimum_value = schema_object.at("minimum").number_value();

  if (number_value < minimum_value) {
    throw json_validation_error(
      str(
        format(
          "Number value %f is less than the minimum value of %f"
        ) % number_value % minimum_value
      )
    );
  } else if (exclusiveMinimum && number_value == minimum_value) {
    throw json_validation_error(
      str(
        format(
          "Number value %f is less or equal to the minimum value of %f"
        ) % number_value % minimum_value
      )
    );
  }
}

// -----------------------------------------------------------------------------

/************************** json_null_type_validator **************************/

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_null_type_validator::validate(
  const JSON& data, const JSON& schema, const JSON::object& original_schema) const
{
  json_primitive_type_validator::validate(data, schema);
}

// -----------------------------------------------------------------------------

/************************* json_object_type_validator *************************/

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_object_type_validator::validate(
  const JSON& data, const JSON& schema, const JSON::object& original_schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor53
   */
  json_primitive_type_validator::validate(data, schema);

  const JSON::object& object_value = data.object_items();
  const JSON::object& schema_object = schema.object_items();

  this->validate_maxProperties(object_value, schema_object, original_schema);
  this->validate_minProperties(object_value, schema_object, original_schema);
  this->validate_required(object_value, schema_object, original_schema);
  this->validate_properties(object_value, schema_object, original_schema);
  this->validate_dependencies(object_value, schema_object, original_schema);
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_object_type_validator::validate_maxProperties(
  const JSON::object& object_value,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor54
   */
  if (schema_object.find("maxProperties") == schema_object.end()) {
    return;
  }

  int max_properties = schema_object.at("maxProperties").int_value();

  if (object_value.size() > static_cast<JSON::object::size_type>(max_properties)) {
    throw json_validation_error(
      str(
        format(
          "Object %s exceeds the maximum properties number of %d"
        ) % JSON(object_value).dump() % max_properties
      )
    );
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_object_type_validator::validate_minProperties(
  const JSON::object& object_value,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor57
   */
  if (schema_object.find("minProperties") == schema_object.end()) {
    return;
  }

  int min_properties = schema_object.at("minProperties").int_value();

  if (object_value.size() < static_cast<JSON::object::size_type>(min_properties)) {
    throw json_validation_error(
      str(
        format(
          "Object %s does not meet the minimum properties number of %d"
        ) % JSON(object_value).dump() % min_properties
      )
    );
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_object_type_validator::validate_required(
  const JSON::object& object_value,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor61
   */
  if (schema_object.find("required") == schema_object.end()) {
    return;
  }

  const JSON::array& json_array = schema_object.at("required").array_items();

  for (auto itr = json_array.begin(); itr != json_array.end(); itr++) {
    const JSON& array_value = static_cast<JSON>(*itr);
    const JSON::string& unique = array_value.string_value();

    if (object_value.find(unique) == object_value.end()) {
      throw json_validation_error(
        str(
          format(
            "Object %s does not have unique property \"%s\""
          ) % JSON(object_value).dump() % unique
        )
      );
    }
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_object_type_validator::validate_properties(
  const JSON::object& object_value,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor64
   */
  if (schema_object.find("properties") == schema_object.end()) {
    return;
  }

  const JSON::object& properties = schema_object.at("properties").object_items();

  bool hasAdditionalProperties = false;
  bool do_normal_validation = true;

  if (schema_object.find("additionalProperties") != schema_object.end()) {
    hasAdditionalProperties = true;
  }

  if (hasAdditionalProperties) {
    const JSON& inner = schema_object.at("additionalProperties");
    if (inner.is_bool() && inner.bool_value() == false) {
      do_normal_validation = false;
    }
  }

  /* Do special validation if "additionalProperties" has boolean value false.
   *
   * Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor64
   **/
  if (do_normal_validation) {
    for (auto itr = properties.begin(); itr != properties.end(); itr++) {
      std::string property = static_cast<std::string>(itr->first);

      if (object_value.find(property) == object_value.end()) {
        continue;
      }

      const JSON& child = object_value.at(property);
      const JSON& property_schema = static_cast<JSON>(itr->second);

      sneaker::json::json_schema_internal::validate(child, property_schema, original_schema);
    }
  } else {
    std::set<std::string> properties_set;

    for (auto itr = object_value.begin(); itr != object_value.end(); itr++) {
      std::string property = static_cast<std::string>(itr->first);
      properties_set.insert(property);
    }

    for (auto itr = properties.begin(); itr != properties.end(); itr++) {
      std::string property = static_cast<std::string>(itr->first);

      if (object_value.find(property) == object_value.end()) {
        continue;
      }

      const JSON& child = object_value.at(property);
      const JSON& property_schema = static_cast<JSON>(itr->second);

      sneaker::json::json_schema_internal::validate(child, property_schema, original_schema);

      properties_set.erase(property);
    }

    bool hasPatternProperties = schema_object.find("patternProperties") != schema_object.end();

    if (hasPatternProperties) {

      const JSON::object& pattern_properties = schema_object.at(
        "patternProperties"
      ).object_items();

      for (auto itr = pattern_properties.begin(); itr != pattern_properties.end(); itr++) {
        std::string property_pattern = static_cast<std::string>(itr->first);
        std::regex property_pattern_regex(property_pattern);

        std::set<std::string> properties_erased;

        for (auto itr_ = properties_set.begin(); itr_ != properties_set.end(); itr_++) {
          std::string property = static_cast<std::string>(*itr_);

          std::smatch sm;
          bool matched = std::regex_match(property, sm, property_pattern_regex);
          if (!matched) {
            continue;
          }

          const JSON& child = object_value.at(property);
          const JSON& property_schema = static_cast<JSON>(itr->second);

          sneaker::json::json_schema_internal::validate(child, property_schema, original_schema);
          properties_erased.insert(property);
        }

        for (auto itr_ = properties_erased.begin(); itr_ != properties_erased.end(); ++itr_) {
          std::string property = static_cast<std::string>(*itr_);
          properties_set.erase(property);
        }
      }

    } /* if (hasPatternProperties) */

    if (!properties_set.empty()) {
      std::stringstream ss;

      ss << "[";

      for (auto itr_ = properties_set.begin(); itr_ != properties_set.end(); )  {
        std::string property_ = static_cast<std::string>(*itr_);
        ss << property_;

        if (++itr_ != properties_set.end()) {
          ss << ", ";
        }
      }

      ss << "]";

      throw json_validation_error(
        str(
          format(
            "Properties %s are invalid in object %s"
          ) % ss.str() % JSON(object_value).dump()
        )
      );
    }
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_object_type_validator::validate_dependencies(
  const JSON::object& object_value,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor70
   */
  if (schema_object.find("dependencies") == schema_object.end()) {
    return;
  }

  const JSON::object& dependencies = schema_object.at("dependencies").object_items();

  for (auto itr = dependencies.begin(); itr != dependencies.end(); ++itr) {
    const JSON::string& dependency_name = static_cast<const JSON::string>(itr->first);
    const JSON& dependency = static_cast<const JSON>(itr->second);

    if (object_value.find(dependency_name) == object_value.end()) {
      continue;
    }

    if (dependency.is_object()) {
      // Schema dependency
      const JSON::object& dependency_object = dependency.object_items();

      sneaker::json::json_schema_internal::validate(
        object_value, dependency_object, original_schema);

    } else if (dependency.is_array()) {
      // Property dependency
      const JSON::array& dependency_array = dependency.array_items();

      for (auto itr = dependency_array.begin(); itr != dependency_array.end(); ++itr) {
        const JSON& dependency_item = static_cast<const JSON>(*itr);

        const JSON::string& dependency_field = dependency_item.string_value();

        // Validation
        if (object_value.find(dependency_field) == object_value.end()) {
          throw json_validation_error(
            str(
              format(
                "Expected property %s cannot be found in object %s"
              ) % dependency_field % JSON(object_value).dump()
            )
          );
        }
      }
    }
  }
}

// -----------------------------------------------------------------------------

/************************* json_string_type_validator *************************/

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_string_type_validator::validate(
  const JSON& data, const JSON& schema, const JSON::object& original_schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor25
   */
  json_primitive_type_validator::validate(data, schema);

  const JSON::string& string_value = data.string_value();
  const JSON::object& schema_object = schema.object_items();

  this->validate_maxLength(string_value, schema_object, original_schema);
  this->validate_minLength(string_value, schema_object, original_schema);
  this->validate_pattern(string_value, schema_object, original_schema);
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_string_type_validator::validate_maxLength(
  const JSON::string& string_value,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor26
   */
  if (schema_object.find("maxLength") == schema_object.end()) {
    return;
  }

  int max_length = schema_object.at("maxLength").int_value();

  if (string_value.size() > static_cast<JSON::string::size_type>(max_length)) {
    throw json_validation_error(
      str(
        format(
          "String %s exceeds maximum length of %d"
        ) % string_value % max_length
      )
    );
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_string_type_validator::validate_minLength(
  const JSON::string& string_value,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor29
   */
  if (schema_object.find("minLength") == schema_object.end()) {
    return;
  }

  int min_length = schema_object.at("minLength").int_value();

  if (string_value.size() < static_cast<JSON::array::size_type>(min_length)) {
    throw json_validation_error(
      str(
        format(
          "String \"%s\" does not meet the minimum length limit of %d"
        ) % string_value % min_length
      )
    );
  }
}

// -----------------------------------------------------------------------------

void
sneaker::json::json_schema_internal::json_string_type_validator::validate_pattern(
  const JSON::string& string_value,
  const JSON::object& schema_object,
  const JSON::object& original_schema) const
{
  /* Spec:
   * http://json-schema.org/latest/json-schema-validation.html#anchor33
   */
  if (schema_object.find("pattern") == schema_object.end()) {
    return;
  }

  const JSON::string& pattern_value = schema_object.at("pattern").string_value();

  std::smatch sm;
  std::string str_to_match = static_cast<std::string>(string_value);
  std::string pattern_str = static_cast<std::string>(pattern_value);
  std::regex regex(pattern_str, std::regex::ECMAScript);

  std::regex_match(str_to_match, sm, regex);

  if (sm.empty()) {
    throw json_validation_error(
      str(
        format(
          "String \"%s\" does not match to regular expression %s"
        ) % string_value % pattern_str
      )
    );
  }
}

// -----------------------------------------------------------------------------
