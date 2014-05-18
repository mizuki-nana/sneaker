*************
4. Containers
*************

Storage containers of objects that serve a broad range of purposes.


4.1 Reservation-based Containers
================================

Container types that store objects on a reservation-based system. Users must
reserve spots before objects are requested to be stored in these containers.

Header file: `sneaker/container/reservation_map.h`

.. cpp:class:: sneaker::allocator::reservation_map<T>
-----------------------------------------------------

  Note: The internal implementation is based on `std::map<token_t, T>`, hence
  elements of type `T` must have their comparators defined for comparison.

  .. cpp:type:: token_t
    :noindex:

    The token type used by the reservation container.

  .. cpp:type:: generator_type
    :noindex:

    The type that is used to generate reservation tokens internally.

  .. cpp:function:: reservation_map()
    :noindex:

    Constructor.

  .. cpp:function:: ~reservation_map()
    :noindex:

    Destructor.

  .. cpp:function:: size_t size() const
    :noindex:

    Gets the number of elements that are currently reserved.

  .. cpp:function:: token_t reserve()
    :noindex:

    Reserve a spot in the container. Returns a token that can be used later
    for storage. The user must call this method before attempts to store
    an object in the container.

  .. cpp:function:: bool member(token_t) const
    :noindex:

    Determines if an object is a member of the container by using a token.
    Returns `true` if a spot has been reserved for the token specified,
    `false` otherwise.

  .. cpp:function:: bool put(token_t, T)
    :noindex:

    Attempts to store an object into the container by using a token. The storage
    fails if the token specified is invalid. Returns `true` if the storage is
    successful, `false` otherwise.

  .. cpp:function:: bool get(token_t, T*)
    :noindex:

    Attempts to retrieve an object from the container by using a token.
    The retrieval fails if the token specified is invalid. Returns `true` if the
    retrieval is successful, `false` otherwise.

  .. cpp:function:: bool unreserve(token_t)
    :noindex:

    Attempts to unreserve a previously reserved spot by using a token.
    The un-reservation fails if no previously reservation has been made by the
    token specified. Returns `true` if the un-reservation is successful, `false`
    otherwise.

  .. cpp:function:: void clear()
    :noindex:

    Removes all the reserved elements. After invoked, all tokens previously
    obtained are no longer valid.


4.2 Assorted-values Map Containers
==================================

Key-value(s) based map containers where each key can be mapped to an assortment
of multiple values of different statically defined types.


