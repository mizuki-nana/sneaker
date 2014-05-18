**************************
6. Functional Abstractions
**************************

These functional abstractions improve the incompatibilites and
interoperabilities between function pointers, functors and lambdas.

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