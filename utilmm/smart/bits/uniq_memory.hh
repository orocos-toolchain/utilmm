/* -*- C++ -*-
 * $Id: uniq_memory.hh 961 2005-03-11 14:05:33Z fpy $
 */
#ifndef UTILMM_SMART_UNIQ_MEMORY_HEADER
# define UTILMM_SMART_UNIQ_MEMORY_HEADER

#include "utilmm/hash/hash_map.hh"

namespace utilmm {
  namespace smart {
    namespace ref_count {

      /** @brief Special hash functor for utilmm::smart::ref_count::uniq_memory
       *
       * This functor is an hash functor used by the
       * @c utilmm::smart::ref_count::uniq_memory class. To extract
       * the hash value of the value pointed by a cont pointer
       *
       * @param Ty The type of pointed cell
       * @param Hash A hash fucntor for @a Ty
       *
       * @author Frédéric Py <fpy@laas.fr>
       * @ingroup smart
       * @ingroup intern
       *
       * @relates utilmm::smart::ref_count::uniq_memory
       */
      template<typename Ty, class Hash>
      struct hash_ptr
	:public std::unary_function<Ty const *, size_t> {
	/** @brief Hashing call operator
	 *
	 * @param arg A cont pointer
	 *
	 * This function compute the hash value of @c *arg
	 *
	 * @return The computed hash value
	 *
	 * @pre @a arg has to point to a valid cell
	 */
	size_t operator()(Ty const *arg) const;
      }; // struct utilmm::smart::ref_count::hash_ptr<>


      /** @brief In depth pointer equality functor.
       *
       * This functor is used by @c utilmm::smart::ref_count::uniq_memory
       * to test if two pointers are pointing to equal cells.
       *
       * @param Ty The type of pointed cells
       * @param Equal equality functor for @a Ty
       *
       * @author Frédéric Py <fpy@laas.fr>
       * @ingroup smart
       * @ingroup intern
       *
       * @relates utilmm::smart::ref_count::uniq_memory
       */
      template<typename Ty, class Equal>
      struct eq_ptr
	:public std::binary_function<Ty const *, Ty const *, bool> {
	/** @brief Call operator
	 *
	 * @param a a const pointer
	 * @param b a const pointer
	 *
	 * This function checks if the pointed values by @a a and @a b are
	 * equals
	 *
	 * @return @c Equal(*a, *b)
	 *
	 * @pre @a a and @a b must not be null pointers
	 */
	bool operator()(Ty const *a, Ty const *b) const;
      }; // struct utilmm::smart::ref_count::eq_ptr<>

      /** @brief Unique instance memory manager
       *
       * @param Ty The type of managed cells
       * @param Hash A hash functor for @a Ty
       * @param Equal An equality functor for @a Ty
       *
       * This class is a memory manager for the
       * @c utilmm::smart::ref_count::manager class. It implements
       * a memory where there's one and only one element with a
       * given value.
       *
       * In this memory we have the guarantee that there's no mem_cell
       * pair with pointed values that return @c true when passed
       * as argument to @a Equal
       *
       * @author Frédéric Py <fpy@laas.fr>
       * @ingroup smart
       * @ingroup intern
       */
      template<typename Ty, class Hash, class Equal>
      class uniq_memory {
      public:
	/** @brief Real pointed type */
	typedef Ty const base_type;
	
      private:
	typedef hash_map< base_type *, size_t, hash_ptr<Ty, Hash>,
			  eq_ptr<Ty, Equal> > mem_type;

	mem_type the_mem;
	
      public:
	/** @brief type of managed mem cells */
	typedef typename mem_type::iterator mem_cell;

	/** @brief null cell creation
	 *
	 * This function create the cell corresponding to null
	 *
	 * @return the null cell
	 */
	mem_cell null_cell() {
	  return the_mem.end();
	}

	/** @brief Attachment of a new pointer
	 *
	 * This function adds a new pointer to the memory
	 * management.
	 *
	 * @param ptr The pointer to manage
	 *
	 * @return The cell associated to @a ptr
	 *
	 * @note if memory has allready a mem_cell equivalent to @a ptr
	 * then @a ptr is deleted 
	 */
	mem_cell create(base_type *ptr);

	/** @brief Destroy a memory cell
	 *
	 * This function Will detroy a memory cell and the real
	 * pointed value
	 *
	 * @param c The cell to destroy
	 */
	void destroy(mem_cell c);
      }; // class utilmm::smart::ref_count::uniq_memory<>

    } // namespace utilmm::smart::ref_count
  } // namespace utilmm::smart
} // namespace utilmm

# define IN_UTILMM_SMART_UNIQ_MEMORY_HEADER
#include "utilmm/smart/bits/uniq_memory.tcc"
# undef IN_UTILMM_SMART_UNIQ_MEMORY_HEADER
#endif // UTILMM_SMART_UNIQ_MEMORY_HEADER
/** @file smart/bits/uniq_memory.hh
 * @brief Defintion of utilmm::smart::ref_count::uniq_memory
 *
 * This header defines the @c utilmm::smart::ref_count::uniq_memory class.
 *
 * @author Frédéric Py <fpy@laas.fr>
 * @ingroup smart
 * @ingroup intern
 */
