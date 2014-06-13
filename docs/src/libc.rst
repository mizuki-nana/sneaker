**************
1. C Utilities
**************


Types, functions and data structures in C.


1.1 Array
=========

Array abstraction that can dynamically increase and shrink capacity.

Header file: `sneaker/libc/vector.h`

.. c:type:: vector_t
--------------------

  .. c:function:: vector_t vector_create()
    :noindex:

    Creates an instance of `vector_t` using dynamically allocated memory.

  .. c:function:: void vector_free(vector_t*)
    :noindex:

    Frees memory from the pointer of an instance of `vector_t` specified.

  .. c:function:: int vector_append(vector_t, void*)
    :noindex:

    Appends an element to the end of a `vector_t` instance specified as the
    first argument that holds a pointer which points to the value specified in
    the second argument. Returns `1` if successful, `0` otherwise.

  .. c:function:: void* vector_get(vector_t, int)
    :noindex:

    Retrieves the element pointer from a specified `vector_t` instance as the
    first argument by the zero-based index from the second argument. If the
    index given is out of bound, then the code exits, so it's better to use
    `vector_size()` to make sure the index is within the bound of the vector.

  .. c:function:: void* vector_remove(vector_t, int)
    :noindex:

    Removes an element from the specified `vector_t` instance at the index in the
    second argument. If the index is within the bound of the vector, the element
    is removed and the pointer held in that element is returned. If the index
    is out of bound, then the code exits, so it's better to use `vector_size()`
    to make sure the index is within the bound of the vector.

  .. c:function:: void* vector_set(vector_t, int, void*)
    :noindex:

    Sets the pointer held of a particular element from the specified `vector_t`
    instance. The new pointer specified in the third argument will replace the
    existing one in the encapsulating element, and the old pointer will be
    returned. Likewise, if the index in the second argument is out of bound,
    then the code will exit.

  .. c:function:: int vector_size(vector_t)
    :noindex:

    Gets the number of elements in the instance of `vector_t` provided.

  .. c:function:: const void** vector_content(vector_t)
    :noindex:

    Retrieves a list of the element pointers contained in the instance of
    `vector_t` specified.


1.2 Bitmap
==========

Two-dimensional bitmap abstraction.

Header file: `sneaker/libc/bitmap.h`

.. c:type:: bitmap_t
--------------------

  .. c:function:: bitmap_t bitmap_create(size_t, size_t)
    :noindex:

    Creates an instance of `bitmap_t` using dynamically allocated memory by
    specifying the width and height of the bitmap, which are the number of bits
    in each respective dimension.

  .. c:function:: void bitmap_free(bitmap_t *)
    :noindex:

    Frees memory from the pointer of an instance of `bitmap_t` specified.

  .. c:function:: size_t bitmap_width(bitmap_t)
    :noindex:

    Gets the width of the `bitmap_t` instance specified.

  .. c:function:: size_t bitmap_height(bitmap_t)
    :noindex:

    Gets the height of the `bitmap_t` instance specified.

  .. c::function:: int bitmap_set_bit(bitmap_t, size_t, size_t)
    :noindex:

    Sets a particular bit to `1` in the `bitmap_t` instance specified. The
    second and third arguments respectively specify the row and column of the
    bit to set, both are zero-based indices. Returns `1` if successful, `0`
    otherwise.

    If either the row or column specified are out of bound, the function returns
    `0`.

  .. c:function:: int bitmap_clear_bit(bitmap_t, size_t, size_t)
    :noindex:

    Clears a particular bit to `0` in the `bitmap_t` instance specified. The
    second and third arguments respectively specify the row and column of the
    bit to clear, both are zero-based indices. Returns `1` if successful, `0`
    otherwise.

    If either the row or column specified are out of bound, the function returns
    `0`.

  .. c:function:: int bitmap_is_set(bitmap_t, size_t, size_t)
    :noindex:

    Returns a value indicating whether a particular bit in the `bitmap_t`
    specified is set to `1`. The second and third arguments respectively specify
    the row and column of the bit, both are zero-based indices.
    Returns `1` if the bit is set, `0` otherwise.

    If either the row or column specified are out of bound, the function returns
    `0`.

  .. c:function:: void bitmap_clear(bitmap_t)
    :noindex:

    Sets every bit in the `bitmap_t` specified to `0`.


