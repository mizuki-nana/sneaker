**************
1. C Utilities
**************


Types, functions and data structures in C.


1.1 Array
=========

Array abstraction that can dynamically increase and shrink capacity.


.. cpp:type:: vector_t
----------------------

  .. cpp:function:: vector_t array_create()
    :noindex:

    Creates an instance of `vector_t` using dynamically allocated memory.

  .. cpp:function:: void vector_free(vector_t*)
    :noindex:

    Frees memory from the pointer of an instance of `vector_t` specified.

  .. cpp:function:: int vector_append(vector_t, void*)
    :noindex:

    Appends an element to the end of the vector specified in the first argument
    that holds a pointer that points to the value specified in the second
    argument. Returns `1` if successful, `0` otherwise.

  .. cpp:function:: void* vector_get(vector_t, int)
    :noindex:

    Retrieves the pointer from the specified `vector_t` instance by the zero-based
    index from the second argument. If the index given is out of bound, then
    the code exits, so it's better to use `array_size()` to make sure the index
    is within the bound of the vector.

  .. cpp:function:: void* vector_remove(vector_t, int)
    :noindex:

    Removes an element from the specified `vector_t` instance at the index in the
    second argument. If the index is within the bound of the vector, the element
    is removed and the pointer held in that element is returned. If the index
    is out of bound, then the code exits.

  .. cpp:function:: void* vector_set(vector_t, int, void*)
    :noindex:

    Sets the pointer held of a particular element from the specified `vector_t`
    instance. The new pointer specified in the third argument will replace the
    existing one in the encapsulating element, and the old pointer will be
    returned. Likewise, if the index is out of bound, then the code will exit.

  .. cpp:function:: int vector_size(vector_t)
    :noindex:

    Gets the number of elements in the instance of `vector_t` provided.

  .. cpp:function:: const void** vector_content(vector_t)
    :noindex:

    Retrieves a list of the pointers contained in the instance of `vector_t`
    specified.


1.2 Bitmap
==========


A two-dimensional bitmap.

.. cpp:type:: bitmap_t
----------------------

  .. cpp:function:: bitmap_t bitmap_create(size_t, size_t)
    :noindex:

    Creates an instance of `bitmap_t` using dynamically allocated memory by
    specifying the width and height of the bitmap, which are the number of bits
    in each respective dimension.

  .. cpp:function:: void bitmap_free(bitmap_t *)
    :noindex:

    Frees memory from the pointer of an instance of `bitmap_t` specified.

  .. cpp:function:: size_t bitmap_width(bitmap_t)
    :noindex:

    Gets the width of the `bitmap_t` instance specified.

  .. cpp:function:: size_t bitmap_height(bitmap_t)
    :noindex:

    Gets the height of the `bitmap_t` instance specified.

  .. cpp::function:: int bitmap_set_bit(bitmap_t, size_t, size_t)
    :noindex:

    Sets a particular bit to `1` in the `bitmap_t` instance specified. The
    second and third arguments respectively specify the row and column of the
    bit to set, both are zero-based indices. Returns `1` if successful, `0`
    otherwise.

    If either the row or column specified are out of bound, the function returns
    `0`.

  .. cpp:function:: int bitmap_clear_bit(bitmap_t, size_t, size_t)
    :noindex:

    Clears a particular bit to `0` in the `bitmap_t` instance specified. The
    second and third arguments respectively specify the row and column of the
    bit to clear, both are zero-based indices. Returns `1` if successful, `0`
    otherwise.

    If either the row or column specified are out of bound, the function returns
    `0`.

  .. cpp:function:: int bitmap_is_set(bitmap_t, size_t, size_t)
    :noindex:

    Returns a value indicating whether a particular bit in the `bitmap_t`
    specified is set to `1`. The second and third arguments respectively specify
    the row and column of the bit, both are zero-based indices.
    Returns `1` if the bit is set, `0` otherwise.

    If either the row or column specified are out of bound, the function returns
    `0`.

  .. cpp:function:: void bitmap_clear(bitmap_t):
    :noindex:

    Sets every bit in the `bitmap_t` specified to `0`.


1.3 Read/Write Buffer
=====================


A buffer implementation that supports reads and writes.

