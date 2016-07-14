*******
Logging
*******

Extensible general-purpose logging facilities.

--------------------------------------------------------------------------------

Exception-safety Tags
=====================

Tags that signify exception-safeties of logging facilities.

Header file: `sneaker/logging/exception_safety_tag.h`

.. cpp:type:: sneaker::logging::exception_safe_tag

  Tag that signifies exception-safe logging facilities.

.. cpp:type:: sneaker::logging::exception_unsafe_tag

  Tag that signifies non exception-safe logging facilities.


Thread-safety Tags
==================

Tags that signify thread-safeties of logging facilities.

Header file: `sneaker/logging/thread_safety_tag.h`

.. cpp:type:: sneaker::logging::thread_safe_tag

  Tag that signifies thread-safe logging facilities.

.. cpp:type:: sneaker::logging::thread_unsafe_tag

  Tag that signifies non thread-safe logging facilities.


Log Levels
==========

Various levels of log severity.

Header file: `sneaker/logging/log_level.h`

.. cpp:enum-class:: sneaker::logging::LogLevel

  Enumerations of various levels of log severity. Values are:
  
  * `LOG_LEVEL_DEBUG`
  * `LOG_LEVEL_INFO`
  * `LOG_LEVEL_WARN`
  * `LOG_LEVEL_ERROR`
  * `LOG_LEVEL_FATAL`


Log Schemes
===========

Mechanisms of logging.

Header file: `sneaker/logging/log_scheme.h`

.. cpp:class:: sneaker::logging::log_scheme
-------------------------------------------

  Abstraction of a particular mechanism of storing log records.

  .. cpp:function:: virtual void write(const char* msg) = 0
    :noindex:

    Base member of writing a log record. To be defined in subclasses.

  .. cpp:function:: virtual ~log_scheme()
    :noindex:

    Base destructor.

.. cpp:class:: sneaker::logging::stream_log_scheme
--------------------------------------------------

  Logging scheme that sends log records to `std::ostream`.

  .. cpp:function:: virtual void write(const char* msg)
    :noindex:

    Writes the log message to `std::ostream`.

.. cpp:class:: sneaker::logging::stdout_log_scheme : public stream_log_scheme
-----------------------------------------------------------------------------

  Logging scheme that sends log records to stdout.

  .. cpp:function:: stdout_log_scheme()
    :noindex:

    Constructor.

  .. cpp:function:: virtual ~stdout_log_scheme()
    :noindex:

    Destructor.

.. cpp:class:: sneaker::logging::stderr_log_scheme : public stream_log_scheme
-----------------------------------------------------------------------------

  Logging scheme that sends log records to stderr.

  .. cpp:function:: stderr_log_scheme()
    :noindex:

    Constructor.

  .. cpp:function:: virtual ~stderr_log_scheme()
    :noindex:

    Destructor.

.. cpp:class:: sneaker::logging::file_log_scheme : public stream_log_scheme
---------------------------------------------------------------------------

  Logging scheme that sends log records to a file.

  .. cpp:function:: file_log_scheme(const char* filename);
    :noindex:

    Constructor that takes a file path.

  .. cpp:function:: virtual ~file_log_scheme()
    :noindex:

    Destructor.


Logger
======

Class that encapsulates the core logging mechanism. Can be customized
with particular exception-safety and thread-safety tags.

Header file: `sneaker/logging/logger.h`

.. cpp:class:: template<typename thread_safety_tag, typename exception_safety_tag> \
               sneaker::logging::logger
---------------------------------------

  .. cpp:function:: explicit logger(log_scheme* log_scheme)
    :noindex:

    Constructor.

  .. cpp:function:: template<size_t LINE_SIZE=1024> \
                    void write(LogLevel log_lvl, const char* format, ...)
    :noindex:

    Writes a free-format log record message.


Helper Utilities
================

Utility functions that faciliate logging.

Header file: `sneaker/logging/logging.h`

.. c:macro:: LOG(lvl, file, line, format, ...)
  
  Convenience macro that logs a log record message by taking the log level,
  file and line number of where the log occurs, and the format and arguments
  that goes into the log message.

.. c:macro:: LOG_DEBUG(format, ...)

  Convenience macro that logs a log record in `LOG_LEVEL_DEBUG` level.

.. c:macro:: LOG_INFO(format, ...)

  Convenience macro that logs a log record in `LOG_LEVEL_INFO` level.

.. c:macro:: LOG_WARN(format, ...)

  Convenience macro that logs a log record in `LOG_LEVEL_WARN` level.

.. c:macro:: LOG_ERROR(format, ...)

  Convenience macro that logs a log record in `LOG_LEVEL_ERROR` level.

.. c:macro:: LOG_FATAL(format, ...)

  Convenience macro that logs a log record in `LOG_LEVEL_FATAL` level.

.. c:macro:: LOG_DETAILED(lvl, file, line, format, ...) 

  Convenience macro that logs a log record message by taking the log level,
  file and line number of where the log occurs, and the format and arguments
  that goes into the log message. Also logs the timestamp of the occurance
  of log.

.. c:macro:: LOG_DEBUG_DETAILED(format, ...)

  Convenience macro that logs a log record in `LOG_LEVEL_DEBUG` level.

.. c:macro:: LOG_INFO_DETAILED(format, ...)

  Convenience macro that logs a log record in `LOG_LEVEL_INFO` level.

.. c:macro:: LOG_WARN_DETAILED(format, ...)

  Convenience macro that logs a log record in `LOG_LEVEL_WARN` level.

.. c:macro:: LOG_ERROR_DETAILED(format, ...)

  Convenience macro that logs a log record in `LOG_LEVEL_ERROR` level.

.. c:macro:: LOG_FATAL_DETAILED(format, ...)

  Convenience macro that logs a log record in `LOG_LEVEL_FATAL` level.