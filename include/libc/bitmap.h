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

/* 2-Dimensional bitmap. */

#ifndef SNEAKER_BITMAP_H_
#define SNEAKER_BITMAP_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct __sneaker_bitmap_s * bitmap_t;


bitmap_t bitmap_create(size_t width, size_t height);

void bitmap_free(bitmap_t *bitmap);

size_t bitmap_width(bitmap_t bitmap);

size_t bitmap_height(bitmap_t bitmap);

int bitmap_set_bit(bitmap_t bitmap, size_t row, size_t col);

int bitmap_clear_bit(bitmap_t bitmap, size_t row, size_t col);

int bitmap_is_set(bitmap_t bitmap, size_t row, size_t col);

void bitmap_clear(bitmap_t bitmap);


#ifdef __cplusplus
}
#endif


#endif /* SNEAKER_BITMAP_H_ */

