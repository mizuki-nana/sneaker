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

#include "../../include/json/json.h"

#include "../../include/json/json_parser.h"

#include <initializer_list>
#include <utility>


namespace sneaker {


namespace json {


class json_value {
public:
  friend class JSON;
  friend class json_int;
  friend class json_double;

  virtual ~json_value() {}

  virtual JSON::Type type() const = 0;

  virtual bool equals(const json_value*) const = 0;
  virtual bool less(const json_value*) const = 0;
  virtual void dump(std::string&) const = 0;

  virtual double number_value() const;
  virtual int int_value() const;
  virtual bool bool_value() const;
  virtual const JSON::string& string_value() const;
  virtual const JSON::array& array_items() const;
  virtual const JSON::object& object_items() const;
  virtual const JSON& operator[](size_t) const;
  virtual const JSON& operator[](const std::string&) const;
};


template<JSON::Type tag, typename T>
class json_value_core : public json_value {
protected:
  json_value_core(const T& value) : _value(value) {}
  json_value_core(T&& value) : _value(std::move(value)) {}

  T value() const {
    return _value;
  }

  JSON::Type type() const {
    return tag;
  }

  virtual bool equals(const json_value* other) const {
    return value() == reinterpret_cast<const json_value_core<tag, T>*>(other)->value();
  }

  virtual bool less(const json_value* other) const {
    return value() < reinterpret_cast<const json_value_core<tag, T>*>(other)->value();
  }

  virtual void dump(std::string& out) const = 0;

  const T _value;
};


class json_double final : public json_value_core<JSON::Type::NUMBER, double> {
public:
  json_double(double value) : json_value_core(value) {}

  double number_value() const {
    return value();
  }

  int int_value() const {
    return value();
  }

  virtual bool equals(const json_value& other) const {
    return value() == other.number_value();
  }

  virtual bool less(const json_value& other) const {
    return value() < other.number_value();
  }

  void dump(std::string& out) const {
    char buf[32];
    snprintf(buf, sizeof buf, "%.17g", value());
    out += buf;
  }
};


class json_int final : public json_value_core<JSON::Type::NUMBER, int> {
public:
  json_int(double value) : json_value_core(value) {}

  double number_value() const {
    return value();
  }

  int int_value() const {
    return value();
  }

  virtual bool equals(const json_value& other) const {
    return value() == other.number_value();
  }

  virtual bool less(const json_value& other) const {
    return value() < other.number_value();
  }

  void dump(std::string &out) const {
    char buf[32];
    snprintf(buf, sizeof buf, "%d", value());
    out += buf;
  }
};


class json_boolean final : public json_value_core<JSON::Type::BOOL, bool> {
public:
  json_boolean(bool value) : json_value_core(value) {}

  bool bool_value() const {
    return value();
  }

  void dump(std::string& out) const {
    out += value() ? "true" : "false";
  }
};


class json_string final : public json_value_core<JSON::Type::STRING, std::string> {
public:
  json_string(const std::string& value) : json_value_core(value) {}
  json_string(std::string&& value) : json_value_core(std::move(value)) {}

  const JSON::string& string_value() const {
    return _value;
  }

  void dump(std::string& out) const {
    json_string::dump(value(), out);
  }

  static void dump(const std::string& value, std::string& out) {
    out += '"';

    for(std::string::size_type i = 0; i < value.length(); i++) {
      const char ch = value[i];

      if(ch == '\\') {
        out += "\\\\";
      } else if(ch == '"') {
        out += "\\\"";
      } else if(ch == '\b') {
        out += "\\b";
      } else if(ch == '\f') {
        out += "\\f";
      } else if(ch == '\n') {
        out += "\\n";
      } else if(ch == '\r') {
        out += "\\r";
      } else if(ch == '\t') {
        out += "\\t";
      } else if((uint8_t)ch <= 0x1f) {
        char buf[8];
        snprintf(buf, sizeof buf, "\\u%04x", ch);
        out += buf;
      } else if((uint8_t)ch == 0xe2 && (uint8_t)value[i+1] == 0x80 && (uint8_t)value[i+2] == 0xa8) {
        out += "\\u2028";
        i += 2;
      } else if((uint8_t)ch == 0xe2 && (uint8_t)value[i+1] == 0x80 && (uint8_t)value[i+2] == 0xa9) {
        out += "\\u2029";
        i += 2;
      } else {
        out += ch;
      }
    } /* end `for (auto i = 0; i < value.length(); i++)` */

    out += '"';
  }
};


class json_array final : public json_value_core<JSON::Type::ARRAY, JSON::array> {
public:
  json_array(const JSON::array& value) : json_value_core(value) {}
  json_array(const JSON::array&& value) : json_value_core(std::move(value)) {}