.. cpp:type:: buffer_t
----------------------

  .. cpp:function:: buffer_t buffer_create(size_t)
    :noindex:

    Creates an instance of `buffer_t` using dynamically allocated memory by
    specifying the initial capacity of the buffer, in number of bytes.

  .. cpp:function:: void buffer_free(buffer*)
    :noindex:

    Frees memory from the pointer of an instance of `buffer_t` specified.

  .. cpp:function:: int buffer_read_complete(buffer_t)
    :noindex:

    Returns a value indicating whether all data in the buffer have been read
    in the `buffer_t` specified. Returns `1` if all have been read,
    `0` otherwise.

  .. cpp:function:: int buffer_write_complete(buffer_t)
    :noindex:

    Returns a value indicating whether all data in the buffer have been written
    in the `buffer_t` specified. Returns `1` if all have been written,
    `0` otherwise.

  .. cpp:function:: char* buffer_get(buffer_t)
    :noindex:

    Gets the data in the `buffer_t` instance specified.

  .. cpp:function:: int buffer_prepare_for_read(buffer_t, size_t)
    :noindex:

    Prepares the `buffer_t` instance specified in the first argument for
    subsequent read, with the number of bytes specified in the second argument.

    The buffer's capacity is increased if the size of data expected to be read
    is larger than the current capacity. Returns `-1` if the buffer's capacity
    fails to be increased, `1` if the operation succeeds.

  .. cpp:function:: ssize_t buffer_read(buffer_t, int)
    :noindex:

    Reads data from the file descriptor specified in the second argument to the
    `buffer_t` instance in the first argument. The number of bytes to be read
    depends on the number of bytes expected to read by calling
    `buffer_prepare_for_read` prior to calling this function.

    If the read fails, returns `-1` and sets an error code on `errno`. Returns
    `0` if EOF, or the new size after the read is completed successfully
    otherwise.

  .. cpp:function:: void buffer_prepare_for_write(buffer_t)
    :noindex:

    Prepares the `buffer_t` instance specified in the first argument for
    subsequent writes.

  .. cpp:function:: ssize_t buffer_write(buffer_t, int)
    :noindex:

    Writes data from the `buffer_t` instance in the first argument to the file
    descriptor specified in the second argument. The number of bytes to be
    written is the number of remaining in the buffer for writes.

    If the write fails, returns `-1` and sets an error code on `errno`. Returns
    `0` if EOF, or the number of remaining bytes to be written after a
    successful write.


1.4 C-String Types
==================


Short-hand notations for C-String types.


.. cpp:type:: c_str
-------------------

  typedef of `char*`


.. cpp:type:: cc_str
---------------------

  typedef of `const char*`


1.5 C Functions
===============


Standard functions in C that might not be present in some compilers.


.. cpp:function:: char* itoa(int, char*, int)

  Integer to ASCII string conversion.

  Converts an integer value specified in the first argument to a null-terminated
  string specified in the second argument. The base of the integer is specified
  in the third argument. Returns the converted ASCII string.

  If base is 10 and value is negative, the resulting string is preceded with
  a minus sign (-). With any other bases, value is always considered unsigned.


.. cpp:function:: char* atoi(const char*)

  ASCII string to integer conversion.

  Converts an ASCII string specified as the first argument to an integer number.

  The function first discards as many whitespace characters as necessary until
  the first non-whitespace character is found. Then, starting from this
  character, takes an optional initial plus or minus sign followed by as many
  numerical digits as possible, and interprets them as a numerical value.


1.6 Dictionary
==============


A dictionary implementation with C-string key type, based on the `hashmap_t`
implementation.

.. cpp:type:: dict_t
--------------------

  .. cpp:function:: dict_t dict_create()
    :noindex:

    Creates an instance of dict_t using dynamically allocated memory.

  .. cpp:function:: size_t dict_size(dict_t)
    :noindex:

    Gets the number of key-value pairs in the `dict_t` instance specified.

  .. cpp:function:: void dict_free(dict_t *)  
    :noindex:

    Frees memory from the pointer of an instance of `dict_t` specified.

  .. cpp:function:: void* dict_put(dict_t, const char *, void*)
    :noindex:

    Insert a key-value pair into the `dict_t` instance specified as the first
    argument. The second argument is the key and the third argument is the
    pointer to be stored. Returns the pointer in the pair inserted.

  .. cpp:function:: void* dict_get(dict_t, const char*)
    :noindex:

    Retrieves the value associated with a key specified as the second argument
    from the `dict_t` instance specified as the first argument. Returns the
    pointer value associated with the key if the key exists, `NULL` otherwise.


1.7 Hashing
===========


Hashing algorithms.


.. cpp:type:: hash_t
--------------------

  Hash value type that's represented as native type `unsigned long long`.


.. cpp:function:: hash_t linear_horners_rule_str_hash(const char*)
  :noindex:

  Calculates the hash value of the specified string using a linear version of
  Horner's Rule.

.. cpp:function:: hash_t log_horners_rule_str_hash(const char*)
  :noindex:

  Calculates the hash value of the specified string using a logarithmic version
  of Horner's Rule.