.. cpp:class:: sneaker::allocator::assorted_value_map<K, ... ValueTypes>
------------------------------------------------------------------------

  An implementation of assorted-values map container based on `std::map`.

  .. cpp:type:: key_type
    :noindex:

    The type of the keys in the mapping.

  .. cpp:type:: mapped_type
    :noindex:

    The type of the assortment of values in the mapping.

  .. cpp:type:: value_type
    :noindex:

    The type of the key-value(s) pairs in the mapping.

  .. cpp:type:: size_type
    :noindex:

    The type that indicates the number of elements in the mapping.

  .. cpp:type:: iterator
    :noindex:

    The type of forward iterator of the mapping.

  .. cpp:type:: const_iterator
    :noindex:

    The type of constant forward iterator of the mapping.

  .. cpp:type:: reverse_iterator
    :noindex:

    The type of reverse iterator of the mapping.

  .. cpp:type:: const_reverse_type
    :noindex:

    The type of constant reverse iterator of the mapping.

  .. cpp:function:: explicit assorted_value_map()
    :noindex:

    Constructor.

  .. cpp:function:: ~assorted_value_map()
    :noindex:

    Destructor.

  .. cpp:function:: assorted_value_map(const assorted_value_map&)
    :noindex:

    Copy constructor. The mapping is copied over.

  .. cpp:function:: bool empty() const
    :noindex:

    Determines whether the mapping is empty. Returns `true` if there are no
    key-value(s) pairs in the mapping, `false` otherwise.

  .. cpp:function:: size_type size() const
    :noindex:

    Determines the number of key-value(s) pairs in the mapping.

  .. cpp:function:: size_type max_size() const
    :noindex:

    Determines the maximum number of key-value(s) pairs that can be in the mapping.

  .. cpp:function:: void insert(K, ValueTypes)
    :noindex:

    Inserts a key-value(s) pair into mapping. If the specified key already exists
    in the mapping, its value(s) will be overwritten.

  .. cpp:function:: void erase(iterator)
    :noindex:

    Erases a particular key-value(s) pair in the mapping by an iterator.
    The iterator must point to a valid pair in the mapping to be effective.

  .. cpp:function:: size_type erase(const K&)
    :noindex:

    Erases a particular key-value(s) pair in the mapping by a key.
    Returns the number of elements erased. Note if the specified key does not exist
    in the mapping, then the number of elements returned is `0`.

  .. cpp:function:: void erase(iterator, iterator)
    :noindex:

    Erases a range of key-value(s) in the mapping in between in the two specified
    iterators, inclusively.

  .. cpp:function:: void swap(assorted_value_map&)
    :noindex:

    Swaps the mapping with another instance of `assorted_value_map` with the
    same types for the key and values.

  .. cpp:function:: void clear() noexcept
    :noindex:

    Clears the content in the mapping.

  .. cpp:funciton:: mapped_type& at(K)
    :noindex:

    Retrieves the value(s) associated with the specified key by reference.

  .. cpp:function:: const mapped_type& at(K) const
    :noindex:

    Retrieves the value(s) associated with the specified key by constant
    reference.

  .. cpp:function:: iterator begin()
    :noindex:

    Gets a forward iterator that marks the beginning of the mapping.

  .. cpp:function:: const_iterator begin() const
    :noindex:

    Gets a constant forward iterator that marks the beginning of the mapping.

  .. cpp:function:: iterator end()
    :noindex:

    Gets a forward iterator that marks the end of the mapping.

  .. cpp:function:: const_iterator end() const
    :noindex:

    Gets a constant forward iterator that marks the end of the mapping.

  .. cpp:function:: reverse_iterator rbegin()
    :noindex:

    Gets a reverse iterator that marks the beginning of the mapping in reverse order.

  .. cpp:function:: const_reverse_iterator rbegin()
    :noindex:

    Gets a constant reverse iterator that marks the beginning of the mapping
    in reverse order.

  .. cpp:function:: reverse_iterator rend()
    :noindex:

    Gets a reverse iterator that marks the end of the mapping in reverse order.

  .. cpp:function:: const_reverse_order rend() const
    :noindex:

    Gets a constant reverse iterator that marks the end of the mapping in
    reverse order.

  .. cpp:function:: iterator find(K)
    :noindex:

    Attempts to find the value(s) associated in the specified key. Returns an
    instance of forward iterator that points to the key-value(s) pair. If the
    key does not exist in the mapping, then the iterator returned points to
    `end()`.

  .. cpp:function:: const_iterator find(K) const
    :noindex:

    Attempts to find the value(s) associated in the specified key. Returns an
    instance of constant forward iterator that points to the key-value(s) pair.
    If the key does not exist in the mapping, then the iterator returned points
    to `end()`.


