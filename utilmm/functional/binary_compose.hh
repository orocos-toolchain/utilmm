/* -*- C++ -*-
 * $Id: binary_compose.hh 957 2005-03-07 16:01:20Z sjoyeux $
 */
#ifndef UTILMM_FUNCTIONAL_BINARY_COMPOSE_HEADER
# define UTILMM_FUNCTIONAL_BINARY_COMPOSE_HEADER

# include <functional>

#include "utilmm/functional/arg_traits.hh"

namespace utilmm {
  
  /** @brief Binary functor composition with two unary functors
   *
   * This class helps programmers to compose one @c std::binary_function
   * with two @c std::unary_function
   *
   * @param BinaryFun The binary functor
   * @param UnaryFun1 The first unary function
   * @param UnaryFun2 The second unary function
   *
   * The resulting class is a functor corresponding to
   * @c BinaryFun(UnaryFun1(x), UnaryFun2(y))
   *
   * @pre The @c result_type of @a UnaryFun1 must be default convertible to the
   * @c first_argument_type of @a BinaryFun
   * @pre The @c result_type of @a UnaryFun2 must be default convertible to the
   * @c second_argument_type of @a BinaryFun
   *
   * @author Frédéric Py <fpy@laas.fr>
   *
   * @sa utilmm::unary_compose
   */
  template<class BinaryFun, class UnaryFun1, class UnaryFun2>
  struct binary_compose_2
    :public std::binary_function<typename UnaryFun1::argument_type,
				 typename UnaryFun2::argument_type,
				 typename BinaryFun::result_type> {
  private:
    BinaryFun bin_fn;
    UnaryFun1 fun_1;
    UnaryFun2 fun_2;    

    typedef typename 
      arg_traits<typename UnaryFun1::argument_type>::type first_arg;
    typedef typename 
      arg_traits<typename UnaryFun2::argument_type>::type second_arg;

  public:
    /** @brief Constructor
     *
     * Create a new instance having the three functors instances to
     * compose.
     *
     * @param bin The binary functor
     * @param one The first unary functor
     * @param two The second unary functor
     *
     * @post newly created instance will have exactly the
     * same behavior as bin(one(x), two(y)).
     *
     * @sa utilmm::compose2_2
     */
    binary_compose_2(BinaryFun const &bin, UnaryFun1 const &one,
		     UnaryFun2 const &two)
      :bin_fn(bin), fun_1(one), fun_2(two) {}
    
    /** @brief Call operator
     *
     * Compute the result of the composition for given arguments
     *
     * @param a The first argument
     * @param b The second argument
     *
     * @return The result of the operation
     */
    typename BinaryFun::result_type operator()(first_arg a, second_arg b)
      const {
      return bin_fn(fun_1(a), fun_2(b));
    }

  }; // class utilmm::binary_compose_2<>

  /** @brief Composition function.
   *
   * This function create a new @c binary_compose_2 based on one binary_function
   * and two unary functions. It only ease the declaration of @c binary_compose_2
   * instances.
   *
   * @param f The binary function
   * @param a The first unary function
   * @param b The second unary function
   *
   * @return a functor corresponding to @c f(a(x), b(y))
   *
   * @relates utilmm::binary_compose_2
   *
   * @sa binary_compose_2::binary_compose_2
   */
  template<class Bin, class Fun1, class Fun2>
  binary_compose_2<Bin, Fun1, Fun2> compose2_2(Bin const &f, Fun1 const &a,
					       Fun2 const &b) {
    return binary_compose_2<Bin, Fun1, Fun2>(f, a, b);
  }

} // namespace utilmm

#endif // UTILMM_FUNCTIONAL_BINARY_COMPOSE_HEADER
/** @file functional/binary_compose.hh
 * @brief utilmm::binary_compose_2 definition
 *
 * This header define the @c utilmm::binary_compose_2 class.
 *
 * @author Frédéric Py <fpy@laas.fr>
 */
