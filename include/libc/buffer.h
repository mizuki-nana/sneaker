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

/* Generic buffer that can read/write from/to files.  */

#ifndef SNEAKER_BUFFER_H_
#define SNEAKER_BUFFER_H_

#include <stddef.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
 * Byte buffer of known size. Keeps track of how much data has been read
 * into or written out of the buffer.
 */
typedef struct __sneaker_buffer_s * buffer_t;


buffer_t buffer_create(size_t capacity);


void buffer_free(buffer_t *buffer);

/*
 * Returns true if all data has been read into the buffer.
 */
int buffer_read_complete(buffer_t buffer);

/*
 * Returns true if the buffer has been completely written.
 */
int buffer_write_complete(buffer_t buffer);


char* buffer_get(buffer_t buffer);


int buffer_prepare_for_read(buffer_t buffer, size_t expected);

/*
 * Reads some data into a buffer. Returns -1 in case of an error and sets 
 * errno (see read()). Returns 0 for EOF. Updates buffer->size and returns
 * the new size after a succesful read. 
 *
 * Precondition: buffer->size <= buffer->expected
 */
ssize_t buffer_read(buffer_t buffer, int fd);


void buffer_prepare_for_write(buffer_t buffer);


/*
 * Writes data from buffer to the given fd. Returns -1 and sets errno in case
 * of an error. Updates buffer->remaining and returns the number of remaining
 * bytes to be written after a successful write. 
 *
 * Precondition: buffer->remaining > 0
 */
ssize_t buffer_write(buffer_t buffer, int fd);


#ifdef __cplusplus
}
#endif


#endif /* SNEAKER_BUFFER_H_ */