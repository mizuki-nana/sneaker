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

/* Helper functions to manipulate null-terminated C strings. */

#include <stddef.h>

#ifndef SNEAKER_STRUTILS_H_
#define SNEAKER_STRUTILS_H_

#ifdef __cplusplus
extern "C" {
#endif


char* strtoupper(char *s);

char* strtolower(char *s);

char* strtrim(char *s);

char* strcpy_hard(char *dst, const char *src);

char* strncpy_safe(char *dst, const char *src, size_t size);

size_t strlcpy2(char *dst, const char *src, size_t size);


#ifdef __cplusplus
}
#endif


#endif /* SNEAKER_STRUTILS_H_ */