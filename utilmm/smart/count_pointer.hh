/* -*- C++ -*-
 * $Id: count_pointer.hh 957 2005-03-07 16:01:20Z sjoyeux $
 */
#ifndef UTILMM_SMART_COUNT_POINTER_HEADER
# define UTILMM_SMART_COUNT_POINTER_HEADER
#include "utilmm/config/config.h"

#include "utilmm/smart/bits/ref_count_manager.hh"
#include "utilmm/smart/bits/simple_memory.hh"
#include "utilmm/smart/pointer.hh"

namespace utilmm {
  namespace smart {

    /** @brief reference counting pointer definition traits
     *
     * This structure is just an helper to have direct access
     * for the basic smart pointer using a reference counting
     * life time manager.
     *
     * @param Ty The type we want to point to
     *
     * To use a smart pointer with reference count managament user just
     * need to declare the type like in following code.
     *
     * @code
     * #include "smart/count_pointer.hh"
     *
     * // [...]
     *
     * utilmm::smart::count_pointer<int>::type pi(new int(5)), pj;
     *
     * *pi = 6;
     * pj = pi;
     * @endcode
     *
     * @sa utilmm::smart::pointer
     * @sa utilmm::smart::ref_count::manager
     *
     * @author Frédéric Py <fpy@laas.fr>
     * @ingroup smart
     */
    template<typename Ty>
    struct count_pointer {
    private:
      typedef ref_count::simple_memory<Ty> mem_type;
      typedef ref_count::manager<mem_type> manager_type;
      
    public:
      /** @brief The smart pointer type
       *
       * This the type of the smart pointer with
       * reference counting management. 
       */
      typedef pointer<manager_type> type; 
    }; // struct utilmm::smart::count_pointer

  } // namespace utilmm::smart
} // namespace utilmm

#endif // UTILMM_SMART_COUNT_POINTER_HEADER
/** @file smart/count_pointer.hh
 * @brief Definition of a reference counting smart pointer
 *
 * This header defines a smart pointer using reference counting garbage
 * collection for life time management
 *
 * @author Frédéric Py <fpy@laas.fr>
 * @ingroup smart
 */
