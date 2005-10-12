/* -*- C++ -*-
 * $Id: basic_symbol_fwd.hh 953 2005-02-17 21:08:23Z fpy $
 */
#ifndef UTILMM_SMART_TYPES_BASIC_SYMBOL_FWD
# define UTILMM_SMART_TYPES_BASIC_SYMBOL_FWD

# include <string>

namespace utilmm {

  template< class CharT, class Traits=std::char_traits<CharT>,
	    class Alloc=std::allocator<CharT> >
  class basic_symbol;

  /** @brief printing function
   *
   * @param out An output stream
   * @param s A symbol
   *
   * This operator writes the names of @a s in @a out
   *
   * @relates utilmm::basic_symbol
   */
  template<class CharT, class Traits, class Alloc>
  std::ostream &operator<< (std::ostream &out, 
			    basic_symbol<CharT, Traits, Alloc> const &s);

} // namespace utilmm

#endif // UTILMM_SMART_TYPES_BASIC_SYMBOL_FWD
/** @file types/bits/basic_symbol_fwd.hh
 * @brief Forward declaration of utilmm::basic_symbol
 */
