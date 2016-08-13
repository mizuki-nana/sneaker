*****************************
Thread Management and Daemons
*****************************

Components that faciliate thread management and asynchronous executions.


Daemon Service
==============

An abstraction that provides the necessary functionalities to execute code as a
daemon service running in a background thread.

Header file: `sneaker/threading/daemon_service.h`

.. cpp:class:: sneaker::threading::daemon_service
-------------------------------------------------

  .. cpp:function:: explicit daemon_service(bool wait_for_termination=false)
    :noindex:

    Contructor. Takes a boolean argument specifying whether the foreground
    thread is be blocked for the during which the code in `handle` runs in the
    background thread. Defaults to `false`.

  .. cpp:function:: virtual ~daemon_service()
    :noindex:

    Destructor. The background thread created is destroyed.

  .. cpp:function:: protected virtual void handle()=0
    :noindex:

    Encapsulates the code that is executed when `start` is called to run the
    daemon service in the background thread. This method should be overriden in
    deriving classes.

  .. cpp:function:: virtual bool start()
    :noindex:

    Starts the daemon service in a background thread and executes the code in
    defined in `handle` immediately after the background thread is created.

    Returns a boolean indicating whether the background thread was created
    successfully, `true` is successful, `false` otherwise.


Fixed-time Interval Daemon Service
==================================

A deriving type of `sneaker::threading::daemon_service` where the code is
executed over a regular time interval.

Header file: `sneaker/threading/fixed_time_interval_daemon_service.h`

.. cpp:class:: sneaker::threading::fixed_time_interval_daemon_service
---------------------------------------------------------------------

  .. cpp:type:: typedef void(*ExternalHandler)(void*)
    :noindex:

    Type of the external handler to be invoked.

  .. cpp:function:: fixed_time_interval_daemon_service(size_t, ExternalHandler, bool, size_t)
    :noindex:

    Constructor. The first argument specifies the interval duration in
    milliseconds. The second argument takes an external handler which gets
    called once during each interval, of type `void(*ExternalHandler)(void)`.
    The third argument specifies if the foreground thread should wait for the
    background thread which it is running, and the last argument specifies the
    maximum number of iterations to run.

  .. cpp:function:: virtual ~fixed_time_interval_daemon_service()
    :noindex:

    Destructor. The background thread created is destroyed.

  .. cpp:function:: size_t interval() const
    :noindex:

    Returns the time interval of the daemon.


Atomic Incrementor
==================

An abstraction that encapsulates the logic to handle atomic incremental
operations.

Header file: `sneaker/threading/atomic.h`

.. cpp:class:: sneaker::threading::atomic<T, T UPPER_LIMIT>
-----------------------------------------------------------

  .. cpp:function:: atomic()
    :noindex:

    Constructor that takes no arguments and initializes the initial value
    of the encapsulating type to `0`.

  .. cpp:function:: atomic(T value)
    :noindex:

    Constructor that takes a value of the encapsulating type and makes that
    the initial value.

  .. cpp:function:: atomic(const atomic<T, UPPER_LIMIT>&)
    :noindex:

    Copy constructor.

  .. cpp:function:: atomic<T, UPPER_LIMIT>& operator=(const T&)
    :noindex:

    Assignment operator.

  .. cpp:function:: atomic<T, UPPER_LIMIT>& operator++()
    :noindex:

    Pre-increment operator. If the value exceeds `UPPER_LIMIT` after
    the increment, the value wraps backs to `0`.

  .. cpp:function:: atomic<T, UPPER_LIMIT>& operator++(int)
    :noindex:

    Post-increment operator. If the value exceeds `UPPER_LIMIT` after
    the increment, the value wraps backs to `0`.

  .. cpp:function:: operator T() const
    :noindex:

    Conversion operator.

  .. cpp:function:: bool operator==(const T&) const
    :noindex:

    Equality operator.

  .. cpp:function:: bool operator!=(const T&) const
    :noindex:

    Inequality operator.
