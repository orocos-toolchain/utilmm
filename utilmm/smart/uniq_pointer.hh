/* -*- C++ -*-
 * $Id: uniq_pointer.hh 957 2005-03-07 16:01:20Z sjoyeux $
 */
#ifndef UTILMM_SMART_UNIQ_POINTER_HEADER
# define UTILMM_SMART_UNIQ_POINTER_HEADER
#include "utilmm/config/config.h"

#include "utilmm/smart/bits/ref_count_manager.hh"
#include "utilmm/smart/bits/uniq_memory.hh"
#include "utilmm/smart/pointer.hh"

namespace utilmm {
  namespace smart {

    /** @brief Unique instance memory pointer definition traits
     *
     * This structure is just an helper to have direct access
     * for the unique insatnce memory smart pointer with reference
     * counting management
     * 
     * @param Ty The type we want to point to
     * @param Hash A hashing functor for @a Ty
     * @param Equal An equality functor for @a Ty
     *
     * To use a smart pointer to unique memory user just
     * need to declare the type like in following code.
     *
     * @code
     * #include "smart/uniq_pointer.hh"
     *
     * // [...]
     *
     * utilmm::smart::uniq_pointer<int>::type pi(new int(5)), pj;
     *
     * if( *pi==5 )
     *   pj = pi;
     * @endcode
     *
     * @sa utilmm::smart::pointer
     * @sa utilmm::smart::ref_count::manager
     * @sa utilmm::smart::ref_count::uniq_memory
     *
     * @author Frédéric Py <fpy@laas.fr>
     * @ingroup smart
     */
    template< typename Ty, class Hash=hash<Ty>, class Equal=std::equal_to<Ty> >
    struct uniq_pointer {
    private:
      typedef ref_count::uniq_memory<Ty, Hash, Equal> mem_type;
      typedef ref_count::manager<mem_type> manager_type;
      
    public:
      /** @brief The smart pointer type
       *
       * This the type of the smart pointer with
       * unique memory and reference counting management. 
       */
      typedef pointer<manager_type> type; 
    }; // struct utilmm::smart::uniq_pointer

  }; // namespace utilmm::smart

  template<class Manager>
  struct hash< smart::pointer<Manager> >
    :public std::unary_function<smart::pointer<Manager>, size_t> {
    size_t operator()(smart::pointer<Manager> const &x) const {
      hash< typename smart::pointer<Manager>::pointer_type > hf;

      return size_t(x.null()?0:hf(x));
    }
  }; // struc utilmm::hash< smart::pointer<> >

} // namespace utilmm

#endif // UTILMM_SMART_UNIQ_POINTER_HEADER
/** @file smart/uniq_pointer.hh
 * @brief Definition of a reference counting smart pointer
 *
 * This header defines a smart pointer using reference counting garbage
 * collection for life time management and unique instance memory
 *
 * @author Frédéric Py <fpy@laas.fr>
 * @ingroup smart
 */
