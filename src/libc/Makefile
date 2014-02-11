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
CFLAGS=-Wall -arch x86_64
AR=ar
ARFLAGS=rvs

array.o: array.c
	$(CC) $(CFLAGS) -c array.c

bitmap.o: bitmap.c
	$(CC) $(CFLAGS) -c bitmap.c

cutils.o:  cutils.c
	$(CC) $(CFLAGS) -c cutils.c

dict.o: dict.c
	$(CC) $(CFLAGS) -c dict.c

hash.o: hash.c
	$(CC) $(CFLAGS) -c hash.c

hashmap.o: hashmap.c
	$(CC) $(CFLAGS) -c hashmap.c

queue.o: queue.c
	$(CC) $(CFLAGS) -c queue.c

stack.o: stack.c
	$(CC) $(CFLAGS) -c -D_STRUCT_SIGALTSTACK -D_STRUCT_UCONTEXT stack.c

strbuf.o: strbuf.c
	$(CC) $(CFLAGS) -c strbuf.c

strutils.o: strutils.c
	$(CC) $(CFLAGS) -c strutils.c

utils.o: utils.c
	$(CC) $(CFLAGS) -c utils.c

uuid.o: uuid.c
	$(CC) $(CFLAGS) -c -D_UUID_T uuid.c


obj: 					\
	array.o   	\
	bitmap.o 		\
	cutils.o    \
	dict.o  		\
	hash.o  		\
	hashmap.o 	\
	queue.o 		\
	stack.o 		\
	strbuf.o 		\
	strutils.o 	\
	utils.o 		\
	uuid.o

libc.a: obj
	$(AR) $(ARFLAGS) libc.a *.o

all: libc.a

.PHONY : clean
clean:
	rm *.gch *.o *.a *.out