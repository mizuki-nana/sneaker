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

#include <stdlib.h>
#include "../../include/libc/assert.h"
#include "../../include/libc/c_str.h"
#include "../../include/libc/utils.h"

void
set_nth_bit(int *val, char bit)
{
  int b = (1 << (bit-1));
  *val = *val | b;
}

void
clear_nth_bit(int *val, char bit)
{
  int b = (1 << (bit-1));
  *val = *val & (~b);
}

int
is_bit_set(int val, char bit)
{
  unsigned int v = val;
  int b = (1 << (bit-1));
  v = v & b;
  return (v >> (bit-1)) == 1;
}

void
set_nth_bit_uint32(uint32_t *val, char bit)
{
  uint32_t b = (1 << (bit-1));
  *val = *val | b;
}

void
clear_nth_bit_uint32(uint32_t *val, char bit)
{
  uint32_t b = (1 << (bit-1));
  *val = *val & (~b);
}

int
is_bit_set_uint32(uint32_t val, char bit)
{
  uint32_t v = val;
  uint32_t b = (1 << (bit-1));
  v = v & b;
  return (v >> (bit-1)) == 1;
}

int
inline rand_top(int max)
{
  return rand() % max + 1;
}

int
inline rand_range(int min, int max)
{
  int _rand = rand_top(max);
  return MAX(min, _rand);
}

double
inline randf_top(double max)
{
  double f = (double)rand() / RAND_MAX;
  double _rand = f * max;

  if(_rand < 1.0f) {
    _rand += 1.0f;
  }

  return _rand;
}

double
inline randf_range(double min, double max)
{
  double f = (double)rand() / RAND_MAX;
  return min + f * (max - min);
}

static
inline char _rand_ascii()
{
  return '0' + rand_top(127);
}

c_str
generate_text(size_t len, size_t max)
{
  c_str text =  NULL;
  size_t _len = 0;

  if(len && max) {
    _len = rand_range(len, max); 
  } else if (len && !max) {
    _len = len;
  } else if (!len && max) {
    _len = rand_range(1, max);
  } else {
    return NULL;
  }

  text = (c_str)malloc(_len+1);
  memset(text, 0, _len+1);

  int i;
  for(i = 0; i < _len; i++) {
    text[i] = _rand_ascii();
  }

  return text;
}

c_str
generate_loremipsum()
{
  char ipsum[] = "Lorem ipsum dolor sit amet, consectetur adipisicing elit, "
    "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
    "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi "
    "ut aliquip ex ea commodo consequat. Duis aute irure dolor in "
    "reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla "
    "pariatur. Excepteur sint occaecat cupidatat non proident, sunt in "
    "culpa qui officia deserunt mollit anim id est laborum.";

  return strdup(ipsum);
}