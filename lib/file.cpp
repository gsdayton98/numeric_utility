// -*- mode: c++ -*-
// @copyright  2025 Glen S. Dayton. All rights reserved. Project Euler has confidentiality rules so do not copy nor publish this code.
//  @author Glen S.Dayton

#include "file.hpp"
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>


std::pair<void*, size_t> oscpp::File::map() {
  struct stat stats;
  fstat(stats);

  mappedLen = stats.st_size;
  mappedFile = mmap(nullptr, mappedLen, PROT_READ, MAP_FILE | MAP_PRIVATE, fd, 0L);
  if (mappedFile == MAP_FAILED) throw SysException{};

  return std::make_pair(mappedFile, mappedLen);
}


struct stat& oscpp::File::fstat(struct stat& stats) const {
  if (::fstat(fd, &stats) < 0) throw SysException{};
  return stats;
}


int oscpp::File::open(const char* filename, const int flag, const int mode) {
  const int fildes = ::open(filename, flag, mode);
  if (fildes < 0) throw SysException{};
  return fildes;
}


void oscpp::File::close() {
  if (mappedFile) {
    (void) munmap( mappedFile,  mappedLen);
    mappedFile = nullptr;
    mappedLen = 0;
  }
  (void)::close(fd);
}

