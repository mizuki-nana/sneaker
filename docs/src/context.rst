*********************
5. Context Management
*********************

Components that faciliate automatic context management.

Context management unfetters users from the cumbersomness and efforts associated
with trivial cleanup actions, such as deallocations or destructions of resources
after exiting the scope where they are no longer needed.


.. cpp:class:: sneaker::context::context_manager
------------------------------------------------

An interface of context manager implementations.

Header file: `sneaker/context/context_manager.h`

  .. cpp:function:: virtual void __enter__()
    :noindex:

    Method invoked when entering the scope of the context. This method needs to
    be overriden with code that prepares the context, such as resource
    allocations, aquiring and opening file handles, etc.

  .. cpp:function:: virtual void __exit__()
    :noindex:

    Method invoked when exiting the scope of the context. This method needs to
    be overriden with code that closes the context, such as resource
    deallocations, closing and releasing file handles, etc.


.. cpp:function:: template<class F, class Args ...>
                  void scoped_context(context_manager*, F, ... Args)
--------------------------------------------------------------------

Function that allows a block of code to run under the context of a context
manager, in which the scope of the context is bounded by the scope of this
function. In other words, the context starts immediately after the invocation
of this function, and ends right before this function goes out of scope.

The first argument specifies a pointer of a context manager, the second argument
specifies the block of code or a function that executes under the context of
the context manager, and the remaining arguments are passed to the function
when it's invoked under the context.

Header file: `sneaker/context/scoped_context.h`


.. cpp:function:: template<class F, class Args ...>
                  void nested_context(std::vector<context_manager*>, F, ... Args)
---------------------------------------------------------------------------------

Function that allows a block of code to run under the nested contexts of a list
of context managers, in which the scope of the contexts are bounded by the scope
of this function. In other words, the contexts start immediately after the
invocation of this function, and end right before this function goes out of
scope.

The contexts from the list of context managers are nested, meaning that the
context of the second context manager in the list executes under the context of
the first context manager, and so on.

The first argument specifies the list of context managers, the second argument
specifies the block of code or a function that executes under the contexts of
the context managers, and the remaining arguments are passed to the function
when it's invoked under the contexts.

Header file: `sneaker/context/scoped_context.h`