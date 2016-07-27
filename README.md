![logo](https://raw.github.com/yanzhengli/sneaker/dev/resources/banner.png)

[![Build status](https://travis-ci.org/yanzhengli/sneaker.svg?branch=dev)](https://travis-ci.org/yanzhengli/sneaker)
[![Docs build status](https://readthedocs.org/projects/sneaker/badge/?version=latest)](https://sneaker.readthedocs.io/en/latest/)
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

* [libc](https://sneaker.readthedocs.io/en/latest/src/libc.html) - C utilities
* [algorithm](https://sneaker.readthedocs.io/en/latest/src/algorithm.html) - a collection of advanced algorithms
* [allocator](https://sneaker.readthedocs.io/en/latest/src/allocator.html) - STL container allocator
* [cache](https://sneaker.readthedocs.io/en/latest/src/cache.html) - in-memory cache management
* [container](https://sneaker.readthedocs.io/en/latest/src/container.html) - versatile data structures
* [context](https://sneaker.readthedocs.io/en/latest/src/context.html) - context management
* [functional](https://sneaker.readthedocs.io/en/latest/src/functional.html) - versatile function wrappers that support asynchronous invocations
* [I/O](https://sneaker.readthedocs.io/en/latest/src/io.html) - general I/O utilities
* [logging](https://sneaker.readthedocs.io/en/latest/src/logging.html) - extensible logging facilities
* [json](https://sneaker.readthedocs.io/en/latest/src/json.html) - JSON serialization, deserialization and schema validation
* [threading](https://sneaker.readthedocs.io/en/latest/src/threading.html) - thread management and daemons
* [utility](https://sneaker.readthedocs.io/en/latest/src/utility.html) - miscellaneous utilities
* [testing](https://sneaker.readthedocs.io/en/latest/src/testing.html) - testing utilities


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
  * [Goals and Roadmap](https://github.com/yanzhengli/sneaker/wiki/Goals-and-Roadmap)

## Latest Version
`version 0.3.0`


## License
*Sneaker* is licensed under [The MIT License](http://opensource.org/licenses/MIT).
