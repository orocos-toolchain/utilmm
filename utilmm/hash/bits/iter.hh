/* -*- C++ -*-
 * $Id: iter.hh 957 2005-03-07 16:01:20Z sjoyeux $
 */
#ifndef UTILMM_UTILS_HASH_ITER_HEADER
# define UTILMM_UTILS_HASH_ITER_HEADER

# include "utilmm/hash/bits/table_fwd.hh"

namespace utilmm {
  /** @brief toolbox for hash containers
   *
   * This namespace contains all the classes used to implement hash
   * based containers. It is the template private part of the
   * implementation and final user does not need to know the compoents
   * in this namespace
   *
   * @ingroup hashing
   * @ingroup intern
   */
  namespace hash_toolbox {
    
    template< typename Key, typename Value, class Extract,
	      class Hash, class Equal >
    class iter;

    /** @brief const iterator for @c table
     *
     * This class implements a const iterator for 
     * @c utilmm::hash_toolbox::table. A const iterator is an
     * iterator with guarantees that it will never modify the
     * container it points to.
     *
     * @author Frédéric Py <fpy@laas.fr>
     * @ingroup hashing
     */
    template< typename Key, typename Value, class Extract,
	      class Hash, class Equal >
    class const_iter {
    public:
      /** @brief Type of the pointed elements
       *
       * This the type of the elements in the container
       */
      typedef Value value_type;
      /** @brief Pointer type  
       *
       * This is the type of the pointer used to access to @c Value.
       * For cont iterator it is a const pointer
       */
      typedef value_type const *pointer; 
      /** @brief Reference type  
       *
       * This is the type of the reference used to access to @c Value.
       * For cont iterator it is a const reference
       */
      typedef value_type const &reference;
      /** @brief Size type  
       *
       * This is the type used to advance the iterator or other
       * things like that. 
       */
      typedef size_t size_type;

      /** @brief default crontructor
       *
       * Create an iterator not attached to any table
       */
      const_iter();
      /** @brief Convertion constructor
       *
       * This constructor is used to make implicit conversion
       * from non cont iterator to const iterator when needed.
       * 
       * @param other the instance to copy
       */
      const_iter(iter<Key, Value, Extract, Hash, Equal> const &other);

      /** @brief Equality test
       *
       * @param other The instance to compare
       *
       * @retval true if @c *this is equal to @a other
       * @retval false else
       */
      bool operator==(const_iter const &other) const;
      /** @brief Difference test
       *
       * @param other The instance to compare
       *
       * @retval true if @c *this is not equal to @a other
       * @retval false else
       */
      bool operator!=(const_iter const &other) const;

      /** @brief Advance operation
       *
       * @param delta distance to advance
       * 
       * This function increments the iterator position of
       * the given distance @a delta .
       *
       * @return @c *this after operation
       */
      const_iter &operator+=(size_type delta);
      /** @brief Addition operation
       *
       * @param delta distance to add
       * 
       * This function returns an iterator @a delta
       * cells after current instance 
       *
       * @return the itrerator at @a delta
       */
      const_iter operator+ (size_type delta) const;

      /** @brief Pre-incremant operation
       *
       * Advance iterator of one cell
       *
       * @return the iterator after operation.
       */
      const_iter &operator++();
      /** @brief Post-increment operation
       *
       * Advance iterator of one cell
       *
       * @return value of the iterator just before the operation.
       */
      const_iter operator++(int);
      
      /** @brief access operator
       *
       * This operator allow user to acces directly to attributes
       * of pointed cell.
       *
       * @return a pointer to the cell
       */
      pointer operator->() const;
      /** @brief dereference operator
       *
       * @return a refence to pointed cell
       */
      reference operator* () const;

    private:
      typedef table<Key, Value, Extract, Hash, Equal> container_type;
      typedef typename container_type::node_type node_type;

      container_type const *owner;
      node_type const *current;

      const_iter(node_type const *, container_type const *);

      template<typename K, typename V, class Ex, class H, class Eq> 
      friend class table;
    }; // class utilmm::hash_toolbox::const_iter<>

