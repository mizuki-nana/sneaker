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

CXX=c++
CFLAGS=-Wall -Wno-deprecated-writable-strings -std=c++11 -arch x86_64 -DGTEST_HAS_TR1_TUPLE=0

LIBGTEST=/usr/lib/libgtest.a
TESTING=../../src/testing/testing.a

LIBRARIES=$(LIBGTEST) $(TESTING)

dependencies:
	cd ../../src/testing/ && make all
	cd ../../src/cache/ && make all

generic_cache_unittest.test: dependencies generic_cache_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) generic_cache_unittest.cc -o generic_cache_unittest.test

all: \
	generic_cache_unittest.test


.PHONY : clean
clean:
	rm *.gch *.o *.a *.out *.test