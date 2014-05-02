****************
2. STL Allocator
****************

Components that make up the structure of memory allocation and deallocation for
STL containers.


2.1 Allocation Policy
=====================

Abstraction that encapsulates the logic to allocate and deallocate memory.


.. cpp:class:: sneaker::allocator::standard_alloc_policy<T>
-----------------------------------------------------------

  A default implementation of allocation policy by allocating and deallocating
  memory directly from the operating system.

  .. cpp:type:: value_type
    :noindex:

    The underling type that memory will be allocated for.

  .. cpp:type:: pointer
    :noindex:

    Pointer type of the memory allocated by this policy.

  .. cpp:type:: const_pointer
    :noindex:

    Constant pointer type of the memory allocated by this policy.

  .. cpp:type:: reference
    :noindex:

    Reference type of memory allocated by this policy.

  .. cpp:type:: const_reference
    :noindex:

    Constant reference type of memory allocated by this policy.

  .. cpp:type:: size_type
    :noindex:

    Type used for denoting the quantity of elements allocated by this policy.

  .. cpp:type:: difference_type
    :noindex:

    Type used for denoting the size difference between two pointers allocated by
    this policy.

  .. cpp:type:: propagate_on_container_move_assignment
    :noindex:

    Indicates that the policy shall propagate when the STL container is move-assigned.

  .. cpp:type:: rebind<U>
    :noindex:

    Rebinds to the equivalent allocator type to allocate elements of type `U`.

  .. cpp:function:: explicit standard_alloc_policy()
    :noindex:

    Explicit constructor.

  .. cpp:function:: ~standard_alloc_policy()
    :noindex:

    Destructor.

  .. cpp:function:: explicit standard_alloc_policy(standard_alloc_policy const&)
    :noindex:

    Explicit copy constructor. The argument is an instance of `standard_alloc_policy`
    with the same encapsulating type. Nothing is copied over.

  .. cpp:function:: template<typename U>
                    explicit standard_alloc_policy(standard_alloc_policy<U> const&)
    :noindex:

    Explicit copy constructor. The argument is an instance of `standard_alloc_policy`
    with a different encapsulating type. Nothing is copied over.

  .. cpp:function:: pointer allocate(size_type, typename std::allocator<void>::const_pointer=0)
    :noindex:

    Allocates a specified number of memory in bytes. The first argument specifies
    the number of bytes requested for allocation. The second argument may be a
    value previously obtained by another call to `allocate` and not yet freed
    with `deallocate`. This value may be used as a hint to improve performance
    by allocating the new block near the one specified.

    If allocation is successful, a pointer that points to the memory allocated
    is returned, otherwise `std::bad_alloc` is raised.

  .. cpp:function:: void deallocate(pointer, size_type)
    :noindex:

    Deallocates the pre-allocated memory. The first argument is a pointer that
    points to the memory that needs to be freed, and the second argument specifies
    the size of the memory in bytes.

  .. cpp:function:: size_type max_size() const
    :noindex:

    Get the maximum amount of memory that can be allocated, in number of bytes.


2.2 Object Traits
=================

Abstraction that control the construction and destruction of the
encapsulating type.


.. cpp:class:: sneaker::allocator::object_traits<T>
---------------------------------------------------

  A default implementation of object traits, where the construction and
  destruction of the underlying type is carried over by calling the type's
  constructor and destruction respectively.

  .. cpp:function:: explicit object_traits()
    :noindex:

    Explicit constructor.

  .. cpp:function:: ~object_traits()
    :noindex:

    Destructor.

  .. cpp:function:: template<typename U>
                    explicit object_traits(object_traits<U> const&)
    :noidex:

    Explicit copy constructor. The argument is an instance of `object_traits`
    that has a different encapsulating type. Nothing is copied over.

  .. cpp:function:: T* address(T&)
    :noindex:

    Get the address on a reference of an instance of the encapsulating type.

  .. cpp:function:: T const* address(T const&)
    :noindex:

    Get the address on a constant reference of an instance of the encapsulating
    type.

  .. cpp:function:: void construct(T*, const T&)
    :noindex:

    Instantiates an instance of the encapsulating type through copy semantics
    by using the specified allocated memory and an instance of the encapsulating type
    to be passed to the copy constructor of the instance to be created.

  .. cpp:function:: template<class U, class... Args>
                    void construct(U*, Args&&...)
    :noindex:

    Instantiates an instance of type `U` by using the specified allocated memory
    and the arguments used for instantiation. The first argument is a pointer
    that points to the pre-allocated memory and the remain arguments are arguments
    passed to the constructor of type `U`.

  .. cpp:function:: void destroy(T*)
    :noindex:

    Destroy an instantiated instance of the encapsulating type by calling the
    type's destructor. The specified argument is a pointer that points to an
    instance of the encapsulating type.


