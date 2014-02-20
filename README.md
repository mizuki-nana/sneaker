sneaker
=======

![logo](https://raw.github.com/yanzhengli/sneaker/dev/logo_128x128.png)

A C/C++ library that contains a vast set of re-usable functions and objects that are useful for many application development scenarios.


## Content
* `libc` - _utilities for C_
* \\- `array_t` - _generic array structure_
* \\- `bitmap_t` - _2-dimensional bitmap_
* \\- `buffer_t` - _generic data buffer_
* \\- `dict_t` - _key-value hash table keyed by strings_
* \\- `hashmap_t` - _generic hash table structure_
* \\- `queue_t` - _FIFO structure_
* \\- `stack_t` - _FILO structure_
* \\- `strbuf_t` - _dynamically managed string buffer_
* \\- `uuid_t` - _universally unique identifier abstraction_
* `cache` - _cache management_
* \\- `generic_cache<K, T, CreateHandler, DestroyHandler>` - _generic in-memory cache_
* `container` - _high performance containers_
* \\- `reservation_map<T>` - _reservation-based associative container_
* \\- `assorted_value_map<K, ValueTypes...>` - _associative container with multi-typed values_
* `functional` - _versatile function wrappers that support asynchronous invocations_
* \\- `function<R, Args...>` - _statically-typed generic function wrapper for all functions_
* \\- `call` - _function wrapper for functions with no arguments and return value_
* \\- `action<Args...>` - _function wrapper for functions with arguments but no return value_
* \\- `predicate<Args...>` - _function wrapper for functions with arguments and a boolean return value_
* `threading` - _thread management and daemons_
* \\- `daemon_service` - _base class for a generic thread-based daemon service_
* \\- `fixed_time_interval_daemon_service` - _daemon service that triggers on a fixed time interval_


## Install
`make install`


## Version
version 0.1.0