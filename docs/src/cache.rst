**************************
In-memory Cache Management
**************************

Components that manage caching of in-memory objects.


Cache Interface
===============

*Sneaker* provides a generic and extensible cache interface that allows the use
of different caching schemes by the choices of users. This is the public
interface that users whom wish to use the caching mechanism will interact with.


Header file: `sneaker/cache/cache_interface.h`

.. cpp:class:: sneaker::cache::cache_interface<class CacheScheme, class OnInsert, class OnErase>
------------------------------------------------------------------------------------------------

  This class allows users to define a cache scheme, as well as handler types
  that are to be invoked during cache value insertions and erasures.

  .. cpp:type:: key_type
    :noindex:

    Type of the keys in the cache.

  .. cpp:type:: value_type
    :noindex:

    Type of the values in the cache.

  .. cpp:function:: cache_interface(const OnInsert& on_insert, const OnErase& on_erase)
    :noindex:

    Constructor that takes a reference of `OnInsert` and `OnErase` instances
    each.

  .. cpp:function:: bool empty() const
    :noindex:

    Determines whether the cache has any elements. Returns `true` if the cache
    has any element, `false` otherwise.

  .. cpp:function:: bool full() const
    :noindex:

    Determines whether the cache is full.

  .. cpp:function:: size_t size() const
    :noindex:

    Gets the number of elements in the cache.

  .. cpp:function:: bool find(key_type) const
    :noindex:

    Determines if the value associated with the specified key is in the cache.
    Returns `true` if there is a value associated with the key in the cache,
    `false` otherwise.

  .. cpp:function:: bool get(key_type, value_type&)
    :noindex:

    Retrieves the element associated with the specified key in the cache.
    The first argument is the key, and the second argument is a reference
    of the result value associated with the key. Returns `true` if the
    value is found, `false` otherwise.

  .. cpp:function:: void insert(key_type, const value_type&)
    :noindex:

    Inserts a key-value pair into the cache.

  .. cpp:function: bool erase(key_type)
    :noindex:

    Erase the element associated with the specified key in the cache. The first
    argument is the key associated with the value that needs to be erased.
    Returns `true` is the key-value pair is erased, `false` otherwise.

  .. cpp:function: void clear()
    :noindex:

    Clears the cache by destroying all elements within.


Cache Schemes
=============

*Sneaker* provides abstractions of some of the most well-known caching schemes
to users. These abstractions are meant to be used with the cache interface
described above.

LRU Cache
---------

This class encapsulates the logic of the *Least-Recently Used* caching scheme.

Header file: `sneaker/cache/lru_cache.h`

.. cpp:class:: sneaker::cache::lru_cache<typename K, typename V, size_t N>
--------------------------------------------------------------------------

  .. cpp:type:: key_type
    :noindex:

    Type of the keys in the cache.

  .. cpp:type:: value_type
    :noindex:

    Type of the values in the cache.

  .. cpp:member:: size_t N
    :noindex:

    The size of the cache.

  .. cpp:function:: bool empty() const
    :noindex:

    Determines whether the cache has any elements. Returns `true` if the cache
    has any element, `false` otherwise.

  .. cpp:function:: bool full() const
    :noindex:

    Determines whether the cache is full.

  .. cpp:function:: size_t size() const
    :noindex:

    Gets the number of elements in the cache.

  .. cpp:function:: bool find(key_type) const
    :noindex:

    Determines if the value associated with the specified key is in the cache.
    Returns `true` if there is a value associated with the key in the cache,
    `false` otherwise.

  .. cpp:function:: bool get(key_type, value_type&)
    :noindex:

    Retrieves the element associated with the specified key in the cache.
    The first argument is the key, and the second argument is a reference
    of the result value associated with the key. Returns `true` if the
    value is found, `false` otherwise.

  .. cpp:function:: void next_erasure_pair(key_type** key_ptr, value_type** value_ptr)
    :noindex:

    Gets the next key-value pair to be erased when inserting a new key-value
    pair while the cache is full.

  .. cpp:function:: void insert(key_type, const value_type&)
    :noindex:

    Inserts a key-value pair into the cache.

  .. cpp:function:: bool erase(key_type)
    :noindex:

    Erase the element associated with the specified key in the cache. The first
    argument is the key associated with the value that needs to be erased.
    Returns `true` is the key-value pair is erased, `false` otherwise.

  .. cpp:function:: void clear()
    :noindex:

    Clears the cache by erasing all elements within.

Example showing using `lru_cache` with `cache_interface`:

.. code-block:: cpp

    #include <sneaker/cache/cache_interface.h>
    #include <sneaker/cache/lru_cache.h>

    typedef int KeyType;
    typedef const char* ValueType;

    struct InsertHandler
    {
        bool operator()(KeyType key, const ValueType& value)
        {
            // Do something here.
        }
    };

    struct EraseHandler
    {
        bool operator()(KeyType key, const ValueType& value)
        {
            // Do something here.
        }
    };

    typedef sneaker::cache::cache_interface<
      sneaker::cache::lru_cache<KeyType, ValueType, 10>, InsertHandler, EraseHandler> CacheType;

    // Make use of CacheType..
    CacheType cache;
    cache.insert(1, "Hello world");