.. cpp:function:: hash_t hash32shift(unsigned int)
  :noindex:

  Calculates the hash of a 32-bit unsigned integer.

.. cpp:function:: hash_t hash64shift(unsigned long)
  :noindex:

  Calculates the hash of a 64-bit unsigned integer.

.. cpp:function:: hash_t hash_str_jenkins_one_at_a_time(const char*)
  :noindex:

  Calculates the hash value of the specified string using the
  "Jenkins's one-at-a-time" algorithm.

.. cpp:function:: hash_t hash_robert_jenkin(unsigned int)
  :noindex:

  Calculates the hash value of the specified string using the
  "Robert Jenkin" algorithm.


1.8 Hash map
============


A hash map implementation that offers near constant-time lookups and inserts.


.. cpp:type:: HashFunc
----------------------

  Hash function used to hash keys in `hashmap_t`, defined as `hash_t(*HashFunc)(void*)`.


.. cpp:type:: KeyCmpFunc
------------------------

  Key comparison function used to compare keys in the hash map, defined as
  `int(*KeyCmpFunc)(void*, void*)`.


.. cpp:type:: hashmap_t
-----------------------

  .. cpp:function:: hashmap_t hashmap_create(size_t, HashFunc, KeyCmpFunc)
    :noindex:

    Creates an instance of `hashmap_t` using dynamically allocated memory, and
    by specifying the initial capacity as the first argument, as well as the
    hashing and key comparison functions as the second and third arguments.

  .. cpp:function:: size_t hashmap_size(hashmap_t)
    :noindex:

    Gets the number of elements in the specified `hashmap_t` instance.

  .. cpp:function:: void hashmap_lock(hashmap_t)
    :noindex:

    Locks the specified `hashmap_t` instance so that updates from other threads
    are blocks until `hashmap_unlock` is called on the instance.

  .. cpp:function:: void hashmap_unlock(hashmap_t)
    :noindex:

    Unlocks the specified `hashmap_t` instance following a previous call of
    `hashmap_lock` so that updates from other threads are unblocked.

  .. cpp:function:: void hashmap_free(hashmap_t*)
    :noindex:

    Frees memory from the pointer of an instance of `hashmap_t` specified.

  .. cpp:function:: void* hashmap_put(hashmap_t, void*, void*)
    :noindex:

    Inserts a key-value pair into the `hashmap_t` instance as the first argument.
    If the key already exists, its associated value will be updated. Returns the
    value inserted if successful, `NULL` otherwise.

  .. cpp:function:: void* hashmap_get(hashmap_t, void*)
    :noindex:

    Retrieves the value associated with the key specified as the second argument
    from the `hashmap_t` instance specified as the first argument. Returns the
    value if its associated key is in the instance, `NULL` otherwise.

  .. cpp:function:: int hashmap_contains_key(hashmap_t, void*)
    :noindex:

    Returns a value indicating if the key specified as the second argument exists
    in the `hashmap_t` instance specified as the first argument. Returns `1` if
    the key is in the instance, `0` otherwise.

  .. cpp:function:: void* hashmap_remove(hashmap_t, void*)
    :noindex:

    Removes the key-value pair from the `hashmap_t` instance specified as the
    first argument using the key as the second argument. Returns the value
    associated with the key if found, `NULL` otherwise.

  .. cpp:function:: void* hashmap_lookup(hashmap_t, void*, void*)
    :noindex:

    Looks up a particular key-value pair from the `hashmap_t` instance as the
    first argument. The lookup function is the second argument that has the
    signature `int(*lookup)(void *key, void *value, void* arg)`, which takes
    a key, its associated value and an optional argument which is specified as
    the third argument. The lookup function returns `1` for a key-value pair
    considered as found, at which the function stops the searching and returns
    the value found. If no key-value is found, returns `NULL`.

  .. cpp:function:: void hashmap_iterate(hashmap_t, void*, int)
    :noindex:

    Iterates through all key-value pairs in the `hashmap_t` instance as the
    first argument. The second argument specifies a function of signature
    `int(*callback)(void *key, void *value)` that receives every key-value pair
    iterated. The third argument specifies a value that indicates whether the
    iteration should continue when the callback function returns a non-truthy value.

  .. cpp:function:: size_t hashmap_bucketcount(hashmap_t)
    :noindex:

    Returns the number of buckets in the `hashmap_t` instance specified as the
    first argument.

  .. cpp:function:: size_t hashmap_capacity(hashmap_t)
    :noindex:

    Returns the capacity of the `hashmap_t` instance specified as the
    first argument.

  .. cpp:function:: int hashmap_equal(hashmap_t, hashmap_t)
    :noindex:

    Determines equality between the two instances of `hashmap_t` specified. The
    two instances would only be considered equal if they have exactly the same
    keys and all associated values of the same keys must be identical as well.
    Returns `1` if they are equal, `0` otherwise.


