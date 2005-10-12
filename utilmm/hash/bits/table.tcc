/* -*- C++ -*-
 * $Id: table.tcc 959 2005-03-11 13:19:19Z fpy $
 */
#ifndef IN_UTILMM_UTILS_HASH_TABLE_HEADER
# error "Cannot include template files directly"
#else

# include <limits>

namespace utilmm {
  namespace hash_toolbox {

    /*
     * struct utilmm::hash_toolbox::node<>
     */
    template<typename Value>
    node<Value>::node(Value const &v, node<Value> *n)
      :val(v), next(n) {}

    /*
     * class utilmm::hash_toolbox::table<>
     */
    // structors
    template<typename K, typename V, class Ex, class H, class Eq>
    table<K, V, Ex, H, Eq>::table()
      :node_count(0ul), avg_bucket_count(64ul) {}

    template<typename K, typename V, class Ex, class H, class Eq>
    table<K, V, Ex, H, Eq>::table(table<K, V, Ex, H, Eq> const &other)
      :bucket(copy_bucket(other.bucket)),
       node_count(other.node_count),
       avg_bucket_count(other.avg_bucket_count) {}

    template<typename K, typename V, class Ex, class H, class Eq>
    table<K, V, Ex, H, Eq>::~table() {
      clear();
    }

