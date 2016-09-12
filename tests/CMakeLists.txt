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


# Need to find `../src/libsneaker.a`.
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/)


# Build executable `run_tests`.
ADD_EXECUTABLE(run_tests
    algorithm/tarjan_unittest.cc
    allocator/allocator_unittest.cc
    cache/cache_interface_unittest.cc
    cache/lru_cache_unittest.cc
    container/assorted_value_map_unittest.cc
    container/reservation_map_unittest.cc
    container/unordered_assorted_value_map_unittest.cc
    context/context_unittest.cc
    functional/decorators_unittest.cc
    functional/function_unittest.cc
    io/file_reader_unittest.cc
    json/json_schema_unittest.cc
    json/json_unittest.cc
    libc/bitmap_unittest.cc
    libc/cutils_unittest.cc
    libc/dict_unittest.cc
    libc/hashmap_unittest.cc
    libc/hash_unittest.cc
    libc/math_unittest.cc
    libc/queue_unittest.cc
    libc/stack_unittest.cc
    libc/strbuf_unittest.cc
    libc/strutils_unittest.cc
    libc/utils_unittest.cc
    libc/uuid_unittest.cc
    libc/vector_unittest.cc
    logging/logger_unittest.cc
    threading/atomic_unittest.cc
    threading/daemon_service_unittest.cc
    threading/fixed_time_interval_daemon_service_unittest.cc
    utility/cmdline_program_unittest.cc
    utility/io_unittest.cc
    utility/os_unittest.cc
    utility/stack_trace_unittest.cc
    utility/uniform_table_unittest.cc
    utility/util_numeric_unittest.cc
    main.cc
    )


# Override unit test compilations with `-Wno-weak-vtables` compiler flag,
# as classes inherited from `::testing::Test` do not have out-of-line virtual
# member functions.
SET_TARGET_PROPERTIES(run_tests PROPERTIES COMPILE_FLAGS "-Wno-weak-vtables -fprofile-arcs")


# Platform specific compiler flags.
if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  target_compile_options(run_tests PRIVATE -arch x86_64 -Wno-old-style-cast)
endif()


# Linker flags.
SET_TARGET_PROPERTIES(run_tests PROPERTIES LINK_FLAGS "--coverage")


TARGET_LINK_LIBRARIES(run_tests
    sneaker_dbg_cov
    pthread
    ${LIBGTEST_LIBRARY}
    ${Boost_LIBRARIES})


# Run tests.
if(RUN_TESTS_ONCE)
    set(GTEST_REPEAT 1)
else()
    set(GTEST_REPEAT 3)
endif()

add_custom_command(TARGET run_tests POST_BUILD COMMAND
    ./run_tests --gtest_color=yes --gtest_shuffle --gtest_repeat=${GTEST_REPEAT})
