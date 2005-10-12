/* -*- C++ -*-
 * $Id: basic_symbol.tcc 961 2005-03-11 14:05:33Z fpy $
 */
#ifndef IN_UTILMM_TYPES_BASIC_SYMBOL_HEADER
# error "cannot include template files directly"
#else

namespace utilmm {

  /*
   * class utilmm::basic_symbol<>
   */
  // modifiers
  template<class CharT, class Traits, class Alloc>
  basic_symbol<CharT, Traits, Alloc> &basic_symbol<CharT, Traits, Alloc>::append
  (basic_symbol<CharT, Traits, Alloc> const &other) {
    if( empty() )
      return operator= (other);
    else if( !other.empty() )
      name = create(str()+other.str());
    return *this;
  }

  // observers
  template<class CharT, class Traits, class Alloc>
  bool basic_symbol<CharT, Traits, Alloc>::operator< 
    (basic_symbol<CharT, Traits, Alloc> const &other) const {
    return !other.empty() &&( empty() || *name<*other.name );
  }

  template<class CharT, class Traits, class Alloc>
  bool basic_symbol<CharT, Traits, Alloc>::starts_with
  (basic_symbol<CharT, Traits, Alloc> const &sub) const {
    if( operator==(sub) || sub.empty() )
      return true;
    else if( empty() )
      return false;
    else
      return name->find(*sub.name)==0;
  }

  
  // operations
  template<class CharT, class Traits, class Alloc>
  basic_symbol<CharT, Traits, Alloc> 
  basic_symbol<CharT, Traits, Alloc>::operator+ 
  (basic_symbol<CharT, Traits, Alloc> const &other) const {
    return basic_symbol(*this).append(other);
  }

  // statics
  template<class CharT, class Traits, class Alloc>
  typename basic_symbol<CharT, Traits, Alloc>::ref_type
  basic_symbol<CharT, Traits, Alloc>::create
  (typename basic_symbol<CharT, Traits, Alloc>::str_type const &str) {
    if( str.empty() )
      return ref_type();
    else 
      return ref_type(new str_type(str));
  }

  // related functions
  template<class CharT, class Traits, class Alloc>
  std::ostream &operator<< (std::ostream &out,
			    basic_symbol<CharT, Traits, Alloc> const &s) {
    if( !s.empty() )
      out<<s.str();
    return out;
  }

  /*
   * struct utilmm::hash< utilmm::basic_symbol<> >
   */
  template<class CharT, class Traits, class Alloc>
  size_t hash< basic_symbol<CharT, Traits, Alloc> >::operator()
    (basic_symbol<CharT, Traits, Alloc> const &x) const {
    hash<typename basic_symbol<CharT, Traits, Alloc>::ref_type> href;
    
    return href(x.name);
  }
  

} // namespace utilmm

#endif // IN_UTILMM_TYPES_BASIC_SYMBOL_HEADER
