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
#include "../../include/json/json_parser.h"

#include "../../include/json/json.h"

#include <cassert>
#include <cstdlib>
#include <limits>
#include <map>
#include <string>
#include <vector>


using namespace sneaker::json;


const int json_parser::MAX_DEPTH = 200;


template<typename T>
T
sneaker::json::json_parser::fail(std::string&& msg, const T err_ret)
{
  if (!failed) {
    err = std::move(msg);
  }

  failed = true;

  return err_ret;
}

void
sneaker::json::json_parser::consume_whitespace()
{
  while (str[i] == ' ' || str[i] == '\r' || str[i] == '\n' || str[i] == '\t') {
    i++;
  }
}

JSON
sneaker::json::json_parser::fail(std::string&& msg)
{
  return fail(std::move(msg), JSON());
}

char
sneaker::json::json_parser::get_next_token()
{
  consume_whitespace();

  if (i == str.size()) {
    return fail("Unexpected end of input", 0);
  }

  return str[i++];
}

void
sneaker::json::json_parser::encode_utf8(long pt, std::string& out)
{
  if (pt < 0) {
    return;
  }

  if (pt < 0x80) {
    out += pt;
  } else if (pt < 0x800) {
    out += (pt >> 6) | 0xC0;
    out += (pt & 0x3F) | 0x80;
  } else if (pt < 0x10000) {
    out += (pt >> 12) | 0xE0;
    out += ((pt >> 6) & 0x3F) | 0x80;
    out += (pt & 0x3F) | 0x80;
  } else {
    out += (pt >> 18) | 0xF0;
    out += ((pt >> 12) & 0x3F) | 0x80;
    out += ((pt >> 6) & 0x3F) | 0x80;
    out += (pt & 0x3F) | 0x80;
  }
}

JSON
sneaker::json::json_parser::parse_number()
{
  size_t start_pos = i;

  if (str[i] == '-') {
    i++;
  }

  if (str[i] == '0') {
    i++;
    if (in_range(str[i], '0', '9')) {
      return fail("Leading 0s not permitted in numbers");
    }
  } else if (in_range(str[i], '1', '9')) {
    i++;
    while (in_range(str[i], '0', '9')) {
      i++;
    }
  } else {
    return fail("Invalid " + esc(str[i]) + " in number");
  }

  if (str[i] != '.' && str[i] != 'e' && str[i] != 'E' &&
    (i - start_pos) <= (size_t)std::numeric_limits<int>::digits10)
  {
    return std::atoi(str.c_str() + start_pos);
  }

  // Decimal part.
  if (str[i] == '.') {
    i++;
    if (!in_range(str[i], '0', '9')) {
      return fail("At least one digit required in fractional part");
    }
    while (in_range(str[i], '0', '9')) {
      i++;
    }
  }

  // Exponent part.
  if (str[i] == 'e' || str[i] == 'E') {
    i++;

    if (str[i] == '+' || str[i] == '-') {
      i++;
    }

    if (!in_range(str[i], '0', '9')) {
      return fail("At least one digit required in exponent");
    }
  }

  while (in_range(str[i], '0', '9')) {
    i++;
  }

  return std::atof(str.c_str() + start_pos);
}

JSON
sneaker::json::json_parser::expect(const std::string& expected, JSON res)
{
  assert(i != 0);
  i--;

  const std::string found = str.substr(i, expected.length());
  if (expected == found) {
    i += expected.length();
    return res;
  } else {
    return fail("Parse error: expected " + expected + ", got " + found);
  }
}

