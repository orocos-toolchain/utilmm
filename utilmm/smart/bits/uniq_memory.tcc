/* -*- C++ -*-
 * $Id: uniq_memory.tcc 937 2005-01-09 14:35:39Z fpy $
 */
#ifndef IN_UTILMM_SMART_UNIQ_MEMORY_HEADER
# error "Cannot include template files directly"
#else

namespace utilmm {
  namespace smart {
    namespace ref_count {

      /*
       * struct utilmm::smart::ref_count::hash_ptr<>
       */
      template<typename Ty, class Hash>
      size_t hash_ptr<Ty, Hash>::operator()(Ty const *ptr) const {
	Hash hf;

	return hf(*ptr); 
      }

      /*
       * struct utilmm::smart::ref_count::eq_ptr<>
       */
      template<typename Ty, class Equal>
      bool eq_ptr<Ty, Equal>::operator()(Ty const *a, Ty const *b) const {
	Equal eq;
	
	return a==b || eq(*a, *b);
      }

      /*
       * class utilmm::smart::ref_count::uniq_memory<>
       */
      template<typename Ty, class Hash, class Equal>
      typename uniq_memory<Ty, Hash, Equal>::mem_cell
      uniq_memory<Ty, Hash, Equal>::create
      (typename uniq_memory<Ty, Hash, Equal>::base_type *ptr) {
	typename mem_type::value_type to_ins(ptr, 0);
	std::pair<mem_cell, bool> ins_res = the_mem.insert(to_ins);
	
	if( !ins_res.second && ins_res.first->first!=ptr )
	  delete ptr;
	return ins_res.first;
      }

      template<typename Ty, class Hash, class Equal>
      void uniq_memory<Ty, Hash, Equal>::destroy
      (typename uniq_memory<Ty, Hash, Equal>::mem_cell c) {
	base_type *to_del = c->first;
	
	the_mem.erase(c);
	delete to_del;
      }
      
    } // namespace utilmm::smart::ref_count
  } // namespace utilmm::smart
} // namespace utilmm

#endif // IN_UTILMM_SMART_UNIQ_MEMORY_HEADER 
