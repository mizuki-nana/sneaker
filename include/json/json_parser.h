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
#ifndef SNEAKER_JSON_PARSER_H_
#define SNEAKER_JSON_PARSER_H_

#include <string>
#include "json.h"


namespace sneaker {


namespace json {


struct json_parser {
public:
  const std::string &str;
  size_t i;
  std::string &err;
  bool failed;

  static const int MAX_DEPTH;

  JSON parse_json();
private:
  void consume_whitespace();

  JSON parse_json(uint32_t depth);

  std::string esc(char);

  JSON fail(std::string&&);

  template<typename T>
  T fail(std::string&&, const T);

  char get_next_token();

  void encode_utf8(long, std::string&);

  JSON parse_number();

  std::string parse_string();

  JSON expect(const std::string&, JSON);

  bool in_range(long, long, long);
};


} /* end namespace json */


} /* end namespace sneaker */


#endif /* SNEAKER_JSON_PARSER_H_ */