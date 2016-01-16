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
#include "../../include/libc/hashmap.h"

#include "../../include/libc/memory.h"
#include "../../include/libc/utils.h"

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

#include <sys/types.h>


// -----------------------------------------------------------------------------

#define LOAD_FACTOR 0.75f

// -----------------------------------------------------------------------------

#define HASHMAP_DEFAULT_BUCKETCOUNT 8

// -----------------------------------------------------------------------------

/* Hashmap entry(bucket) */
typedef struct __sneaker_hashmap_entry_s {
  void *key;
  unsigned long int hash;
  void *value;
  struct __sneaker_hashmap_entry_s *next;
} * hashmap_entry_t;

// -----------------------------------------------------------------------------

struct __sneaker_hashmap_s {
  hashmap_entry_t * buckets;
  size_t bucketCount;
  HashFunc hash;
  KeyCmpFunc keycmp;
  pthread_mutex_t lock;
  size_t size;
};

// -----------------------------------------------------------------------------

hashmap_t hashmap_create(size_t initial_capacity,
  HashFunc hashfunc, KeyCmpFunc keycmpfunc)
{
  assert(hashfunc);
  assert(keycmpfunc);

  hashmap_t hashmap = NULL;
  hashmap = MALLOC(struct __sneaker_hashmap_s);

  if (!hashmap) {
    errno = ENOMEM;
    return NULL;
  }

  size_t min_bucket_count = initial_capacity * LOAD_FACTOR;

  hashmap->bucketCount = 1;

  while (hashmap->bucketCount <= min_bucket_count) {
    /* bucket count must be power of 2 */
    hashmap->bucketCount <<= 1; 
  }

  hashmap->buckets = calloc(
    hashmap->bucketCount,
    sizeof(struct __sneaker_hashmap_entry_s)
  );

  if (!hashmap->buckets) {
    FREE(hashmap);
    return NULL;
  }

  hashmap->size = 0;
  hashmap->hash = hashfunc;
  hashmap->keycmp = keycmpfunc;

  pthread_mutex_init(&hashmap->lock, 0);

  return hashmap;
}

// -----------------------------------------------------------------------------

/* Secondary hashing against bad hashses. */
static
inline unsigned long int _hash_key(hashmap_t hashmap, void *key)
{
  assert(hashmap);

  RETURN_VAL_IF_NULL(key, -1);

  unsigned long int h = hashmap->hash(key);

  h += ~(h << 9);
  h ^= (((unsigned int) h) >> 14);
  h += (h << 4);
  h ^= (((unsigned int) h) >> 10);

  return h;
}

// -----------------------------------------------------------------------------

size_t hashmap_size(hashmap_t hashmap)
{
  assert(hashmap);
  return hashmap->size;
}

// -----------------------------------------------------------------------------

static
inline size_t _calculate_index(size_t bucketCount, int hash)
{
  return ((size_t)hash) & (bucketCount - 1);
}

// -----------------------------------------------------------------------------

static
void _hashmap_expand(hashmap_t hashmap)
{
  assert(hashmap);

  if (hashmap->size > (hashmap->bucketCount * LOAD_FACTOR)) {
    size_t newBucketCount = hashmap->bucketCount << 1;

    hashmap_entry_t *newBuckets = calloc(
      newBucketCount, sizeof(struct __sneaker_hashmap_entry_s));

    RETURN_IF_NULL(newBuckets);

    size_t i;
    for (i = 0; i < hashmap->bucketCount; ++i) {
      hashmap_entry_t entry = hashmap->buckets[i];

      while (entry) {
        hashmap_entry_t next = entry->next;
        size_t index = _calculate_index(newBucketCount, entry->hash);
        entry->next = newBuckets[index];
        newBuckets[index] = entry;
        entry = next;
      }
    }

    FREE(hashmap->buckets);
    hashmap->buckets = newBuckets;
    hashmap->bucketCount = newBucketCount;
  }
}

// -----------------------------------------------------------------------------

void hashmap_lock(hashmap_t hashmap)
{
  assert(hashmap);
  pthread_mutex_lock(&hashmap->lock);
}

// -----------------------------------------------------------------------------

void hashmap_unlock(hashmap_t hashmap)
{
  assert(hashmap);
  pthread_mutex_unlock(&hashmap->lock);
}

// -----------------------------------------------------------------------------

