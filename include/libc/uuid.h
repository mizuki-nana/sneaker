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

/* Universally Unique Idetifier */

#ifndef SNEAKER_UUID_H_
#define SNEAKER_UUID_H_

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


/*
 * struct __sneaker_uuid_s
 * A 16-bytes structure
 */
struct __sneaker_uuid_s {
  char data[16];
};


typedef struct __sneaker_uuid_s uuid128_t;


uuid128_t uuid_create();

int uuid_compare(uuid128_t lhs, uuid128_t rhs);

__uint128_t uuid_to_hash(uuid128_t);

__uint128_t uuid_create_and_hash();


#ifdef __cplusplus
}
#endif


#endif /* SNEAKER_UUID_H_ */
