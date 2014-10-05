#!/bin/bash

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

VERSION=v0.2.6

AR=ar
ARFLAGS=rvs
CPFLAGS=-vr

INCLUDE=./include
SRC=./src
TESTS=./tests
DOCS=./docs
DOCUMENTATION=./documentation
SUBDIRS=$(SRC) $(TESTS)

LIBSNEAKER=libsneaker
LIBSNEAKER_A=libsneaker.a
LIBSNEAKER_GZIP=$(LIBSNEAKER)-$(VERSION).tar.gz

### Environment variables ###
export GTEST_COLOR=true


.PHONY: all
all: src
	@find . -name "*.o" | xargs $(AR) $(ARFLAGS) $(LIBSNEAKER_A)
	@echo "\033[35mGenerated $(LIBSNEAKER_A)"


.PHONY: src
src:
	@$(MAKE) -C $(SRC) all


.PHONY: test
test:
	@$(MAKE) -C $(TESTS) all
	@echo "\033[32mGoing to run all tests...\033[39m";
	@-for dir in $(TESTS); do (find $$dir -type f -name "*.test" -exec '{}' \;); done
	@echo "\033[32mTests run completed...\033[39m";


.PHONY: docs
docs:
	@$(MAKE) -C $(DOCS) html
	@mkdir -p $(DOCUMENTATION) && cp -R docs/_build/* $(DOCUMENTATION)/


.PHONY: clean
clean:
	@-for dir in $(SUBDIRS); do ($(MAKE) -C $$dir clean;); done
	@-for dir in $(TESTS); do ($(MAKE) -C $$dir clean;); done
	@rm -rf ./$(LIBSNEAKER_A)
	@rm -rf $(DOCUMENTATION)


.PHONY: install
install: docs
	@echo "\033[32mInstalling $(VERSION)...\033[39m";
	@sudo mkdir -p /usr/local/include/sneaker
	@sudo cp -vr include/* /usr/local/include/sneaker
	@sudo cp -vr $(LIBSNEAKER_A) /usr/local/lib/
	@tar -zcvf $(LIBSNEAKER_GZIP) $(INCLUDE) $(SRC) $(TESTS) LICENSE Makefile README.md ./*.png
	@echo "\033[32mInstall complete...\033[39m";
	@echo "\033[32mHeader files: \033[35m/usr/local/include/sneaker/\033[39m";
	@echo "\033[32mStatic lib: \033[35m/usr/local/lib/libsneaker.a\033[39m";


.PHONY: uninstall
uninstall:
	@echo "\033[32mUninstalling current version...\033[39m";
	@sudo rm -rf /usr/local/include/sneaker
	@sudo rm -rf /usr/local/lib/libsneaker.a
	@rm -rf ./*.tar.gz
	@rm -rf ./$(LIBSNEAKER)
	@echo "\033[32mUninstalling complete.\033[39m";
