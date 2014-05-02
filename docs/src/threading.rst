********************************
8. Thread Management and Daemons
********************************


.. cpp:class:: sneaker::threading::daemon_service
-------------------------------------------------

An abstraction that provides the necessary functionalities to execute code as a
daemon service running in a background thread.

  .. cpp:function:: daemon_service(bool=false)
    :noindex:

    Contructor. Takes a boolean argument specifying whether the foreground
    thread is be blocked for the during which the code in `handle` runs
    in the background thread. Defaults to `false`.

  .. cpp:function:: ~daemon_service()
    :noindex:

    Destructor. The background thread created is destroyed.

  .. cpp:function:: virtual void handle()=0
    :noindex:

    Defines the code that is executed when `start` is called to run the daemon
    service in the background thread. This method should be overriden in
    deriving classes.

  .. cpp:function:: virtual bool start()
    :noindex:

    Starts the daemon service in a background thread and executes the code in
    defined in `handle` immediately after the background thread is created.

    Returns a boolean indicating whether the background thread was created
    successfully, `true` is successful, `false` otherwise.


.. cpp:class:: sneaker::threading::fixed_time_interval_daemon_service
---------------------------------------------------------------------


A deriving type of `sneaker::threading::daemon_service` where the code is
executed over a regular time interval.


  .. cpp:function:: fixed_time_interval_daemon_service(size_t, ExternalHandler, bool, size_t)
    :noindex:

    Constructor. The first argument specifies the interval duration in milliseconds.
    The second argument takes an external handler which gets called once during
    each interval. The third argument specifies if the foreground thread should
    wait for the background thread which it is running, and the last argument
    specifies the maximum number of iterations to run.

  .. cpp:function:: ~fixed_time_interval_daemon_service()
    :noindex:

    Destructor. The background thread created is destroyed.