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
CXXFLAGS=-Wall -std=c++11
CXXFLAGS_EXTRA=-Wno-overloaded-virtual

LIBGTEST=/usr/lib/libgtest.a

LIBSNEAKER_A=../../libsneaker.a
LIBRARIES=$(LIBGTEST)

LFLAGS=$(LIBSNEAKER_A) -lgtest -lpthread

LIBC=./libc/
IO=./io/
ALGORITHM=./algorithm/
ALLOCATOR=./allocator/
ATOMIC=./atomic/
CACHE=./cache/
CONTAINER=./container/
CONTEXT=./context/
FUNCTIONAL=./functional/
JSON=./json/
THREADING=./threading/

SUBDIRS=$(LIBC) $(IO) $(ALGORITHM) $(ALLOCATOR) $(ATOMIC) $(CACHE) $(CONTAINER) $(CONTEXT) $(FUNCTIONAL) $(JSON) $(THREADING)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	CXXFLAGS += -D LINUX
	LFLAGS += -lboost_system -lboost_regex
endif
ifeq ($(UNAME_S), Darwin)
	CXXFLAGS += -D DARWIN
	CXXFLAGS += -arch x86_64 -DGTEST_HAS_TR1_TUPLE=0
	LFLAGS += -lboost_system-mt -lboost_regex-mt
endif

ifeq ($(UNAME_S), Linux)
	LIBCFLAGS += -D_SIGNAL_H=0
endif
ifeq ($(UNAME_S), Darwin)
	LIBCFLAGS += -arch x86_64 -D_STRUCT_SIGALTSTACK -D_STRUCT_UCONTEXT
endif

.PHONY: libc
libc:
	-cd $(LIBC) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) $(LIBCFLAGS) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: io
io:
	-cd $(IO) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: algorithm
algorithm:
	-cd $(ALGORITHM) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: allocator
allocator:
	-cd $(ALLOCATOR) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: atomic
atomic:
	-cd $(ATOMIC) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: cache
cache:
	-cd $(CACHE) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: container
container:
	-cd $(CONTAINER) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: context
context:
	-cd $(CONTEXT) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: functional
functional:
	-cd $(FUNCTIONAL) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: json
json:
	-cd $(JSON) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) $(CXXFLAGS_EXTRA) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: threading
threading:
	-cd $(THREADING) && for f in ./*.cc; do ($(CXX) $(CXXFLAGS) $(LIBRARIES) $$f $(LFLAGS) -o $$f.test); done

.PHONY: all
all: libc io algorithm allocator atomic cache container context functional json threading

.PHONY: clean
clean:
	-for dir in $(SUBDIRS); do (rm -rf $$dir*.test); done
