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

# SUBDIRS = testing libc container cache io threading


#.PHONY: all
#all:
#	-for dir in $(SUBDIRS); do ($(MAKE) -C $$dir all;); done


#.PHONY: clean
#clean:
#	-for dir in $(SUBDIRS); do ($(MAKE) -C $$dir clean;); done

CC=`which clang`
CXX=`which clang++`
CFLAGS=-Wall
CXXFLAGS=-Wall -std=c++11
CXXFLAGS_EXTRA=-Wno-overloaded-virtual
AR=ar
ARFLAGS=rvs

LIBC=./libc/
IO=./io/
JSON=./json/
TESTING=./testing/
THREADING=./threading/

SUBDIRS=$(LIBC) $(IO) $(TESTING) $(THREADING)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	CFLAGS += -D LINUX
	CXXFLAGS += -D LINUX
endif
ifeq ($(UNAME_S), Darwin)
	CXXFLAGS += -arch x86_64 -DGTEST_HAS_TR1_TUPLE=0
	CFLAGS += -arch x86_64 -D_STRUCT_SIGALTSTACK -D_STRUCT_UCONTEXT -D_UUID_T
endif

.PHONY: libc
libc:
	-cd $(LIBC) && for f in ./*.c; do ($(CC) $(CFLAGS) -c $$f); done

.PHONY: io
io:
	-cd $(IO) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) -c $$f); done

.PHONY: json
json:
	-cd $(JSON) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) $(CXXFLAGS_EXTRA) -c $$f); done

.PHONY: testing
testing:
	-cd $(TESTING) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) -c $$f); done

.PHONY: threading
threading:
	-cd $(THREADING) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) -c $$f); done

.PHONY: all
all: libc io json testing threading

.PHONY: clean
clean:
	-for dir in $(SUBDIRS); do (rm -rf $$dir*.o $$dir*.a); done
