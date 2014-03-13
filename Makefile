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

# Top level Makefile.

VERSION=v0.1.3

AR=ar
ARFLAGS=rvs
CPFLAGS=-vr

INCLUDE=./include
SRC=./src
TESTS=./tests
SUBDIRS=$(SRC) $(TESTS)

LIBSNEAKER=libsneaker
LIBSNEAKER_A=libsneaker.a
LIBSNEAKER_GZIP=$(LIBSNEAKER)-$(VERSION).tar.gz


.PHONY: build
build:
	-for dir in $(SRC); do ($(MAKE) -C $$dir all;); done


.PHONY: all
all: build
	find . -name "*.o" | xargs $(AR) $(ARFLAGS) $(LIBSNEAKER_A)


.PHONY: tests
tests: build
	-for dir in $(TESTS); do ($(MAKE) -C $$dir all;); done


.PHONY: clean
clean:
	-for dir in $(SUBDIRS); do ($(MAKE) -C $$dir clean;); done
	rm -rf ./$(LIBSNEAKER_A)

.PHONY: install
install: all
	mkdir -p /usr/local/include/sneaker
	cp -vr include/* /usr/local/include/sneaker
	cp -vr libsneaker.a /usr/local/lib/
	tar -zcvf $(LIBSNEAKER_GZIP) $(INCLUDE) $(SRC) $(TESTS) LICENSE Makefile README.md ./*.png


.PHONY: uninstall
uninstall:
	rm -rf /usr/local/include/sneaker
	rm -rf /usr/local/lib/libsneaker.a
	rm -rf ./*.tar.gz
	rm -rf ./$(LIBSNEAKER)