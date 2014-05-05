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

CC=cc
CXX=c++
CFLAGS=-Wall -arch x86_64 -D_STRUCT_SIGALTSTACK -D_STRUCT_UCONTEXT -D_UUID_T
AR=ar
ARFLAGS=rvs


INPUTS = \
	bitmap.c \
	cutils.c \
	dict.c \
	hash.c \
	hashmap.c \
	math.c \
	queue.c \
	stack.c \
	strbuf.c \
	strutils.c \
	utils.c \
	uuid.c \
	vector.c \

OBJECTS = \
	bitmap.o \
	cutils.o \
	dict.o \
	hash.o \
	hashmap.o \
	math.o \
	queue.o \
	stack.o \
	strbuf.o \
	strutils.o \
	utils.o \
	uuid.o \
	vector.o

LIBC = libc.a

OUTPUTS = $(OBJECTS) $(LIBC)

objects: $(INPUTS)
	-for f in $(INPUTS); do $(CC) $(CFLAGS) -c $$f; done

libc.a: objects
	$(AR) $(ARFLAGS) $(LIBC) $(OBJECTS)

all: objects libc.a


.PHONY: clean
clean:
	-for f in $(OUTPUTS); do rm -f $$f; done;
