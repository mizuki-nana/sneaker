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


# Library `sneaker`.
add_library(sneaker
    io/file_reader.cc
    json/json.cc
    json/json_parser.cc
    json/json_schema.cc
    libc/bitmap.c
    libc/cutils.c
    libc/dict.c
    libc/hash.c
    libc/hashmap.c
    libc/math.c
    libc/queue.c
    libc/stack.c
    libc/strbuf.c
    libc/strutils.c
    libc/utils.c
    libc/uuid.c
    libc/vector.c
    threading/daemon_service.cc
    threading/fixed_time_interval_daemon_service.cc
    utility/cmdline_program.cc
    utility/os.cc
    utility/stack_trace.cc
)


# Platform specific compiler flags.
if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  target_compile_options(sneaker PRIVATE -arch x86_64 -DGTEST_HAS_TR1_TUPLE=0)
endif()
