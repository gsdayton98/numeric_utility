// -*- mode: c++ -*-
// @copyright  2025 Glen S. Dayton. All rights reserved. Project Euler has confidentiality rules so do not copy nor publish this code.
//  @author Glen S.Dayton

#include "file.hpp"
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>


std::pair<void*, size_t> oscpp::File::map() {
  struct stat stats;

  File::fstat(stats);

  mappedLen = stats.st_size;
  void* mappedFile = ::mmap(0, mappedLen, PROT_READ, MAP_FILE | MAP_PRIVATE, fd, 0L);
  if (mappedFile == MAP_FAILED) throw SysException{};

  return std::make_pair(mappedFile, mappedLen);
}


struct stat& oscpp::File::fstat(struct stat& statbuf) {
  if (::fstat(fd, &statbuf) < 0) throw SysException{};
  return statbuf;
}


int oscpp::File::open(const char* filename, int flags, int mode) {
  int fildes = ::open(filename, flags, mode);
  if (fildes < 0) throw SysException{};
  return fildes;
}


void oscpp::File::close() {
  if (mappedFile) {
    (void) ::munmap( mappedFile,  mappedLen);
    mappedFile = nullptr;
    mappedLen = 0;
  }
  (void)::close(fd);
}

