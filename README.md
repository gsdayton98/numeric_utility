# Utility Library

I originally developed these routines to for Project Euler solutions.

As time went on, I added operating system utilities, and functions for benchmarking -- 
functions not strictly related to Project Euler.

Currently, the library is configured as a shared library to be installed locally on the
users home directory. 

  * On MacOS, remember to set the environment variable `DYLD_LIBRARY_PATH` to include the
directory where the library is installed.

  * On Linux, the environment variable `LD_LIBRARY_PATH` should be set.

The included benchmark application, benchmark_isprime, does require numeric_utility to be installed.
The benchmark also depends upon the oscpp utility library.

At this time I do not support installation or use on Windows.