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

LIBRARIES=$(LIBGTEST) $(LIBC)

OBJECTS=../_unittest.o


libc:
	cd ../../src/libc/ && make all

array_unittest.test: libc array_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) $(OBJECTS) array_unittest.cc -o array_unittest.test

bitmap_unittest.test: libc bitmap_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) $(OBJECTS) bitmap_unittest.cc -o bitmap_unittest.test

cutils_unittest.test: libc cutils_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) $(OBJECTS) cutils_unittest.cc -o cutils_unittest.test

dict_unittest.test: libc dict_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) $(OBJECTS) dict_unittest.cc -o dict_unittest.test

hash_unittest.test: libc hashmap_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) $(OBJECTS) hash_unittest.cc -o hash_unittest.test

hashmap_unittest.test: libc hashmap_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) $(OBJECTS) hashmap_unittest.cc -o hashmap_unittest.test

queue_unittest.test: libc queue_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) $(OBJECTS) queue_unittest.cc -o queue_unittest.test

stack_unittest.test: libc stack_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) -D_STRUCT_SIGALTSTACK -D_STRUCT_UCONTEXT $(OBJECTS) stack_unittest.cc -o stack_unittest.test

strbuf_unittest.test: libc strbuf_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) $(OBJECTS) strbuf_unittest.cc -o strbuf_unittest.test

strutils_unittest.test: libc strutils_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) $(OBJECTS) strutils_unittest.cc -o strutils_unittest.test

utils_unittest.test: libc utils_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) $(OBJECTS) utils_unittest.cc -o utils_unittest.test

uuid_unittest.test: libc uuid_unittest.cc
	$(CXX) $(CFLAGS) $(LIBRARIES) -D__DARWIN_C_LEVEL=0 -D_UUID_T $(OBJECTS) uuid_unittest.cc -o uuid_unittest.test


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