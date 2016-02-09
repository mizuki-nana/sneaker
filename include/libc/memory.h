/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2016 Yanzheng Li

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
#ifndef SNEAKER_MEMORY_H_
#define SNEAKER_MEMORY_H_

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif


//===========================================================================
// convenience malloc macro.
// USE WITH CAUTION
//===========================================================================
#ifndef MALLOC

  #define MALLOC(T)                    \
    (T*)malloc(sizeof(T))

#endif /* MALLOC */


//===========================================================================
// convenience malloc macro.
// USE WITH CAUTION
//===========================================================================
#ifndef MALLOC_BY_SIZE

  #define MALLOC_BY_SIZE(size)         \
    malloc((size))

#endif /* MALLOC_BY_SIZE */


//===========================================================================
// Convenience malloc macro.
// If malloc fails, set errno to ENOMEM and return null.
// USE WITH CAUTION
//===========================================================================
#ifndef MALLOC_OR_ENOMEM

  #define MALLOC_OR_ENOMEM(T, obj)     \
    (obj) = (T*)malloc(sizeof((T)));   \
    do {                               \
      if ((obj) == NULL) {             \
        errno = ENOMEM;                \
      }                                \
    } while (0)

#endif /* MALLOC_OR_ENOMEM */


//===========================================================================
// Derefernece a void* pointer to a data of specified type.
//
// T: The specified data type to be dereferenced.
// v: the value of type void*
//
// e.g.
// void deref_example(void* value) {
//   int i = *((int*)value);
// }
//
// can now be written as:
//
// void deref_example(void* value) {
//   int i = DEREF_VOID(int, value);
// }
//
// USE WITH CAUTION
//===========================================================================
#ifndef DEREF_VOID

  #define DEREF_VOID(T, v)             \
    *( (T*)( (v) ) );

#endif /* DEREF_VOID */


//===========================================================================
// Free-up memory.
// USE WITH CAUTION
//===========================================================================
#ifndef FREE

  #define FREE(x)                      \
    do {                               \
      if ( (x) ) {                     \
        free( (x) );                   \
        (x) = NULL;                    \
      }                                \
    } while (0)

#endif /* FREE */


//===========================================================================
// Free-up memory.
// USE WITH CAUTION
//===========================================================================
#ifndef DELETE

  #define DELETE(x)                    \
    do {                               \
      if ( (x) ) {                     \
        delete (x);                    \
        (x) = NULL;                    \
      }                                \
    } while (0)

#endif /* DELETE */


//===========================================================================
// Free target if val is null.
// USE WITH CAUTION
//===========================================================================
#ifndef FREE_IF_NULL

  #define FREE_IF_NULL(val, target)    \
    do {                               \
      if ( (val) == NULL) {            \
        FREE( (target) );              \
      }                                \
    } while (0)

#endif /* FREE_IF_NULL */


#ifdef __cplusplus
}
#endif


#endif /* SNEAKER_MEMORY_H_ */

