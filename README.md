sneaker
=======

![logo](https://raw.github.com/yanzhengli/sneaker/dev/logo_128x128.png)

[![Build Status](https://api.travis-ci.org/yanzhengli/sneaker.png)](https://travis-ci.org/yanzhengli/sneaker)


Sneaker is a collection of modern and versatile C++ components that are useful
in many application development scenarios.

Components are designed as independent entities that each has its own
responsibility in answering one or more of the six aspects of all software
systems, namely the “what”, “when”, “where”, “who”, “why” and “how”.

The elegance and succinctness of the code intrinsically demonstrates the “how”,
and the separation of functionalities among components and the interactions
between them focus on the “who”, “when” and “where”. Finally, good documentation
and comments reflects the “what” and “why” to users.

In addition, the design of many components are heavily inspired from features
and models from other high level programming languages that are either absent or
not first-class citizens in the C++ world, such as the use of scoped context
management and function decorators, which are all prevalent features in Python.


## Content
* `libc` - _C utilities_
* `algorithm` - _a collection of advanced algorithms_
* `allocator` - _STL container allocator_
* `atomic` - _utilities for atomic types_
* `cache` - _in-memory cache management_
* `container` - _versatile data structures_
* `context` - _context management_
* `functional` - _versatile function wrappers that support asynchronous invocations_
* `json` - _JSON serialization, deserialization and validation_
* `threading` - _thread management and daemons_
* `utility` - _miscellaneous utilities_
* `testing` - _testing utilities_


## Get started

Build binary
<br/>`make`

Run all tests
<br/>`make test`

Install
<br/>`make install`


## Version
`version 0.2.8`


## Documentation
[Link](http://www.libsneaker.org/documentation/)


## Compilation Environments

### OS X
Operating System:<br/>
`OS X 10.10 (14A389)`

Compiler:<br/>
`Apple LLVM version 5.1 (clang-503.0.40) (based on LLVM 3.4svn)
Target: x86_64-apple-darwin14.0.0
Thread model: posix`

Make:<br/>
`GNU Make 3.81`
`built for i386-apple-darwin11.3.0`

Sphinx:<br/>
`Sphinx 1.2.2`

### Debian
Operating System:<br/>
`Linux 3.8.0-29-generic #42~precise1-Ubuntu SMP x86_64 GNU/Linux (Ubuntu 14.04.1 LTS)`

Compiler:<br/>
`Ubuntu clang version 3.4-1ubuntu3 (tags/RELEASE_34/final) (based on LLVM 3.4)
Target: x86_64-pc-linux-gnu
Thread model: posix`

Make:<br/>
`GNU Make 3.81`
`This program built for x86_64-pc-linux-gnu`

Sphinx:<br/>
`Sphinx v1.1.3`


## Dependencies
* [Boost] (http://www.boost.org/) `Version 1.55`
* [Google Test] (https://code.google.com/p/googletest/) `Version 1.6.0`


## License
*sneaker* is licensed under [The MIT License](http://opensource.org/licenses/MIT).
