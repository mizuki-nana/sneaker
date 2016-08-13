**********************
Build and Installation
**********************

This reference manual contains useful information for developers who wish to
build the project from source.

The project strives the make the build and installation process as
straightforward as possible. Currently, the build system requires
`GNU Make <https://www.gnu.org/software/make/>`_ and
`CMake <https://cmake.org/>`_.

Steps for building Sneaker:

1. Update submodules for all dependencies

  `git submodule update --init --recursive`

  Sneaker currently has the following dependencies referenced as submodules:

    * `Google Test <https://code.google.com/p/googletest/>`_ version 1.7.0

2. Build and install Google Test from source

  `make gtest`

3. Build libraries, unit tests and documentations

  `make`

  This builds the following main binaries:

  ========================  ================================================================
  	     Binary                                     Description
  ========================  ================================================================
    libsneaker.a              Sneaker static library (release version)
    libsneaker_dbg.a          Sneaker static library (debug version)
    libsneaker_dbg_cov.a      Sneaker static library with coverage support (debug version)
  ========================  ================================================================

  If you are unsure whether your system meets the requirements for building Sneaker, please refer to `this page <https://github.com/yanzhengli/sneaker/wiki/Build-Environments-and-Dependencies>`_.

4. Install the library binaries and header files

  `make install`

  This places the libraries `libsneaker.a` and `libsneaker_dbg.a` to under
  `/usr/local/lib`, and copies the header files to `/usr/local/include/sneaker/`.

5. Uninstall the library

  `make uninstall`

  This will undo the actions done in step 4) above.
