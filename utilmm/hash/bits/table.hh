/* -*- C++ -*-
 * $Id: table.hh 957 2005-03-07 16:01:20Z sjoyeux $
 */
#ifndef UTILMM_UTILS_HASH_TABLE_HEADER
# define UTILMM_UTILS_HASH_TABLE_HEADER

# include <utility> 
# include <vector>

#include "utilmm/functional/arg_traits.hh"

#include "utilmm/hash/hash_fwd.hh"
#include "utilmm/hash/bits/iter.hh"

namespace utilmm {
  namespace hash_toolbox {

    template<typename Value>
    struct node {
      explicit node(Value const &v, node *n=0);

      Value val;
      node *next;
    }; // struct utilmm::hash_toolbox::node<>
    
    /** @brief Hashing based table
     *
     * This class is used as internal basis for all hashing based
     * containers presented here. It is mainly insdpired on SGI STL
     * implementation
     *
     * @param Key the entry type for this table
     * @param Value The value type for cells in table
     * @param Extract @a Key extractor from @a Value
     * @param Hash hashing functor for @a Key
     * @param Equal equality functor for @a Key
     *
     * @sa utilmm::hash
     *
     * @author Frédéric Py <fpy@laas.fr>
     * @ingroup hashing
     * @ingroup intern
     */
    template<typename Key, typename Value, 
	     class Extract, class Hash, class Equal>
    class table {
    public:
      /** @brief Value type for cells */
      typedef Value  value_type;
      /** @brief Key type */
      typedef Key    key_type;
      /** @brief Size type */
      typedef size_t size_type;
      
      typedef typename arg_traits<value_type>::type value_arg;
      typedef typename arg_traits<key_type>::type key_arg;

    private:
      typedef node<value_type> node_type;
      typedef std::vector<node_type *> bucket_type;

    public:
      /** @brief iterator type
       *
       * The type used to iterate through and manipulate this class
       */
      typedef iter<Key, Value, Extract, Hash, Equal>       iterator;
      /** @brief const iterator type
       *
       * The type used to iterate through this class without any
       * modification
       */
      typedef const_iter<Key, Value, Extract, Hash, Equal> const_iterator;
      
      /** @brief Default constructor
       *
       * Create an empty table
       */
      table();
      /** @brief Copy constructor */
      table(table const &);
      
      /** @brief Copy destructor */
      ~table();

      /** @brief swapping values function
       *
       * This function exchange the value of current instance with
       * the value of another instance.
       *
       * @param other The other instance
       */
      void swap(table &other);
      /** @brief Copy operator */
      table &operator= (table const &other);
      
      /** @brief element count
       *
       * @return the number of element present in
       * current container
       */
      size_type size() const;
      /** @brief max elmement number
       *
       * @return The maximum possible number of elements
       */
      size_type max_size() const;

      /** @brief Emptyness test
       *
       * @retval true if this instance has no element
       * @retval false else
       *
       * @sa size() const
       */
      bool empty() const;

      /** @brief Beginning of table
       *
       * @return an iterator pointing to the beginning of
       * the container
       *
       * @sa end()
       */
      iterator begin();
      /** @brief End of table
       *
       * @return an iterator pointing to the end of
       * the container
       *
       * @sa begin()
       */
      iterator end();

      /** @brief Beginning of table
       *
       * @return a const iterator pointing to the beginning of
       * the container
       *
       * @sa end() const
       */
      const_iterator begin() const;
      /** @brief End of table
       *
       * @return a const iterator pointing to the end of
       * the container
       *
       * @sa begin() const
       */
      const_iterator end() const;

      /** @brief equality range
       *
       * @param key a @c Key 
       * 
       * This function tries to fuind all the elements of this table
       * with a key equal to @a key
       *
       * @return a pair where [first, second[ corresponds to all the
       * elements whose key is equal to @a key
       */
      std::pair<iterator, iterator> equal_range(key_arg key);
      /** @brief equality range.
       * 
       * @copydoc equal_range(key_arg key)
       */
      std::pair< const_iterator,
		 const_iterator > equal_range(key_arg key) const;

      /** @brief Remove elements
       *
       * @param first an iterator
       * @param last an iterator
       *
       * removes all the element of the table in the range [@a first, @a last [
       */
      void erase(iterator const &first, iterator const &last);

      /** @brief Unique key insertion
       *
       * @param v The value to insert
       *
       * This function tries to insert @a v in the table except if there's
       * already an element whose keys is equal to the key of @a v
       *
       * @return a pair where @c first is an iterator pointing to the
       * element whose key is equal to the key of @a v and @c second is
       * true if we have inserted @a v
       */
      std::pair<iterator, bool> insert_unique(value_arg v);
      /** @brief multiple insertion
       *
       * param v The value to insert
       *
       * Thsi function insert @a v in table
       *
       * @return an iterator pointing to the position where @a v was
       * inserted
       */
      iterator insert_multiple(value_arg v);

      /** @brief remove all elements
       *
       * This function is strictly equivelent to @c erase(begin(), end())
       */
      void clear();

    private:
      bucket_type bucket;
      size_type   node_count, avg_bucket_count;
      
      void resize(size_type size);
      
      node_type *insert(node_type **helper, value_arg v);
      
      static size_t hash_key(key_arg k, size_type mod);
      static size_t hash_node(value_arg v, size_type mod);
      static key_type const &get_key(value_arg v);

      static bucket_type copy_bucket(bucket_type const &other);

      size_t hash_node(value_arg v);
      
      node_type **find_node(key_arg k);
      node_type *find_node(key_arg k) const;

      template<typename K, typename V, class Ex, class H, class Eq>
      friend class iter;
      
      template<typename K, typename V, class Ex, class H, class Eq>
      friend class const_iter;
    }; // class utilmm::hash_toolbox::table<>

  } // namespace utilmm::hash_toolbox
} // namespace utilmm

# define IN_UTILMM_UTILS_HASH_TABLE_HEADER
#include "utilmm/hash/bits/table.tcc"
# undef IN_UTILMM_UTILS_HASH_TABLE_HEADER
#endif // UTILMM_UTILS_HASH_TABLE_HEADER

/** @file hash/bits/table.hh
 * @brief Declaration of utilmm::hash_toolbox::table
 *
 * This header defines the class @c utilmm::hash_toolbox::table. This is the
 * base for all hashing based containers.
 *
 * @author Frédéric Py <fpy@laas.fr>
 * @ingroup hashing
 * @ingroup intern
 */
