/* -*- C++ -*-
 * $Id: arg_traits.hh 957 2005-03-07 16:01:20Z sjoyeux $
 */
#ifndef UTILMM_UTILS_ARG_TRAITS_HEADER
# define UTILMM_UTILS_ARG_TRAITS_HEADER

#include "boost/type_traits.hpp"
#include "boost/mpl/if.hpp"

namespace utilmm {
  
  /** @brief argument type traits
   * 
   * This traits is an helper to determine the classical argument type
   * used to pass a given type as argument to a function.
   *
   * @param Ty the type to pass as argument
   *
   * In fact this class just determine if the givent type is a complex
   * one and in this case proposes to pass it as a const reference, otherwise
   * -- it is a POD -- the argument type is the same as @c Ty. 
   *
   * @author Frédéric Py <fpy@laas.fr>
   */
  template<typename Ty>
  struct arg_traits {
  private:
    typedef typename boost::remove_cv<Ty>::type real_type;

    typedef typename boost::mpl::if_c< boost::is_pod<real_type>::value,
				       real_type, 
				       real_type const &>::type computed_type;
  public:
    /** @brief argument type
     *
     * This typedef is the type used to pass the given type as argument
     */
    typedef computed_type type;
    
  }; // namespace utilmm::arg_traits<>

} // namespace utilmm

#endif // UTILMM_UTILS_ARG_TRAITS_HEADER
/** @file functional/arg_traits.hh
 * @brief Definition of utilmm::arg_traits class
 *
 * This header is a small extension of @c boost/type_traits.hpp offering a
 * small traits used to determine what is the classical type used to pass a
 * specific variable type as argument 
 */
