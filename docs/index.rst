.. sneaker documentation master file, created by
   sphinx-quickstart on Sat Apr 26 00:44:30 2014.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

sneaker v0.2.0 documentation
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
management, which is a prevalent feature in Python.


Get Started
-----------

1 Build binary
   `make all`
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
   src/cache
   src/container
   src/context
   src/functional
   src/io
   src/threading


Compilation Environment
-----------------------

* Compiler
   `Apple LLVM version 5.1 (clang-503.0.38) (based on LLVM 3.4svn) Target: x86_64-apple-darwin13.1.0 Thread model: posix`
* Build tool
   `GNU Make 3.81 | built for i386-apple-darwin11.3.0`
* Operating System
   `OS X 10.9.2 (13C64)`
* Documentation
   `Sphinx 1.2.2`


Dependencies
------------

* boost
   `Version 1.53`
* googletest
   `Version 1.6.0`


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
