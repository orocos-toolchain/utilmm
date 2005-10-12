/* -*- C++ -*-
 * $Id: ref_count_manager.hh 957 2005-03-07 16:01:20Z sjoyeux $
 */
#ifndef UTILMM_SMART_REF_COUNT_MANAGER_HEADER
# define UTILMM_SMART_REF_COUNT_MANAGER_HEADER

#include "utilmm/singleton/wrapper_fwd.hh"

namespace utilmm {
  namespace smart {
    /** @brief Toolbox for reference counting life time management
     *
     * This namespace embeds all the classes used to define a manager
     * for @c smart::pointer using a reference counting life time
     * management
     */
    namespace ref_count {
      
      /** @brief Reference counting management
       *
       * @param Memory a model for the memory 
       *
       * This class is a manager for @c smart::pointer that control the life
       * time policy of memory cells managed by @a Memory
       *
       * @note @c utilmm::smart::simple_memory is a quite simple and
       * illustrative model for @a Memory
       *
       * @author Frédéric Py <fpy@laas.fr>
       *
       * @ingroup smart
       * @ingroup intern
       */
      template<class Memory>
      class manager {
      public:
	/** @brief public type of pointed cells */
	typedef typename Memory::base_type base_type;
	/** @brief real type of pointed cells
	 *
	 * This type will store all the information of
	 * the reference counter it may be compatible with
	 * the @c std::pair<base_type,size_t>* type.
	 */
	typedef typename Memory::mem_cell mem_cell;

	/** @brief Add new pointer in management
	 *
	 * @param ptr The pointer to manage
	 * 
	 * This function is called by @c smart::pointer when this
	 * one wants to add a new pointer to the manager management.
	 *
	 * @return The cell corresponding to this pointer.
	 */
	mem_cell manage(base_type *ptr);
	/** @brief Assignment of a cell
	 *
	 * This function is called by @c utilmm::smart::pointer to indicate
	 * to the manager that a given cell is assigned to a new pointer.
	 *
	 * @param c The cell assigned
	 *
	 * @post reference counter of @a c is incremented by 1
	 *
	 * @return @a c ater the operation
	 */
	mem_cell assign(mem_cell c);
	
	/** @brief Unassignement of a cell
	 *
	 * This function is called by @c utilmm::smart::pointer to indicate
	 * that a pointer has stopped to used a given cell.
	 *
	 * @param c The cell released
	 *
	 * @post The counter for @a c is decrmented by 1 and, if this counter
	 * has reached the 0 value the cell is detroyed. 
	 */
	void release(mem_cell c);

	/** @brief Check if null
	 *
	 * Check if a given cell points to nothing
	 *
	 * @param c The cell to check
	 * @retval true if @a c is null
	 * @retval false else
	 */
	bool null(mem_cell c) {
	  return c==mem.null_cell();
	}

	/** @brief real pointer value.
	 *
	 * This  function give an access to the real pointer value
	 * of a given cell.
	 *
	 * @param c The cell
	 *
	 * @return the pointer value of the cell
	 */
	base_type *get_ptr(mem_cell c) {
	  return c->first;
	}

      private:
	manager() {}

	~manager() {}

	Memory mem;

	template<class Ty>
	friend class singleton::wrapper;
      }; // class utilmm::smart::ref_count::manager<>

    } // namespace utilmm::smart::ref_count
  } // namespace utilmm::smart
} // namespace utilmm

# define IN_UTILMM_SMART_REF_COUNT_MANAGER_HEADER
#include "utilmm/smart/bits/ref_count_manager.tcc"
# undef IN_UTILMM_SMART_REF_COUNT_MANAGER_HEADER
#endif // UTILMM_SMART_REF_COUNT_MANAGER_HEADER
/** @file smart/bits/ref_count_manager.hh
 * @brief Definition of utilmm::smart::ref_count::manager
 *
 * This class defines the class @c utilmm::smart::ref_count::manager
 * which may be used as a manager for @c utilmm::smart::pointer
 *
 * @author Frédéric Py <fpy@laas.fr>
 *
 * @ingroup smart
 * @ingroup intern
 */
