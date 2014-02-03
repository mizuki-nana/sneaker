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
CFLAGS=-Wno-implicit -Wall -arch x86_64

AR=ar
ARFLAGS=rvs

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

cutils.o: cutils.h cutils.c
	$(CC) $(CFLAGS) -c cutils.c

hash.o: hash.c hash.h
	$(CC) $(CFLAGS) -c hash.c

hashmap.o: hashmap.c
	$(CC) $(CFLAGS) -c hashmap.c

dict.o: dict.c dict.h
	$(CC) $(CFLAGS) -c dict.c

strutils.o: strutils.c strutils.h
	$(CC) $(CFLAGS) -c strutils.c

strbuf.o: strbuf.c strbuf.h
	$(CC) $(CFLAGS) -c strbuf.c

obj: 					\
	utils.o 		\
	cutils.o    \
	hash.o  		\
	hashmap.o 	\
	dict.o  		\
	strutils.o 	\
	strbuf.o

_libbase.a: obj
	$(AR) $(ARFLAGS) _libbase.a *.o

all: _libbase.a

.PHONY : clean
clean:
	rm *.gch *.o *.a *.out