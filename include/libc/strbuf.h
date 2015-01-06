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

/* Generic string buffer */

#ifndef SNEAKER_STRBUF_H_
#define SNEAKER_STRBUF_H_

#include "c_str.h"

#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct __sneaker_strbuf_s * strbuf_t;

strbuf_t strbuf_create();

void strbuf_free(strbuf_t *strbuf);

void strbuf_empty(strbuf_t strbuf);

size_t strbuf_len(strbuf_t strbuf);

const c_str strbuf_cstr(strbuf_t strbuf);

size_t strbuf_capacity(strbuf_t strbuf);

int strbuf_append(strbuf_t strbuf, const c_str in_str);


#ifdef __cplusplus
}
#endif


#endif /* SNEAKER_STRBUF_H_ */
