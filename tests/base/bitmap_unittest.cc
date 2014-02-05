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

/* Unit test for src/base/bitmap.c */

#include <stddef.h>
#include "../_unittest.h"
#include "../../src/base/assert.h"
#include "../../src/base/c_str.h"
#include "../../src/base/memory.h"
#include "../../src/base/bitmap.h"

TEST(bitmapTest, bitmap_createTest) {
 Bitmap bitmap = NULL;

 bitmap = bitmap_create(0, 10);
 ASSERT(bitmap == NULL);

 bitmap = bitmap_create(1, 0);
 ASSERT(bitmap == NULL);

 bitmap = bitmap_create(0, 0);
 ASSERT(bitmap == NULL);
}

TEST(bitmapTest, bitmap_createTest2) {
  Bitmap bitmap = NULL;
  size_t width = 1;
  size_t height = 1;

  bitmap = bitmap_create(width, height);
  ASSERT(bitmap);

  ASSERT_EQ(width, bitmap_width(bitmap));
  ASSERT_EQ(height, bitmap_height(bitmap));

  bitmap_free(&bitmap);

  ASSERT(bitmap == NULL);
}

TEST(bitmapTest, bitmap_set_bitTest1) {
  Bitmap bitmap = NULL;
  size_t width = 1;
  size_t height = 1;
  int res;

  bitmap = bitmap_create(width, height);
  ASSERT(bitmap);

  ASSERT_EQ(width, bitmap_width(bitmap));
  ASSERT_EQ(height, bitmap_height(bitmap));

  res = bitmap_set_bit(bitmap, 1, 0);
  ASSERT_EQ(0, res);

  bitmap_free(&bitmap);
  ASSERT(bitmap == NULL);
}

TEST(bitmapTest, bitmap_set_bitTest2) {
  Bitmap bitmap = NULL;
  size_t width = 1;
  size_t height = 1;
  int res;

  bitmap = bitmap_create(width, height);
  ASSERT(bitmap);

  ASSERT_EQ(width, bitmap_width(bitmap));
  ASSERT_EQ(height, bitmap_height(bitmap));

  res = bitmap_set_bit(bitmap, 0, 1);
  ASSERT_EQ(0, res);

  bitmap_free(&bitmap);
  ASSERT(bitmap == NULL);
}

TEST(bitmapTest, bitmap_set_bitTest3) {
  Bitmap bitmap = NULL;
  size_t width = 1;
  size_t height = 1;
  int res;

  bitmap = bitmap_create(width, height);
  ASSERT(bitmap);

  ASSERT_EQ(width, bitmap_width(bitmap));
  ASSERT_EQ(height, bitmap_height(bitmap));

  res = bitmap_set_bit(bitmap, 0, 0);
  ASSERT_EQ(1, res);

  res = bitmap_is_set(bitmap, 0, 0);
  ASSERT_EQ(1, res);

  bitmap_free(&bitmap);
  ASSERT(bitmap == NULL);
}

TEST(bitmapTest, BitmapSetAndClearBitTest1) {
  Bitmap bitmap = NULL;
  size_t width = 2;
  size_t height = 2;

  bitmap = bitmap_create(width, height);
  ASSERT(bitmap);

  ASSERT_EQ(width, bitmap_width(bitmap));
  ASSERT_EQ(height, bitmap_height(bitmap));

  ASSERT_EQ(1, bitmap_set_bit(bitmap, 0, 0));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 1, 0));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 0, 1));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 1, 1));

  ASSERT_EQ(1, bitmap_is_set(bitmap, 0, 0));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 1, 0));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 0, 1));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 1, 1));

  // now clear [1,0] and [0, 1]
  bitmap_clear_bit(bitmap, 1, 0);
  bitmap_clear_bit(bitmap, 0, 1);

  ASSERT_EQ(1, bitmap_is_set(bitmap, 0, 0));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 1, 0));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 0, 1));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 1, 1));

  bitmap_free(&bitmap);
  ASSERT(bitmap == NULL);
}

