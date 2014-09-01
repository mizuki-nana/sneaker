#include "../../include/json/json.h"
#include "../../include/json/json_parser.h"

struct Statics {
  const std::shared_ptr<sneaker::json::json_value> null = std::make_shared<sneaker::json::json_null>();
  const std::shared_ptr<sneaker::json::json_value> t = std::make_shared<sneaker::json::json_boolean>(true);
  const std::shared_ptr<sneaker::json::json_value> f = std::make_shared<sneaker::json::json_boolean>(false);
  const sneaker::json::json_type::string_type empty_string;
  const sneaker::json::json_type::array_type empty_vector;
  const sneaker::json::json_type::object_type empty_map;
  Statics() {}
};

const Statics & statics()
{
  static const Statics s {};
  return s;
}

const sneaker::json::json_type& static_null()
{
  static const sneaker::json::json_type json_null_;
  return json_null_;
}

sneaker::json::json_type::json_type() noexcept :
  _ptr(statics().null)
{
  // Do nothing here.
}

sneaker::json::json_type::json_type(null_type) noexcept :
  _ptr(statics().null)
{
  // Do nothing here.
}

sneaker::json::json_type::json_type(double value) noexcept :
  _ptr(std::make_shared<json_double>(value))
{
  // Do nothing here.
}

sneaker::json::json_type::json_type(int value) noexcept :
  _ptr(std::make_shared<json_int>(value))
{
  // Do nothing here.
}

sneaker::json::json_type::json_type(bool value) noexcept :
  _ptr(std::make_shared<json_boolean>(value ? statics().t : statics().f))
{  
  // Do nothing here.
}

sneaker::json::json_type::json_type(const string_type& value) noexcept:
  _ptr(std::make_shared<json_string>(value))
{
  // Do nothing here.
}

sneaker::json::json_type::json_type(string_type&& value) noexcept :
  _ptr(std::make_shared<json_string>(std::move(value)))
{
  // Do nothing here.
}

sneaker::json::json_type::json_type(const char * value) noexcept :
  _ptr(std::make_shared<json_string>(value))
{
  // Do nothing here.
}

sneaker::json::json_type::json_type(const array_type & value) noexcept :
  _ptr(std::make_shared<json_array>(value))
{
  // Do nothing here.
}

sneaker::json::json_type::json_type(array_type&& value) noexcept :
  _ptr(std::make_shared<json_array>(std::move(value)))
{
  // Do nothing here.
}

sneaker::json::json_type::json_type(const object_type& value) noexcept :
  _ptr(std::make_shared<json_object>(value))
{
  // Do nothing here.
}

sneaker::json::json_type::json_type(object_type&& value) noexcept :
  _ptr(std::make_shared<json_object>(std::move(value)))
{
  // Do nothing here.
}

sneaker::json::json_type::Type
sneaker::json::json_type::type() const
{
  return _ptr->type();
}

double
sneaker::json::json_type::number_value() const
{
  return _ptr->number_value();
}

int
sneaker::json::json_type::int_value() const
{
  return _ptr->int_value();
}

const sneaker::json::json_type::string_type&
sneaker::json::json_type::string_value() const
{
  return _ptr->string_value();
}

const sneaker::json::json_type::array_type&
sneaker::json::json_type::array_items() const
{
  return _ptr->array_items();
}

const sneaker::json::json_type::object_type&
sneaker::json::json_type::object_items() const
{
  return _ptr->object_items();
}

const sneaker::json::json_type&
sneaker::json::json_type::operator[](size_t i) const
{
  return (*_ptr)[i];
}

const sneaker::json::json_type&
sneaker::json::json_type::operator[](const std::string& key) const
{
  return (*_ptr)[key];
}

bool
sneaker::json::json_type::operator==(const json_type& other) const
{
  if(_ptr->type() != other._ptr->type()) {
    return false;
  }

  return _ptr->equals(other._ptr.get());
}

bool
sneaker::json::json_type::operator<(const json_type& other) const
{
  if(_ptr->type() != other._ptr->type()) {
    return _ptr->type() < other._ptr->type();
  }

  return _ptr->less(other._ptr.get());
}

void
sneaker::json::json_type::dump(std::string& out) const
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

const sneaker::json::json_type::string_type&
sneaker::json::json_value::string_value() const
{
  return statics().empty_string;
}

const sneaker::json::json_type::array_type&
sneaker::json::json_value::array_items() const
{
  return statics().empty_vector;
}

const sneaker::json::json_type::object_type&
sneaker::json::json_value::object_items() const
{
  return statics().empty_map;
}

const sneaker::json::json_type&
sneaker::json::json_value::operator[](size_t i) const
{
  return static_null();
}

const sneaker::json::json_type&
sneaker::json::json_value::operator[](const std::string& key) const
{
  return static_null();
}

const sneaker::json::json_type&
sneaker::json::json_array::operator[](size_t i) const
{
  if(i >= _value.size()) {
    return static_null();
  }

  return _value[i];
}

const sneaker::json::json_type&
sneaker::json::json_object::operator[](const std::string& key) const
{
  auto itr = _value.find(key);
  return (itr == _value.end()) ? static_null() : itr->second;
}

sneaker::json::json_type
sneaker::json::json_type::parse(const std::string& in, std::string& err)
{
  json_parser parser { in, 0, err, false };
  json_type result = parser.parse_json(0);

  // Check for any trailing garbage
  parser.consume_whitespace();

  if (parser.i != in.size()) {
    return parser.fail("unexpected trailing " + parser.esc(in[parser.i]));
  }

  return result;
}

std::vector<sneaker::json::json_type>
sneaker::json::json_type::parse_multi(const std::string& in, std::string& err)
{
  json_parser parser { in, 0, err, false };

  std::vector<sneaker::json::json_type> json_vec;

  while (parser.i != in.size() && !parser.failed) {
    json_vec.push_back(parser.parse_json(0));
    // Check for another object
    parser.consume_whitespace();
  }

  return json_vec;
}
