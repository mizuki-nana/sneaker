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

/* Generic hashmap */

#ifndef SNEAKER_HASHMAP_H_
#define SNEAKER_HASHMAP_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct __sneaker_hashmap_s * hashmap_t;


/* Hash function. */
typedef unsigned long int(*HashFunc)(void*);


/* Key comparison function. */
typedef int(*KeyCmpFunc)(void*, void*);


hashmap_t hashmap_create(size_t initial_capacity,
  HashFunc hashfunc, KeyCmpFunc keycmpfunc);

size_t hashmap_size(hashmap_t hashmap);

void hashmap_lock(hashmap_t hashmap);

void hashmap_unlock(hashmap_t hashmap);

void hashmap_free(hashmap_t* hashmap_free);

void* hashmap_put(hashmap_t hashmap, void* key, void* val);

void* hashmap_get(hashmap_t hashmap, void* key);

int hashmap_contains_key(hashmap_t hashmap, void* key);

void* hashmap_remove(hashmap_t hashmap, void* key);

void* hashmap_lookup(hashmap_t hashmap, int(*lookup)(void*, void*, void*), void* arg);

void hashmap_iterate(hashmap_t hashmap, int(*callback)(void*, void*), int halt_on_fail);

size_t hashmap_bucketcount(hashmap_t hashmap);

size_t hashmap_capacity(hashmap_t hashmap);

size_t hashmap_count_collisions(hashmap_t hashmap);

int hashmap_int_equals(void* key_a, void* key_b);

int hashmap_equal(hashmap_t lhs, hashmap_t rhs);


#ifdef __cplusplus
}
#endif


#endif /* SNEAKER_HASHMAP_H_ */
