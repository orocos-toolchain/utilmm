/* -*- C++ -*-
 * $Id: cached_fn.tcc 937 2005-01-09 14:35:39Z fpy $
 */
#ifndef IN_UTILMM_FUNCTIONAL_CACHED_FN_HEADER
# error "Cannot include template files directly"
#else 

namespace utilmm {
  
  /*
   * class utilmm::cached_fn<>
   */
  template<typename Arg, typename Ret, class Hash, class Equal>
  Ret const &cached_fn<Arg, Ret, Hash, Equal>::operator()
    (typename cached_fn<Arg, Ret, Hash, Equal>::arg_type arg) {
    typename cache_type::iterator cached_res = the_cache.find(arg);
    
    if( cached_res==the_cache.end() ) {
      typename cache_type::value_type cache_cell(arg, call(arg));

      if( the_cache.size()==the_cache.max_size() )
	the_cache.clear();
      cached_res = the_cache.insert(cache_cell).first;
    }
    return cached_res->second;
  }

} // namespace utilmm

#endif // IN_UTILMM_FUNCTIONAL_CACHED_FN_HEADER 
