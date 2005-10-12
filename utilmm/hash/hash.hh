/* -*- C++ -*-
 * $Id: hash.hh 957 2005-03-07 16:01:20Z sjoyeux $
 */
#ifndef UTILMM_HASH_HEADER
# define UTILMM_HASH_HEADER
#include "utilmm/config/config.h"

# include <string>

#include "utilmm/hash/hash_fwd.hh"

namespace utilmm {

  /** @brief hash functor
   *
   * This class implements a functor to compute the hash value
   * of an object.
   *
   * @note Except for some basic types this class is not fully defined
   * user may define a specialization of some of its function to use
   * it properly. A short example is given in @c hash/hash.hh file
   * documentation.
   *
   * @param Ty the type of objects we want to hash
   *
   * @author Frédéric Py <fpy@laas.fr>
   * @ingroup hashing
   */
  template<typename Ty>
  struct hash
    :public std::unary_function<Ty, size_t> {

    /** @brief hashing function
     *
     * This function will compute the hash value of a given instance.
     *
     * @param arg The instance we want to hash
     * 
     * @return The hash value for @a arg
     */
    size_t operator()(Ty const &arg) const; 

  }; // struct utilmm::hash<>
  

  template<>
  struct hash<bool>
    :public std::unary_function<bool, size_t> {
    size_t operator()(bool x) const {
      return size_t(x?1:0);
    }
  }; // struct utilmm::hash<bool>

  template<>
  struct hash<char>
    :public std::unary_function<char, size_t> {
    size_t operator()(char x) const {
      return static_cast<size_t>(x);
    }
  }; // struct utilmm::hash<char>

  template<>
  struct hash<signed char>
    :public std::unary_function<signed char, size_t> {
    size_t operator()(signed char x) const {
      return static_cast<size_t>(x);
    }
  }; // struct utilmm::hash<signed char>

  template<>
  struct hash<unsigned char>
    :public std::unary_function<unsigned char, size_t> {
    size_t operator()(unsigned char x) const {
      return static_cast<size_t>(x);
    }
  }; // struct utilmm::hash<unsigned char>

  template<>
  struct hash<signed int>
    :public std::unary_function<signed int, size_t> {
    size_t operator()(signed int x) const {
      return static_cast<size_t>(x);
    }
  }; // struct utilmm::hash<signed int>

  template<>
  struct hash<unsigned int>
    :public std::unary_function<unsigned int, size_t> {
    size_t operator()(unsigned int x) const {
      return static_cast<size_t>(x);
    }
  }; // struct utilmm::hash<unsigned int>

  template<>
  struct hash<signed short int>
    :public std::unary_function<signed short int, size_t> {
    size_t operator()(signed short int x) const {
      return static_cast<size_t>(x);
    }
  }; // struct utilmm::hash<signed short int>

  template<>
  struct hash<unsigned short int>
    :public std::unary_function<unsigned short int, size_t> {
    size_t operator()(unsigned short int x) const {
      return static_cast<size_t>(x);
    }
  }; // struct utilmm::hash<unsigned short int>

  template<>
  struct hash<signed long int>
    :public std::unary_function<signed long int, size_t> {
    size_t operator()(signed long int x) const {
      return static_cast<size_t>(x);
    }
  }; // struct utilmm::hash<signed long int>

  template<>
  struct hash<unsigned long int>
    :public std::unary_function<unsigned long int, size_t> {
    size_t operator()(unsigned long int x) const {
      return static_cast<size_t>(x);
    }
  }; // struct utilmm::hash<unsigned long int>

  template<>
  struct hash<float>
    :public std::unary_function<float, size_t> {
    size_t operator()(float x) const;
  }; // struct utilmm::hash<float>

  template<>
  struct hash<double>
    :public std::unary_function<double, size_t> {
    size_t operator()(double x) const;
  }; // struct utilmm::hash<double>

  template<>
  struct hash<long double>
    :public std::unary_function<long double, size_t> {
    size_t operator()(long double x) const;
  }; // struct utilmm::hash<long double>

  template<typename Ty>
  struct hash<Ty *>
    :public std::unary_function<Ty *, size_t> {
    size_t operator()(Ty *x) const {
      return reinterpret_cast<size_t>(x);
    }
  }; // struct utilmm::hash<Ty *>

  template<class CharT, class Traits, class Alloc>
  struct hash< std::basic_string<CharT, Traits, Alloc> >
    :public std::unary_function<std::basic_string<CharT, Traits, Alloc>, 
				size_t> {
    size_t operator()(std::basic_string<CharT, Traits, Alloc> const &x) const;
  }; // struct utilmm::hash< std::basic_string<> >

} // namespace utilmm

# define IN_UTILMM_HASH_HEADER
#include "utilmm/hash/bits/hash.tcc"
# undef IN_UTILMM_HASH_HEADER
#endif // UTILMM_HASH_HEADER
/** @file hash/hash.hh
 * @brief definition of the basic hashing functor
 *
 * This header defines the @c utilmm::hash functor and some specialization
 * of this one to some standard types.
 *
 * To define your specialization to a class @c foo user can do like in the
 * following example :
 * @li file foo.hh
 * @code
 * 
 * namespace foo_ns {
 *   class foo {
 *     // [...]
 *   }; // class foo_ns::foo
 * }
 *
 * @endcode
 *
 * @li file foo.cc
 * @code
 * #include "utilmm/foo.hh"
 * #include "utilmm/hash/hash.hh"
 *
 * size_t utilmm::hash<foo_ns::foo>::operator()(Ty const &x) const {
 *   // Insert hashing code  here
 * }
 * @endcode
 *
 * @author Frédéric Py <fpy@laas.fr>
 * @ingroup hashing
 */

/** @defgroup hashing Hash function based containers
 * @brief Implementation of containers based on a hash function 
 *
 * This modules embeds all the classes used to implement
 * containers using a hash function to access to contained data
 */
