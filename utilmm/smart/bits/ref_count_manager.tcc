/* -*- C++ -*-
 * $Id: ref_count_manager.tcc 937 2005-01-09 14:35:39Z fpy $
 */
#ifndef IN_UTILMM_SMART_REF_COUNT_MANAGER_HEADER
# error "Cannot include template files directly"
#else

namespace utilmm {
  namespace smart {
    namespace ref_count {

      /*
       * class utilmm::smart::ref_count::manager<>
       */
      template<class Mem>
      typename manager<Mem>::mem_cell 
      manager<Mem>::manage(typename manager<Mem>::base_type *ptr) {
	if( 0==ptr )
	  return mem.null_cell();
	else
	  return assign(mem.create(ptr));
      }

      template<class Mem>
      typename manager<Mem>::mem_cell
      manager<Mem>::assign(typename manager<Mem>::mem_cell c) {
	if( !null(c) ) 
	  ++(c->second);
	return c;
      }

      template<class Mem>
      void manager<Mem>::release(typename manager<Mem>::mem_cell c) {
	if( !null(c) && (c->second--)<=1 )
	  mem.destroy(c);
      }

    } // namespace utilmm::smart::ref_count
  } // namespace utilmm::smart
} // namespace utilmm

#endif // IN_UTILMM_SMART_REF_COUNT_MANAGER_HEADER 
