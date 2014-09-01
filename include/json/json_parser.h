#ifndef SNEAKER_JSON_PARSER_H_
#define SNEAKER_JSON_PARSER_H_

#include <string>
#include "json.h"


namespace sneaker {


namespace json {


struct json_parser {

  // state
  const std::string &str;
  size_t i;
  std::string &err;
  bool failed;

  json_type fail(std::string&&);

  template<typename T>
  T fail(std::string&&, const T);

  void consume_whitespace();

  char get_next_token();

  void encode_utf8(long, std::string&);

  json_type parse_number();

  std::string parse_string();

  json_type expect(const std::string&, json_type);

  json_type parse_json(int depth);

  std::string esc(char);

  bool in_range(long, long, long);

  static const int MAX_DEPTH;
};

const int json_parser::MAX_DEPTH = 200;


} /* end namespace json */


} /* end namespace sneaker */


#endif /* SNEAKER_JSON_PARSER_H_ */