1.9 Math
========


General functions for mathematical computations.


.. cpp:function:: unsigned int nearest_exp2_ceil(unsigned int)

  Computes the least largest exponent of base 2 that is closest to the input value.


.. cpp:function:: unsigned int nearest_exp2_floor(unsigned int)

  Computes the most largest exponent of base 2 that is closest to the input value.



1.10 Queue
==========


A FIFO storage container.


.. cpp:type:: queue_t
---------------------

  .. cpp:function:: queue_t queue_create()
    :noindex:

    Creates an instance of `queue_t` using dynamically allocated memory.

  .. cpp:function:: size_t queue_size(queue_t)
    :noindex:

    Gets the number of elements in the `queue_t` instance specified.

  .. cpp:function:: void* queue_front(queue_t)
    :noindex:

    Gets the element at the front of the `queue_t` instance specified. If the
    queue is empty, the `NULL` is returned.

  .. cpp:function:: void* queue_back(queue_t)
    :noindex:

    Gets the element at the back of the `queue_t` instance specified. If the
    queue is empty, the `NULL` is returned.

  .. cpp:function:: int queue_push(queue_t, void*, size_t)
    :noindex:

    Pushes an element to the back of the `queue_t` instance specified as
    the first argument. The second argument is a pointer that points to the
    value to be pushed onto the queue, and the third argument is the size of
    the value to be pushed, in number of bytes. Returns `-1` if the push failed,
    `1` if successful.

  .. cpp:function:: void* queue_pop(queue_t)
    :noindex:

    Pops the element at the front of the `queue_t` instance specified and
    returns it. If the queue is empty, `NULL` is returned.

  .. cpp:function:: void queue_free(queue_t *)
    :noindex:

    Frees memory from the pointer of an instance of `queue_t` specified.


1.11 Stack
==========


A FILO storage container.


.. cpp:type:: stack_t
---------------------

  .. cpp:function:: stack_t stack_create()
    :noindex:

    Creates an instance of `stack_t` using dynamically allocated memory.

  .. cpp:function:: size_t stack_size(stack_t)
    :noindex:

    Returns the number of elements in the `stack_t` instance specified.

  .. cpp:function:: void* stack_top(stack_t)
    :noindex:

    Gets the element at the top of the `stack_t` instance provided. Returns
    `NULL` if the stack is empty.

  .. cpp:function:: int stack_push(stack_t, void*, size_t)
    :noindex:

    Pushes an element to the top of the `stack_t` instance specified as
    the first argument. The second argument is a pointer that points to the
    value to be pushed onto the stack, and the third argument is the size of
    the value to be pushed, in number of bytes. Returns `-1` if the push failed,
    `1` if successful.

  .. cpp:function:: void* stack_pop(stack_t)
    :noindex:

    Pops the element at the top of the `stack_t` instance specified and
    returns it. If the stack is empty, `NULL` is returned.

  .. cpp:function:: void stack_free(stack*)
    :noindex:

    Frees memory from the pointer of an instance of `stack_t` specified.


1.12 String Buffer
==================


A C-string buffer abstraction that can dynamically change capacity.


.. cpp:type:: strbuf_t
----------------------

  .. cpp:function:: strbuf_t strbuf_t_create()
    :noindex:

    Creates an instance of `strbuf_t` using dynamically allocated memory.

  .. cpp:function:: void strbuf_free(strbuf_t*)
    :noindex:

    Frees memory from the pointer of an instance of `strbuf_t` specified.

  .. cpp:function:: void strbuf_empty(strbuf_t)
    :noindex:

    Empties the `strbuf_t` instance specified as the argument and reduces
    its capacity to `0`.

  .. cpp:function:: size_t strbuf_len(strbuf_t)
    :noindex:

    Gets the length of the string currently held in the `strbuf_t` instance
    specified as the argument.

  .. cpp:function:: const char* strbuf_cstr(strbuf_t)
    :noindex:

    Gets the C-string held in the `strbuf_t` instance specified as the argument.

  .. cpp:function:: size_t strbuf_capacity(strbuf_t)
    :noindex:

    Gets the capacity of the `strbuf_t` instance specified as the argument.

  .. cpp:function:: int strbuf_append(strbuf_t, const char*)
    :noindex:

    Appends a C-string into the `strbuf_t` instance specified as the first
    argument. The second argument is the C-string to be appended. Returns `1`
    if the append is successful, `0` otherwise.


