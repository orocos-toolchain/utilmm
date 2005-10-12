/* -*- C++ -*-
 * $Id: utils.hh 957 2005-03-07 16:01:20Z sjoyeux $
 */
#ifndef UTILMM_FUNCTIONAL_UTILS_HEADER
# define UTILMM_FUNCTIONAL_UTILS_HEADER

#include "utilmm/functional/arg_traits.hh"
# include <functional>

namespace utilmm {

  /** @brief Identity functor
   *
   * This functor correspond to the identity function. It is a function
   * that will return exactly the same value as passed as argument. It also
   * uses arg_traits to improve performances avoiding the copy of complex
   * objects
   *
   * @param Ty the type of arguments
   *
   * @author Frédéric Py <fpy@laas.fr>
   */
  template<typename Ty>
  struct identity :public std::unary_function<Ty, Ty> {
  private:
    typedef typename arg_traits<Ty>::type arg_type;
    
  public:
    /** @brief Identity call
     * @param x The argument
     * @return @a x without modification
     */
    arg_type operator()(arg_type x) const {
      return x;
    }
  }; // struct utilmm::identity<>

  /** @brief First pair attribute access
   *
   * This functor gives a const access to the @c first
   * attribute of a @c std::pair
   *
   * @param Pair the complete type of the @c std::pair we want to pass as
   * argument 
   *
   * @author Frédéric Py <fpy@laas.fr>
   * @sa select_2nd
   */
  template<class Pair>
  struct select_1st
    :public std::unary_function<Pair, typename Pair::first_type> {
    /** @brief call operator
     *
     * @param p The pair
     * @return A const reference to @c p.first
     */
    typename Pair::first_type const &operator()(Pair const &p) const {
      return p.first;
    }
  }; // struct utilmm::select_1st<>

  /** @brief Second pair attribute access
   *
   * This functor gives a const access to the @c second
   * attribute of a @c std::pair
   *
   * @param Pair the complete type of the @c std::pair we want to pass as
   * argument 
   *
   * @author Frédéric Py <fpy@laas.fr>
   * @sa select_1st
   */
  template<class Pair>
  struct select_2nd
    :public std::unary_function<Pair, typename Pair::second_type> {
    /** @brief call operator
     *
     * @param p The pair
     * @return A const reference to @c p.second
     */
    typename Pair::second_type const &operator()(Pair const &p) const {
      return p.second;
    }
  }; // struct utilmm::select_2nd<>

} // namespace utilmm

#endif // UTILMM_FUNCTIONAL_UTILS_HEADER 
/** @file functional/utils.hh
 * @brief Som usefull functors
 *
 * This header defines some classic and usefull functors. Many of these
 * functors are part of the SGI STL extension.
 *
 * @author Frédéric Py <fpy@laas.fr>
 */
