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

/* General C utilities */

#ifndef SNEAKER_C_UTILS_H_
#define SNEAKER_C_UTILS_H_


#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
 * Convert integer to ASCII string.
 *
 * Converts an integer value to a null-terminated string using the
 * specified base and stores the result in the buffer given by `str`.
 *
 * If base is 10 and value is negative, the resulting string is preceded with
 * a minus sign (-). With any other base, value is always considered unsigned.
 ******************************************************************************/
char*
itoa(int value, char *str, int base=10);


/*******************************************************************************
 * Convert ASCII string to integer.
 *
 * Parses the C string `str` interpreting its content as an integral number,
 * which is returned as an int value.
 *
 * The function first discards as many whitespace characters as necessary
 * until the first non-whitespace character is found. Then, starting
 * from this character, takes an optional initial plus or minus sign
 * followed by as many numerical digits as possible,
 * and interprets them as a numerical value.
 *
 * The string can contain additional characters after those that
 * form the integral number, which are ignored and have no effect on
 * the behavior of this function.
 *
 * If the first sequence of non-whitespace characters in `str` is not a valid
 * integral number, or if no such sequence exists because either `str` is empty
 * or it contains only whitespace characters, no conversion is performed.
 *
 * On success, the function returns the converted integer number.
 * If no valid conversion could be performed, a zero value is returned.
 * If the correct value is out of the range of representable values,
 * `INT_MAX` or `INT_MIN` is returned.
 ******************************************************************************/
int
atoi(const char *str);


#ifdef __cplusplus
}
#endif


#endif /* SNEAKER_C_UTILS_H_ */