1.3 C-String Types
==================

Short-hand notations for C-String types.

Header file: `sneaker/libc/cstr.h`

.. c:type:: c_str
-----------------

  typedef of `char*`


.. c:type:: cc_str
------------------

  typedef of `const char*`


1.4 Standard Library Functions
==============================

Standard functions in C that might not be available in some compilers.

Header file: `sneaker/libc/cutils.h`

.. c:function:: char* itoa(int, char*, int)

  Integer to ASCII string conversion.

  Converts an integer value specified as the first argument to a null-terminated
  string specified as the second argument. The base of the integer can be
  optionally specified as the third argument, which defaults to `10` if not
  specified. Returns the converted ASCII string.

  If base is `10` and value is negative, the resulting string is preceded with
  a minus sign (-). With any other bases, value is always considered unsigned.


.. c:function:: char* atoi(const char*)

  ASCII string to integer conversion.

  Converts an ASCII string specified as the first argument to an integer number.

  The function first discards as many whitespace characters as necessary until
  the first non-whitespace character is found. Then, starting from this
  character, takes an optional initial plus or minus sign followed by as many
  numerical digits as possible, and interprets them as a numerical value.


1.5 Dictionary
==============

A dictionary implementation with C-string typed keys, based on the
:c:func:`hashmap_t` implementation.

Header file: `sneaker/libc/dict.h`

.. c:type:: dict_t
------------------

  .. c:function:: dict_t dict_create()
    :noindex:

    Creates an instance of `dict_t` using dynamically allocated memory.

  .. c:function:: size_t dict_size(dict_t)
    :noindex:

    Gets the number of key-value pairs in the `dict_t` instance specified.

  .. c:function:: void dict_free(dict_t*)  
    :noindex:

    Frees memory from the pointer of an instance of `dict_t` specified.

  .. c:function:: void* dict_put(dict_t, const char*, void*)
    :noindex:

    Insert a key-value pair into the `dict_t` instance specified as the first
    argument. The second argument is the key and the third argument is a
    pointer of the value to be stored. Returns the pointer in the pair inserted.
    If the key already exists, then the existing value is replaced.

  .. c:function:: void* dict_get(dict_t, const char*)
    :noindex:

    Retrieves the value associated with a key specified as the second argument
    from the `dict_t` instance specified as the first argument. Returns the
    pointer of the value associated with the key if the key exists, `NULL`
    otherwise.


1.6 Hashing
===========

Hashing algorithms.

Header file: `sneaker/libc/hash.h`

.. c:function:: unsigned long int linear_horners_rule_str_hash(const char*)

  Calculates the hash value of the specified string using a linear version of
  Horner's Rule.

.. c:function:: unsigned long int hash32shift(unsigned int)
  :noindex:

  Calculates the hash of a 32-bit unsigned integer.

.. c:function:: unsigned long int hash64shift(unsigned long)
  :noindex:

  Calculates the hash of a 64-bit unsigned integer.

.. c:function:: unsigned long int hash_str_jenkins_one_at_a_time(const char*)
  :noindex:

  Calculates the hash value of the specified string using the
  "Jenkins's one-at-a-time" algorithm.

.. c:function:: unsigned long int hash_robert_jenkin(unsigned int)
  :noindex:

  Calculates the hash value of the specified string using the
  "Robert Jenkin" algorithm.


1.7 Hash Map
============

A hash map implementation that offers near constant-time lookups and inserts.

Header file: `sneaker/libc/hashmap.h`

