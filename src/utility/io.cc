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
#include "utility/io.h"

#include <cassert>


namespace sneaker {


namespace utility {


// -----------------------------------------------------------------------------

namespace {

// ASCII table
// http://www.asciitable.com
static const unsigned char ASCII_TABLE[128] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,               // gap
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,                                 // 0 - 9
  0, 0, 0, 0, 0, 0, 0,                                          // :;<=>?@
  10, 11, 12, 13, 14, 15,                                       // A, B, C, D, E, F
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // GHIJKLMNOPQRSTUVWXYZ
  0, 0, 0, 0, 0, 0,                                             // [\]^_`
  10, 11, 12, 13, 14, 15,                                       // a - f
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // ghijklmnopqrstuvwxyz
  0, 0, 0, 0, 0
};

static const unsigned char HEX_DIGITS[16] = {
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
  'a', 'b', 'c', 'd', 'e', 'f'
};

} /* anonymous namespace */

// -----------------------------------------------------------------------------

void hex_to_bytes(const unsigned char* src, size_t src_len, unsigned char* dst)
{
  assert(dst);
  for (size_t i = 0; i < src_len; i += 2)
  {
    *dst++ = static_cast<unsigned char>(ASCII_TABLE[src[i]] << 4) |
      ASCII_TABLE[src[i + 1]];
  }
}

// -----------------------------------------------------------------------------

void hex_to_bytes(const std::string& src, std::string* dst)
{
  assert(dst);
  dst->resize(src.size() / 2, 0);
  hex_to_bytes(reinterpret_cast<const unsigned char*>(&src[0]), src.size(),
    reinterpret_cast<unsigned char*>(&(*dst)[0]));
}

// -----------------------------------------------------------------------------

void hex_to_bytes(const unsigned char* src, size_t src_len,
  std::vector<unsigned char>* dst)
{
  assert(dst);
  dst->resize(src_len, 0);
  hex_to_bytes(src, src_len, dst->data());
}

// -----------------------------------------------------------------------------

void bytes_to_hex(const unsigned char* src, size_t src_len, unsigned char* dst)
{
  assert(dst);
  for (size_t i = 0; i < src_len; ++i)
  {
    const unsigned char value = src[i];
    const unsigned char lower = value & ((1 << 4) - 1);
    const unsigned char upper = (value - lower) >> 4;
    size_t j = i << 1;
    dst[j] = HEX_DIGITS[upper];
    dst[j + 1] = HEX_DIGITS[lower];
  }
}

// -----------------------------------------------------------------------------

void bytes_to_hex(const std::string& src, std::string* dst)
{
  assert(dst);
  dst->resize(src.size() * 2, 0);
  bytes_to_hex(reinterpret_cast<const unsigned char*>(&src[0]), src.size(),
    reinterpret_cast<unsigned char*>(&(*dst)[0]));
}

// -----------------------------------------------------------------------------


} /* end namespace utility */


} /* end namespace sneaker */