.. cpp:class:: sneaker::allocator::unordered_assorted_value_map<K, ... ValueTypes>
----------------------------------------------------------------------------------

  An implementation of assorted-values map container based on `std::unordered_map`.

  .. cpp:type:: key_type
    :noindex:

    The type of the keys in the mapping.

  .. cpp:type:: mapped_type
    :noindex:

    The type of the assortment of values in the mapping.

  .. cpp:type:: value_type
    :noindex:

    The type of the key-value(s) pairs in the mapping.

  .. cpp:type:: hasher
    :noindex:

    The type used to hash the keys.

  .. cpp:type:: key_equal
    :noindex:

    The type used to evaluate equality between two keys.

  .. cpp:type:: allocator_type
    :noindex:

    The type of allocator used to allocate memory.

  .. cpp:type:: reference
    :noindex:

    The reference type fot a key-value(s) pair.

  .. cpp:type:: const_reference
    :noindex:

    The constant reference type for a key-value(s) pair.

  .. cpp:type:: iterator
    :noindex:

    A bidirectional iterator to `value_type`.

  .. cpp:type:: const_iterator
    :noindex:

    A bidirectional iterator to `const value_type`.

  .. cpp:type:: reverse_iterator
    :noindex:

    A reverse order iterator to `value_type`.

  .. cpp:type:: const_reverse_iterator
    :noindex:

    A reverse order iterator to `const value_type`.

  .. cpp:type:: size_type
    :noindex:

    The type that indicates the number of elements in the mapping.

  .. cpp:type:: difference_type
    :noindex:

    A type that represents the difference between two iterators.

  .. cpp:function:: unordered_assorted_value_map()
    :noindex:

    Constructor.

  .. cpp:function:: unordered_assorted_value_map(const unordered_assorted_value_map&)
    :noindex:

    Copy constructor. The mapping from the argument is copied over.

  .. cpp:function:: ~unordered_assorted_value_map()
    :noindex:

    Destructor. The mapping is destroyed.

  .. cpp:function:: bool empty() const
    :noindex:

    Determines whether the mapping is empty. Returns `true` if there are no
    key-value(s) pairs in the mapping, `false` otherwise.

  .. cpp:function:: size_type size() const
    :noindex:

    Determines the number of key-value(s) pairs in the mapping.

  .. cpp:function:: size_type max_size() const
    :noindex:

    Determines the maximum number of key-value(s) pairs that can be in the mapping.

  .. cpp:function:: void insert(K, ValueTypes)
    :noindex:

    Inserts a key-value(s) pair into mapping. If the specified key already exists
    in the mapping, its value(s) will be overwritten.

  .. cpp:function:: void erase(iterator)
    :noindex:

    Erases a particular key-value(s) pair in the mapping by an iterator.
    The iterator must point to a valid pair in the mapping to be effective.

  .. cpp:function:: size_type erase(const K&)
    :noindex:

    Erases a particular key-value(s) pair in the mapping by a key.
    Returns the number of elements erased. Note if the specified key does not exist
    in the mapping, then the number of elements returned is `0`.

  .. cpp:function:: void erase(iterator, iterator)
    :noindex:

    Erases a range of key-value(s) in the mapping in between in the two specified
    iterators, inclusively.

  .. cpp:function:: void swap(assorted_value_map&)
    :noindex:

    Swaps the mapping with another instance of `assorted_value_map` with the
    same types for the key and values.

  .. cpp:function:: void clear() noexcept
    :noindex:

    Clears the content in the mapping.

  .. cpp:funciton:: mapped_type& at(K)
    :noindex:

    Retrieves the value(s) associated with the specified key by reference.

  .. cpp:function:: const mapped_type& at(K) const
    :noindex:

    Retrieves the value(s) associated with the specified key by constant
    reference.

  .. cpp:function:: template<class A, size_t Index>
                    A get(K)
    :noindex:

    Retrieves a particular value among the assortment of values associated
    with the specified key. Type `A` is the type of the value, and `Index` is
    a zero-based index that specifies the position of the value to be retrieved,
    among the list of values.

  .. cpp:function:: template<class A, size_t Index>
                    const A& get(K) const
    :noindex:

    Retrieves a particular value by reference among the assortment of values
    associated with the specified key. Type `A` is the type of the value,
    and `Index` is a zero-based index that specifies the position of the value
    to be retrieved, among the list of values.

  .. cpp:function:: iterator begin()
    :noindex:

    Gets a forward iterator that marks the beginning of the mapping.

  .. cpp:function:: const_iterator begin() const
    :noindex:

    Gets a constant forward iterator that marks the beginning of the mapping.

  .. cpp:function:: iterator end()
    :noindex:

    Gets a forward iterator that marks the end of the mapping.

  .. cpp:function:: const_iterator end() const
    :noindex:

    Gets a constant forward iterator that marks the end of the mapping.

  .. cpp:function:: iterator find(K)
    :noindex:

    Attempts to find the value(s) associated in the specified key. Returns an
    instance of forward iterator that points to the key-value(s) pair. If the
    key does not exist in the mapping, then the iterator returned points to
    `end()`.

  .. cpp:function:: const_iterator find(K) const
    :noindex:

    Attempts to find the value(s) associated in the specified key. Returns an
    instance of constant forward iterator that points to the key-value(s) pair.
    If the key does not exist in the mapping, then the iterator returned points
    to `end()`.

  .. cpp:function:: float load_factor() const noexcept
    :noindex:

    Gets the current load factor of the mapping, which is the ratio between the
    number of key-value(s) pair in the mapping and the number of buckets.

  .. cpp:function:: float max_load_factor() const noexcept
    :noindex:

    Get the maximum load factor the mapping can have.

  .. cpp:function:: void rehash(size_type)
    :noindex:

    Sets the number of buckets in the mapping to `n` or more by enforcing
    a rehash on all the keys in the mapping.

    If `n` is greater than the current number of buckets in the mapping
    (bucket_count), a rehash is forced. The new bucket count can either be equal
    or greater than `n`.

    If `n` is lower than the current number of buckets in the mapping
    (bucket_count), the function may have no effect on the bucket count and may
    not force a rehash.

    Rehashes are automatically performed by the container whenever its load
    factor is going to surpass its max_load_factor in an operation.

  .. cpp:function:: void reserve(size_type)
    :noindex:

    Request a capacity change on the mapping by setting the number of buckets
    in the mapping to the most appropriate to contain at least the number of
    key-value(s) pairs specified by the first argument.

    If `n` is greater than the current bucket_count multiplied by the
    `max_load_factor`, the container's bucket_count is increased and a rehash is
    forced.

    If `n` is lower than that, the function may have no effect.

  .. cpp:function:: hasher hash_function() const
    :noindex:

    Returns the hash function object used by the mapping.

  .. cpp:function: key_equal key_eq() const
    :noindex:

    Returns the key equivalence comparison predicate used by the mapping.

  .. cpp:function: allocator_type get_allocator() const noexcept
    :noindex:

    Returns the allocator object used to construct the mapping.