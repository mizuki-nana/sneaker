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

#include <errno.h>
#include "../../include/libc/assert.h"
#include "../../include/libc/common.h"
#include "../../include/libc/memory.h"
#include "../../include/libc/utils.h"
#include "../../include/libc/queue.h"


struct HexQueue_s {
  SinglyNode head;      /* pop at head */
  SinglyNode tail;      /* push at tail */
  size_t size;
};


Queue queue_create()
{
  Queue queue = MALLOC(struct HexQueue_s);

  if(queue == NULL) {
    errno = ENOMEM;
    return NULL;
  }

  queue->head = NULL;
  queue->tail = NULL;
  queue->size = 0;

  return queue;
}

size_t queue_size(Queue queue)
{
  ASSERT(queue);
  return queue->size;
}

void* queue_front(Queue queue)
{
  ASSERT(queue);
  RETURN_VAL_IF_NULL(queue->head, NULL);
  return queue->head->value;
}

void* queue_back(Queue queue)
{
  ASSERT(queue);
  RETURN_VAL_IF_NULL(queue->tail, NULL);
  return queue->tail->value;
}

int queue_push(Queue queue, void *val, size_t size)
{
  ASSERT(queue);

  RETURN_VAL_IF_NULL(val, 0);

  SinglyNode node = MALLOC(struct SinglyNode_s);

  if(!node) {
    errno = ENOMEM;
    return -1;
  }

  memset(node, 0, sizeof(struct SinglyNode_s));

  node->value = MALLOC_BY_SIZE(size);
  RETURN_VAL_IF_NULL(node->value, -1);

  memcpy(node->value, val, size);
  node->next = NULL;

  if(queue->tail) {
    queue->tail->next = node;
  }
  queue->tail = node;

  if(queue->size == 0) {
    queue->head = queue->tail;
  }

  queue->size++;

  return 1;
}

void* queue_pop(Queue queue)
{
  ASSERT(queue);

  RETURN_VAL_IF_EQ(queue->size, 0, NULL);

  ASSERT(queue->head);

  void *val = queue->head->value;

  SinglyNode next = queue->head->next;

  FREE(queue->head);
  queue->head = next;
  queue->size--;

  if(queue->size == 0) {
    queue->tail = NULL;
    ASSERT(queue->head == NULL);
  }

  return val;
}

void queue_free(Queue *queue)
{
  Queue _queue = *queue;
  ASSERT(_queue);

  while(queue_size(_queue) > 0) {
    queue_pop(_queue);
  }

  ASSERT(_queue->head == NULL);
  ASSERT(_queue->tail == NULL);
  ASSERT(_queue->size == 0);

  FREE(_queue);

  *queue = _queue;
}