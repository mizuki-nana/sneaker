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

/* Miscellaneous utility functions. */

#ifndef SNEAKER_UTILS_H_
#define SNEAKER_UTILS_H_

#include "c_str.h"

#include <stddef.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define CONTINUE_IF_EQUALS(val1, val2)                \
  if( (val1) == (val2) ) continue;

#define CONTINUE_IF_NULL(val)                         \
  CONTINUE_IF_EQUALS( (val), (NULL) )

#define CONTINUE_IF_TRUE(val)                         \
  CONTINUE_IF_EQUALS( (val), (1) )

#define CONTINUE_IF_FALSE(val)                        \
  CONTINUE_IF_EQUALS( (val), (0) )

#define BREAK_IF_EQUALS(val1, val2)                   \
  if( (val1) == (val2) ) break;

#define BREAK_IF_NULL(val)                            \
  BREAK_IF_EQUALS( (val), (NULL) )

#define BREAK_IF_TRUE(val)                            \
  BREAK_IF_EQUALS( (val), (1) )

#define BREAK_IF_FALSE(val)                           \
  BREAK_IF_EQUALS( (val), (0) )

#define RETURN_IF_EQUALS(val1, val2)                  \
  do {                                                \
    if( (val1) == (val2) ) {                          \
      return;                                         \
    }                                                 \
  } while(0)

#define RETURN_IF_NULL(val)                           \
  RETURN_IF_EQUALS( (val), (NULL) )

#define RETURN_IF_TRUE(val)                           \
  RETURN_IF_EQUALS( (val), (1) )

#define RETURN_IF_FALSE(val)                          \
  RETURN_IF_EQUALS( (val), (0) )

#define RETURN_VAL_IF_EQ(val1, val2, returnval)       \
  do {                                                \
    if( (val1) == (val2) ) {                          \
      return (returnval);                             \
    }                                                 \
  } while(0)

#define RETURN_VAL_IF_NE(val1, val2, returnval)       \
  do {                                                \
    if( (val1) != (val2) ) {                          \
      return (returnval);                             \
    }                                                 \
  } while(0)

#define RETURN_VAL_IF_NULL(val, returnval)            \
  RETURN_VAL_IF_EQ( (val), (NULL), (returnval) )

#define RETURN_VAL_IF_TRUE(boolval, returnval)        \
  do {                                                \
    if((boolval)) {                                   \
      return (returnval);                             \
    }                                                 \
  } while(0)

#define RETURN_VAL_IF_FALSE(boolval, returnval)       \
  do {                                                \
    if(!(boolval)) {                                  \
      return (returnval);                             \
    }                                                 \
  } while(0)

#define GOTO_IF_NULL(val, goto_target)                \
  do {                                                \
    if( (val) == NULL ) {                             \
      goto goto_target;                               \
    }                                                 \
  } while(0)

#define GOTO_IF_TRUE(boolval, goto_target)            \
  do {                                                \
    if( (boolval) ) {                                 \
      goto goto_target;                               \
    }                                                 \
  } while(0)

#define GOTO_IF_FALSE(boolval, goto_target)           \
  do {                                                \
    if( !(boolval) ) {                                \
      goto goto_target;                               \
    }                                                 \
  } while(0)


/*
 * void
 * set_nth_bit(int *val, char bit)
 *
 * Set a specific bit of a given number to 1.
 *
 * Parameters:
 *  int *val: pointer points to the number to set.
 *  char bit: the nth bit to set to 1.
 *
 * Return:
 *  None.
 */
void
set_nth_bit(int *val, char bit);

/*
 * void
 * clear_nth_bit(int *val, char bit)
 *
 * Clear a specific bit of a given number to 0.
 *
 * Parameters:
 *  int *val: pointer points to the number to clear.
 *  char bit: the nth bit to clear to 0.
 *
 * Return:
 *  None.
 */
void
clear_nth_bit(int *val, char bit);

/*
 * int
 * is_bit_set(int val, char bit)
 *
 * Checks if a specific bit of a given number is set to 1.
 *
 * Parameters:
 *  int val: the value to check.
 *  char bit: the number of bit from the right to check.
 *
 * Return:
 *  An integer value representing whether or not the specific.
 *  bit of the given is set. Returns 1 if set, 0 otherwise.
 */
int
is_bit_set(int val, char bit);

/*
 * void
 * set_nth_bit_uint32(uint32_t *val, char bit)
 *
 * Set a specific bit of a given number to 1.
 *
 * Parameters:
 *  uint32_t *val: pointer points to the number to set.
 *  char bit: the nth bit to set to 1.
 *
 * Return:
 *  None.
 */
void
set_nth_bit_uint32(uint32_t *val, char bit);

/*
 * void
 * clear_nth_bit_uint32(uint32_t *val, char bit)
 *
 * Clear a specific bit of a given number to 0.
 *
 * Parameters:
 *  uint32_t *val: pointer points to the number to clear.
 *  char bit: the nth bit to clear to 0.
 *
 * Return:
 *  None.
 */
void
clear_nth_bit_uint32(uint32_t *val, char bit);

/*
 * int
 * is_bit_set_uint32(uint32_t val, char bit)
 *
 * Checks if a specific bit of a given number is set to 1.
 *
 * Parameters:
 *  uint32_t val: the value to check.
 *  char bit: the number of bit from the right to check.
 *
 * Return:
 *  An integer value representing whether or not the specific.
 *  bit of the given is set. Returns 1 if set, 0 otherwise.
 */
int
is_bit_set_uint32(uint32_t val, char bit);

/*
 * int
 * rand_top(int max)
 *
 * Generated a pseudo-random integer number in the range between 1 and 'max'.
 */
int
rand_top(int max);

/*
 * int
 * rand_range(int min, int max)
 *
 * Generates a pseudo-random integer number in the range between 'min' and 'max'.
 */
int
rand_range(int min, int max);

/*
 * double
 * hex_randf_top(double max)
 *
 * Generated a pseudo-random floating number in the range between 1.0 and 'max'.
 */
double
randf_top(double max);

/*
 * int
 * randf_range(double min, double max)
 *
 * Generates a pseudo-random floating number in the range between 'min' and 'max'.
 */
double
randf_range(double min, double max);

/*
 * c_str
 * generate_text(size_t length=0, size_max=0)
 *
 * Generated a random string of specified length(optional).
 *
 * Parameters:
 *  size_t len: The specified length of the string. If not specified,
 *    generated a random string of length less or equal to 'max'.
 *    This parameter is optional.
 *  size_t max: The max number of characters of the text string generated.
 *    This parameter is optional.
 *
 * Preconditions:
 *  There are four scenarios based on the input of this function:
 *    1. Both 'len' and 'max' are non-zero:
 *      The generated text string will have a length in between 'len' and 'max'.
 *    2. Only 'len' is non-zero:
 *      The generated text string will have exactly 'len' characters in length.
 *    3. Only max is non-zero:
 *      The generated text string will have a random length between 1 and 'max'.
 *    4. Both of 'len' and 'max' are zero: 
 *      NULL is returned.
 */
c_str
generate_text(size_t len, size_t max);

/*
 * c_str
 * generate_loremipsum()
 *
 * Generate Lorem Ipsum.
 *
 * Reference:
 * http://www.lipsum.com/
 */
c_str
generate_loremipsum();


#ifdef __cplusplus
}
#endif


#endif /* SNEAKER_UTILS_H_ */
