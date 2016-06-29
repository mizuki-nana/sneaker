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
#ifndef SNEAKER_UTILITY_IO_H_
#define SNEAKER_UTILITY_IO_H_

#include <cstdlib>
#include <string>
#include <vector>


namespace sneaker {


namespace utility {


void hex_to_bytes(const unsigned char* src, size_t src_len, unsigned char* dst);


void hex_to_bytes(const std::string& src, std::string* dst);


void hex_to_bytes(const unsigned char* src, size_t src_len, std::vector<unsigned char>* dst);


void bytes_to_hex(const unsigned char* src, size_t src_len, unsigned char* dst);


void bytes_to_hex(const std::string& src, std::string* dst);


} /* end namespace utility */


} /* end namespace sneaker */


#endif /* SNEAKER_UTILITY_IO_H_ */
