/* -*- C++ -*-
 * $Id: iter.tcc 961 2005-03-11 14:05:33Z fpy $
 */
#ifndef IN_UTILMM_UTILS_HASH_ITER_HEADER
# error "Cannot include template files directly."
#else

namespace utilmm {
  namespace hash_toolbox {

    /*
     * class utilmm::hash_toolbox::const_iter<>
     */
    // structors 
    template<typename K, typename V, class Ex, class H, class Eq>
    const_iter<K, V, Ex, H, Eq>::const_iter()
      :owner(0), current(0) {}

    template<typename K, typename V, class Ex, class H, class Eq>
    const_iter<K, V, Ex, H, Eq>::const_iter(iter<K, V, Ex, H, Eq> const &other)
      :owner(other.owner), current(other.current) {}

    template<typename K, typename V, class Ex, class H, class Eq>
    const_iter<K, V, Ex, H, Eq>::const_iter
    (typename const_iter<K, V, Ex, H, Eq>::node_type const *node,
     typename const_iter<K, V, Ex, H, Eq>::container_type const *creator)
      :owner(creator), current(node) {}
    
    // modifiers 
    template<typename K, typename V, class Ex, class H, class Eq>
    const_iter<K, V, Ex, H, Eq> &const_iter<K, V, Ex, H, Eq>::operator+=
    (typename const_iter<K, V, Ex, H, Eq>::size_type delta) {
      size_t pos = owner->hash_node(current->val)+1;
      
      while( 0!=current && 0<delta ) {
	current = current->next;

	while( 0==current && pos<owner->bucket.size() ) {
	  current = owner->bucket[pos];
	  pos += 1;
	}
	delta -= 1;
      }
      return *this;
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    const_iter<K, V, Ex, H, Eq> &const_iter<K, V, Ex, H, Eq>::operator++() {
      return operator+=(1);
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    const_iter<K, V, Ex, H, Eq> const_iter<K, V, Ex, H, Eq>::operator++(int) {
      const_iter tmp(*this);
      
      operator++();
      return tmp;
    }

    // operations
    template<typename K, typename V, class Ex, class H, class Eq>
    const_iter<K, V, Ex, H, Eq> const_iter<K, V, Ex, H, Eq>::operator+
    (typename const_iter<K, V, Ex, H, Eq>::size_type delta) const {
      return const_iter(*this).operator+=(delta);
    }

    // observers 
    template<typename K, typename V, class Ex, class H, class Eq>
    bool const_iter<K, V, Ex, H, Eq>::operator==
    (const_iter<K, V, Ex, H, Eq> const &other) const {
      return current==other.current;
    }
    
    template<typename K, typename V, class Ex, class H, class Eq>
    bool const_iter<K, V, Ex, H, Eq>::operator!=
    (const_iter<K, V, Ex, H, Eq> const &other) const {
      return !operator==(other);
    }
    

    template<typename K, typename V, class Ex, class H, class Eq>
    typename const_iter<K, V, Ex, H, Eq>::reference 
    const_iter<K, V, Ex, H, Eq>::operator* () const {
      return current->value;
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    typename const_iter<K, V, Ex, H, Eq>::pointer
    const_iter<K, V, Ex, H, Eq>::operator->() const {
      return &operator* ();
    }

    /*
     * class utilmm::hash_toolbox::iter<>
     */
    // structors 
    template<typename K, typename V, class Ex, class H, class Eq>
    iter<K, V, Ex, H, Eq>::iter()
      :owner(0), current(0) {}

    template<typename K, typename V, class Ex, class H, class Eq>
    iter<K, V, Ex, H, Eq>::iter
    (typename iter<K, V, Ex, H, Eq>::node_type *node,
     typename iter<K, V, Ex, H, Eq>::container_type *creator)
      :owner(creator), current(node) {}
    
    // modifiers 
    template<typename K, typename V, class Ex, class H, class Eq>
    iter<K, V, Ex, H, Eq> &iter<K, V, Ex, H, Eq>::operator+=
    (typename iter<K, V, Ex, H, Eq>::size_type delta) {
      size_t pos = owner->hash_node(current->val)+1;
      
      while( 0!=current && 0<delta ) {
	current = current->next;

	while( 0==current && pos<owner->bucket.size() ) {
	  current = owner->bucket[pos];
	  pos += 1;
	}
	delta -= 1;
      }
      return *this;
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    iter<K, V, Ex, H, Eq> &iter<K, V, Ex, H, Eq>::operator++() {
      return operator+=(1);
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    iter<K, V, Ex, H, Eq> iter<K, V, Ex, H, Eq>::operator++(int) {
      iter tmp(*this);
      
      operator++();
      return tmp;
    }

    // operations
    template<typename K, typename V, class Ex, class H, class Eq>
    iter<K, V, Ex, H, Eq> iter<K, V, Ex, H, Eq>::operator+
    (typename iter<K, V, Ex, H, Eq>::size_type delta) const {
      return iter(*this).operator+=(delta);
    }

    // observers 
    template<typename K, typename V, class Ex, class H, class Eq>
    bool iter<K, V, Ex, H, Eq>::operator==(iter<K, V, Ex, H, Eq> const &other)
      const {
      return current==other.current;
    }
    
    template<typename K, typename V, class Ex, class H, class Eq>
    bool iter<K, V, Ex, H, Eq>::operator!=(iter<K, V, Ex, H, Eq> const &other)
      const {
      return !operator==(other);
    }
    
    template<typename K, typename V, class Ex, class H, class Eq>
    typename iter<K, V, Ex, H, Eq>::reference 
    iter<K, V, Ex, H, Eq>::operator* () const {
      return current->val;
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    typename iter<K, V, Ex, H, Eq>::pointer
    iter<K, V, Ex, H, Eq>::operator->() const {
      return &operator* ();
    }

  } // namespace utilmm::hash_toolbox
} // namespace utilmm

#endif // IN_UTILMM_UTILS_HASH_ITER_HEADER
