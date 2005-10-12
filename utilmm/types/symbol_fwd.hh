/* -*- C++ -*-
 * $Id: symbol_fwd.hh 957 2005-03-07 16:01:20Z sjoyeux $
 */
#ifndef UTILMM_SMART_TYPES_SYMBOL_FWD
# define UTILMM_SMART_TYPES_SYMBOL_FWD

#include "utilmm/types/bits/basic_symbol_fwd.hh"

namespace utilmm {

  /** @brief specialization of utilmm::basic_symbol using char
   *
   * You can see it as std::string compared to std::basic_string
   */
  typedef basic_symbol<char> symbol;

} // namespace utilmm

#endif // UTILMM_SMART_TYPES_SYMBOL_FWD
/** @file types/symbol_fwd.hh
 * @brief Forward defclaration of utilmm::symbol
 */
