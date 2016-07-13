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
#include "json/json.h"

#include "json/json_parser.h"
#include "utility/util.numeric.h"

#include <initializer_list>
#include <sstream>
#include <type_traits>
#include <utility>


namespace sneaker {


namespace json {


// -----------------------------------------------------------------------------

class json_value {
public:
  friend class JSON;
  friend class json_int;
  friend class json_double;

  virtual ~json_value() {}

  virtual JSON::Type type() const = 0;

  virtual bool equals(const json_value* other) const = 0;
  virtual bool less(const json_value* other) const = 0;
  virtual void dump(std::string& out) const = 0;

  virtual double number_value() const;
  virtual int64_t int_value() const;
  virtual bool bool_value() const;
  virtual const JSON::string& string_value() const;
  virtual const JSON::array& array_items() const;
  virtual const JSON::object& object_items() const;
  virtual const JSON& operator[](size_t i) const;
  virtual const JSON& operator[](const std::string& out) const;
};

// -----------------------------------------------------------------------------

template<JSON::Type tag, typename T>
class json_value_core : public json_value {
public:
  typedef T intrinsic_type;

  json_value_core(const T& value) : m_value(value) {}
  json_value_core(T&& value) : m_value(std::move(value)) {}

  T value() const {
    return m_value;
  }

  JSON::Type type() const {
    return tag;
  }

  virtual bool equals(const json_value* other) const;

  virtual bool less(const json_value* other) const;

  virtual void dump(std::string& out) const = 0;

  const T m_value;
};

// -----------------------------------------------------------------------------

template<typename T>
bool
values_equal(const T& lhs, const T& rhs)
{
  return lhs == rhs;
}

// -----------------------------------------------------------------------------

template<>
bool
values_equal(const double& lhs, const double& rhs)
{
  return utility::floats_equal(lhs, rhs);
}

// -----------------------------------------------------------------------------

/* virtual */
template<JSON::Type tag, typename T>
bool
json_value_core<tag, T>::equals(const json_value* other) const
{
  return values_equal(
    value(), reinterpret_cast<const json_value_core<tag, T>*>(other)->value());
}

// -----------------------------------------------------------------------------

/* virtual */
template<JSON::Type tag, typename T>
bool
json_value_core<tag, T>::less(const json_value* other) const
{
  return value() < reinterpret_cast<const json_value_core<tag, T>*>(other)->value();
}

// -----------------------------------------------------------------------------

class json_double final : public json_value_core<JSON::Type::NUMBER, double> {
public:
  json_double(double value) : json_value_core(value) {}

  double number_value() const {
    return value();
  }

  int64_t int_value() const {
    return static_cast<int64_t>(value());
  }

  virtual bool equals(const json_value* other) const;

  virtual bool less(const json_value* other) const;

  void dump(std::string& out) const {
    char buf[32];
    snprintf(buf, sizeof buf, "%.17g", value());
    out += buf;
  }
};

// -----------------------------------------------------------------------------

/* virtual */
bool
json_double::equals(const json_value* other) const
{
  return utility::floats_equal(number_value(), other->number_value());
}

// -----------------------------------------------------------------------------

/* virtual */
bool
json_double::less(const json_value* other) const
{
  return number_value() < other->number_value();
}

// -----------------------------------------------------------------------------

class json_int final : public json_value_core<JSON::Type::NUMBER, int64_t> {
public:
  json_int(int64_t value) : json_value_core(value) {}

  double number_value() const {
    return value();
  }

  int64_t int_value() const {
    return value();
  }

  virtual bool equals(const json_value* other) const;

  virtual bool less(const json_value* other) const;

  void dump(std::string &out) const {
    std::stringstream ss;
    ss << value();
    out += ss.str();
  }
};

// -----------------------------------------------------------------------------

/* virtual */
bool
json_int::equals(const json_value* other) const
{
  return utility::floats_equal(
    static_cast<double>(value()), other->number_value());
}

// -----------------------------------------------------------------------------

/* virtual */
bool
json_int::less(const json_value* other) const
{
  return value() < other->number_value();
}

// -----------------------------------------------------------------------------

class json_boolean final : public json_value_core<JSON::Type::BOOL, bool> {
public:
  json_boolean(bool value) : json_value_core(value) {}

  bool bool_value() const {
    return value();
  }

  virtual void dump(std::string& out) const;
};

// -----------------------------------------------------------------------------

/* virtual */
void
json_boolean::dump(std::string& out) const
{
  out += value() ? "true" : "false";
}

// -----------------------------------------------------------------------------

class json_string final : public json_value_core<JSON::Type::STRING, std::string> {
public:
  json_string(const std::string& value) : json_value_core(value) {}
  json_string(std::string&& value) : json_value_core(std::move(value)) {}

  const JSON::string& string_value() const {
    return m_value;
  }

  virtual void dump(std::string& out) const;