    template<typename K, typename V, class Ex, class H, class Eq>
    const_iter<K, V, Ex, H, Eq> operator+ 
    (typename const_iter<K, V, Ex, H, Eq>::size_type d, 
     const_iter<K, V, Ex, H, Eq> const &i) {
      return i+d;
    }

    /** @brief iterator for @c table
     *
     * This class implements a iterator for 
     * @c utilmm::hash_toolbox::table. 
     *
     * @author Frédéric Py <fpy@laas.fr>
     * @ingroup hashing
     */
    template< typename Key, typename Value, class Extract,
	      class Hash, class Equal >
    class iter {
    public:
      /** @brief Type of the pointed elements
       *
       * This the type of the elements in the container
       */
      typedef Value value_type;
      /** @brief Pointer type  
       *
       * This is the type of the pointer used to access to @c Value.
       * For cont iterator it is a const pointer
       */
      typedef value_type *pointer;
      /** @brief Reference type  
       *
       * This is the type of the reference used to access to @c Value.
       */
      typedef value_type &reference;
      /** @brief Size type  
       *
       * This is the type used to advance the iterator or other
       * things like that. 
       */
      typedef size_t size_type;

      /** @brief default crontructor
       *
       * Create an iterator not attached to any table
       */
      iter();

      /** @brief Equality test
       *
       * @param other The instance to compare
       *
       * @retval true if @c *this is equal to @a other
       * @retval false else
       */
      bool operator==(iter const &other) const;
      /** @brief Difference test
       *
       * @param other The instance to compare
       *
       * @retval true if @c *this is not equal to @a other
       * @retval false else
       */
      bool operator!=(iter const &other) const;

      /** @brief Advance operation
       *
       * @param pos distance to advance
       * 
       * This function increments the iterator position of
       * the given distance @a pos .
       *
       * @return @c *this after operation
       */
      iter &operator+=(size_type pos);
      /** @brief Addition operation
       *
       * @param pos distance to add
       * 
       * This function returns an iterator @a pos
       * cells after current instance 
       *
       * @return the itrerator at @a pos
       */
      iter operator+ (size_type pos) const;

      /** @brief Pre-incremant operation
       *
       * Advance iterator of one cell
       *
       * @return the iterator after operation.
       */
      iter &operator++();
      /** @brief Post-increment operation
       *
       * Advance iterator of one cell
       *
       * @return value of the iterator just before the operation.
       */
      iter operator++(int);
      
      /** @brief access operator
       *
       * This operator allow user to acces directly to attributes
       * of pointed cell.
       *
       * @return a pointer to the cell
       */
      pointer operator->() const;
      /** @brief dereference operator
       *
       * @return a refence to pointed cell
       */
      reference operator* () const;

    private:
      typedef table<Key, Value, Extract, Hash, Equal> container_type;
      typedef typename container_type::node_type node_type;

      container_type *owner;
      node_type *current;

      iter(node_type *, container_type *);

      template<typename K, typename V, class Ex, class H, class Eq> 
      friend class const_iter;

      template<typename K, typename V, class Ex, class H, class Eq> 
      friend class table;
    }; // class utilmm::hash_toolbox::iter<>

    template<typename K, typename V, class Ex, class H, class Eq>
    iter<K, V, Ex, H, Eq> operator+ 
    (typename iter<K, V, Ex, H, Eq>::size_type d, 
     iter<K, V, Ex, H, Eq> const &i) {
      return i+d;
    }

  } // namespace utilmm::hash_toolbox
} // namespace utilmm

# define IN_UTILMM_UTILS_HASH_ITER_HEADER
#  include "utilmm/hash/bits/iter.tcc"
# undef IN_UTILMM_UTILS_HASH_ITER_HEADER
#endif // UTILMM_UTILS_HASH_ITER_HEADER

/** @file hash/bits/iter.hh
 * @brief Definition of iterator for hash containers
 *
 * This header defines classes used to implement
 * iterators for @c utilmm::hash_toolbox::table
 *
 * @author Frédéric Py <fpy@laas.fr>
 * @ingroup hashing
 * @ingroup intern
 */