    // modifiers
    template<typename K, typename V, class Ex, class H, class Eq>
    void table<K, V, Ex, H, Eq>::swap(table<K, V, Ex, H, Eq> &other) {
      bucket.swap(other.bucket);
      std::swap(node_count, other.node_count);
      std::swap(avg_bucket_count, other.avg_bucket_count);
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    table<K, V, Ex, H, Eq> &table<K, V, Ex, H, Eq>::operator=
    (table<K, V, Ex, H, Eq> const &other) {
      table tmp(other);
      swap(tmp);
      return *this;
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    void table<K, V, Ex, H, Eq>::erase
    (typename table<K, V, Ex, H, Eq>::iterator const &first,
     typename table<K, V, Ex, H, Eq>::iterator const &last) {
      if( first!=last ) {
	size_t hval = hash_node(*first);
	node_type **iter = find_node(get_key(*first));
	size_t removed = 0;

	while( last.current!=*iter ) {
	  node_type *tmp = *iter;
	  
	  *iter = tmp->next;
	  delete tmp;
	  ++removed;
	  while( 0==*iter && hval+1<bucket.size() )
	    iter = &bucket[++hval];
	}
	resize(node_count-removed);
      }
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    std::pair<typename table<K, V, Ex, H, Eq>::iterator, bool>
    table<K, V, Ex, H, Eq>::insert_unique
    (typename table<K, V, Ex, H, Eq>::value_arg v) {
      node_type **pos = find_node(get_key(v));
      
      if( 0!=*pos )
	return std::make_pair(iterator(*pos, this), false);
      else
	return std::make_pair(iterator(insert(pos, v), this), true);
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    typename table<K, V, Ex, H, Eq>::iterator 
    table<K, V, Ex, H, Eq>::insert_multiple
    (typename table<K, V, Ex, H, Eq>::value_arg v) {
      node_type **pos = find_node(get_key(v));

      return iterator(insert(pos, v), this);
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    void table<K, V, Ex, H, Eq>::clear() {
      typename bucket_type::iterator i = bucket.begin(),
	endi = bucket.end();
      
      for(; endi!=i; ++i )
	while( 0!=*i ) {
	  node_type *to_del = *i;
	  
	  *i = to_del->next;
	  delete to_del;
	}
      resize(0);
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    void table<K, V, Ex, H, Eq>::resize
    (typename table<K, V, Ex, H, Eq>::size_type size) {
      if( 0==size ) {
	bucket_type tmp(1, (node_type *)(0));
	
	bucket.swap(tmp);
      } else {
	size_t new_bcount = 1+(size/avg_bucket_count), hval;
	Eq eq;
      
	if( new_bcount>bucket.size() ) {
	  bucket_type tmp(new_bcount, (node_type *)(0));
	  typename bucket_type::iterator i, endi;
	  
	  for( i=bucket.begin(), endi=bucket.end(); endi!=i; ++i ) {
	    while( 0!=*i ) {
	      node_type *n = *i, *beg = n;
	      
	      while( 0!=n->next 
		     && eq(get_key((*i)->val), get_key(n->next->val)) )
		n = n->next;
	      *i = n->next;
	      hval = hash_node(beg->val, new_bcount);
	      n->next = tmp[hval];
	      tmp[hval] = beg;
	    }
	  }
	  bucket.swap(tmp);
	}
      }
      node_count = size;
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    typename table<K, V, Ex, H, Eq>::node_type *table<K, V, Ex, H, Eq>::insert
    (typename table<K, V, Ex, H, Eq>::node_type **position, 
     typename table<K, V, Ex, H, Eq>::value_arg v) {
      node_type *new_node = new node_type(v, *position);

      *position = new_node;
      resize(node_count+1);
      return *position;
    }

    // observers
    template<typename K, typename V, class Ex, class H, class Eq>
    typename table<K, V, Ex, H, Eq>::size_type table<K, V, Ex, H, Eq>::size()
      const {
      return node_count;
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    typename table<K, V, Ex, H, Eq>::size_type 
    table<K, V, Ex, H, Eq>::max_size() const {
      return std::numeric_limits<size_type>::max();
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    bool table<K, V, Ex, H, Eq>::empty() const {
      return size()==0;
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    typename table<K, V, Ex, H, Eq>::iterator table<K, V, Ex, H, Eq>::begin() {
      typename bucket_type::iterator i = bucket.begin(),
	endi = bucket.end();

      while( endi!=i && 0!=*i )
	++i;
      return iterator(*i, this);
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    typename table<K, V, Ex, H, Eq>::iterator table<K, V, Ex, H, Eq>::end() {
      return iterator(0, this);
    }
    
    template<typename K, typename V, class Ex, class H, class Eq>
    typename table<K, V, Ex, H, Eq>::const_iterator 
    table<K, V, Ex, H, Eq>::begin() const {
      typename bucket_type::const_iterator i = bucket.begin(),
	endi = bucket.end();

      while( endi!=i && 0!=*i )
	++i;
      return const_iterator(*i, this);
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    typename table<K, V, Ex, H, Eq>::const_iterator 
    table<K, V, Ex, H, Eq>::end() const {
      return const_iterator(0, this);
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    std::pair< typename table<K, V, Ex, H, Eq>::iterator,
	       typename table<K, V, Ex, H, Eq>::iterator >
    table<K, V, Ex, H, Eq>::equal_range
    (typename table<K, V, Ex, H, Eq>::key_arg key) {
      iterator start(*find_node(key), this), stop = start;
      Eq eq;
      
      for(; stop!=end() && eq(get_key(*stop), key); ++stop);
      return std::make_pair(start, stop);
    }
    
    template<typename K, typename V, class Ex, class H, class Eq>
    std::pair< typename table<K, V, Ex, H, Eq>::const_iterator,
	       typename table<K, V, Ex, H, Eq>::const_iterator >
    table<K, V, Ex, H, Eq>::equal_range
    (typename table<K, V, Ex, H, Eq>::key_arg key) const {
      const_iterator start(find_node(key), this), stop = start;
      Eq eq;
      
      for(; stop!=end() && eq(get_key(*stop), key); ++stop);
      return std::make_pair(start, stop);
    }
    
    template<typename K, typename V, class Ex, class H, class Eq>
    size_t table<K, V, Ex, H, Eq>::hash_node
    (typename table<K, V, Ex, H, Eq>::value_arg v) {
      return hash_node(v, bucket.size());
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    typename table<K, V, Ex, H, Eq>::node_type **
    table<K, V, Ex, H, Eq>::find_node
    (typename table<K, V, Ex, H, Eq>::key_arg key) {
      node_type **res = &bucket[hash_key(key, bucket.size())];
      Eq eq;

      while( 0!=*res && !eq(key, get_key((*res)->val)) )
	res = &((*res)->next);
      return res;
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    typename table<K, V, Ex, H, Eq>::node_type *
    table<K, V, Ex, H, Eq>::find_node
    (typename table<K, V, Ex, H, Eq>::key_arg key) const {
      node_type *res = bucket[hash_key(key, bucket.size())];
      Eq eq;

      while( 0!=res && !eq(key, get_key(res->val)) )
	res = *res->next;
      return res;
    }

    // statics
    template<typename K, typename V, class Ex, class H, class Eq>
    size_t table<K, V, Ex, H, Eq>::hash_key
    (typename table<K, V, Ex, H, Eq>::key_arg key,
     typename table<K, V, Ex, H, Eq>::size_type mod) {
      H hf;
      
      return hf(key)%mod;
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    size_t table<K, V, Ex, H, Eq>::hash_node
    (typename table<K, V, Ex, H, Eq>::value_arg v,
     typename table<K, V, Ex, H, Eq>::size_type mod) {
      return hash_key(get_key(v), mod);
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    typename table<K, V, Ex, H, Eq>::key_type const &
    table<K, V, Ex, H, Eq>::get_key
    (typename table<K, V, Ex, H, Eq>::value_arg v) {
      Ex extract;
      
      return extract(v);
    }

    template<typename K, typename V, class Ex, class H, class Eq>
    typename table<K, V, Ex, H, Eq>::bucket_type
    table<K, V, Ex, H, Eq>::copy_bucket
    (typename table<K, V, Ex, H, Eq>::bucket_type const &other) {
      bucket_type res(other.size(), (node_type *)(0));
      typename bucket_type::const_iterator i = other.begin(),
	endi = other.end();
      typename bucket_type::iterator j = res.begin();
      
      for( ; endi!=i; ++i, ++j ) {
	node_type *iter = *i;
	node_type **tmp = &*j;
	
	while( 0!=iter ) {
	  *tmp = new node_type(iter->val, 0);
	  tmp = &((*tmp)->next);
	  iter = iter->next;
	}
      }
      return res;      
    }
    
  } // namespace utilmm::hash_toolbox
} // namespace utilmm

#endif // IN_UTILMM_UTILS_HASH_TABLE_HEADER
