.. sneaker documentation master file, created by
   sphinx-quickstart on Sat Apr 26 00:44:30 2014.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

sneaker v0.2.7 documentation
============================

Overview
--------

*Sneaker* is a collection of modern and versatile C++ components that are useful
in many application development scenarios.

Components are designed as independent entities that each has its own
responsibility in answering one or more of the six aspects of all software
systems, namely the "*what*", "*when*", "*where*", "*who*", "*why*" and "*how*".

The elegance and succinctness of the code intrinsically demonstrates the
"*how*", and the separation of functionalities among components and the
interactions between them focus on the "*who*", "*when*" and "*where*".
Finally, good documentation and comments reflects the "*what*" and "*why*"
to users.

In addition, the design of many components are heavily inspired from features
and models from other high level programming languages that are either absent or
not first-class citizens in the C++ world, such as the use of scoped context
management and function decorators, which are all prevalent features in Python.


Get Started
-----------

1 Build binary
   `make`
2 Run all tests
   `make test`
3 Install
   `make install`


Content
-------

.. toctree::
   :maxdepth: 2

   src/libc
   src/allocator
   src/atomic
   src/cache
   src/container
   src/context
   src/functional
   src/io
   src/json
   src/threading
   src/algorithm


Compilation Environments
------------------------

OS X

* Operating System:
  `OS X 10.9.5 (13F34)`
* Compiler:
  `Apple LLVM version 5.1 (clang-503.0.40) (based on LLVM 3.4svn) Target: x86_64-apple-darwin13.4.0 Thread model: posix`
* Build tool:
  `GNU Make 3.81 built for i386-apple-darwin13.4.0`
* Sphinx:
  `Sphinx 1.2.2`

Debian

* Operating System:
  `Linux 3.8.0-29-generic #42~precise1-Ubuntu SMP x86_64 GNU/Linux (Ubuntu 14.04.1 LTS)`
* Compiler:
  `Ubuntu clang version 3.4-1ubuntu3 (tags/RELEASE_34/final) (based on LLVM 3.4) Target: x86_64-pc-linux-gnu Thread model: posix`
* Build tool:
  `GNU Make 3.81 This program built for x86_64-pc-linux-gnu`
* Sphinx:
  `Sphinx v1.1.3`


Dependencies
------------

* boost
   `Version 1.55`
* googletest
   `Version 1.6.0`


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

