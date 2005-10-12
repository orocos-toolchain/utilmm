/* -*- C++ -*-
 * $Id: pointer.hh 1004 2005-10-10 12:55:33Z sjoyeux $
 */
#ifndef UTILMM_SMART_POINTER_HEADER
# define UTILMM_SMART_POINTER_HEADER
#include "utilmm/config/config.h"

# include <stdexcept>

#include "utilmm/singleton/use.hh"

namespace utilmm {
  /** @brief Smart pointers implementation
   *
   * This namespace embeds all the classes used to implement smart pointers
   *
   * @ingroup smart
   */
  namespace smart {

    /** @brief Null pointer access exception
     *
     * This class represent an exception that may be throwned when user
     * tries to access a @c NULL pointer.
     *
     * @author Frédéric Py <fpy@laas.fr>
     * @ingroup smart
     */
    class null_access :public std::runtime_error {
    public:
      /** @brief Constructor
       *
       * @param message The message attached to this instance
       */
      null_access(std::string const &message)
	: std::runtime_error(message) {}
    }; // class utilmm::smart::null_access
    
    /** @brief Generic smart pointer
     *
     * This class is a generic definition of smart pointer. It
     * uses a memory manager to manage the pointers.
     *
     * @param Manager The memory manager
     *
     * @note @a Manager is used as a singleton using the @c s
     *
     * @author Frédéric Py <fpy@laas.fr>
     * @ingroup smart
     */
    template<class Manager>
    class pointer {
    private:
      singleton::use<Manager> manager;
      
    public:
      /** @brief Pointed type
       *
       * This is the type of the element pointed by this pointer
       */
      typedef typename Manager::base_type base_type;
      /** @brief basic pointer type
       *
       * This is the type of the classical C/C++ pointer for
       * @a base_type
       */
      typedef base_type *pointer_type;
      /** @brief Basic reference type
       *
       * This is the type of the classical C++ reference for
       * @a base_type
       */
      typedef base_type &reference_type;

      /** @brief Constructor
       *
       * @param ptr The basic pointer we want to manage
       * 
       * Create a new instance managing @a ptr
       *
       * @warning @a ptr is now managed by the pointer manager. Its life
       * time is controlled by this manager an user must not interfere
       * with it (do not manipulate @a ptr changing its pointed value
       * or deleting it !!!!
       */
      explicit pointer(pointer_type ptr=0);
      /** @brief Copy constructor
       * @param other The instance to copy.
       *
       * 
       * Create a new instance pôinting to the same cell as @a other.
       *
       * This will probably influence the life time of the pointed cell
       */
      pointer(pointer const &other);

      /** @brief Destructor
       *
       * This destructor informs the manager that the smart pointer
       * does not use enymore the stored pointer. It may have probably
       * a strong influence on the pointed cell life time.
       */
      ~pointer();

      /** @brief Checnging pointed value
       *
       * @param ptr The new pointer to manage
       *
       * This function release the old pointed value (ie it informa the
       * manager that this instnace no more use the pointed value) and
       * sets the new pointed value to @a ptr.
       *
       * @warning @a ptr is now managed by the pointer manager. Its life
       * time is controlled by this manager an user must not interfere
       * with it (do not manipulate @a ptr changing its pointed value
       * or deleting it !!!!
       *
       * @note This function informs the manager that this insatnce 
       * does not use enymore the previously stored pointer.
       * It may have probably a strong influence on the pointed cell
       * life time.
       */
      void reset(pointer_type ptr=0);

      /** @brief pointed cell exchange
       *
       * @param other another instance
       *
       * This function swaps the values of current instance and @a other.
       */
      void swap(pointer &other);
      
      /** @brief Assignement operator
       *
       * @param other The instance to assign
       *
       * Change the value of current instance to a value pointing to the
       * same cell as @a toher.
       *
       * @sa reset
       */
      pointer &operator= (pointer const &other);

      /** @brief Check if null
       *
       * @retval true if current insatnce is pointing to nothing
       * @retval false else
       *
       * @sa operator!()
       */
      bool null() const;
      /** @brief Check if null
       *
       * @retval true if current insatnce is pointing to nothing
       * @retval false else
       *
       * @sa null()
       */
      bool operator! () const;

      /** @brief Equality test
       *
       * @param other The instance to compare
       *
       * @retval true If current insatnce points to the samme cell as @a other
       * @retval false else
       *
       * @sa operator!=
       */
      bool operator==(pointer const &other) const; 
      /** @brief Difference test
       *
       * @param other The instance to compare
       *
       * @return @c !operator==(other)
       *
       * @sa operator==
       */
      bool operator!=(pointer const &other) const;

      /** @brief Access operator
       *
       * Thsi operator gives direct access to the attributes
       * of pointed value
       *
       * @throw utilmm::null_access This insatnce points to nothing
       *
       * @sa operator*()
       */
      pointer_type operator->() const;
      /** @brief dereference operator
       *
       * @return A reference to the pointed cell 
       * @throw utilmm::null_access This insatnce points to nothing
       *
       * @sa operator->()
       */
      reference_type operator* () const;

    private:
      typename Manager::mem_cell pointee;
    }; // class utilmm::smart::pointer<>

  } // namespace utilmm::smart
} // namespace utilmm

# define IN_UTILMM_SMART_POINTER_HEADER
#include "utilmm/smart/bits/pointer.tcc"
# undef IN_UTILMM_SMART_POINTER_HEADER
#endif // UTILMM_SMART_POINTER_HEADER
/** @file smart/pointer.hh
 * @brief Definition of utilmm::smart::pointer
 *
 * This header includes all the basis to define and manipulate smart pointers
 * in C++
 *
 * @author Frédéric Py <fpy@laas.fr>
 * @ingroup smart
 */

/** @defgroup smart Smart pointers
 * @brief Smart memory management with pseudo pointers
 *
 * This module embeds all the component used to make smart pointers. A smart
 * pointer is a class that overload the operators * and -> to behave like a
 * pointer. Generally it offers a solution to ease the memory management in
 * C++.
 *
 * @warning smart pointers are not the ultimate solution to manage memory
 * indeed genrally each smart pointer have a weakness and you have to know
 * exactly what this pseudo pointer do to check if it is usefull or not for
 * your program.
 */
