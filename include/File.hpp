// -*- mode: c++ -*-
////
// @copyright  2021 Glen S. Dayton. All rights reserved. Project Euler has confidentiality rules so do not copy nor publish this code.
//  @author Glen S.Dayton
//
//  Wrapper arpund Posix file operations that protect against inadvertant copying of the file descriptor, and guaranteee
//  release of resources.
//
// All of the methods may throw a oscpp::SysException (a type of std::runtime_error) on errors.

#ifndef __FILE_H_INCL__
#define __FILE_H_INCL__
#include <fcntl.h>
#include <sys/stat.h>
#include "sysexception.hpp"

namespace oscpp {

/**
 * File -- open, close, memmap
 */
class File {
 public:
  File(const char *filename, int flags = O_RDONLY | O_CLOEXEC, int mode = 0)
  : fd {File::open(filename, flags, mode)},
        mappedFile {nullptr},
        mappedLen {0}
        {        }

  explicit File(int fildes) : fd {fildes} {}
  
  File(const File&) = delete;
  File& operator=(const File&) = delete;
  ~File() { File::close(); }

  std::pair<void*, size_t> map();
  struct stat& fstat(struct stat& statbuf);

 private:
  int fd;
  void *mappedFile;
  size_t mappedLen;

  static int open(const char *filename, int flag = O_RDONLY | O_CLOEXEC, int mode = 0);
  void close();
};

}
#endif // __FILE_H_INCL__
