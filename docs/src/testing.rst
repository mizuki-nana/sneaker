*******
Testing
*******

Utilities for tests based on Google Test.

Header file: `sneaker/testing/testing.h`


Fixture Based Test
==================

Base class of test suite that allows one or more fixtures of the same type, and
automatically manages their teardowns.

Header file: `sneaker/testing/fixture_based_test.h`

.. cpp:class:: sneaker::testing::fixture_based_test<T>
------------------------------------------------------

  .. cpp:type:: T
    :noindex:

    Type of the fixture managed by this class.

  .. cpp:type:: typedef void(*FixtureTeardownHandler)(T)
    :noindex:

    Type of fixture teardown handler.

  .. cpp:function:: fixture_based_test(FixtureTeardownHandler)
    :noindex:

    Constructor that takes a fixture teardown handler.

  .. cpp:function:: add_fixture(T)
    :noindex:

    Adds a fixture to be managed by this class.

  .. cpp:function:: virtual void TearDown()
    :noindex:

    Handles teardown of fixtures managed by the class.