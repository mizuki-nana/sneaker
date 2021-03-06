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

/* Stack - FILO container */

#ifndef SNEAKER_STACK_H_
#define SNEAKER_STACK_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct __sneaker_stack_s * sstack_t;


sstack_t stack_create();

size_t stack_size(sstack_t stack);

void* sstack_top(sstack_t stack);

int stack_push(sstack_t stack, void *val, size_t size);

void* stack_pop(sstack_t stack);

void stack_free(sstack_t *stack);


#ifdef __cplusplus
}
#endif


#endif /* SNEAKER_STACK_H_ */
