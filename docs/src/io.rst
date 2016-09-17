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


Data Stream Abstractions
========================

Abstractions and interfaces for manipulating and interacting with data streams.

Input Stream
############

Header file: `sneaker/io/input_stream.h`

.. cpp:class:: sneaker::io::input_stream
----------------------------------------

  Abstract class of an input stream.

  .. cpp:function:: input_stream()
    :noindex:

    Default constructor.

  .. cpp:function:: virtual ~input_stream()
    :noindex:

    Destructor.

  .. cpp:function:: virtual bool next(const uint8_t** data, size_t* len) = 0
    :noindex:

    Reads data from the stream.

    Returns true if some data is successfully read, false if no more data is
    available or an error has occurred.

  .. cpp:function:: virtual void skip(size_t len) = 0
    :noindex:

    Skips a specified number of bytes.

  .. cpp:function:: virtual size_t bytes_read() const = 0
    :noindex:

    Returns the number of bytes read from this stream so far.

.. cpp:class:: sneaker::io::stream_reader
-----------------------------------------

  Convinience class that facilitates reading from an instance of `input_stream`.

  .. cpp:function:: explicit stream_reader(input_stream*)
    :noindex:

    Constructor. Takes an instance of `input_stream`.

  .. cpp:function:: bool read(uint8_t*)
    :noindex:

    Read one byte from the underlying stream. Returns `true` if the read is
    successful, `false` otherwise.

  .. cpp:function:: bool read_bytes(uint8_t* blob, size_t n)
    :noindex:

    Reads the given number of bytes from the underlying stream.
    Returns `true` if there are enough bytes to read, `false` otherwise.

  .. cpp:function:: void skip_bytes(size_t n)
    :noindex:

    Skips the given number of bytes.

  .. cpp:function:: bool has_more()
    :noindex:

    Returns `true` if and only if the end of stream is not reached.


Helper functions:

.. cpp:function:: std::unique_ptr<input_stream> sneaker::io::file_input_stream(\
                  const char* filename, size_t buffer_size)

  Returns a new instance of `input_stream` whose contents come from the
  specified file. Data is read in chunks of given buffer size.

.. cpp:function:: std::unique_ptr<input_stream> sneaker::io::istream_input_stream(\
                  std::istream& stream, size_t buffer_size)

  Returns a new instance of `input_stream` whose contents come from the given
  `std::istream`. The `std::istream` object should outlive the returned result.

.. cpp:function:: std::unique_ptr<input_stream> sneaker::io::memory_input_stream(\
                  const uint8_t* data, size_t len)

  Returns a new instance of `input_stream` whose data comes from the specified
  byte array.

Output Stream
#############

Header file: `sneaker/io/output_stream.h`

.. cpp:class:: sneaker::io::output_stream
-----------------------------------------

  Abstract class of an output stream.

  .. cpp:function:: output_stream()
    :noindex:

    Default constructor.

  .. cpp:function:: ~output_stream()
    :noindex:

    Destructor.

  .. cpp:function:: virtual bool next(uint8_t** data, size_t* len) = 0
    :noindex:

    Returns a buffer that can be written into.
    On successful return, `data` has the pointer to the buffer
    and `len` has the number of bytes available at data.

  .. cpp:function:: virtual size_t bytes_written() const = 0
    :noindex:

    Returns the number of bytes written so far into this stream.
    The whole buffer returned by `next()` is assumed to be written.

  .. cpp:function:: virtual void backup(size_t len) = 0
    :noindex:

    "Returns" back to the stream some of the buffer obtained from in the last
    call to `next()`.

  .. cpp:function:: virtual void flush() = 0
    :noindex:

    Flushes any data remaining in the buffer to the stream's underlying
    store, if any.

.. cpp:class:: sneaker::io::stream_writer
-----------------------------------------

  Convinience class that facilitates writing to an instance of `output_stream`.

  .. cpp:function:: explicit stream_writer(output_stream*)
    :noindex:

    Constructor. Takes an instance of `output_stream`.

  .. cpp:function:: bool write(uint8_t c)
    :noindex:

    Writes a single byte to the stream.

  .. cpp:function:: bool write_bytes(const uint8_t* blob, size_t n)
    :noindex:

    Writes the specified number of bytes to the stream.

  .. cpp:function:: void flush()
    :noindex:

    Backs up upto the currently written data and flushes the underlying stream.
    Users should call this member method before finalizing the writing
    operation.


Helper functions:

.. cpp:function:: std::unique_ptr<output_stream> sneaker::io::file_output_stream(\
                  const char* filename, size_t buffer_size)

  Returns a new instance of `output_stream` whose contents are to be written
  to a file, in chunks of given buffer size.

  If there is a file with the given name, it is truncated and overwritten.
  If there is no file with the given name, it is created.

.. cpp:function:: std::unique_ptr<output_stream> sneaker::io::ostream_output_stream(\
                  std::ostream& stream, size_t buffer_size)

  Returns a new instance `output_stream` whose contents are to be written to
  the specified `std::ostream`.

Temporary Files Management
==========================

Interfaces that facilitate handling and management of temporary files.

Header file: `sneaker/io/tmp_file.h`

.. cpp:function:: const char* sneaker::io::get_tmp_file_path()

  Generates a unique file path for storing a temporary file. The generated path
  is not used by any existing files and is a platform-specific location.
  Note that files stored at this file path are not persisted between program
  invocations and system reboots. If such persistency are desired, please use
  `sneaker::io::get_persistent_tmp_file_path()`.

.. cpp:function:: const char* sneaker::io::get_persistent_tmp_file_path()

  Generates a unique file path for storing a temporary file. The generated path
  is not used by any existing files and is a platform-specific location.
  Note that files stored at this file path are persistent across program
  invocations and system reboots.
