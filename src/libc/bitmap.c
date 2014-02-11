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

#include <stddef.h>
#include <math.h>
#include "../../include/libc/assert.h"
#include "../../include/libc/bitmap.h"
#include "../../include/libc/memory.h"
#include "../../include/libc/utils.h"


#define ROW(row) floor((double)(row) / sizeof(char))
#define COL(col) floor((double)(col) / sizeof(char))

struct __sneaker_bitmap_s {
  char **_content;
  size_t _width;
  size_t _height;
};


bitmap_t bitmap_create(size_t width, size_t height)
{
  RETURN_VAL_IF_FALSE(width > 0, NULL);
  RETURN_VAL_IF_FALSE(height > 0, NULL);

  bitmap_t bitmap = MALLOC(struct __sneaker_bitmap_s);

  if(!bitmap) {
    errno = ENOMEM;
    return NULL;
  }

  unsigned int _rows = ceil((double)height / sizeof(char));
  unsigned int _cols = ceil((double)width / sizeof(char));
  int i;

  char **_content = (char**)calloc(_rows, sizeof(char*));

  if(!bitmap) {
    errno = ENOMEM;
    return NULL;
  }

  for(i = 0; i < _rows; i++) {
    _content[i] = (char*)calloc(_cols, sizeof(char));
    memset(_content[i], 0, _cols);
  }

  bitmap->_content = _content;
  bitmap->_width = width;
  bitmap->_height = height;

  return bitmap;
}

void bitmap_free(bitmap_t *bitmap)
{
  bitmap_t _bitmap = *bitmap;

  ASSERT(_bitmap);

  int i;
  for(i = 0; i < _bitmap->_height; i++) {
    FREE(_bitmap->_content[i]);
  }

  FREE(_bitmap);

  *bitmap = _bitmap;
}

size_t bitmap_width(bitmap_t bitmap)
{
  ASSERT(bitmap);
  return bitmap->_width;
}

size_t bitmap_height(bitmap_t bitmap)
{
  ASSERT(bitmap);
  return bitmap->_height;
}

int bitmap_set_bit(bitmap_t bitmap, size_t row, size_t col)
{
  ASSERT(bitmap);

  RETURN_VAL_IF_FALSE(bitmap->_width > col, 0);
  RETURN_VAL_IF_FALSE(bitmap->_height > row, 0);

  size_t r = ROW(row);
  size_t c = COL(col);

  int val = (int)bitmap->_content[r][c];

  /*
   * row % sizeof(char) - MSB [0,1,2,3,4,5,6,7] LSB
   * set_nth_bit        - MSB [8,7,6,5,4,3,2,1] LSB
   */
  int bit = sizeof(char) - row % sizeof(char);

  ASSERT((bit >= 0));
  ASSERT((bit <= sizeof(char)));

  set_nth_bit(&val, bit);

  bitmap->_content[r][c] = (char)val;

  return 1;
}

int bitmap_clear_bit(bitmap_t bitmap, size_t row, size_t col)
{
  ASSERT(bitmap);

  RETURN_VAL_IF_FALSE(bitmap->_width > col, 0);
  RETURN_VAL_IF_FALSE(bitmap->_height > row, 0);

  size_t r = ROW(row);
  size_t c = COL(col);

  int val = (int)bitmap->_content[r][c];

  int bit = sizeof(char) - row % sizeof(char);

  ASSERT((bit >= 0));
  ASSERT((bit <= sizeof(char)));

  clear_nth_bit(&val, bit);

  bitmap->_content[r][c] = (char)val;

  return 1;
}

int bitmap_is_set(bitmap_t bitmap, size_t row, size_t col)
{
  ASSERT(bitmap);

  RETURN_VAL_IF_FALSE(bitmap->_width > col, 0);
  RETURN_VAL_IF_FALSE(bitmap->_height > row, 0);

  size_t r = ROW(row);
  size_t c = COL(col);

  int val = (int)bitmap->_content[r][c];

  int bit = sizeof(char) - row % sizeof(char);

  ASSERT((bit >= 0));
  ASSERT((bit <= sizeof(char)));

  return is_bit_set(val, bit);
}

void bitmap_clear(bitmap_t bitmap)
{
  ASSERT(bitmap);

  int i;
  for(i = 0; i < bitmap->_height; i++) {
    char *row = bitmap->_content[i];
    memset(row, 0, bitmap->_width);
  }
}