.. c:type:: hashmap_t
---------------------

  .. c:type:: HashFunc

    Hash function used to hash keys in `hashmap_t`, defined as
    `unsigned long int(*HashFunc)(void*)`.

  .. c:type:: KeyCmpFunc

    Key comparison function used to compare keys in the hash map, defined as
    `int(*KeyCmpFunc)(void*, void*)`.

  .. c:function:: hashmap_t hashmap_create(size_t, HashFunc, KeyCmpFunc)
    :noindex:

    Creates an instance of `hashmap_t` using dynamically allocated memory, and
    by specifying the initial capacity as the first argument, as well as the
    hashing and key comparison functions as the second and third arguments.

  .. c:function:: size_t hashmap_size(hashmap_t)
    :noindex:

    Gets the number of elements in the specified `hashmap_t` instance.

  .. c:function:: void hashmap_lock(hashmap_t)
    :noindex:

    Locks the specified `hashmap_t` instance so that updates from other threads
    are blocks until `hashmap_unlock` is called on the instance.

  .. c:function:: void hashmap_unlock(hashmap_t)
    :noindex:

    Unlocks the specified `hashmap_t` instance following a previous call of
    `hashmap_lock` so that updates from other threads are unblocked.

  .. c:function:: void hashmap_free(hashmap_t*)
    :noindex:

    Frees memory from the pointer of an instance of `hashmap_t` specified.

  .. c:function:: void* hashmap_put(hashmap_t, void*, void*)
    :noindex:

    Inserts a key-value pair into the `hashmap_t` instance as the first argument.
    If the key already exists, its associated value will be updated. Returns the
    value inserted if successful, `NULL` otherwise.

  .. c:function:: void* hashmap_get(hashmap_t, void*)
    :noindex:

    Retrieves the value associated with the key specified as the second argument
    from the `hashmap_t` instance specified as the first argument. Returns the
    value if its associated key is in the instance, `NULL` otherwise.

  .. c:function:: int hashmap_contains_key(hashmap_t, void*)
    :noindex:

    Returns a value indicating if the key specified as the second argument exists
    in the `hashmap_t` instance specified as the first argument. Returns `1` if
    the key is in the instance, `0` otherwise.

  .. c:function:: void* hashmap_remove(hashmap_t, void*)
    :noindex:

    Removes the key-value pair from the `hashmap_t` instance specified as the
    first argument using the key as the second argument. Returns the value
    associated with the key if found, `NULL` otherwise.

  .. c:function:: void* hashmap_lookup(hashmap_t, void*, void*)
    :noindex:

    Looks up a particular key-value pair from the `hashmap_t` instance as the
    first argument. The lookup function is the second argument that has the
    signature `int(*lookup)(void *key, void *value, void* arg)`, which takes
    a key, its associated value and an optional argument which is specified as
    the third argument. The lookup function returns `1` for a key-value pair
    considered as found, at which the function stops the searching and returns
    the value found. If no key-value is found, returns `NULL`.

  .. c:function:: void hashmap_iterate(hashmap_t, void*, int)
    :noindex:

    Iterates through all key-value pairs in the `hashmap_t` instance as the
    first argument. The second argument specifies a function of signature
    `int(*callback)(void *key, void *value)` that receives every key-value pair
    iterated. The third argument specifies a value that indicates whether the
    iteration should continue when the callback function returns a non-truthy
    value.

  .. c:function:: size_t hashmap_bucketcount(hashmap_t)
    :noindex:

    Returns the number of buckets in the `hashmap_t` instance specified as the
    first argument.

  .. c:function:: size_t hashmap_capacity(hashmap_t)
    :noindex:

    Returns the capacity of the `hashmap_t` instance specified as the
    first argument.

  .. c:function:: int hashmap_equal(hashmap_t, hashmap_t)
    :noindex:

    Determines equality between the two instances of `hashmap_t` specified. The
    two instances would only be considered equal if they have exactly the same
    keys and all associated values of the same keys must be identical as well.
    Returns `1` if they are considered equal, `0` otherwise.


1.8 Math
========

General functions for mathematical computations.

Header file: `sneaker/libc/math.h`

.. c:function:: unsigned int nearest_exp2_ceil(unsigned int)

  Computes the least largest exponent of base 2 that is closest to the input
  value.


.. c:function:: unsigned int nearest_exp2_floor(unsigned int)

  Computes the most largest exponent of base 2 that is closest to the input
  value.



1.9 Queue
=========

A FIFO storage container.

Header file: `sneaker/libc/queue.h`

