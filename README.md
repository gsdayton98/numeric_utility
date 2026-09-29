# Utility Library

I originally developed these routines for Project Euler solutions, and they proved to be generally useful
outside of Project Euler, or least usable across many Project Euler problems.

Currently, the library is configured as a shared library to be installed locally on the
users home directory. 

  * On MacOS, remember to set the environment variable `DYLD_LIBRARY_PATH` to include the
directory where the library is installed.

  * On Linux, the environment variable `LD_LIBRARY_PATH` should be set.

At this time I do not support installation or use on Windows.