  static void dump(const std::string& value, std::string& out) {
    out += '"';

    for (std::string::size_type i = 0; i < value.length(); i++) {
      const char ch = value[i];

      if (ch == '\\') {
        out += "\\\\";
      } else if (ch == '"') {
        out += "\\\"";
      } else if (ch == '\b') {
        out += "\\b";
      } else if (ch == '\f') {
        out += "\\f";
      } else if (ch == '\n') {
        out += "\\n";
      } else if (ch == '\r') {
        out += "\\r";
      } else if (ch == '\t') {
        out += "\\t";
      } else if (static_cast<uint8_t>(ch) <= 0x1f) {
        char buf[8];
        snprintf(buf, sizeof buf, "\\u%04x", ch);
        out += buf;
      } else if (static_cast<uint8_t>(ch) == 0xe2 &&
                 static_cast<uint8_t>(value[i+1]) == 0x80 &&
                 static_cast<uint8_t>(value[i+2]) == 0xa8) {
        out += "\\u2028";
        i += 2;
      } else if (static_cast<uint8_t>(ch) == 0xe2 &&
                 static_cast<uint8_t>(value[i+1]) == 0x80 &&
                 static_cast<uint8_t>(value[i+2]) == 0xa9) {
        out += "\\u2029";
        i += 2;
      } else {
        out += ch;
      }
    } /* end `for (auto i = 0; i < value.length(); i++)` */

    out += '"';
  }
};

// -----------------------------------------------------------------------------

/* virtual */
void
json_string::dump(std::string& out) const
{
  json_string::dump(value(), out);
}

// -----------------------------------------------------------------------------

class json_array final : public json_value_core<JSON::Type::ARRAY, JSON::array> {
public:
  json_array(const JSON::array& value) : json_value_core(value) {}
  json_array(const JSON::array&& value) : json_value_core(std::move(value)) {}

  const JSON::array& array_items() const {
    return m_value;
  }

  const JSON& operator[](size_t i) const;

  void dump(std::string& out) const {
    bool first = true;
    out += "[";
    for (auto &value_ : value()) {
      if (!first) {
        out += ", ";
      }

      value_.dump(out);

      first = false;
    }
    out += "]";
  }
};

// -----------------------------------------------------------------------------

class json_object final : public json_value_core<JSON::Type::OBJECT, JSON::object> {
public:
  json_object(const JSON::object &value) : json_value_core(value) {}
  json_object(JSON::object &&value) : json_value_core(std::move(value)) {}

  const JSON::object& object_items() const {
    return m_value;
  }

  const JSON& operator[](const std::string& key) const;

  void dump(std::string& out) const {
    bool first = true;

    out += "{";

    for (const std::pair<std::string, JSON> &kv : value()) {
      if (!first) {
        out += ", ";
      }

      std::string str = kv.first;
      json_string::dump(str, out);

      out += ": ";

      kv.second.dump(out);

      first = false;
    }

    out += "}";
  }
};

// -----------------------------------------------------------------------------

class json_null final : public json_value_core<JSON::Type::NUL, JSON::null> {
public:
  json_null() : json_value_core(nullptr) {}

