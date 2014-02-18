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
CFLAGS=-Wall -std=c++11 -arch x86_64 -DGTEST_HAS_TR1_TUPLE=0
AR=ar
ARFLAGS=rvs


INPUTS = _unittest.cc

OBJECTS = _unittest.o

LIBTESTING = testing.a

OUTPUTS = $(OBJECTS) $(LIBTESTING)


objects: $(INPUTS)
	-for f in $(INPUTS); do $(CXX) $(CFLAGS) -c $$f; done

testing.a: objects
	$(AR) $(ARFLAGS) $(LIBTESTING) $(OBJECTS)

all: objects testing.a


.PHONY: clean
clean:
	-for f in $(OUTPUTS); do rm -f $$f; done;