void hashmap_free(hashmap_t *hashmap)
{
  assert(*hashmap);

  hashmap_t _hashmap = *hashmap;

  size_t i;
  for (i = 0; i < _hashmap->bucketCount; ++i) {
    hashmap_entry_t entry = _hashmap->buckets[i];
    while (entry) {
      hashmap_entry_t next = entry->next;
      FREE(entry);
      entry = next;
    }
  }

  FREE(_hashmap->buckets);
  pthread_mutex_destroy(&_hashmap->lock);

  FREE(_hashmap);
  *hashmap = NULL;
}

// -----------------------------------------------------------------------------

static
hashmap_entry_t _create_entry(void *key, unsigned long int hash, void *val)
{
  hashmap_entry_t entry=NULL;
  entry = MALLOC(struct __sneaker_hashmap_entry_s);

  if (entry == NULL) {
    errno = ENOMEM;
    return NULL;
  }

  entry->key = key;
  entry->hash = hash;
  entry->value = val;
  entry->next = 0;

  return entry;
}

// -----------------------------------------------------------------------------

static
inline int _equals_key(
  void *keyA,
  unsigned long int hashA,
  void *keyB,
  unsigned long int hashB,
  KeyCmpFunc keycmp
)
{
  RETURN_VAL_IF_EQ(hashA, hashB, 1);
  return keycmp(keyA, keyB);
}

// -----------------------------------------------------------------------------

void* hashmap_put(hashmap_t hashmap, void *key, void *val)
{
  assert(hashmap);

  RETURN_VAL_IF_NULL(key, NULL);
  RETURN_VAL_IF_NULL(val, NULL);

  unsigned long int hash = _hash_key(hashmap, key);
  size_t index = _calculate_index(hashmap->bucketCount, hash);

  hashmap_entry_t *p = &(hashmap->buckets[index]);
  assert(p);

  while (1) {
    hashmap_entry_t current = *p;

    if (!current) {
      *p = _create_entry(key, hash, val);

      if (*p == NULL) {
        errno = ENOMEM;
        return NULL;
      }

      hashmap->size++;
      _hashmap_expand(hashmap);

      index = _calculate_index(hashmap->bucketCount, hash);
      p = &(hashmap->buckets[index]);
      current = *p;

      // TODO: clean up...
      // assert(p);
      // assert(*p);
      // assert((*p)->value);

      return (*p)->value;
    }

    if (_equals_key(
      current->key,
      current->hash,
      key,
      hash,
      hashmap->keycmp)
    ) {
      void *oldValue = current->value;
      current->value = val;
      return oldValue;
    }

    p = &current->next;
  }

  return NULL;
}

// -----------------------------------------------------------------------------

void* hashmap_get(hashmap_t hashmap, void *key)
{
  assert(hashmap);

  RETURN_VAL_IF_NULL(key, NULL);
  RETURN_VAL_IF_TRUE(hashmap->size == 0, NULL);

  unsigned long int hash = _hash_key(hashmap, key);
  int index = _calculate_index(hashmap->bucketCount, hash);

  hashmap_entry_t *p = &(hashmap->buckets[index]);
  hashmap_entry_t entry = NULL;

  while ((entry=*p)) {
    if (_equals_key(entry->key, entry->hash, key, hash, hashmap->keycmp)) {
      return entry->value;
    }
    p = &(entry->next);
  }

  return NULL;
}

// -----------------------------------------------------------------------------

int
hashmap_contains_key(hashmap_t hashmap, void *key)
{
  assert(hashmap);

  RETURN_VAL_IF_NULL(key, 0);

  unsigned long int hash = _hash_key(hashmap, key);
  int index = _calculate_index(hashmap->bucketCount, hash);

  hashmap_entry_t entry = hashmap->buckets[index];

  while (entry) {
    if (_equals_key(entry->key, entry->hash, key, hash, hashmap->keycmp)) {
      return 1;
    }
    entry = entry->next;
  }

  return 0;
}

// -----------------------------------------------------------------------------

int hashmap_remove_bucket(hashmap_t hashmap, void *key)
{
  assert(hashmap);

  RETURN_VAL_IF_NULL(key, 0);

  unsigned long int hash = _hash_key(hashmap, key);
  int index = _calculate_index(hashmap->bucketCount, hash);

  hashmap_entry_t *p = &(hashmap->buckets[index]);
  hashmap_entry_t current = NULL;

  while ((current = *p)) {
    p = &current->next;
    FREE(current);
  }

  hashmap->buckets[index] = NULL;

  _hashmap_expand(hashmap);

  return 1;
}

