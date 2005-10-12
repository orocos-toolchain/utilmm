/* -*- C++ -*-
 * $Id: unary_compose.hh 957 2005-03-07 16:01:20Z sjoyeux $
 */
#ifndef UTILMM_FUNCTIONAL_UNARY_COMPOSE_HEADER
# define UTILMM_FUNCTIONAL_UNARY_COMPOSE_HEADER

# include <functional>

#include "utilmm/functional/arg_traits.hh"

namespace utilmm {
  
  /** @brief Unary functors composition
   *
   * This class helps programmers to compose two @c std::unary_function
   * in one
   *
   * @param UnaryFun1 The first unary function
   * @param UnaryFun2 The second unary function
   *
   * The resulting class is a functor corresponding to @c UnaryFun1(UnaryFun2(x))
   *
   * @pre The @c result_type of @a UnaryFun2 must be default convertible to the
   * @c argument_type of @a UnaryFun1
   *
   * @author Frédéric Py <fpy@laas.fr>
   *
   * @sa utilmm::binary_compose_2
   */
  template<class UnaryFun1, class UnaryFun2>
  struct unary_compose
    :public std::unary_function<typename UnaryFun2::argument_type,
				 typename UnaryFun1::result_type> {
  private:
    UnaryFun1 fun_1;
    UnaryFun2 fun_2;    

    typedef typename 
      arg_traits<typename UnaryFun2::argument_type>::type arg_param;

  public:
    /** @brief Constructor
     *
     * Create a new instance having the two functors instances to
     * compose.
     *
     * @param one The first functor
     * @param two The second functor
     *
     * @post newly created instance will have exactly the
     * same behavior as one(two(x)).
     *
     * @sa utilmm::compose1
     */
    unary_compose(UnaryFun1 const &one, UnaryFun2 const &two)
      :fun_1(one), fun_2(two) {}
    
    /** @brief Call operator
     *
     * Compute the result of the composition for a given argument
     *
     * @param x The argument
     *
     * @return The result of the operation
     */
    typename UnaryFun1::result_type operator()(arg_param x)
      const {
      return fun_1(fun_2(x));
    }

  }; // class utilmm::binary_compose_2<>

  /** @brief Composition function.
   *
   * This function create a new @c unary_compose instance based on two given
   * function. It only ease the declaration of @c unary_compose instances.
   *
   * @param f The first function
   * @param g The second function
   *
   * @return a functor corresponding to @c f(g(x))
   *
   * @relates utilmm::unary_compose
   *
   * @sa unary_compose::unary_compose
   */
  template<class Fun1, class Fun2>
  unary_compose<Fun1, Fun2> compose1(Fun1 const &f, Fun2 const &g) {
    return unary_compose<Fun1, Fun2>(f, g);
  }

} // namespace utilmm

#endif // UTILMM_FUNCTIONAL_BINARY_COMPOSE_HEADER
/** @file functional/unary_compose.hh
 * @brief utilmm::unary_compose definition
 *
 * This header define the @c utilmm::unary_compose class.
 *
 * @author Frédéric Py <fpy@laas.fr>
 */
