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
#include "../../include/libc/stack.h"

#include "../../include/libc/common.h"
#include "../../include/libc/memory.h"
#include "../../include/libc/utils.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>


// -----------------------------------------------------------------------------

struct __sneaker_stack_s {
  singly_node_t top;
  size_t size;
};

// -----------------------------------------------------------------------------

sstack_t stack_create()
{
  sstack_t stack = MALLOC(struct __sneaker_stack_s);

  if (stack == NULL) {
    errno = ENOMEM;
    return NULL;
  }

  stack->top = NULL;
  stack->size = 0;

  return stack;
}

// -----------------------------------------------------------------------------

size_t stack_size(sstack_t stack)
{
  assert(stack);
  return stack->size;
}

// -----------------------------------------------------------------------------

void* sstack_top(sstack_t stack)
{
  assert(stack);
  RETURN_VAL_IF_NULL(stack->top, NULL);
  return stack->top->value;
}

// -----------------------------------------------------------------------------

int stack_push(sstack_t stack, void *val, size_t size)
{
  assert(stack);

  RETURN_VAL_IF_NULL(val, 0);

  singly_node_t node = MALLOC(struct __sneaker_singly_node_s);

  if (!node) {
    errno = ENOMEM;
    return -1;
  }

  memset(node, 0, sizeof(struct __sneaker_singly_node_s));

  node->value = MALLOC_BY_SIZE(size);
  RETURN_VAL_IF_NULL(node->value, -1);

  memcpy(node->value, val, size);
  node->next = NULL;

  if (stack->top) {
    node->next = stack->top;
  }

  stack->top = node;
  stack->size++;

  return 1;
}

// -----------------------------------------------------------------------------

void* stack_pop(sstack_t stack) 
{
  assert(stack);

  RETURN_VAL_IF_NULL(stack->top, NULL);

  singly_node_t top = stack->top;

  assert(top);
  stack->top = top->next;

  void* val = top->value;

  FREE(top);
  stack->size--;

  return val;
}

// -----------------------------------------------------------------------------

void stack_free(sstack_t *stack)
{
  sstack_t _stack = *stack;
  assert(_stack);

  while (stack_size(_stack) > 0) {
    stack_pop(_stack);
  }

  assert(_stack->top == NULL);
  assert(_stack->size == 0);

  FREE(_stack);

  *stack = _stack;
}

// -----------------------------------------------------------------------------
