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

/* General assertion */

#ifndef _ASSERT_H_
#define _ASSERT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ASSERT
  #define ASSERT(expr)                                                         \
    do {                                                                       \
      if((expr) == 0) {                                                        \
        fprintf(stderr, "Assertion %s failed, "                                \
          "at %s[line %d]\n", #expr, __FILE__, __LINE__);                      \
        abort();                                                               \
      }                                                                        \
    } while(0)
#endif

#ifndef ASSERT_STREQ
  #define ASSERT_STREQ(str1, str2)                                             \
    do {                                                                       \
      if(strcmp((str1), (str2)) != 0) {                                        \
        fprintf(stderr, "Assertion of \"%s\" == \"%s\" failed, "               \
          "at %s[line %d]\n", (str1), (str2), __FILE__, __LINE__);             \
        abort();                                                               \
      }                                                                        \
    } while(0)
#endif

#endif /* _ASSERT_H_ */