  virtual void dump(std::string& out) const;
};

// -----------------------------------------------------------------------------

/* virtual */
void
json_null::dump(std::string& out) const
{
  out += "null";
}

// -----------------------------------------------------------------------------

struct Statics {
  const std::shared_ptr<json_value> null = std::make_shared<json_null>();
  const std::shared_ptr<json_value> t = std::make_shared<json_boolean>(true);
  const std::shared_ptr<json_value> f = std::make_shared<json_boolean>(false);
  const JSON::string empty_string;
  const JSON::array empty_vector;
  const JSON::object empty_map;
  Statics() {}
};

const Statics& statics();

const JSON& static_null();

// -----------------------------------------------------------------------------

const Statics & statics()
{
  static const Statics s {};
  return s;
}

// -----------------------------------------------------------------------------

const JSON& static_null()
{
  static const JSON json_null_value;
  return json_null_value;
}

// -----------------------------------------------------------------------------

JSON::JSON()
  :
  m_ptr(statics().null)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

JSON::JSON(null)
  :
  m_ptr(statics().null)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

JSON::JSON(double value)
  :
  m_ptr(std::make_shared<json_double>(value))
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

JSON::JSON(int value)
  :
  m_ptr(std::make_shared<json_int>(value))
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

JSON::JSON(bool value)
  :
  m_ptr(value ? statics().t : statics().f)
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

JSON::JSON(const string& value)
  :
  m_ptr(std::make_shared<json_string>(value))
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

JSON::JSON(string&& value)
  :
  m_ptr(std::make_shared<json_string>(std::move(value)))
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

JSON::JSON(const char * value)
  :
  m_ptr(std::make_shared<json_string>(value))
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

JSON::JSON(const array & value)
  :
  m_ptr(std::make_shared<json_array>(value))
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

JSON::JSON(array&& value)
  :
  m_ptr(std::make_shared<json_array>(std::move(value)))
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

JSON::JSON(const object& value)
  :
  m_ptr(std::make_shared<json_object>(value))
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

JSON::JSON(object&& value)
  :
  m_ptr(std::make_shared<json_object>(std::move(value)))
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

JSON::Type
JSON::type() const
{
  return m_ptr->type();
}

// -----------------------------------------------------------------------------

double
JSON::number_value() const
{
  return m_ptr->number_value();
}

// -----------------------------------------------------------------------------

int64_t
JSON::int_value() const
{
  return m_ptr->int_value();
}

// -----------------------------------------------------------------------------

bool
JSON::bool_value() const
{
  return m_ptr->bool_value();
}

// -----------------------------------------------------------------------------

const JSON::string&
JSON::string_value() const
{
  return m_ptr->string_value();
}

// -----------------------------------------------------------------------------

const JSON::array&
JSON::array_items() const
{
  return m_ptr->array_items();
}

// -----------------------------------------------------------------------------

const JSON::object&
JSON::object_items() const
{
  return m_ptr->object_items();
}

// -----------------------------------------------------------------------------

bool
JSON::operator!=(const JSON& rhs) const
{
  return !(*this == rhs);
}

// -----------------------------------------------------------------------------

bool
JSON::operator<=(const JSON& rhs) const
{
  return !(rhs < *this);
}

// -----------------------------------------------------------------------------

bool
JSON::operator>(const JSON& rhs) const
{
  return rhs < *this;
}

// -----------------------------------------------------------------------------

bool
JSON::operator>=(const JSON& rhs) const
{
  return !(*this < rhs);
}

// -----------------------------------------------------------------------------

const JSON&
JSON::operator[](size_t i) const
{
  return (*m_ptr)[i];
}

// -----------------------------------------------------------------------------

const JSON&
JSON::operator[](const std::string& key) const
{
  return (*m_ptr)[key];
}

// -----------------------------------------------------------------------------

bool
JSON::operator==(const JSON& other) const
{
  if (m_ptr->type() != other.m_ptr->type()) {
    return false;
  }

  return m_ptr->equals(other.m_ptr.get());
}

// -----------------------------------------------------------------------------

bool
JSON::operator<(const JSON& other) const
{
  if (m_ptr->type() != other.m_ptr->type()) {
    return m_ptr->type() < other.m_ptr->type();
  }

  return m_ptr->less(other.m_ptr.get());
}

// -----------------------------------------------------------------------------

std::string
JSON::dump() const
{
  std::string out;
  dump(out);
  return out;
}

// -----------------------------------------------------------------------------

void
JSON::dump(std::string& out) const
{
  m_ptr->dump(out);
}

// -----------------------------------------------------------------------------

double
json_value::number_value() const
{
  return 0;
}

// -----------------------------------------------------------------------------

int64_t
json_value::int_value() const
{
  return 0;
}

// -----------------------------------------------------------------------------

bool
json_value::bool_value() const
{
  return false;
}

// -----------------------------------------------------------------------------

const JSON::string&
json_value::string_value() const
{
  return statics().empty_string;
}

// -----------------------------------------------------------------------------

const JSON::array&
json_value::array_items() const
{
  return statics().empty_vector;
}

// -----------------------------------------------------------------------------

const JSON::object&
json_value::object_items() const
{
  return statics().empty_map;
}

// -----------------------------------------------------------------------------

const JSON&
json_value::operator[](size_t /* i */) const
{
  return static_null();
}

// -----------------------------------------------------------------------------

const JSON&
json_value::operator[](const std::string& /* key */) const
{
  return static_null();
}

// -----------------------------------------------------------------------------

const JSON&
json_array::operator[](size_t i) const
{
  if (i >= m_value.size()) {
    return static_null();
  }

  return m_value[i];
}

// -----------------------------------------------------------------------------

const JSON&
json_object::operator[](const std::string& key) const
{
  auto itr = m_value.find(key);
  return (itr == m_value.end()) ? static_null() : itr->second;
}

// -----------------------------------------------------------------------------

JSON
parse(const std::string& in)
{
  std::string err;
  json_parser parser { in, 0, err, false };
  JSON result = parser.parse_json();

  if (!parser.err.empty()) {
    throw invalid_json_error(parser.err);
  }

  return result;
}

// -----------------------------------------------------------------------------

JSON::JSON(int64_t value, char)
  :
  m_ptr(std::make_shared<json_int>(value))
{
  // Do nothing here.
}

// -----------------------------------------------------------------------------

/* static */
JSON
JSON::from_int64(int64_t value)
{
  return JSON(value, '\0');
}

// -----------------------------------------------------------------------------


} /* end namespace json */


} /* end namespace sneaker */