.. c:type:: queue_t
-------------------

  .. c:function:: queue_t queue_create()
    :noindex:

    Creates an instance of `queue_t` using dynamically allocated memory.

  .. c:function:: size_t queue_size(queue_t)
    :noindex:

    Gets the number of elements in the `queue_t` instance specified.

  .. c:function:: void* queue_front(queue_t)
    :noindex:

    Gets the element pointer at the front of the `queue_t` instance specified.
    If the queue is empty, `NULL` is returned.

  .. c:function:: void* queue_back(queue_t)
    :noindex:

    Gets the element pointer at the back of the `queue_t` instance specified.
    If the queue is empty, `NULL` is returned.

  .. c:function:: int queue_push(queue_t, void*, size_t)
    :noindex:

    Pushes an element pointer to the back of the `queue_t` instance specified as
    the first argument. The second argument is a pointer that points to the
    value to be pushed onto the queue, and the third argument is the size of
    the value to be pushed, in number of bytes. Returns `-1` if the push failed,
    `1` if successful.

  .. c:function:: void* queue_pop(queue_t)
    :noindex:

    Pops the element pointer at the front of the `queue_t` instance specified
    and returns it. If the queue is empty, `NULL` is returned.

  .. c:function:: void queue_free(queue_t *)
    :noindex:

    Frees memory from the pointer of an instance of `queue_t` specified.


1.10 Stack
==========

A FILO storage container.

Header file: `sneaker/libc/stack.h`

.. c:type:: stack_t
-------------------

  .. c:function:: stack_t stack_create()
    :noindex:

    Creates an instance of `stack_t` using dynamically allocated memory.

  .. c:function:: size_t stack_size(stack_t)
    :noindex:

    Returns the number of elements in the `stack_t` instance specified.

  .. c:function:: void* stack_top(stack_t)
    :noindex:

    Gets the element pointer at the top of the `stack_t` instance provided.
    Returns `NULL` if the stack is empty.

  .. c:function:: int stack_push(stack_t, void*, size_t)
    :noindex:

    Pushes an element pointer to the top of the `stack_t` instance specified as
    the first argument. The second argument is a pointer that points to the
    value to be pushed onto the stack, and the third argument is the size of
    the value to be pushed, in number of bytes. Returns `-1` if the push failed,
    `1` if successful.

  .. c:function:: void* stack_pop(stack_t)
    :noindex:

    Pops the element pointer at the top of the `stack_t` instance specified and
    returns it. If the stack is empty, `NULL` is returned.

  .. c:function:: void stack_free(stack*)
    :noindex:

    Frees memory from the pointer of an instance of `stack_t` specified.


1.11 String Buffer
==================

A C-string buffer abstraction that can dynamically change capacity.

Header file: `sneaker/libc/strbuf.h`

.. c:type:: strbuf_t
--------------------

  .. c:function:: strbuf_t strbuf_t_create()
    :noindex:

    Creates an instance of `strbuf_t` using dynamically allocated memory.

  .. c:function:: void strbuf_free(strbuf_t*)
    :noindex:

    Frees memory from the pointer of an instance of `strbuf_t` specified.

  .. c:function:: void strbuf_empty(strbuf_t)
    :noindex:

    Empties the `strbuf_t` instance specified as the argument and reduces
    its capacity to `0`.

  .. c:function:: size_t strbuf_len(strbuf_t)
    :noindex:

    Gets the length of the string currently held in the `strbuf_t` instance
    specified as the argument.

  .. c:function:: const char* strbuf_cstr(strbuf_t)
    :noindex:

    Gets the C-string held in the `strbuf_t` instance specified as the argument.

  .. c:function:: size_t strbuf_capacity(strbuf_t)
    :noindex:

    Gets the capacity of the `strbuf_t` instance specified as the argument.

  .. c:function:: int strbuf_append(strbuf_t, const char*)
    :noindex:

    Appends a C-string into the `strbuf_t` instance specified as the first
    argument. The second argument is the C-string to be appended. Returns `1`
    if the append is successful, `0` otherwise.


1.12 String Manipulation
========================

String manipulation functions.

Header file: `sneaker/libc/strutils.h`

.. c:function:: char* strtoupper(char *)

  Converts the C-string specified as the argument to its uppercase form. Returns
  the converted string.

.. c:function:: char* strtolower(char *)

  Converts the C-string specified as the argument to its lowercase form. Returns
  the converted string.

.. c:function:: char* strtrim(char *)

  Trims off empty spaces at the front and end of the string specified. Returns
  the trimmed string.

