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

/* Buffer abstraction */

#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <stddef.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
 * Byte buffer of known size. Keeps track of how much data has been read
 * into or written out of the buffer.
 */
typedef struct HexBuffer_s *Buffer;

/*
 * Returns true if all data has been read into the buffer.
 */
#define buffer_read_complete(buffer) ((buffer)->expected == (buffer)->size)

/*
 * Returns true if the buffer has been completely written.
 */
#define buffer_write_complete(buffer) ((buffer)->remaining == 0)

Buffer buffer_create(size_t capacity);

void buffer_free(Buffer *buffer);

char* buffer_get(Buffer buffer);

int buffer_prepare_for_read(Buffer buffer, size_t expected);

/*
 * Reads some data into a buffer. Returns -1 in case of an error and sets 
 * errno (see read()). Returns 0 for EOF. Updates buffer->size and returns
 * the new size after a succesful read. 
 *
 * Precondition: buffer->size <= buffer->expected
 */
ssize_t buffer_read(Buffer buffer, int fd);

void buffer_prepare_for_write(Buffer buffer);

/*
 * Writes data from buffer to the given fd. Returns -1 and sets errno in case
 * of an error. Updates buffer->remaining and returns the number of remaining
 * bytes to be written after a successful write. 
 *
 * Precondition: buffer->remaining > 0
 */
ssize_t buffer_write(Buffer buffer, int fd);


#ifdef __cplusplus
}
#endif


#endif /* _BUFFER_H_ */