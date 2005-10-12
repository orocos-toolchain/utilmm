/* -*- C++ -*-
 * $Id: hash_set.hh 959 2005-03-11 13:19:19Z fpy $
 */
#ifndef UTILMM_HASH_SET_HEADER
# define UTILMM_HASH_SET_HEADER
#include "utilmm/config/config.h"

#include "utilmm/functional/utils.hh"

#include "utilmm/hash/bits/table.hh"

namespace utilmm {
  
  /** @brief set with hashing access
   *
   * This is an unsorted set where elements are accessed using a hash
   * function
   *
   * @param Key the element type
   * @param Hash hashing functor fo @a Key
   * @param Eqaul equality functor for @a Key
   *
   * @sa utilmm::hash
   *
   * @author Frédéric Py <fpy@laas.fr>
   * @ingroup hashing
   */
  template< typename Key, class Hash = hash<Key>, 
	    class Equal = std::equal_to<Key> >
  class hash_set {
  private:
    typedef identity<Key> key_extractor;
    typedef hash_toolbox::table< Key, Key const, key_extractor,
				 Hash, Equal > container_type;
    typedef typename container_type::key_arg key_arg;

    container_type the_table;
    
  public:
    /** @brief Value type for cells.
     *
     * @copydoc utilmm::hash_toolbox::table::value_type
     *
     * For @c hash_map @c value_type is @c Key
     */ 
    typedef typename container_type::value_type     value_type;
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
    void swap(hash_set &other) {
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

    /** @brief Search for element
     *
     * @param key the element to find.
     *
     * This function tries to search in table an element
     * equal to @a key
     *
     * @return An iterator pointing to the element with value @a key
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
     * @param key The value to insert
     * 
     * This function tries to insert @a key in current instance.
     * It will not insert it if there's already a cell equals to @a key
     *
     * @return An @c iterator pointing to cell with value @a key.
     */
    iterator insert(key_arg key) {
      return the_table.insert_unique(key).first;
    }

    /** @brief Remove range
     *
     * @copydoc utilmm::hash_toolbox::table::erase(iterator const&,iterator const&)
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
     * @param key the element to remove
     *
     * This function erase, if any, the element whose value is @a key
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
    
  }; // class utilmm::hash_set<>
  
} // namespace utilmm

#endif // UTILMM_HASH_SET_HEADER
/** @file hash/hash_set.hh
 * @brief Declaration of utilmm::hash_set
 *
 * This header defines the class @c utilmm::hash_set. 
 *
 * @author Frédéric Py <fpy@laas.fr>
 * @ingroup hashing
 */
