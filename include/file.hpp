// -*- mode: c++ -*-
////
// @copyright 2021, Glen S. Dayton. All rights reserved. Project Euler has confidentiality rules, so do not copy nor publish this code.
// @author Glen S. Dayton
//
//  Wrapper around Posix File operations that protect against inadvertent copying of the File descriptor and guarantee
//  release of resources.
//
// All the methods may throw an oscpp::SysException (a type of std::runtime_error) on errors.

#ifndef FILE_H_
#define FILE_H_
#include <fcntl.h>
#include <sys/stat.h>
#include "sysexception.hpp"

namespace oscpp {

/**
 * File -- open, close, memory map
 */
class File {
 public:
  explicit File(const char *filename, const int flags = O_RDONLY | O_CLOEXEC, const int mode = 0)
  : fd {open(filename, flags, mode)},
        mappedFile {nullptr},
        mappedLen {0}
        { }

  explicit File(int fileDescriptor) : fd {fileDescriptor}, mappedFile {nullptr}, mappedLen {0} {}
  
  File(const File&) = delete;
  File& operator=(const File&) = delete;
  ~File() { File::close(); }

  std::pair<void*, size_t> map();
  struct stat& fstat(struct stat& stats) const;

 private:
  int fd;
  void *mappedFile;
  size_t mappedLen;

  static int open(const char *filename, int flag = O_RDONLY | O_CLOEXEC, int mode = 0);
  void close();
};

}
#endif //FILE_H_
