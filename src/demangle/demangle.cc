/* -*- C++ -*-
 * $Id$
 */
#include "utilmm/demangle/demangle.hh"

#if 1 // This will be replaced by a test checking <cxxabi.h>
# include <cxxabi.h> 
#include <cstdlib>

std::string utilmm::demangle(char const *name) {
  int status;
  char *real_name;

  real_name = abi::__cxa_demangle(name, 0, 0, &status);
  /*
   * I have to know exactly the meaning of status (The only thing I know is
   * that 0 => OK)
   *
   * Currently we will consider that the name is OK
   */
  std::string result(real_name);
  free(real_name); // CXX abi is in C
                   // so we use C functions for memory
  return result;
}

#else 

# warning "Name demangling is not supported here"

std::string utilmm::demangle(char const *name) {
  std::string result(name);
  return result;
}

#endif // if 1 
