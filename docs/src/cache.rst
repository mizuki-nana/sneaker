*****************************
4. In-memory Cache Management
*****************************

Components that manage caching of in-memory objects.


4.1 Generic Cache
=================

A generic key-value based in-memory cache, where the creation and destruction of
elements in the cache are controlled by custom handler types.

Header file: `sneaker/cache/generic_cache.h`

.. cpp:class:: sneaker::cache::generic_cache<K, T, CreateHandler, DestroyHandler>
---------------------------------------------------------------------------------

  .. cpp:function:: explicit generic_cache(CreateHandler, DestroyHandler)
    :noindex:

    Constructor. Takes an instance of `CreationHandler`
    (signature `bool(*)(K, T*)`) and an instance of `DestroyHandler`
    (signature `bool(*)(K, T*)`). The creation handler is invoked every time an
    element is created or updated, and the destroy handler is invoked when an
    element is erased from the cache.

  .. cpp:function:: ~generic_cache()
    :noindex:

    Destructor. Upon invoked, all the values in the cache are destroyed by the
    destroy handler.

  .. cpp:function:: bool empty() const
    :noindex:

    Determines whether the cache has any elements. Returns `true` if the cache
    has any element, `false` otherwise.

  .. cpp:function:: size_t size() const
    :noindex:

    Gets the number of elements in the cache.

  .. cpp:function:: bool member(K) const
    :noindex:

    Determines if the value associated with the specified key is in the cache.
    Returns `true` if there is a value associated with the key in the cache,
    `false` otherwise.

  .. cpp:function:: bool get(K, T*)
    :noindex:

    Retrieves the element associated with the specified key in the cache.
    The first argument is the key, and the second argument is a pointer that is
    set to point to the value if found. If the key-value pair does not exist,
    it is automaticaly created. Returns `true` if the value is found, `false`
    otherwise.

  .. cpp:function:: bool put(K, bool=false)
    :noindex:

    Constructs the element with the specified key and stores it in the cache.
    The first argument specifies the key, and the second argument is a boolean
    value indicating whether to force update if the key-value pair already
    exists. If set to force update, then the existing key-value pair is
    destroyed and re-created. Returns a boolean indicating whether an update has
    occured.

  .. cpp:function: bool erase(K)
    :noindex:

    Erase the element associated with the specified key in the cache. The first
    argument is the key associated with the value that needs to be erased.
    Returns `true` is the key-value pair is erased, `false` otherwise.

  .. cpp:function: void clear()
    :noindex:

    Clears the cache by destroying all elements within.
