/* -*- C++ -*-
 * $Id: table_fwd.hh 950 2005-02-17 14:53:54Z fpy $
 */
#ifndef UTILMM_UTILS_HASH_TABLE_FWD
# define UTILMM_UTILS_HASH_TABLE_FWD

namespace utilmm {
  namespace hash_toolbox {

    template< typename Key, typename Data, class Extract,
	      class Hash, class Equal >
    class table;

  } // namespace utilmm::hash_toolbox
} // namespace utilmm

#endif // UTILMM_UTILS_HASH_TABLE_FWD

/** @file hash/bits/table_fwd.hh
 * @brief forward declaration of utilmm::hash_toolbox::table
 *
 * @author Frédéric Py <fpy@laas.fr>
 * @ingroup hashing
 * @ingroup intern
 */
