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
#include "../../include/libc/queue.h"

#include "../../include/libc/common.h"
#include "../../include/libc/memory.h"
#include "../../include/libc/utils.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>


// -----------------------------------------------------------------------------

struct __sneaker_queue_s {
  singly_node_t head;      /* pop at head */
  singly_node_t tail;      /* push at tail */
  size_t size;
};

// -----------------------------------------------------------------------------

queue_t queue_create()
{
  queue_t queue = MALLOC(struct __sneaker_queue_s);

  if (queue == NULL) {
    errno = ENOMEM;
    return NULL;
  }

  queue->head = NULL;
  queue->tail = NULL;
  queue->size = 0;

  return queue;
}

// -----------------------------------------------------------------------------

size_t queue_size(queue_t queue)
{
  assert(queue);
  return queue->size;
}

// -----------------------------------------------------------------------------

void* queue_front(queue_t queue)
{
  assert(queue);
  RETURN_VAL_IF_NULL(queue->head, NULL);
  return queue->head->value;
}

// -----------------------------------------------------------------------------

void* queue_back(queue_t queue)
{
  assert(queue);
  RETURN_VAL_IF_NULL(queue->tail, NULL);
  return queue->tail->value;
}

// -----------------------------------------------------------------------------

int queue_push(queue_t queue, void *val, size_t size)
{
  assert(queue);

  RETURN_VAL_IF_NULL(val, 0);

  singly_node_t node = MALLOC(struct __sneaker_singly_node_s);

  if (!node) {
    errno = ENOMEM;
    return -1;
  }

  memset(node, 0, sizeof(struct __sneaker_queue_s));

  node->value = MALLOC_BY_SIZE(size);
  RETURN_VAL_IF_NULL(node->value, -1);

  memcpy(node->value, val, size);
  node->next = NULL;

  if (queue->tail) {
    queue->tail->next = node;
  }
  queue->tail = node;

  if (queue->size == 0) {
    queue->head = queue->tail;
  }

  queue->size++;

  return 1;
}

// -----------------------------------------------------------------------------

void* queue_pop(queue_t queue)
{
  assert(queue);

  RETURN_VAL_IF_EQ(queue->size, 0, NULL);

  assert(queue->head);

  void *val = queue->head->value;

  singly_node_t next = queue->head->next;

  FREE(queue->head);
  queue->head = next;
  queue->size--;

  if (queue->size == 0) {
    queue->tail = NULL;
    assert(queue->head == NULL);
  }

  return val;
}

// -----------------------------------------------------------------------------

void queue_free(queue_t *queue)
{
  queue_t _queue = *queue;
  assert(_queue);

  while (queue_size(_queue) > 0) {
    queue_pop(_queue);
  }

  assert(_queue->head == NULL);
  assert(_queue->tail == NULL);
  assert(_queue->size == 0);

  FREE(_queue);

  *queue = _queue;
}

// -----------------------------------------------------------------------------
