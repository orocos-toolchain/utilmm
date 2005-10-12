/* -*- C++ -*-
 * $Id: hash_map.hh 957 2005-03-07 16:01:20Z sjoyeux $
 */
#ifndef UTILMM_HASH_MAP_HEADER
# define UTILMM_HASH_MAP_HEADER
#include "utilmm/config/config.h"

#include "utilmm/functional/utils.hh"

#include "utilmm/hash/bits/table.hh"

namespace utilmm {
  
  /** @brief map with hashing access
   *
   * This is an unsorted map where keys are accessed using a hash
   * function
   *
   * @param Key the key
   * @param Data The data associated to @a Key
   * @param Hash hashing functor fo @a Key
   * @param Eqaul equality functor for @a Key
   *
   * @sa utilmm::hash
   *
   * @author Frédéric Py <fpy@laas.fr>
   * @ingroup hashing
   */
  template< typename Key, typename Data, class Hash = hash<Key>, 
	    class Equal = std::equal_to<Key> >
  class hash_map {
  public:
    /** @brief Key type */ 
    typedef Key  key_type;
    /** @brief Data type
     *
     * This is the type of the data asociated to the key
     */ 
    typedef Data data_type;
    /** @brief Value type for cells.
     *
     * @copydoc utilmm::hash_toolbox::table::value_type
     *
     * For @c hash_map @c value_type is a @c std::pair containing the
     * key and the associated data.
     */ 
    typedef std::pair<Key const, Data> value_type;

  private:
    typedef hash_toolbox::table<key_type, value_type, select_1st<value_type>,
				Hash, Equal> container_type;
    typedef typename container_type::value_arg value_arg;
    typedef typename container_type::key_arg key_arg;

    container_type the_table;
    
  public:
    /** @brief iterator type
     * 
     * @copydoc utilmm::hash_toolbox::table::iterator 
     */
    typedef typename container_type::iterator       iterator;
    /** @brief const iterator type
     * 
     * @copydoc utilmm::hash_toolbox::table::const_iterator
     */
    typedef typename container_type::const_iterator const_iterator;

    /** @brief swapping function
     *
     * @copydoc utilmm::hash_toolbox::table::swap
     */
    void swap(hash_map &other) {
      the_table.swap(other.the_table);
    }

    /** @brief element count
     * 
     * @copydoc utilmm::hash_toolbox::table::size
     */
    size_t size() const {
      return the_table.size();
    }
    /** @brief max element number
     * 
     * @copydoc utilmm::hash_toolbox::table::max_size
     */
    size_t max_size() const {
      return the_table.max_size();
    }
    
    /** @brief Emptyness test
     * 
     * @copydoc utilmm::hash_toolbox::table::empty
     */
    bool empty() const {
      return the_table.empty();
    }

    /** @brief beginning of table
     * 
     * @copydoc utilmm::hash_toolbox::table::begin()
     */
    iterator begin() {
      return the_table.begin();
    }
    /** @brief end of table
     * 
     * @copydoc utilmm::hash_toolbox::table::end()
     */
    iterator end() {
      return the_table.end();
    }
    /** @brief beginning of table
     * 
     * @copydoc utilmm::hash_toolbox::table::begin() const
     */
    const_iterator begin() const {
      return the_table.begin();
    }
    /** @brief end of table
     * 
     * @copydoc utilmm::hash_toolbox::table::end()
     */
    const_iterator end() const {
      return the_table.end();
    }
    
    /** @brief Search for key
     *
     * @param key the key to find.
     *
     * This function tries to search in table an element
     * with key @a key
     *
     * @return An iterator pointing to the element with key @a key
     * or @c end() if not found.
     */
    iterator find(key_arg key) {
      return the_table.equal_range(key).first;
    }
    /** @brief Search key.
     *
     * @copydoc find(key_arg)
     */
    const_iterator find(key_arg key) const {
      return the_table.equal_range(key).first;
    }

    /** @brief Cell insertion
     *
     * @param val The value to insert
     * 
     * This function tries to insert @a val in current instance.
     * It will not insert it if ther's already a cell with the same key
     * as @a val
     *
     * @return A pair where @c first is an @c iterator pointing to
     * cell with the same key as @a val and @c second is true if and
     * only if @c val was inserted.
     */
    std::pair<iterator, bool> insert(value_arg val) {
      return the_table.insert_unique(val);
    }

    /** @brief Remove range
     *
     * @copydoc utilmm::hash_toolbox::table::erase(iterator const&,iterator const &)
     */
    void erase(iterator const &first, iterator const &last) {
      the_table.erase(first, last);
    }
    /** @brief Remove element
     *
     * @param i The lement to remove
     *
     * This function removes the element pointed by @a i form the table
     */
    void erase(iterator const &i) {
      if( end()!=i )
        erase(i, i+1);
    }
    /** @brief remove element
     *
     * @param key the key of the element
     *
     * This function erase, if any, the element whose key is @a key
     */
    void erase(key_arg key) {
      erase(find(key));
    }
    
    /** @brief Remove all elements
     *
     * @copydoc utilmm::hash_toolbox::table::clear
     */
    void clear() {
      the_table.clear();
    }
    
  }; // class utilmm::hash_map<>

} // namespace utilmm

#endif // UTILMM_HASH_MAP_HEADER
/** @file hash/hash_map.hh
 * @brief Declaration of utilmm::hash_map
 *
 * This header defines the class @c utilmm::hash_map. 
 *
 * @author Frédéric Py <fpy@laas.fr>
 * @ingroup hashing
 */
