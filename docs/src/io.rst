***
I/O
***

Components that faciliates I/O operations.


File Reader
===========

  Class that facilitates reads from files.

.. cpp:class:: sneaker::io::file_reader
---------------------------------------

  Header file: `sneaker/io/file_reader.h`

  .. cpp:function:: file_reader()
    :noindex:

    Default constructor.

  .. cpp:function:: explicit file_reader(const char*)
    :noindex:

    Constructor. Takes a path of a file to be read.

  .. cpp:function:: explicit file_reader(const std::string& path)
    :noindex:

    Constructor. Takes a path of a file to be read.

  .. cpp:function:: const char* file_path() const
    :noindex:

    Gets the path of the file to be read.

  .. cpp:function:: void set_path(const char*)
    :noindex:

    Sets the path of the file to be read.

  .. cpp:function:: bool read_file(const char** p, size_t* size_read=NULL)
    :noindex:

    Reads the file and copies the content read to the pointer specified as the first argument.
    Also can specify an optional output argument that gets the number of bytes read.
    Returns `true` if the read is successful, `false` otherwise.

  .. cpp:function:: bool read(std::vector<char>* buf, size_t* size_read=NULL) const
    :noindex:

    Reads the file and copies content read to the byte array specified as the first argument.
    Also can specify an optional output argument that gets the number of bytes read.
    Returns `true` if the read is successful, `false` otherwise.
