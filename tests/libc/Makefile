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
CFLAGS=-Wall -Wno-deprecated-writable-strings -arch x86_64 -DGTEST_HAS_TR1_TUPLE=0

LIBGTEST=/usr/lib/libgtest.a
LIBC=../../src/libc/libc.a
TESTING=../../src/testing/testing.a

LIBRARIES=$(LIBGTEST) $(LIBC) $(TESTING)

dependencies:
	cd ../../src/testing/ && make all
	cd ../../src/libc/ && make all

array_unittest.test: dependencies array_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) array_unittest.cc -o array_unittest.test

bitmap_unittest.test: dependencies bitmap_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) bitmap_unittest.cc -o bitmap_unittest.test

cutils_unittest.test: dependencies cutils_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) cutils_unittest.cc -o cutils_unittest.test

dict_unittest.test: dependencies dict_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) dict_unittest.cc -o dict_unittest.test

hash_unittest.test: dependencies hashmap_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) hash_unittest.cc -o hash_unittest.test

hashmap_unittest.test: dependencies hashmap_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) hashmap_unittest.cc -o hashmap_unittest.test

queue_unittest.test: dependencies queue_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) queue_unittest.cc -o queue_unittest.test

stack_unittest.test: dependencies stack_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) -D_STRUCT_SIGALTSTACK -D_STRUCT_UCONTEXT stack_unittest.cc -o stack_unittest.test

strbuf_unittest.test: dependencies strbuf_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) strbuf_unittest.cc -o strbuf_unittest.test

strutils_unittest.test: dependencies strutils_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) strutils_unittest.cc -o strutils_unittest.test

utils_unittest.test: dependencies utils_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) utils_unittest.cc -o utils_unittest.test

uuid_unittest.test: dependencies uuid_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) -D__DARWIN_C_LEVEL=0 -D_UUID_T uuid_unittest.cc -o uuid_unittest.test


all: 											\
	array_unittest.test 		\
	bitmap_unittest.test 		\
	cutils_unittest.test 		\
	dict_unittest.test			\
	hash_unittest.test    	\
	hashmap_unittest.test 	\
	queue_unittest.test 		\
	stack_unittest.test 		\
	strbuf_unittest.test 		\
	strutils_unittest.test 	\
	utils_unittest.test 		\
	uuid_unittest.test


.PHONY : clean
clean:
	rm *.gch *.o *.a *.out *.test