TEST(bitmapTest, BitmapSetAndClearBitTest2) {
  Bitmap bitmap = NULL;
  size_t width = 8;
  size_t height = 8;

  bitmap = bitmap_create(width, height);
  ASSERT(bitmap);

  ASSERT_EQ(width, bitmap_width(bitmap));
  ASSERT_EQ(height, bitmap_height(bitmap));

  ASSERT_EQ(1, bitmap_set_bit(bitmap, 0, 0));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 1, 1));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 2, 2));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 3, 3));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 4, 4));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 5, 5));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 6, 6));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 7, 7));
  ASSERT_EQ(0, bitmap_set_bit(bitmap, 8, 8));

  ASSERT_EQ(1, bitmap_is_set(bitmap, 0, 0));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 1, 1));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 2, 2));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 3, 3));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 4, 4));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 5, 5));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 6, 6));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 7, 7));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 8, 8));
  
  ASSERT_EQ(0, bitmap_is_set(bitmap, 0, 1));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 1, 2));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 2, 3));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 3, 4));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 4, 5));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 5, 6));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 6, 7));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 7, 8));
 
  bitmap_clear_bit(bitmap, 0, 0);
  bitmap_clear_bit(bitmap, 1, 1);
  bitmap_clear_bit(bitmap, 2, 2);
  bitmap_clear_bit(bitmap, 3, 3);
  bitmap_clear_bit(bitmap, 4, 4);
  bitmap_clear_bit(bitmap, 5, 5);
  bitmap_clear_bit(bitmap, 6, 6);
  bitmap_clear_bit(bitmap, 7, 7);
  bitmap_clear_bit(bitmap, 8, 8);
  
  ASSERT_EQ(0, bitmap_is_set(bitmap, 0, 0));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 1, 1));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 2, 2));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 3, 3));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 4, 4));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 5, 5));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 6, 6));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 7, 7));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 8, 8));

  bitmap_free(&bitmap);
  ASSERT(bitmap == NULL);
}

TEST(bitmapTest, BitmapSetAndClearBitTest3) {
  Bitmap bitmap = NULL;
  size_t width = 16;
  size_t height = 16;

  bitmap = bitmap_create(width, height);
  ASSERT(bitmap);

  ASSERT_EQ(width, bitmap_width(bitmap));
  ASSERT_EQ(height, bitmap_height(bitmap));

  ASSERT_EQ(1, bitmap_set_bit(bitmap, 0, 0));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 1, 1));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 2, 2));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 3, 3));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 4, 4));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 5, 5));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 6, 6));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 7, 7));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 8, 8));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 9, 9));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 10, 10));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 11, 11));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 12, 12));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 13, 13));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 14, 14));
  ASSERT_EQ(1, bitmap_set_bit(bitmap, 15, 15));
  ASSERT_EQ(0, bitmap_set_bit(bitmap, 16, 16));

  ASSERT_EQ(1, bitmap_is_set(bitmap, 0, 0));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 1, 1));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 2, 2));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 3, 3));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 4, 4));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 5, 5));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 6, 6));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 7, 7));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 8, 8));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 9, 9));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 10, 10));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 11, 11));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 12, 12));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 13, 13));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 14, 14));
  ASSERT_EQ(1, bitmap_is_set(bitmap, 15, 15));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 16, 16));

  size_t r = 0;
  size_t c = 0;
  for(r = 0; r < height; r++) {
    for(c = 0; c < width; c++) {
      ASSERT_EQ(r == c, bitmap_is_set(bitmap, r, c));
    }
  }
 
  bitmap_clear_bit(bitmap, 0, 0);
  bitmap_clear_bit(bitmap, 1, 1);
  bitmap_clear_bit(bitmap, 2, 2);
  bitmap_clear_bit(bitmap, 3, 3);
  bitmap_clear_bit(bitmap, 4, 4);
  bitmap_clear_bit(bitmap, 5, 5);
  bitmap_clear_bit(bitmap, 6, 6);
  bitmap_clear_bit(bitmap, 7, 7);
  bitmap_clear_bit(bitmap, 8, 8);
  bitmap_clear_bit(bitmap, 9, 9);
  bitmap_clear_bit(bitmap, 10, 10);
  bitmap_clear_bit(bitmap, 11, 11);
  bitmap_clear_bit(bitmap, 12, 12);
  bitmap_clear_bit(bitmap, 13, 13);
  bitmap_clear_bit(bitmap, 14, 14);
  bitmap_clear_bit(bitmap, 15, 15);
  
  ASSERT_EQ(0, bitmap_is_set(bitmap, 0, 0));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 1, 1));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 2, 2));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 3, 3));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 4, 4));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 5, 5));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 6, 6));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 7, 7));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 8, 8));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 9, 9));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 10, 10));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 11, 11));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 12, 12));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 13, 13));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 14, 14));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 15, 15));
  ASSERT_EQ(0, bitmap_is_set(bitmap, 16, 16));

  bitmap_free(&bitmap);
  ASSERT(bitmap == NULL);
}