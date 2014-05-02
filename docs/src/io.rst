******
7. I/O
******

Components that faciliates I/O operations.

.. cpp:class:: sneaker::io::file_reader
---------------------------------------

  Class that facilitates reading from files.

  .. cpp:class:: file_reader()
    :noindex:

    Constructor.

  .. cpp:class:: file_reader(const char*)
    :noindex:

    Constructor. Takes a path of a file to be read.

  .. cpp:class:: const char* file_path() const
    :noindex:

    Gets the path of the file to be read.

  .. cpp:class:: void set_path(const char*)
    :noindex:

    Sets the path of the file to be read.

  .. cpp:class:: bool read_file(const char*)
    :noindex:

    Reads the file from the file specified, and copies the pointer points to the
    content read to the pointer to a C-string specified in the argument.
    Returns `true` if the read is successful, `false` otherwise.