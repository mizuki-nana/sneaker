***********************
Functional Abstractions
***********************

These functional abstractions improve the incompatibilites and
interoperabilities between function pointers, functors and lambdas.


Function abstractions
=====================

Function abstractions that offer interoperabilities between
function pointers, functors and lambdas, as well as asynchronous
invocations.

Header file: `sneaker/functional/function.h`

.. cpp:class:: sneaker::functional::function<R, ... Args>
---------------------------------------------------------

  A function wrapper that takes statically defined return type and argument types,
  also supports asynchronous invocation.

  .. cpp:type:: implicit_type
    :noindex:

    The underlying type of the function pointers, functors and lambas that it's
    compatible with by signature.

  .. cpp:function:: return_value
    :noindex:

    The return type of this function.

  .. cpp:function:: function(implicit_type)
    :noindex:

    Constructor that takes a compatible function pointer, functor or lambda,
    and makes itself a owner of a copy of the argument.
    Note that this constructor is not declared as `explicit` as that implicit
    conversion from compatible types are possible.

  .. cpp:function:: template<class Functor>
                    function(Functor)
    :noindex:

    Constructor that takes a compatible function pointer, functor or lambda,
    and makes itself a owner of a copy of the argument.
    Note that this constructor is not declared as `explicit` as that implicit
    conversion from compatible types are possible.

  .. cpp:function:: const function<R, ... Args>& operator=(implicit_type)
    :noindex:

    Assignment operator that marks assignment from compatible function types.

  .. cpp:function:: R operator() (... Args) const
    :noindex:

    Invocation operator that takes arguments that are compatible with this
    function type and returns the result of the function.

  .. cpp:function:: operator implicit_type()
    :noindex:

    Conversion operator that converts this instance to the underlying compatible
    function type.

  .. cpp:function:: void invoke_async(... Args)
    :noindex:

    Invokes the function asynchronously.


.. cpp:class:: sneaker::functional::call
----------------------------------------

A variant of `sneaker::functional::function` that is compatible with functions,
functors and lambdas whose signatures take no arguments and has no return type.


.. cpp:class:: sneaker::functional::action< ...Args>
---------------------------------------------------------

A variant of `sneaker::functional::function` that is compatible with functions,
functors and lambdas whose signatures take a list of statically typed arguments
but has no return type.


.. cpp:class:: sneaker::functional::predicate< ...Args>
------------------------------------------------------------

A variant of `sneaker::functional::function` that is compatible with functions,
functors and lambdas whose signatures take a list of statically typed arguments,
and has a return type of `bool`.


Decorators
==========

Function abstractions that facilitate the use of the decorator pattern. These decorators
provide a higher level of operations on top of the encapsulating functions without having
to modifiy their functionalities. Examples such as retries, error handling and logging are
good examples of using decorators. Multiple decorators can be chained together together so
that different operations can be stacked on top of each other.

.. cpp:class:: sneaker::functional::retry<R, ...Args>
-----------------------------------------------------

  A decorator that retries on the encapsulating function upon invocations that has an exception
  thrown. User can specify the type of exception to catch and the number of retries allowed
  for the encapsulating function.

  Here is an example:

  .. code-block:: cpp

    #include <vector>
    #include <mysql> // ficticious
    #include <sneaker/functional/retry.h>

    // Suppose we have a function that takes an instance of a
    // MySQL connection object, tries to connect to it, and
    // queries some results. This can potentially have a connection
    // issue sporadically, so we want to issue a maximum of 5 retries.
    retry<void> wrapper = [](mysql::db_connection& connection) -> void {
      mysql::connection_result conn_result = connection.connect();
      mysql::query_result = conn_result.query(MyModel.list());
      std::vector<MyModel> models = query_result.normalize();
      printModels(models);
    };

    const int MAX_RETRY = 5;

    // Invokes the function above to connect to the MySQL instance
    // and queries the results, can retry 5 timees on connection error.
    wrapper.operator()<mysql::connection_error, MAX_RETRY>();

  .. cpp:function:: template<typename ExceptionType, uint32_t MaxRetries>
                    R operator() (... Args) const
    :noindex:

    Invocation operator that takes arguments that are compatible with this
    encapsulating function type and returns the result of the function.
    Also specifies the exception type and max count on retry. 
