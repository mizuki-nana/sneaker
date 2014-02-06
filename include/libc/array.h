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

/* Array abstraction */

#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct __sneaker_array_s * array_t;

array_t array_create();

void array_free(array_t *array);

int array_append(array_t array, void* ptr);

void* array_get(array_t array, int index);

void* array_remove(array_t array, int index);

void* array_set(array_t array, int index, void* ptr);

int array_size(array_t array);

const void** array_content(array_t array);


#ifdef __cplusplus
}
#endif


#endif /* _ARRAY_H_ */