2.3 Allocator
=============

Abstraction that's responsible for allocating and deallocating memory used by
STL containers.


.. cpp:class:: sneaker::allocator::allocator<T, Policy, Traits>
---------------------------------------------------------------

  A default implementation of STL allocator based on the abstraction of
  allocation policy and object traits.

  .. cpp:type:: value_type
    :noindex:

    The underlying type that memory will be allocated for.

  .. cpp:type:: pointer
    :noindex:

    Pointer type of the memory allocated by this allocator.

  .. cpp:type:: const_pointer
    :noindex:

    Constant pointer type of the memory allocated by this allocator.

  .. cpp:type:: reference
    :noindex:

    Reference type of memory allocated by this allocator.

  .. cpp:type:: const_reference
    :noindex:

    Constant reference type of memory allocated by this allocator.

  .. cpp:type:: size_type
    :noindex:

    Type used for denoting the quantity of elements allocated by this allocator.

  .. cpp:type:: difference_type
    :noindex:

    Type used for denoting the size difference between two pointers allocated by
    this allocator.

  .. cpp:function:: explicit allocator()
    :noindex:

    Explicit constructor.

  .. cpp:function:: ~allocator()
    :noindex:

    Destructor.

  .. cpp:function:: allocator(allocator const& rhs)
    :noindex:

    Copy constructor that takes another allocator of the same encapsulating type.
    Nothing is copied over.

  .. cpp:function:: template<typename U>
                    allocator(allocator<U> const&)
    :noindex:

    Copy constructor that takes another allocator of a different encapsulating
    type. Nothing is copied over.

  .. cpp:function:: template <typename U, typename P, typename T2>
                    allocator(allocator<U, P, T2> const& rhs)
    :noindex:

    Copy constructor that takes another allocator of a different encapsulating
    type, as well as different allocation policy and object traits. Nothing is
    copied over.

  .. cpp:function:: template<typename T, typename P, typename Tr>
                    bool operator==(allocator<T, P, Tr> const&, allocator<T, P, Tr> const&)
    :noindex:

    Equality operator that evaluates equality between two instances of
    `allocator` that have the same encapsulating type, and are based on the same
    allocation policy and object traits.

  .. cpp:function:: template<typename T, typename P, typename Tr, typename T2, typename P2, typename Tr2>
                    bool operator==(allocator<T, P, Tr> const&, allocator<T2, P2, Tr2> const&)
    :noindex:

    Equality operator that evaluates equality between two instances of
    `allocator` that have different encapsulating types, and are based on different
    allocation policy and object traits.

  .. cpp:function:: template<typename T, typename P, typename Tr, typename other_allocator>
                    bool operator==(allocator<T, P, Tr> const&, other_allocator const&)
    :noindex:

    Equality operator that evaluates equality between two instances of
    `allocator` that have potentially the same encapsulating types, and are
    based on the same allocation policy and object traits.

  .. cpp:function:: template<typename T, typename P, typename Tr>
                    bool operator!=(allocator<T, P, Tr> const&, allocator<T, P, Tr> const&)
    :noindex:

    Inequality operator that evaluates inequality between two instances of
    `allocator` that have the same encapsulating type, and are based on the same
    allocation policy and object traits.

  .. cpp:function:: template<typename T, typename P, typename Tr, typename T2, typename P2, typename Tr2>
                    bool operator!=(allocator<T, P, Tr> const&, allocator<T2, P2, Tr2> const&)
    :noindex:

    Inequality operator that evaluates inequality between two instances of
    `allocator` that have different encapsulating types, and are based on different
    allocation policy and object traits.

  .. cpp:function:: template<typename T, typename P, typename Tr, typename other_allocator>
                    bool operator!=(allocator<T, P, Tr> const&, other_allocator const&)
    :noindex:

    Inequality operator that evaluates inequality between two instances of
    `allocator` that have potentially different encapsulating types, and are
    based on different allocation policy and object traits.