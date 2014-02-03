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
CFLAGS=-Wno-implicit -Wall -arch x86_64 -DGTEST_HAS_TR1_TUPLE=0

LIBGTEST=/usr/lib/libgtest.a

base:
	cd ../../src/base/ && make all

utils_unittest.test: base utils_unittest.cc
	$(CXX) $(CFLAGS) $(LIBGTEST) ../../src/base/_libbase.a ../_unittest.o utils_unittest.cc -o utils_unittest.test

hash_unittest.test: base hashmap_unittest.cc
	$(CXX) $(CFLAGS) $(LIBGTEST) ../../src/base/_libbase.a ../_unittest.o hash_unittest.cc -o hash_unittest.test

hashmap_unittest.test: base hashmap_unittest.cc
	$(CXX) $(CFLAGS) $(LIBGTEST) ../../src/base/_libbase.a ../_unittest.o hashmap_unittest.cc -o hashmap_unittest.test

dict_unittest.test: base dict_unittest.cc
	$(CXX) $(CFLAGS) $(LIBGTEST) ../../src/base/_libbase.a ../_unittest.o dict_unittest.cc -o dict_unittest.test

strutils_unittest.test: base strutils_unittest.cc
	$(CXX) $(CFLAGS) $(LIBGTEST) ../../src/base/_libbase.a ../_unittest.o strutils_unittest.cc -o strutils_unittest.test

all: 											\
	utils_unittest.test 		\
	hash_unittest.test    	\
	hashmap_unittest.test 	\
	dict_unittest.test			\
	strutils_unittest.test 	\

.PHONY : clean
clean:
	rm *.gch *.o *.a *.out *.test