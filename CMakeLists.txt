#!/bin/bash
#
# The MIT License (MIT)
#
# Copyright (c) 2017 Yanzheng Li
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


# Options.
option(RUN_TESTS_ONCE "Run unit test suite once" OFF)
option(SKIP_TESTS "Skip building and running unit test suite" OFF)


# Release mode.
set(CMAKE_BUILD_TYPE Release ... FORCE)


# Compilers.
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")


# Linker options.
# Suppressing passing -rdynamic.
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")


# Packages.
find_package(Boost 1.55 REQUIRED COMPONENTS
    program_options
    filesystem
    regex
    system
    )


message("Boost libraries found: ${Boost_LIBRARIES}")
message("Boost include directory found: ${Boost_INCLUDE_DIR}")


# Find googletest.
find_library(LIBGTEST_LIBRARY NAMES "gtest"
    PATH_SUFFIXES "lib" "lib32" "lib64")
message("gtest library path found: " ${LIBGTEST_LIBRARY})


find_path(LIBGTEST_INCLUDE_DIR NAMES "gtest/gtest.h")
message("gtest include directory found: " ${LIBGTEST_INCLUDE_DIR})


# Paths.
include_directories("${CMAKE_SOURCE_DIR}/include/")
include_directories(SYSTEM ${Boost_INCLUDE_DIR})
include_directories(SYSTEM ${LIBGTEST_INCLUDE_DIR})
SET(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
SET(TESTS_DIR ${PROJECT_SOURCE_DIR}/tests)


# General compiler flags.
set(CMAKE_CXX_FLAGS "-std=c++11 -Weverything -Werror -Wno-global-constructors -Wno-c++98-compat -Wno-padded -Wno-exit-time-destructors -Wno-format-nonliteral -Wno-c++98-compat-pedantic -Wno-gnu-zero-variadic-macro-arguments")


# Settings for local OS X dev machine.
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang" AND ${CMAKE_CXX_COMPILER_VERSION} MATCHES "7.3.0")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reserved-id-macro -Wno-double-promotion")
endif()


# Sub-directories.
ADD_SUBDIRECTORY(${SRC_DIR})
if (NOT SKIP_TESTS)
  ADD_SUBDIRECTORY(${TESTS_DIR})
endif ()
