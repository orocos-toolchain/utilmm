/* -*- C++ -*-
 * $Id: cached_fn.hh 957 2005-03-07 16:01:20Z sjoyeux $
 */
#ifndef UTILMM_FUNCTIONAL_CACHED_FN_HEADER
# define UTILMM_FUNCTIONAL_CACHED_FN_HEADER

#include "utilmm/config/config.h"
#include "utilmm/hash/hash_map.hh"

namespace utilmm {

  /** @brief A functor with cache
   * 
   * This class is the basis for functors needing a cache. It
   * allows programmer to implement easily a functor that will use
   * a cache to store previously computed results. It can be usefull
   * when the function is complex and may be called many times with the
   * same argument.
   *
   * @param Arg The argument type of the functor
   * @param Ret The return type of the functor
   * @param Hash hashing functor used to hash argument keys
   * @param Equal equality test for arguments
   *
   * The cache of this functor is based on a @c utilmm::hash_map with
   * @a Arg as key and @a ret as attached value.  
   *
   * A small example illustrating the way to use it and showing
   * an alternative way to use it is the id generator :
   * @code
   * #include "utilmm/cached_fn.hh"
   * 
   * template< class Obj, class Hash=utilmm::hash<Obj>,
   *           class Eq = std::equal_to<Obj> >
   * class id_gen
   *    :public cached_fn<Obj, size_t, Hash, Eq> {
   * private:
   *   typedef cached_fn<Obj, size_t, Hash, Eq>::mother_class;
   * 
   *   size_t call(Obj const &arg) {
   *     return mother_class::cache_size()+1;
   *   }
   * };
   * 
   * @endcode
   *
   * This functor will then attach a unique id to an @c Obj instance
   * and will return this id until will clear the cache.
   *
   * @author Frédéric Py <fpy@laas.fr>
   * @todo Current implementation uses inheritance polymorphism and may be
   * more efficient to use static polymorphism to ease the encapsulation of
   * non cached functors into this cached one.
   *
   * @todo It could be interresting to generalize the argument number of this
   * functor using boost::tuple or another boost utility 
   */
  template< typename Arg, typename Ret, class Hash=hash<Arg>,
	    class Equal=std::equal_to<Arg> >
  class cached_fn
    :public std::unary_function<Arg, Ret> {
  private:
    typedef hash_map<Arg, Ret, Hash, Equal> cache_type;
    typedef typename arg_traits<Arg>::type arg_type;
    
    cache_type the_cache;
    
  protected:
    /** @brief the real function.
     *
     * This pure virtual function will embed the function code. It is called
     * by @c operator() if and only if the cache does not include any cached
     * result for @a arg.
     *
     * @param arg The argument of the function
     * @return the result of the function
     */
    virtual Ret call(Arg const &arg) =0;

  public:
    /** @brief Default constructor
     *
     * Create a new instance with an empty cache
     */
    cached_fn() {}
    /** @brief Destructor */ 
    virtual ~cached_fn() {}

    /** @brief Call operator
     *
     * This is the public interface used to call the function. It will check
     * if the function was allready called with this argument. If it was called
     * it returns the value in cache else it compute the new result and
     * store it in cache
     *
     * @param arg The argument of the function
     * @return The value attached to @a arg
     *
     * @post The cache is not empty
     *
     * @sa call(Arg const &arg)
     */
    Ret const &operator()(arg_type arg);

    /** @brief clear the cache
     *
     * Thsi function clear the cache
     *
     * @post The cache is empty
     */
    void empty_cache() {
      the_cache.clear();
    }
    
    /** @brief Cache size
     *
     * @return the number of calls stored in cache
     */
    size_t cache_size() const {
      return the_cache.size();
    }
  }; // struct utilmm::cached_fn<>

} // namespace utilmm

# define IN_UTILMM_FUNCTIONAL_CACHED_FN_HEADER
#include "utilmm/functional/bits/cached_fn.tcc" 
# undef IN_UTILMM_FUNCTIONAL_CACHED_FN_HEADER 
#endif // UTILMM_FUNCTIONAL_CACHED_FN_HEADER
/** @file functional/cached_fn.hh
 * @brief definition of utilmm::cached_fn
 *
 * This header defines the utilmm::cached_fn functor.
 *
 * @author Frédéric Py <fpy@laas.fr>
 */
