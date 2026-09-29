# Utility Library

I originally developed these routines for Project Euler solutions, and they proved to be generally useful
outside of Project Euler, or least usable across many Project Euler problems.

Currently, the library is configured as a shared library to be installed locally on the
users home directory. 

  * On macOS, the installed library records its absolute path, so programs that link it run without
any environment variables.

  * On Linux, a program that links the installed library needs an rpath to it, or `LD_LIBRARY_PATH` set to
the directory where the library is installed. With CMake, set `CMAKE_INSTALL_RPATH_USE_LINK_PATH` to `ON`
before defining the program's targets, or set `CMAKE_INSTALL_RPATH` to the library directory.

At this time I do not support installation or use on Windows.