// -----------------------------------------------------------------------------

void* hashmap_remove(hashmap_t hashmap, void *key)
{
  assert(hashmap);

  RETURN_VAL_IF_NULL(key, NULL);

  unsigned long int hash = _hash_key(hashmap, key);
  int index = _calculate_index(hashmap->bucketCount, hash);

  hashmap_entry_t *p = &(hashmap->buckets[index]);
  hashmap_entry_t current = NULL;

  while ((current = *p)) {
    if (_equals_key(current->key, current->hash, key, hash, hashmap->keycmp)) {
      void *value = current->value;
      *p = current->next;
      FREE(current);
      hashmap->size--;
      return value;
    }

    p = &(current->next);
  }

  return NULL;
}

// -----------------------------------------------------------------------------

void* hashmap_lookup(hashmap_t hashmap,
  int(*lookup)(void *key, void *value, void* arg), void *arg)
{
  assert(hashmap);
  assert(lookup);

  size_t i;
  for (i = 0; i < hashmap->bucketCount; i++) {
    hashmap_entry_t entry = hashmap->buckets[i];
    while (entry != NULL) {
      hashmap_entry_t next = entry->next;
      if (lookup(entry->key, entry->value, arg)) {
        return entry->value;
      }
      entry = next;
    }
  } /* end of for-loop */

  return NULL;
}

// -----------------------------------------------------------------------------

void hashmap_iterate(hashmap_t hashmap,
    int(*callback)(void *key, void *value), int haltOnFail)
{
  assert(hashmap);
  assert(callback);

  size_t i;
  for (i = 0; i < hashmap->bucketCount; i++) {
    hashmap_entry_t entry = hashmap->buckets[i];
    while (entry != NULL) {
      hashmap_entry_t next = entry->next;
      if (!callback(entry->key, entry->value) && haltOnFail) {
        return;
      }
      entry = next;
    }
  } /* end of for-loop */
}

// -----------------------------------------------------------------------------

size_t hashmap_bucketcount(hashmap_t hashmap)
{
  assert(hashmap);
  return hashmap->bucketCount;
}

// -----------------------------------------------------------------------------

size_t hashmap_capacity(hashmap_t hashmap)
{
  assert(hashmap);
  size_t bucketCount = hashmap->bucketCount;
  return bucketCount * LOAD_FACTOR;
}

// -----------------------------------------------------------------------------

size_t hashmap_count_collisions(hashmap_t hashmap)
{
  assert(hashmap);

  size_t collisions = 0;
  size_t i;

  for (i = 0; i < hashmap->bucketCount; ++i) {
    hashmap_entry_t entry = hashmap->buckets[i];
    while (entry) {
      if (entry->next) {
        collisions++;
      }
      entry = entry->next;
    }
  }

  return collisions;
}

// -----------------------------------------------------------------------------

int
hashmap_int_equals(void *keyA, void *keyB)
{
  assert(keyA);
  assert(keyB);

  int a = DEREF_VOID(int, keyA);
  int b = DEREF_VOID(int, keyB);

  return a == b;
}

// -----------------------------------------------------------------------------

int hashmap_equal(hashmap_t hashmap1, hashmap_t hashmap2)
{
  assert(hashmap1);
  assert(hashmap2);

  if (hashmap_bucketcount(hashmap1) != hashmap_bucketcount(hashmap2)) {
    return 0;
  }

  int i;
  for (i = 0; i < hashmap1->bucketCount; i++) {
    hashmap_entry_t entry = hashmap1->buckets[i];
    while (entry != NULL) {
      hashmap_entry_t next = entry->next;
      if (hashmap_get(hashmap2, entry->key)==NULL) {
        return 0;
      }
      entry = next;
    }
  } /* end of for-loop */

  for (i = 0; i < hashmap2->bucketCount; i++) {
    hashmap_entry_t entry = hashmap2->buckets[i];
    while (entry != NULL) {
      hashmap_entry_t next = entry->next;
      if (hashmap_get(hashmap1, entry->key)==NULL) {
        return 0;
      }
      entry = next;
    }
  } /* end of for-loop */

  return 1;
}

// -----------------------------------------------------------------------------
