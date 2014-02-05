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
#include "../../include/libc/stack.h"


struct HexStack_s {
  SinglyNode top;
  size_t size;
};


Stack stack_create()
{
  Stack stack = MALLOC(struct HexStack_s);

  if(stack == NULL) {
    errno = ENOMEM;
    return NULL;
  }

  stack->top = NULL;
  stack->size = 0;

  return stack;
}

size_t stack_size(Stack stack)
{
  ASSERT(stack);
  return stack->size;
}

void* stack_top(Stack stack)
{
  ASSERT(stack);
  RETURN_VAL_IF_NULL(stack->top, NULL);
  return stack->top->value;
}

int stack_push(Stack stack, void *val, size_t size)
{
  ASSERT(stack);

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

  if(stack->top) {
    node->next = stack->top;
  }

  stack->top = node;
  stack->size++;

  return 1;
}

void* stack_pop(Stack stack) 
{
  ASSERT(stack);

  RETURN_VAL_IF_NULL(stack->top, NULL);

  SinglyNode top = stack->top;

  ASSERT(top);
  stack->top = top->next;

  void* val = top->value;

  FREE(top);
  stack->size--;

  return val;
}

void stack_free(Stack *stack)
{
  Stack _stack = *stack;
  ASSERT(_stack);

  while(stack_size(_stack) > 0) {
    stack_pop(_stack);
  }

  ASSERT(_stack->top == NULL);
  ASSERT(_stack->size == 0);

  FREE(_stack);

  *stack = _stack;
}