1.13 String Manipulation
========================


String manipulation functions.


.. cpp:function:: char* strtoupper(char *)

  Converts the C-string specified as the argument to its uppercase form. Returns
  the converted string.

.. cpp:function:: char* strtolower(char *)

  Converts the C-string specified as the argument to its lowercase form. Returns
  the converted string.

.. cpp:function:: char* strcpy_hard(char*, const char*)

  Copies the source string specified as the second argument to the destination
  string specified as the first argument. If the length of the source string
  is greater than the length of the destination string, then the destination
  string is freed and re-allocated to have the same length as the source string.
  Returns the destination string.

.. cpp:function:: char* strtrim(char *)

  Trims away empty spaces at the front and end of the string specified. Returns
  the trimmed string.

.. cpp:function:: size_t strlcpy2(char*, const char*, size_t)

  Copies the source string specified as the second argument to the destination
  string specified as the first argument. The number of characters copied
  is at most the number specified as the third argument minus `1`. Returns the
  number of number of characters copied.

.. cpp:function:: char* strncpy_safe(char*, const char*, size_t)

  Copies the source string specified as the second argument to the destination
  string specified as the first argument. The number of characters copied
  is the minimum between the length of the destination string and the size
  specified as the third argument. Returns the destination string.


1.14 General Utilities
======================


General utility functions.


.. cpp:function:: void set_nth_bit(int*, char)

  Set a specific bit of a given number to 1. The first argument specifies
  a pointer to the number to be set, and the second argument is the `n`th bit
  to set from the LSB.

.. cpp:function:: void clear_nth_bit(int*, char)

  Clears a specific bit of a given number to 0. The first argument specifies
  a pointer to the number to be cleared, and the second argument is the `n`th
  bit to clear from the LSB.

.. cpp:function:: int is_bit_set(int, char)

  Checks if a specific bit of a given number is set to 1. The first argument
  specifies a pointer to the number to be checked, and the second argument is
  the `n`th bit to check from the LSB. Returns `1` if the bit is set to `1`,
  `0` otherwise.

.. cpp:function:: void set_nth_bit_uint32(uint32_t*, char)

  32-bit version of `set_nth_bit`.

.. cpp:function:: void clear_nth_bit_uint32(uint32_t*, char)

  32-bit version of `clear_nth_bit`.

.. cpp:function:: int is_bit_set_uint32(uint32_t, char);

  32-bit version of `is_bit_set`.

.. cpp:function:: int rand_top(int)

  Returns an pseudo-random integer that's in the range between 1 and the number
  specified as the argument, inclusively.

.. cpp:function:: int rand_range(int, int)

  Returns an pseudo-random integer that's in the range between two integers
  specified as the two arguments, inclusively.

.. cpp:function:: double randf_top(double)

  Returns an pseudo-random double-precision floating number that's in the range
  between 1 and the number specified as the argument, inclusively.

.. cpp:function:: double randf_range(double, double)

  Returns an pseudo-random integer that's in the range between two
  double-precision floating numbers specified as the two arguments, inclusively.

.. cpp:function:: char* generate_text(size_t=0, size_t=0)

  Generates a random string of an optional length specified as the first
  argument, up to the optional maximum length specified as the second argument.

  If both the length and max length are non-zero, the generated text will have a
  length between the two numbers.

  If only the length is non-zero, the generated text will have exactly the
  specified length.

  If only the max length is non-zero, the generated text will have a length
  between `1` and the max length.

  If both the length and max length are zero, `NULL` is returned.

.. cpp:function:: char* generate_loremipsum()

  Generates a string of Lorem Ipsum of arbitrary length.


1.15 Universally Unique Identifier
==================================


A 128 bits implementation of UUID.


.. cpp:type:: uuid128_t
-----------------------

  .. cpp:function:: uuid128_t uuid_create()
    :noindex:

    Creates an instance of `uuid128_t` using dynamically allocated memory.

  .. cpp:function:: int uuid_compare(const uuid128_t, const uuid128_t)
    :noindex:

    Evaluates equality between two instances of `uuid128_t`. Returns `1` if
    the first argument is considered greater than the second, `0` if they are
    equivalent, and `-1` if the first is less than the second.

  .. cpp:function:: uint64_t uuid_to_hash(const uuid128_t)
    :noindex:

    Hashes the `uuid128_t` instance provided as the arugment to an 64-bit
    unsigned integer.

  .. cpp:function:: uint64_t uuid_create_and_hash()
    :noindex:

    Creates an instance of `uuid128_t` and returns its equivalent hash.