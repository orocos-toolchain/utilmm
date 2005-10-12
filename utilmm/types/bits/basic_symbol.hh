/* -*- C++ -*-
 * $Id: basic_symbol.hh 961 2005-03-11 14:05:33Z fpy $
 */
#ifndef UTILMM_TYPES_BASIC_SYMBOL_HEADER
# define UTILMM_TYPES_BASIC_SYMBOL_HEADER

#include "utilmm/smart/uniq_pointer.hh"
#include "utilmm/hash/hash.hh"

#include "utilmm/types/bits/basic_symbol_fwd.hh"

namespace utilmm {
  
  /** @brief Generic string using unique instance memory
   *
   * This class implements constant string named symbols based on
   * @c utilmm::smart::uniq_pointer
   *
   * @param CharT The char type for the string
   * @param Traits The character traits for @a CharT
   * @param Alloc allocator for the string
   *
   * @author Frédéric Py <fpy@laas.fr>
   */
  template<class CharT, class Traits, class Alloc>
  class basic_symbol {
  public:
    /** @brief equivalent string type */
    typedef std::basic_string<CharT, Traits, Alloc> str_type;
    
  private:
    typedef typename smart::uniq_pointer<str_type>::type ref_type;

  public:
    /** @brief Constructor
     *
     * @param str name of the symbol
     *
     * Create a new symbol instance with name @a str
     */
    basic_symbol(str_type const &str = str_type())
      :name(create(str)) {}
    /** @brief Constructor
     *
     * @param str name of the symbol
     *
     * Create a new symbol instance with name @a str
     */
    basic_symbol(CharT const *str)
      :name(create(str_type(str))) {}
    /** @brief Copy constructor */
    basic_symbol(basic_symbol const &other)
      :name(other.name) {}

    /** @brief Destructor */
    ~basic_symbol() {}

    /** @brief Assignment operator */ 
    basic_symbol &operator= (basic_symbol const &other) {
      name = other.name;
      return *this;
    }

    /** @brief Check if empty
     *
     * This function test if this symbol is an empty one (ie it has no name)
     *
     * @retval true if symbol is empty
     * @retval false else
     */
    bool empty() const {
      return name.null();
    }
    /** @brief length of symbol
     *
     * @return length of symbol name
     */
    size_t length() const {
      return size_t(empty()?0:name->size());
    }

    /** @brief Equality test
     *
     * @param other a symbol
     *
     * @retval true if current symbol has the same name as @a other
     * @retval false else
     *
     * @note as @c basic_symbol is based on @a utilmm::smart::uniq_pointer
     * this test is made in constant time
     */
    bool operator==(basic_symbol const &other) const {
      return name==other.name;
    }
    /** @brief Difference test
     *
     * @param other a symbol
     *
     * @return @c !operator==(other)
     */
    bool operator!=(basic_symbol const &other) const {
      return !operator==(other);
    }

    /** @brief Ordering operator */
    bool operator< (basic_symbol const &other) const;
    /** @brief Ordering operator */
    bool operator> (basic_symbol const &other) const {
      return other.operator< (*this);
    }
    /** @brief Ordering operator */
    bool operator<=(basic_symbol const &other) const {
      return !operator> (other);
    }
    /** @brief Ordering operator */
    bool operator>=(basic_symbol const &other) const {
      return !operator< (other);
    }

    /** @brief Check for beginning
     *
     * @param sub a symbol
     *
     * @retval true if the name of current insatnce starts with
     * the name of @a sub
     * @retval false else
     */
    bool starts_with(basic_symbol const &sub) const;

    /** @brief Append function
     *
     * @param other A symbol
     *
     * Thsi function appends to the end of current symbol
     * name the name of @a other
     *
     * @return current instance after operation
     *
     * @sa operator+=(basic_symbol const&)
     * @sa operator+(basic_symbol const&)
     */
    basic_symbol &append(basic_symbol const &other);
    /** @brief Append operator
     *
     * This is an alias for @c append
     */
    basic_symbol &operator+=(basic_symbol const &other) {
      return append(other);
    }

    /** @brief Append operation
     *
     * @param other a symbol
     * 
     * Thsi function creat a new symbol with a name that is the
     * concatenation of the name of current insdtance and the name
     * of @a other
     */
    basic_symbol operator+ (basic_symbol const &other) const;

    /** @brief Name of the symbol
     *
     * @return The name of the symbol
     *
     * @throw utilmm::smart::null_access The name of the
     * symbol is empty
     */
    str_type const &str() const {
      return *name;
    }

  private:
    ref_type name;

    static ref_type create(str_type const &str);

    template<typename Ty>
    friend struct hash;
  }; // class utilmm::basic_symbol<>

  template<class CharT, class Traits, class Alloc>
  struct hash< basic_symbol<CharT, Traits, Alloc> >
    :public std::unary_function< basic_symbol<CharT, Traits, Alloc>,
				 size_t > {
    size_t operator()(basic_symbol<CharT, Traits, Alloc> const &x) const;
  }; // struct utilmm::hash< utilmm::basic_symbol<> >
  
} // namespace utilmm

# define IN_UTILMM_TYPES_BASIC_SYMBOL_HEADER
#include "utilmm/types/bits/basic_symbol.tcc"
# undef IN_UTILMM_TYPES_BASIC_SYMBOL_HEADER
#endif // UTILMM_TYPES_BASIC_SYMBOL_HEADER
/** @file types/bits/basic_symbol.hh
 * @brief Defintion of utilmm::basic_symbol
 */
