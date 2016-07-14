.. sneaker documentation master file, created by
   sphinx-quickstart on Sat Apr 26 00:44:30 2014.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Sneaker v0.3.0 documentation
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

1. Checkout the project documentations

   * `Web Version <https://sneaker.readthedocs.org/en/latest/>`_
   * `PDF Version <https://media.readthedocs.org/pdf/sneaker/latest/sneaker.pdf>`_

2. Build from source and install library

   `make && make install`

3. Read the wiki if you'd like to contribute

  * `Wiki home page <https://github.com/yanzhengli/sneaker/wiki>`_
  * `Styles and Guidelines <https://github.com/yanzhengli/sneaker/wiki/Styles-and-Guidelines>`_
  * `Development and Release Workflow <https://github.com/yanzhengli/sneaker/wiki/Development-and-Release-Workflow>`_
  * `Build Environments and Dependencies <https://github.com/yanzhengli/sneaker/wiki/Build-Environments-and-Dependencies>`_


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
   src/logging
   src/json
   src/threading
   src/algorithm
   src/utility
   src/testing


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

