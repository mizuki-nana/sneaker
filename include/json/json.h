#ifndef SNEAKER_JSON_H_
#define SNEAKER_JSON_H_


#include <initializer_list>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>


namespace sneaker {


namespace json {


// Forward declaration of `sneaker::json::json_value`.
class json_value;


class json_type {
public:
  enum Type {
    NUL, NUMBER, BOOL, STRING, ARRAY, OBJECT
  };

  using string_type = typename std::string;
  using array_type = typename std::vector<json_type>;
  using object_type = typename std::map<std::string, json_type>;
  using null_type = typename std::nullptr_t;

  json_type() noexcept;
  json_type(null_type) noexcept;
  json_type(double) noexcept;
  json_type(int) noexcept;
  json_type(bool) noexcept;
  json_type(const string_type&) noexcept;
  json_type(string_type&&) noexcept;
  json_type(const char *) noexcept;
  json_type(const array_type&) noexcept;
  json_type(array_type&&) noexcept;
  json_type(const object_type&) noexcept;
  json_type(object_type&&) noexcept;

  // Implicit constructor: anything with a to_json() function.
  template <class T, class = decltype(&T::to_json)>
  json_type(const T & t) : json_type(t.to_json()) {}

  // Implicit constructor: map-like objects (std::map, std::unordered_map, etc)
  template <class M, typename std::enable_if<
    std::is_constructible<std::string, typename M::key_type>::value && std::is_constructible<json_type, typename M::mapped_type>::value, int>::type = 0
  >
  json_type(const M & m) : json_type(object_type(m.begin(), m.end())) {}

  // Implicit constructor: vector-like objects (std::list, std::vector, std::set, etc)
  template <class V, typename std::enable_if<
    std::is_constructible<json_type, typename V::value_type>::value, int>::type = 0
  >
  json_type(const V & v) : json_type(array_type(v.begin(), v.end())) {}

  // This prevents Json(some_pointer) from accidentally producing a bool. Use
  // Json(bool(some_pointer)) if that behavior is desired.
  json_type(void *) = delete;

  // Accessors
  Type type() const;

  bool is_null()   const { return type() == NUL; }
  bool is_number() const { return type() == NUMBER; }
  bool is_bool()   const { return type() == BOOL; }
  bool is_string() const { return type() == STRING; }
  bool is_array()  const { return type() == ARRAY; }
  bool is_object() const { return type() == OBJECT; }

  double number_value() const;
  int int_value() const;
  bool bool_value() const;
  const string_type& string_value() const;
  const array_type& array_items() const;
  const object_type& object_items() const;
  const json_type & operator[](size_t i) const;
  const json_type & operator[](const std::string& key) const;

  bool operator==(const json_type& other) const;
  bool operator<(const json_type& other) const;

  bool operator!=(const json_type& rhs) const {
    return !(*this == rhs);
  }

  bool operator<=(const json_type& rhs) const {
    return !(rhs < *this);
  }

  bool operator>(const json_type& rhs) const {
    return (rhs < *this);
  }

  bool operator>=(const json_type& rhs) const {
    return !(*this < rhs);
  }

  std::string dump() const {
    std::string out;
    dump(out);
    return out;
  }

  void dump(std::string& out) const;

  json_type parse(const std::string& in, std::string& err);

  std::vector<json_type> parse_multi(const std::string& in, std::string& err);

private:
  std::shared_ptr<json_value> _ptr;
};


class json_value {
public:
  friend class json_type;
  friend class json_int;
  friend class json_double;

  virtual ~json_value() {}

  virtual json_type::Type type() const = 0;

  virtual bool equals(const json_value*) const = 0;
  virtual bool less(const json_value*) const = 0;
  virtual void dump(std::string&) const = 0;

  virtual double number_value() const;
  virtual int int_value() const;
  virtual bool bool_value() const;
  virtual const json_type::string_type& string_value() const;
  virtual const json_type::array_type& array_items() const;
  virtual const json_type::object_type& object_items() const;
  virtual const json_type& operator[](size_t) const;
  virtual const json_type& operator[](const std::string&) const;
};


template<json_type::Type tag, typename T>
class json_value_core {
protected:
  json_value_core(const T& value) : _value(value) {}
  json_value_core(T&& value) : _value(std::move(value)) {}

  T value() const {
    return _value;
  }

  json_type::Type type() const {
    return tag;
  }

  virtual bool equals(const json_value* other) const {
    return value() == reinterpret_cast<const json_value_core<tag, T>*>(other)->value();
  }

  virtual bool less(const json_value* other) const {
    return value() < reinterpret_cast<const json_value_core<tag, T>*>(other)->value();
  }

  void dump(std::string& out) const {
    //sneaker::json::dump(_value, out);
  }

  const T _value;
};


class json_double final : public json_value_core<json_type::Type::NUMBER, double> {
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


class json_int final : public json_value_core<json_type::Type::NUMBER, int> {
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


class json_boolean final : public json_value_core<json_type::Type::BOOL, bool> {
public:
  json_boolean(bool value) : json_value_core(value) {}

  bool bool_value() const {
    return value();
  }

  void dump(std::string& out) {
    out += value() ? "true" : "false";
  }
};


class json_string final : public json_value_core<json_type::Type::STRING, std::string> {
public:
  json_string(const std::string& value) : json_value_core(value) {}
  json_string(std::string&& value) : json_value_core(std::move(value)) {}

  const std::string string_value() const {
    return value();
  }

  void dump(std::string& out) {
    out += '"';

    for(auto i = 0; i < value().length(); i++) {
      const char ch = value()[i];

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
      } else if((uint8_t)ch == 0xe2 && (uint8_t)value()[i+1] == 0x80 && (uint8_t)value()[i+2] == 0xa8) {
        out += "\\u2028";
        i += 2;
      } else if((uint8_t)ch == 0xe2 && (uint8_t)value()[i+1] == 0x80 && (uint8_t)value()[i+2] == 0xa9) {
        out += "\\u2029";
        i += 2;
      } else {
        out += ch;
      }
    } /* end `for (auto i = 0; i < value().length(); i++)` */

    out += '"';
  }
};


class json_array final : public json_value_core<json_type::Type::ARRAY, json_type::array_type> {
public:
  json_array(const json_type::array_type& value) : json_value_core(value) {}
  json_array(const json_type::array_type&& value) : json_value_core(std::move(value)) {}

  const json_type::array_type array_items() const {
    value();
  }

  const json_type& operator[](size_t i) const;

  void dump(std::string& out) {
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


class json_object final : public json_value_core<json_type::Type::OBJECT, json_type::object_type> {
public:
  json_object(const json_type::object_type &value) : json_value_core(value) {}
  json_object(json_type::object_type &&value) : json_value_core(std::move(value)) {}

  const json_type::object_type object_items() const {
    return value();
  }

  const json_type& operator[](const std::string& key) const;

  void dump(std::string& out) {
    bool first = true;

    out += "{";

    for(const std::pair<std::string, json_type> &kv : value()) {
      if(!first) {
        out += ", ";
      }

      //kv.first.dump(out);

      out += ": ";

      kv.second.dump(out);

      first = false;
    }

    out += "}";
  }
};


class json_null final : public json_value_core<json_type::Type::NUL, json_type::null_type> {
public:
  json_null() : json_value_core(nullptr) {}

  void dump(std::string& out) {
    out += "null";
  }
};


} /* end namespace json */


} /* end namespace sneaker */


#endif /* SNEAKER_JSON_H_ */
