/* -*- C++ -*-
 * $Id: hash.tcc 937 2005-01-09 14:35:39Z fpy $
 */
#ifndef IN_UTILMM_HASH_HEADER
# error "Cannot include template files directly."
#else

# include <cmath>

namespace utilmm {

  /*
   * struct utilmm::hash<float>
   */
  inline size_t hash<float>::operator()(float val) const {
    float to_hash = std::fabs(val),
      int_part = std::floor(to_hash), float_part = to_hash-int_part;
    
    return static_cast<size_t>(int_part)+static_cast<size_t>(1.0/float_part);
  }

  /*
   * struct utilmm::hash<double>
   */
  inline size_t hash<double>::operator()(double val) const {
    double to_hash = std::fabs(val),
      int_part = std::floor(to_hash), float_part = to_hash-int_part;
    
    return static_cast<size_t>(int_part)+static_cast<size_t>(1.0/float_part);
  }

  /*
   * struct utilmm::hash<long double>
   */
  inline size_t hash<long double>::operator()(long double val) const {
    long double to_hash = std::fabs(val),
      int_part = std::floor(to_hash), float_part = to_hash-int_part;
    
    return static_cast<size_t>(int_part)+static_cast<size_t>(1.0/float_part);
  }

  /*
   * struct utilmm::hash< std::basic_string<> >
   */
  template<class CharT, class Traits, class Alloc>
  size_t hash< std::basic_string<CharT, Traits, Alloc> >::operator()
    (std::basic_string<CharT, Traits, Alloc> const &x) const {
    unsigned long res(0);
    typename std::basic_string<CharT, Traits, Alloc>::const_iterator i,
      endi;
    
    for( i=x.begin(), endi=x.end(); endi!=i; ++i )
      res = 5*res + Traits::to_int_type(*i);
    return static_cast<size_t>(res);
  }
  
} // namespace utilmm

#endif // IN_UTILMM_HASH_HEADER
