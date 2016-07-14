Sneaker
=======

![logo](https://raw.github.com/yanzhengli/sneaker/dev/resources/logo_128x128.png)

[![Build status](https://travis-ci.org/yanzhengli/sneaker.svg?branch=dev)](https://travis-ci.org/yanzhengli/sneaker)
[![Docs build status](https://readthedocs.org/projects/sneaker/badge/?version=latest)](https://readthedocs.org/projects/sneaker/)
[![Coverage Status](https://coveralls.io/repos/github/yanzhengli/sneaker/badge.svg?branch=dev)](https://coveralls.io/github/yanzhengli/sneaker?branch=dev)


_Sneaker_ is a collection of modern and versatile C++ components that are useful
in many application development scenarios.

The designs and implementations in the library are based on the philosophy that
each component has its own responsibility in answering one or more of the six
aspects of all software systems, namely the “what”, “when”, “where”, “who”,
“why” and “how”. The elegance and succinctness of the code intrinsically
demonstrates the “how”. The design of each component and interface focuses on
the “who”, “when” and “where” to clearly illustrate the interactions with
other systems. Finally, good documentation and comments reflects the “what”
and “why” to users.

In addition, the design of many components are heavily inspired from features
and models from other high level programming languages that are either absent or
not first-class citizens in the C++ world, such as the use of scoped context
management and function decorators, which are all prevalent features in Python.


## Overview

_Sneaker_ is consisted of the following major components:

* [libc](https://github.com/yanzhengli/sneaker/tree/dev/include/libc) - C utilities
* [algorithm](https://github.com/yanzhengli/sneaker/tree/dev/include/algorithm) - a collection of advanced algorithms
* [allocator](https://github.com/yanzhengli/sneaker/tree/dev/include/allocator) - STL container allocator
* [cache](https://github.com/yanzhengli/sneaker/tree/dev/include/cache) - in-memory cache management
* [container](https://github.com/yanzhengli/sneaker/tree/dev/include/container) - versatile data structures
* [context](https://github.com/yanzhengli/sneaker/tree/dev/include/context) - context management
* [functional](https://github.com/yanzhengli/sneaker/tree/dev/include/functional) - versatile function wrappers that support asynchronous invocations
* [I/O](https://github.com/yanzhengli/sneaker/tree/dev/include/io) - general I/O utilities
* [logging](https://github.com/yanzhengli/sneaker/tree/dev/include/logging) - extensible logging facilities
* [json](https://github.com/yanzhengli/sneaker/tree/dev/include/json) - JSON serialization, deserialization and schema validation
* [threading](https://github.com/yanzhengli/sneaker/tree/dev/include/threading) - thread management and daemons
* [utility](https://github.com/yanzhengli/sneaker/tree/dev/include/utility) - miscellaneous utilities
* [testing](https://github.com/yanzhengli/sneaker/tree/dev/include/testing) - testing utilities


## Get Started

1. Check out the documentations
  * [Web version](https://sneaker.readthedocs.org/en/latest/)
  * [PDF version](https://media.readthedocs.org/pdf/sneaker/latest/sneaker.pdf)

2. Build from source and install library
> `make && make install`

3. Read the wiki if you'd like to contribute
  * [Wiki home page](https://github.com/yanzhengli/sneaker/wiki)
  * [Styles and Guidelines](https://github.com/yanzhengli/sneaker/wiki/Styles-and-Guidelines)
  * [Development and Release Workflow](https://github.com/yanzhengli/sneaker/wiki/Development-and-Release-Workflow)
  * [Build Environments and Dependencies](https://github.com/yanzhengli/sneaker/wiki/Build-Environments-and-Dependencies)


## Latest Version
`version 0.2.9`


## License
*Sneaker* is licensed under [The MIT License](http://opensource.org/licenses/MIT).
