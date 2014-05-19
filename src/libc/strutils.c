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

#include <ctype.h>
#include <stdlib.h>
#include "../../include/libc/utils.h"
#include "../../include/libc/memory.h"
#include "../../include/libc/strutils.h"


char* strtoupper(char *s)
{
  RETURN_VAL_IF_NULL(s, NULL);

  char *str = s;

  while(*str != '\0') {
    *str = toupper(*str);
    str++;
  }

  return s;
}

char* strtolower(char *s)
{
  RETURN_VAL_IF_NULL(s, NULL);

  char *str = s;

  while(*str) {
    *str = tolower(*str);
    str++;
  }
  return s;
}

char* strtrim(char *s)
{
  RETURN_VAL_IF_NULL(s, NULL);

  size_t l = strlen(s);
  while(l > 1 && isspace(s[l-1])) s[--l] = '\0';
  while(*s && isspace(*s)) *s = '\0', ++s;
  return s;
}

char* strcpy_hard(char *dst, const char *src)
{
  RETURN_VAL_IF_NULL(src, dst);

  size_t srclen = strlen(src);
  size_t dstlen = dst ? strlen(dst) : 0;
  size_t len = MAX(dstlen, srclen);

  if(dst) {
    free(dst);
  }

  dst = (char*)malloc(len+1);
  RETURN_VAL_IF_NULL(dst, NULL);

  strcpy(dst, src);

  return dst;
}

char* strncpy_safe(char *dst, const char *src, size_t size)
{
  RETURN_VAL_IF_NULL(dst, NULL);
  RETURN_VAL_IF_NULL(src, NULL);

  size_t outlen = MIN(strlen(src), size);

  return strncpy(dst, src, outlen);
}

size_t strlcpy2(char *dst, const char *src, size_t size)
{
  RETURN_VAL_IF_NULL(dst, 0);
  RETURN_VAL_IF_NULL(src, 0);

  char *d = dst;
  const char *s = src;
  size_t n = size;

  if(n > 0) {
    while(--n != 0) {
      if((*d++ = *s++) == '\0') {
        break;
      }
    }
  }

  if(n == 0) {
    if(size != 0) {
      *d = '\0';
    }
    while(*s++) {} // do nothing
  }

  return s - src - 1;
}