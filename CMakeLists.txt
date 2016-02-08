#!/bin/bash
#
# The MIT License (MIT)
#
# Copyright (c) 2016 Yanzheng Li
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

cmake_minimum_required(VERSION 2.6)


# Project.
project (sneaker C CXX)


# Compilers.
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")


# Paths.
SET(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
SET(TESTS_DIR ${PROJECT_SOURCE_DIR}/tests)


# Packages.
find_package(Boost 1.55 REQUIRED COMPONENTS
    program_options
    filesystem
    regex
    )


# General compiler flags.
set(CMAKE_CXX_FLAGS "-Wall -std=c++11")


# Sub-directories.
ADD_SUBDIRECTORY(${SRC_DIR})
ADD_SUBDIRECTORY(${TESTS_DIR})
