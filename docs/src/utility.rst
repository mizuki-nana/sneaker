*********
Utilities
*********

Utility components that are useful in day-to-day developments.

--------------------------------------------------------------------------------

Command-line Program Interface
==============================

A base class that encapsulates logic for instantiating and running command-line
programs and parsing command-line arguments.

Header file: `sneaker/utility/cmdline_program.h`


.. cpp:class:: sneaker::utility::cmdline_program
------------------------------------------------

  .. cpp:function:: int run(int argc, char** argv)
    :noindex:

    Member function that takes the command-line argument parameters from
    `int main()` and pass them through to the program to run.

  .. cpp:function:: void add_string_parameter(const char* name, const char* description, std::string* res)
    :noindex:

    Adds a string-value named parameter to the program, along with a
    description.

  .. cpp:function:: void add_uint64_parameter(const char* name, const char* description, std::string* res)
    :noindex:

    Adds a 64-bit integer-value named parameter to the program, along with a
    description.

  .. cpp:function:: void add_uint32_parameter(const char* name, const char* description, std::string* res)
    :noindex:

    Adds a 32-bit integer-value named parameter to the program, along with a
    description.

  .. cpp:function:: void add_float_parameter(const char* name, const char* description, std::string* res)
    :noindex:

    Adds a floating point named parameter to the program, along with a
    description.

  .. cpp:function:: void add_boolean_parameter(const char* name, const char* description, std::string* res)
    :noindex:

    Adds a boolean-value named parameter to the program, along with a
    description.

  .. cpp:function:: void add_positional_parameter(const char* name, int n)
    :noindex:

    Adds a named positional parameter to the program, at index `n`.

  .. cpp:function:: template<typename T>
    void add_array_parameter(const char* name, const char* desc, std::vector<T>* res)
    :noindex:

    Adds a named list of parameters to the program, along with a description.

  .. cpp:function:: bool option_provided(const char* name) const
    :noindex:

    Determines if a named parameter has been specified.

  .. cpp:function:: protected explicit cmdline_program(const char*)
    :noindex:

    Protected constructor that takes the name of the program.

  .. cpp:function:: private virtual int do_run()
    :noindex:

    Member function that invokes the program to run. Returns a status value
    once the program finishes. A value of `0` is returned upon successful run,
    non-zero values otherwise.

    Default implementation does nothing, and is intended to be overriden in
    subclasses.

  .. cpp:function:: private virtual bool check_parameters() const
    :noindex:

    Member function that checks the parameters passed to the program, and
    returns a boolean indicating if the parameters are valid.

    Default implementation does nothing, and is intended to be overriden in
    subclasses.

--------------------------------------------------------------------------------

OS Utilities
============

Utilities that provide OS-level information and services.

Header file: `sneaker/utility/os.h`


.. cpp:function:: void sneaker::utility::get_process_mem_usage(uint64_t* vm_peak, uint64_t* vm_size, uint64_t* vm_hwm, uint64_t* vm_rss)

    Gets the memory usages of the current process, in number of kB.

    * `vm_peak`: Peak virtual memory size.
    * `vm_size`: Virtual memory size.
    * `vm_hwm`: Peak resident set size ("High Water Mark").
    * `vm_rss`: Resident set size.


.. cpp:function:: uint64_t sneaker::utility::get_process_vm_peak()

    Gets the peak virtual memory size of the current process, in kB.


.. cpp:function:: uint64_t sneaker::utility::get_process_vm_size()

    Gets the current virtual memory size of the current process, in kB.


.. cpp:function:: uint64_t sneaker::utility::get_process_vm_hwm()

    Gets the peak resident set size of the current process, in kB.


.. cpp:function:: uint64_t sneaker::utility::get_process_vm_rss()

    Gets the current resident set size of the current process, in kB.


--------------------------------------------------------------------------------

Stacktrace Utilities
====================

Utilities that provide runtime stack trace information.

Header file: `sneaker/utility/stack_trace.h`


.. cpp:class:: sneaker::utility::stack_trace

  A class that encompasses the utility functions.


  .. cpp:function:: static void print_stack_trace(std::ostream& ost, unsigned int max_frames)
    :noindex:

    Retrieves stack trace information and forwards to an output stream, with a
    maximum value on the number of frames to inspect.


--------------------------------------------------------------------------------

Numerics Utilities
==================

Utilities that deal with numerics.

Header file: `sneaker/utility/util.numeric.h`

.. cpp:function:: template<typename T> bool sneaker::utility::floats_equal(T lhs, T rhs, T tolerance=EPSILON(4))

  Safe floating-point equality comparisons.

--------------------------------------------------------------------------------


Uniform Table
==================

A utility class for managing formatting and printing data into a uniform table.
This class is templatized by the sizes of the columns that form the uniform table.

Header file: `sneaker/utility/uniform_table.h`

Example:

.. code-block:: cpp

  #include <sneaker/utility/uniform_table.h>
  #include <iostream>

  sneaker::utility::uniform_table<2, 10, 10> uniform_table;

  uniform_table.write_separator();
  uniform_table.write(2, "Ocean", "Blue");
  uniform_table.write(3, "Forest", "Green");
  uniform_table.write(1, "Volcano", "Red");
  std::cout << uniform_table.str();


  ```
  will give the following output:

  | -- | ---------- | ---------- |
  |  2 |      Ocean |       Blue |
  |  3 |     Forest |      Green |
  |  1 |    Volcano |        Red |
  ```

.. cpp:class:: template<size_t... ColumnSizes> sneaker::utility::uniform_table
------------------------------------------------------------------------------

  .. cpp:function:: std::ostream stream() const

    Returns the stream that content has been written to.

  .. cpp:function:: std::string str() const

    Returns a copy of the string written so far.

  .. cpp:function:: template<typename... Args> void write(Args... args)

    Writes a row in the table, with the supplied variadic arguments.

  .. cpp:function:: void write_separator()

    Writes a horizontal row separator.

--------------------------------------------------------------------------------


I/O Utilities
=============

A set of utility functions for I/O operations.

.. cpp:function:: void sneaker::utility hex_to_bytes(const unsigned char* src, size_t src_len, unsigned char* dst)
  :noindex:

  Converts a string of hexidecimal digits to its byte representation.

.. cpp:function:: void sneaker::utility::hex_to_bytes(const std::string& src, std::string* dst)
  :noindex:

  Converts a string of hexidecimal digits to its byte representation.

.. cpp:function:: void sneaker::utility::bytes_to_hex(const unsigned char* src, size_t src_len, unsigned char* dst)
  :noindex:

  Converts a byte array to its hexidecimal representation in string.

.. cpp:function:: void sneaker::utility::bytes_to_hex(const std::string& src, std::string* dst)
  :noindex:

  Converts a byte array to its hexidecimal representation in string.
