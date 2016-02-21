************
Atomic Types
************

Atomic types and helpers.


Atomic Incrementor
==================

An abstraction that encapsulates the logic to handle
atomic incremental operations.

Header file: `sneaker/atomic/atomic_incrementor.h`

.. cpp:class:: sneaker::atomic::atomic_incrementor<T, T UPPER_LIMIT>
---------------------------------------------------------------------------------

  .. cpp:function:: atomic_incrementor()
    :noindex:

    Constructor that takes no arguments and initializes the initial value
    of the encapsulating type to `0`.

  .. cpp:function:: atomic_incrementor(T value)
    :noindex:

    Constructor that takes a value of the encapsulating type and makes that
    the initial value.

  .. cpp:function:: atomic_incrementor(const atomic_incrementor<T, UPPER_LIMIT>&)
    :noindex:

    Copy constructor.

  .. cpp:function:: atomic_incrementor<T, UPPER_LIMIT>& operator=(const T&)
    :noindex:

    Assignment operator.

  .. cpp:function:: atomic_incrementor<T, UPPER_LIMIT>& operator++()
    :noindex:

    Pre-increment operator.

  .. cpp:function:: operator T() const
    :noindex:

    Conversion operator.

  .. cpp:function:: bool operator==(const T&) const
    :noindex:

    Equality operator.

  .. cpp:function:: bool operator!=(const T&) const
    :noindex:

    Inequality operator.
