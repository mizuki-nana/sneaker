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

/* Generic hashmap */

#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include <stddef.h>
#include "hash.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct HexHashmap *Hashmap;


/* hash function */
typedef hash_t(*HashFunc)(void *key);


/* key comparison function */
typedef int(*KeyCmpFunc)(void *keyA, void *keyB);


Hashmap hashmap_create(size_t initial_capacity,
  HashFunc hashfunc, KeyCmpFunc keycmpfunc);

size_t hashmap_size(Hashmap hashmap);

void hashmap_lock(Hashmap hashmap);

void hashmap_unlock(Hashmap hashmap);

void hashmap_free(Hashmap *hashmap);

int hashmap_hash(void *key, size_t key_size);

void* hashmap_put(Hashmap hashmap, void *key, void *val);

void* hashmap_get(Hashmap hashmap, void *key);

int hashmap_contains_key(Hashmap hashmap, void *key);

void* hashmap_remove(Hashmap hashmap, void *key);

void* hashmap_lookup(
  Hashmap hashmap, int(*lookup)(void *key, void *value, void* arg), void *arg);

void hashmap_iterate(
  Hashmap hashmap, int(*callback)(void *key, void *value), int haltOnFail);

size_t hashmap_bucketcount(Hashmap hashmap);

size_t hashmap_capacity(Hashmap hashmap);

size_t hashmap_count_collisions(Hashmap hashmap);

int hashmap_int_equals(void *keyA, void *keyB);

int hashmap_equal(Hashmap hashmap1, Hashmap hashmap2);


#ifdef __cplusplus
}
#endif

#endif /* _HASHMAP_H_ */