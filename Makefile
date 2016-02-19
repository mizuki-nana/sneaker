#!/bin/bash

# The MIT License (MIT)

# Copyright (c) 2016 Yanzheng Li

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
#
# This Makefile is used to build the entire project, including bootstrapping
# CMake to build from source.


VERSION=v0.2.8


INCLUDE=./include
SRC=./src
BIN=./bin
DOCS=./docs
RESOURCES=./resources
DOCS_BUILD=$(DOCS)/_build
ROOT_DIR=$(shell pwd)

GTEST=gtest
GTEST_DIR=./libs/gtest-1.6.0
GTEST_BUILD=./$(BIN)/$(GTEST)
GTEST_INCLUDE_DIR=./libs/gtest-1.6.0/include/gtest
GTEST_INCLUDE_TARGET_DIR=/usr/local/include/
GTEST_STATIC_LIB=libgtest.a
GTEST_STATIC_LIB_PATH=$(GTEST_BUILD)/$(GTEST_STATIC_LIB)
GTEST_STATIC_LIB_TARGET_DIR=/usr/local/lib

LIBSNEAKER=libsneaker
LIBSNEAKER_A=$(LIBSNEAKER).a
LIBSNEAKER_GZIP=$(LIBSNEAKER)-$(VERSION).tar.gz

CMAKE=`which cmake`

LIB_INSTALL_DIR=/usr/local/lib
LIB_INSTALL_PATH=$(LIB_INSTALL_DIR)/libsneaker.a
HEADER_INSTALL_PATH=/usr/local/include/sneaker/


.PHONY: all
all: gtest src docs


.PHONY: src
src:
	mkdir -p $(BIN)
	cd $(BIN); cmake ../ && make


.PHONY: gtest
gtest:
	git submodule sync
	git submodule update --init
	mkdir -p $(GTEST_BUILD)
	cd $(ROOT_DIR)/$(GTEST_DIR)
	cd $(ROOT_DIR)/$(GTEST_BUILD) && $(CMAKE) $(ROOT_DIR)/$(GTEST_DIR)
	cd $(ROOT_DIR)/$(GTEST_BUILD) && $(MAKE)
	sudo cp -rf $(GTEST_INCLUDE_DIR) $(GTEST_INCLUDE_TARGET_DIR)
	sudo cp $(GTEST_STATIC_LIB_PATH) $(GTEST_STATIC_LIB_TARGET_DIR)


.PHONY: docs
docs:
	$(MAKE) -C $(DOCS) html
	mkdir -p $(BIN)/$(DOCS)/ && cp -R $(DOCS_BUILD)/html $(BIN)/$(DOCS)/


.PHONY: clean
clean:
	rm -rf $(BIN)
	rm -rf $(DOCS_BUILD)


.PHONY: install
install: docs
	@echo "\033[32mInstalling $(VERSION)...\033[39m";
	sudo mkdir -p $(HEADER_INSTALL_PATH)
	sudo cp -vr $(INCLUDE)/* $(HEADER_INSTALL_PATH)
	sudo mkdir -p $(LIB_INSTALL_DIR)
	sudo cp -vr $(BIN)/$(SRC)/$(LIBSNEAKER_A) $(LIB_INSTALL_PATH)
	tar -zcvf $(BIN)/$(LIBSNEAKER_GZIP) $(INCLUDE) $(SRC) $(TESTS) $(RESOURCES) LICENSE Makefile README.md
	@echo "\033[32mInstall complete...\033[39m";
	@echo "\033[32mHeader files: \033[35m$(HEADER_INSTALL_PATH)\033[39m";
	@echo "\033[32mStatic lib: \033[35m$(LIB_INSTALL_PATH)\033[39m";


.PHONY: uninstall
uninstall:
	@echo "\033[32mUninstalling libsneaker version $(VERSION)\033[39m";
	sudo rm -rf $(HEADER_INSTALL_PATH)
	sudo rm -rf $(LIB_INSTALL_PATH)
	rm -rf ./*.tar.gz
	@echo "\033[32mUninstalling complete.\033[39m";
