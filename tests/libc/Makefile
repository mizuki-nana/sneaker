# The MIT License (MIT)

# Copyright (c) 2014 Yanzheng Li

# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

CC=gcc
CXX=c++
CFLAGS=-Wall -arch x86_64 -DGTEST_HAS_TR1_TUPLE=0

LIBGTEST=/usr/lib/libgtest.a
LIBC=../../src/libc/libc.a
TESTING=../../src/testing/testing.a

LIBRARIES=$(LIBGTEST) $(LIBC) $(TESTING)

INPUTS= array_unittest \
				bitmap_unittest \
				cutils_unittest \
				dict_unittest \
				hash_unittest \
				hashmap_unittest \
				math_unittest \
				queue_unittest \
				strbuf_unittest \
				strutils_unittest \
				utils_unittest

INPUTS_SPECIAL_1=stack_unittest

INPUTS_SPECIAL_2=uuid_unittest

OUTPUTS= array_unittest.test \
				bitmap_unittest.test \
				cutils_unittest.test \
				dict_unittest.test \
				hash_unittest.test \
				hashmap_unittest.test \
				math_unittest.test \
				queue_unittest.test \
				stack_unittest.test \
				strbuf_unittest.test \
				strutils_unittest.test \
				utils_unittest.test \
				uuid_unittest.test


inputs:
	-for f in $(INPUTS); do $(CXX) $(CFLAGS) $(LIBRARIES) $$f.cc -o $$f.test; done

inputs_special_1:
	-for f in $(INPUTS_SPECIAL_1); do $(CXX) $(CFLAGS) $(LIBRARIES) -D_STRUCT_SIGALTSTACK -D_STRUCT_UCONTEXT $$f.cc -o $$f.test; done

inputs_special_2:
	-for f in $(INPUTS_SPECIAL_2); do $(CXX) $(CFLAGS) $(LIBRARIES) -D__DARWIN_C_LEVEL=0 -D_UUID_T $$f.cc -o $$f.test; done

all: inputs inputs_special_1 inputs_special_2


.PHONY: clean
clean:
	-for f in $(OUTPUTS); do rm -f $$f; done;