.. c:function:: char* strcpy_hard(char*, const char*)

  Copies the source string specified as the second argument to the destination
  string specified as the first argument. If the length of the source string
  is greater than the length of the destination string, then the destination
  string is freed and re-allocated to have the same length as the source string.
  Returns the destination string.

.. c:function:: char* strncpy_safe(char*, const char*, size_t)

  Copies the source string specified as the second argument to the destination
  string specified as the first argument. The number of characters copied
  is the minimum between the length of the destination string and the size
  specified as the third argument. Returns the destination string.

.. c:function:: size_t strlcpy2(char*, const char*, size_t)

  Copies the source string specified as the second argument to the destination
  string specified as the first argument. The number of characters copied
  is at most the number specified as the third argument minus `1`. Returns the
  number of characters tried to be copied, which is the size of the source
  string.


1.13 General Utilities
======================

General utility functions.

Header file: `sneaker/libc/utils.h`

.. c:function:: void set_nth_bit(int*, char)

  Set a specific bit of a given number to 1. The first argument specifies
  a pointer to the number to be set, and the second argument is the `n`th bit
  to set from the LSB.

.. c:function:: void clear_nth_bit(int*, char)

  Clears a specific bit of a given number to 0. The first argument specifies
  a pointer to the number to be cleared, and the second argument is the `n`th
  bit to clear from the LSB.

.. c:function:: int is_bit_set(int, char)

  Checks if a specific bit of a given number is set to 1. The first argument
  specifies a pointer to the number to be checked, and the second argument is
  the `n`th bit to check from the LSB. Returns `1` if the bit is set to `1`,
  `0` otherwise.

.. c:function:: void set_nth_bit_uint32(uint32_t*, char)

  32-bit version of :c:func:`set_nth_bit`.

.. c:function:: void clear_nth_bit_uint32(uint32_t*, char)

  32-bit version of :c:func:`clear_nth_bit`.

.. c:function:: int is_bit_set_uint32(uint32_t, char)

  32-bit version of :c:func:`is_bit_set`.

.. c:function:: int rand_top(int)

  Returns an pseudo-random integer that's in the range between 1 and the number
  specified as the argument, inclusively.

.. c:function:: int rand_range(int, int)

  Returns an pseudo-random integer that's in the range between two integers
  specified as the two arguments, inclusively.

.. c:function:: double randf_top(double)

  Returns an pseudo-random double-precision floating number that's in the range
  between 1 and the number specified as the argument, inclusively.

.. c:function:: double randf_range(double, double)

  Returns an pseudo-random double-precision floating number that's in the range
  between two double-precision floating numbers specified as the two arguments,
  inclusively.

.. c:function:: char* generate_text(size_t=0, size_t=0)

  Generates a random string of an optional length specified as the first
  argument, up to the optional maximum length specified as the second argument.

  If both the length and max length are non-zero, the generated text will have a
  length between the two numbers.

  If only the length is non-zero, the generated text will have exactly the
  specified length.

  If only the max length is non-zero, the generated text will have a length
  between `1` and the max length, inclusively.

  If both the length and max length are zero, `NULL` is returned.

.. c:function:: char* generate_loremipsum()

  Generates a string of Lorem Ipsum of arbitrary length.


1.14 Universally Unique Identifier
==================================

A 128-bits implementation of UUID.

Header file: `sneaker/libc/uuid.h`

.. c:type:: uuid128_t
---------------------

  .. c:function:: uuid128_t uuid_create()
    :noindex:

    Creates an instance of `uuid128_t` using dynamically allocated memory.

  .. c:function:: int uuid_compare(const uuid128_t, const uuid128_t)
    :noindex:

    Evaluates equality between two instances of `uuid128_t`. Returns `1` if
    the first argument is considered greater than the second, `0` if they are
    equivalent, and `-1` if the first is less than the second.

  .. c:function:: __uint128_t uuid_to_hash(const uuid128_t)
    :noindex:

    Hashes the `uuid128_t` instance provided as the arugment to an 128-bit
    unsigned integer.

  .. c:function:: __uint128_t uuid_create_and_hash()
    :noindex:

    Creates an instance of `uuid128_t` and returns its equivalent hash.