  const JSON::array& array_items() const {
    return _value;
  }

  const JSON& operator[](size_t i) const;

  void dump(std::string& out) const {
    bool first = true;
    out += "[";
    for(auto &value_ : value()) {
      if (!first) {
        out += ", ";
      }

      value_.dump(out);

      first = false;
    }
    out += "]";
  }
};


class json_object final : public json_value_core<JSON::Type::OBJECT, JSON::object> {
public:
  json_object(const JSON::object &value) : json_value_core(value) {}
  json_object(JSON::object &&value) : json_value_core(std::move(value)) {}

  const JSON::object& object_items() const {
    return _value;
  }

  const JSON& operator[](const std::string& key) const;

  void dump(std::string& out) const {
    bool first = true;

    out += "{";

    for(const std::pair<std::string, JSON> &kv : value()) {
      if(!first) {
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


class json_null final : public json_value_core<JSON::Type::NUL, JSON::null> {
public:
  json_null() : json_value_core(nullptr) {}

  void dump(std::string& out) const {
    out += "null";
  }
};


} /* end namespace json */


} /* end namespace sneaker */


struct Statics {
  const std::shared_ptr<sneaker::json::json_value> null = std::make_shared<sneaker::json::json_null>();
  const std::shared_ptr<sneaker::json::json_value> t = std::make_shared<sneaker::json::json_boolean>(true);
  const std::shared_ptr<sneaker::json::json_value> f = std::make_shared<sneaker::json::json_boolean>(false);
  const sneaker::json::JSON::string empty_string;
  const sneaker::json::JSON::array empty_vector;
  const sneaker::json::JSON::object empty_map;
  Statics() {}
};


const Statics & statics()
{
  static const Statics s {};
  return s;
}

const sneaker::json::JSON& static_null()
{
  static const sneaker::json::JSON json_null_value;
  return json_null_value;
}

sneaker::json::JSON::JSON() noexcept :
  _ptr(statics().null)
{
  // Do nothing here.
}

sneaker::json::JSON::JSON(null) noexcept :
  _ptr(statics().null)
{
  // Do nothing here.
}

sneaker::json::JSON::JSON(double value) noexcept :
  _ptr(std::make_shared<json_double>(value))
{
  // Do nothing here.
}

sneaker::json::JSON::JSON(int value) noexcept :
  _ptr(std::make_shared<json_int>(value))
{
  // Do nothing here.
}

sneaker::json::JSON::JSON(bool value) noexcept :
  _ptr(value ? statics().t : statics().f)
{  
  // Do nothing here.
}

sneaker::json::JSON::JSON(const string& value) noexcept:
  _ptr(std::make_shared<json_string>(value))
{
  // Do nothing here.
}

sneaker::json::JSON::JSON(string&& value) noexcept :
  _ptr(std::make_shared<json_string>(std::move(value)))
{
  // Do nothing here.
}

sneaker::json::JSON::JSON(const char * value) noexcept :
  _ptr(std::make_shared<json_string>(value))
{
  // Do nothing here.
}

sneaker::json::JSON::JSON(const array & value) noexcept :
  _ptr(std::make_shared<json_array>(value))
{
  // Do nothing here.
}

sneaker::json::JSON::JSON(array&& value) noexcept :
  _ptr(std::make_shared<json_array>(std::move(value)))
{
  // Do nothing here.
}

sneaker::json::JSON::JSON(const object& value) noexcept :
  _ptr(std::make_shared<json_object>(value))
{
  // Do nothing here.
}

sneaker::json::JSON::JSON(object&& value) noexcept :
  _ptr(std::make_shared<json_object>(std::move(value)))
{
  // Do nothing here.
}

sneaker::json::JSON::Type
sneaker::json::JSON::type() const
{
  return _ptr->type();
}

double
sneaker::json::JSON::number_value() const
{
  return _ptr->number_value();
}

int
sneaker::json::JSON::int_value() const
{
  return _ptr->int_value();
}

bool
sneaker::json::JSON::bool_value() const
{
  return _ptr->bool_value();
}

const sneaker::json::JSON::string&
sneaker::json::JSON::string_value() const
{
  return _ptr->string_value();
}

const sneaker::json::JSON::array&
sneaker::json::JSON::array_items() const
{
  return _ptr->array_items();
}

const sneaker::json::JSON::object&
sneaker::json::JSON::object_items() const
{
  return _ptr->object_items();
}

bool
sneaker::json::JSON::operator!=(const JSON& rhs) const
{
  return !(*this == rhs);
}

bool
sneaker::json::JSON::operator<=(const JSON& rhs) const
{
  return !(rhs < *this);
}

bool
sneaker::json::JSON::operator>(const JSON& rhs) const
{
  return rhs < *this;
}

bool
sneaker::json::JSON::operator>=(const JSON& rhs) const
{
  return !(*this < rhs);
}

const sneaker::json::JSON&
sneaker::json::JSON::operator[](size_t i) const
{
  return (*_ptr)[i];
}

const sneaker::json::JSON&
sneaker::json::JSON::operator[](const std::string& key) const
{
  return (*_ptr)[key];
}

bool
sneaker::json::JSON::operator==(const JSON& other) const
{
  if(_ptr->type() != other._ptr->type()) {
    return false;
  }

  return _ptr->equals(other._ptr.get());
}

bool
sneaker::json::JSON::operator<(const JSON& other) const
{
  if(_ptr->type() != other._ptr->type()) {
    return _ptr->type() < other._ptr->type();
  }

  return _ptr->less(other._ptr.get());
}

std::string
sneaker::json::JSON::dump() const
{
  std::string out;
  dump(out);
  return out;
}

void
sneaker::json::JSON::dump(std::string& out) const
{
  _ptr->dump(out);
}

double
sneaker::json::json_value::number_value() const
{
  return 0;
}

int
sneaker::json::json_value::int_value() const
{
  return 0;
}

bool
sneaker::json::json_value::bool_value() const
{
  return false;
}

const sneaker::json::JSON::string&
sneaker::json::json_value::string_value() const
{
  return statics().empty_string;
}

const sneaker::json::JSON::array&
sneaker::json::json_value::array_items() const
{
  return statics().empty_vector;
}

const sneaker::json::JSON::object&
sneaker::json::json_value::object_items() const
{
  return statics().empty_map;
}

const sneaker::json::JSON&
sneaker::json::json_value::operator[](size_t i) const
{
  return static_null();
}

const sneaker::json::JSON&
sneaker::json::json_value::operator[](const std::string& key) const
{
  return static_null();
}

const sneaker::json::JSON&
sneaker::json::json_array::operator[](size_t i) const
{
  if(i >= _value.size()) {
    return static_null();
  }

  return _value[i];
}

const sneaker::json::JSON&
sneaker::json::json_object::operator[](const std::string& key) const
{
  auto itr = _value.find(key);
  return (itr == _value.end()) ? static_null() : itr->second;
}

sneaker::json::JSON
sneaker::json::parse(const std::string& in) throw(invalid_json_error)
{
  std::string err;
  json_parser parser { in, 0, err, false };
  JSON result = parser.parse_json();

  if(!parser.err.empty()) {
    throw invalid_json_error(parser.err);
  }

  return result;
}