std::string
sneaker::json::json_parser::parse_string()
{
  std::string out;
  long last_escaped_codepoint = -1;

  while (true) {
    if (i == str.size()) {
      return fail("Unexpected end of input in string", "");
    }

    char ch = str[i++];

    if (ch == '"') {
      encode_utf8(last_escaped_codepoint, out);
      return out;
    }

    if (in_range(ch, 0, 0x1f)) {
      return fail("Unescaped " + esc(ch) + " in string", "");
    }

    // The usual case: non-escaped characters.
    if (ch != '\\') {
      encode_utf8(last_escaped_codepoint, out);
      last_escaped_codepoint = -1;
      out += ch;
      continue;
    }

    // Handle escapes.
    if (i == str.size()) {
      return fail("Unexpected end of input in string", "");
    }

    ch = str[i++];

    if (ch == 'u') {
      // Extract 4-byte escape sequence.
      std::string esc = str.substr(i, 4);

      for (int j = 0; j < 4; j++) {
        if (!in_range(esc[j], 'a', 'f') && !in_range(esc[j], 'A', 'F') && !in_range(esc[j], '0', '9')) {
          return fail("Bad \\u escape: " + esc, "");
        }
      }

      long codepoint = strtol(esc.data(), nullptr, 16);

      // JSON specifies that characters outside the BMP shall be encoded as a pair
      // of 4-hex-digit \u escapes encoding their surrogate pair components. Check
      // whether we're in the middle of such a beast: the previous codepoint was an
      // escaped lead (high) surrogate, and this is a trail (low) surrogate.
      if (in_range(last_escaped_codepoint, 0xD800, 0xDBFF) && in_range(codepoint, 0xDC00, 0xDFFF)) {
        // Reassemble the two surrogate pairs into one astral-plane character, per the UTF-16 algorithm.
        encode_utf8((((last_escaped_codepoint - 0xD800) << 10) | (codepoint - 0xDC00)) + 0x10000, out);
        last_escaped_codepoint = -1;
      } else {
        encode_utf8(last_escaped_codepoint, out);
        last_escaped_codepoint = codepoint;
      }

      i += 4;

      continue;
    }

    encode_utf8(last_escaped_codepoint, out);
    last_escaped_codepoint = -1;

    if (ch == 'b') {
      out += '\b';
    } else if (ch == 'f') {
      out += '\f';
    } else if (ch == 'n') {
      out += '\n';
    } else if (ch == 'r') {
      out += '\r';
    } else if (ch == 't') {
      out += '\t';
    } else if (ch == '"' || ch == '\\' || ch == '/') {
      out += ch;
    } else {
      return fail("Invalid escape character " + esc(ch), "");
    }
  } /* end of `while (true)` */
}

JSON
sneaker::json::json_parser::parse_json()
{
  JSON result = parse_json(0);

  consume_whitespace();

  if (i != str.size()) {
    return fail("Unexpected trailing " + esc(str[i]));
  }

  if (result.type() != JSON::Type::ARRAY && result.type() != JSON::Type::OBJECT) {
    return fail("Invalid JSON. Expecting [ or {");
  }

  return result;
}

JSON
sneaker::json::json_parser::parse_json(uint32_t depth)
{
  if (depth > json_parser::MAX_DEPTH) {
    return fail("Exceeded maximum nesting depth");
  }

  char ch = get_next_token();

  if (failed) {
    return JSON();
  }

  if (ch == '-' || (ch >= '0' && ch <= '9')) {
    i--;
    return parse_number();
  }

  if (ch == 't') {
    return expect("true", true);
  }

  if (ch == 'f') {
    return expect("false", false);
  }

  if (ch == 'n') {
    return expect("null", JSON());
  }

  if (ch == '"') {
    return parse_string();
  }

  if (ch == '{') {
    std::map<std::string, JSON> data;
    ch = get_next_token();

    if (ch == '}') {
      return data;
    }

    while (true) {
      if (ch != '"') {
        return fail("Expected '\"' in object, got " + esc(ch));
      }

      std::string key = parse_string();

      if (failed) {
        return JSON();
      }

      ch = get_next_token();

      if (ch != ':') {
        return fail("Expected ':' in object, got " + esc(ch));
      }

      data[std::move(key)] = parse_json(depth + 1);

      if (failed) {
        return JSON();
      }

      ch = get_next_token();

      if (ch == '}') {
        break;
      }

      if (ch != ',') {
        return fail("Expected ',' in object, got " + esc(ch));
      }

      ch = get_next_token();
    } /* end `while (true)` */

    return data;
  } /* end `if (char == '{')` */

  if (ch == '[') {
    std::vector<JSON> data;

    ch = get_next_token();

    if (ch == ']') {
      return data;
    }

    while (true) {
      i--;

      data.push_back(parse_json(depth + 1));

      if (failed) {
        return JSON();
      }

      ch = get_next_token();

      if (ch == ']') {
        break;
      }

      if (ch != ',') {
        return fail("Expected ',' in list, got " + esc(ch));
      }

      ch = get_next_token();

    } /* end `while (true)` */

    return data;
  } /* end `if (ch == '[')` */

  return fail("Expected value, got " + esc(ch));
}

std::string
sneaker::json::json_parser::esc(char c)
{
  char buf[12];

  if ((uint8_t)c >= 0x20 && (uint8_t)c <= 0x7f) {
    snprintf(buf, sizeof buf, "'%c' (%d)", c, c);
  } else {
    snprintf(buf, sizeof buf, "(%d)", c);
  }

  return std::string(buf);
}

bool
sneaker::json::json_parser::in_range(long x, long lower, long upper)
{
  return (x >= lower && x <= upper);
}
