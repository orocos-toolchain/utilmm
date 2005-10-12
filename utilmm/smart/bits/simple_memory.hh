/* -*- C++ -*-
 * $Id: simple_memory.hh 953 2005-02-17 21:08:23Z fpy $
 */
#ifndef UTILMM_SMART_SIMPLE_MEMORY_HEADER
# define UTILMM_SMART_SIMPLE_MEMORY_HEADER

# include <utility>

namespace utilmm {
  namespace smart {
    namespace ref_count {

      /** @brief simple memory manager for utilmm::smart::ref_count::manager
       *
       * This class implements the simplest memory manager we can use for
       * @c utilmm::smart::ref_count::manager class. This manager will
       * just delete the pointed cell when the manager request for it.
       *
       * @param Ty The type of managed cells
       *
       * @note This manager may be buggy as it could create two counter
       * for one pointed cell then it will surely delete twice the pointed
       * cell.
       *
       * @author Frédéric Py <fpy@laas.fr>
       * @ingroup smart
       * @ingroup intern
       */
      template<typename Ty>
      struct simple_memory {
	/** @brief Real type for memory cells */
	typedef Ty base_type;
	/** @brief memory cell type */
	typedef std::pair<base_type, size_t> cell_type;
	/** @brief public memory cell type */
	typedef cell_type *mem_cell;

	/** @brief null cell creation
	 *
	 * This function create the cell corresponding to null
	 *
	 * @return the null cell
	 */
	mem_cell null_cell() {
	  return mem_cell(0);
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
	 * @bug If the same pointer is passed more than one time
	 * to this function there's no control to return a mem_cell
	 * with the good reference counter.
	 */
	mem_cell create(base_type *ptr) {
	  return new cell_type(ptr, 0);
	}

	/** @brief Destroy a memory cell
	 *
	 * This function Will detroy a memory cell and the real
	 * pointed value
	 *
	 * @param c The cell to destroy
	 */
	void destroy(mem_cell c) {
	  delete c->first;
	  delete c;
	}

      }; // class utilmm::smart::ref_count::simple_memory<>

    } // namespace utilmm::smart::ref_count
  } // namespace utilmm::smart
} // namespace utilmm

#endif // UTILMM_SMART_SIMPLE_MEMORY_HEADER
/** @file smart/bits/simple_memory.hh
 * @brief Definition of utilmm::smart::ref_count::simple_memory
 *
 * This header defines the class @c utilmm::smart::ref_count::simple_memory
 * class.
 *
 * @author Frédéric Py <fpy@laas.fr>
 * @ingroup smart
 * @ingroup intern
 */
