/* -*- C++ -*-
 * $Id: pointer.tcc 961 2005-03-11 14:05:33Z fpy $
 */
#ifndef IN_UTILMM_SMART_POINTER_HEADER
# error "Cannot include template files directly"
#else

namespace utilmm {
  namespace smart {
    
    /*
     * class utilmm::smart::pointer<>
     */
    // structors 
    template<class Manager>
    pointer<Manager>::pointer(typename pointer<Manager>::pointer_type ptr)
      :pointee(manager.instance().manage(ptr)) {}

    template<class Manager>
    pointer<Manager>::pointer(pointer<Manager> const &other)
      :pointee(manager.instance().assign(other.pointee)) {}

    template<class Manager>
    pointer<Manager>::~pointer() {
      reset();
    }
    
    // modifiers
    template<class Manager>
    void pointer<Manager>::reset(typename pointer<Manager>::pointer_type ptr) {
      typename Manager::mem_cell tmp = pointee;
      Manager &inst = manager.instance();
      
      pointee = inst.assign(inst.manage(ptr));
      inst.release(tmp);
    }

    template<class Manager>
    void pointer<Manager>::swap(pointer<Manager> &other) {
      std::swap(pointee, other.pointee);
    }

    template<class Manager>
    pointer<Manager> &pointer<Manager>::operator= (pointer<Manager> const &other) {
      pointer tmp = other;

      swap(tmp);
      return *this;
    }

    // observers
    template<class Manager>
    bool pointer<Manager>::null() const {
      return manager.insatnce().null(pointee);
    }

    template<class Manager>
    bool pointer<Manager>::operator! () const {
      return null();
    }

    template<class Manager>
    bool pointer<Manager>::operator==(pointer<Manager> const &other) const {
      return pointee==other.pointee;
    }

    template<class Manager>
    bool pointer<Manager>::operator!=(pointer<Manager> const &other) const {
      return !operator==(other);
    }

    template<class Manager>
    typename pointer<Manager>::pointer_type pointer<Manager>::operator->()
      const {
      if( null() )
	throw null_access("Trying to access to a NULL pointer.");
      return manager.insatnce().get_ptr(pointee);
    }

    template<class Manager>
    typename pointer<Manager>::reference_type pointer<Manager>::operator* ()
      const {
      return *operator->();
    }

  } // namespace utilmm::smart
} // namespace utilmm

#endif // IN_UTILMM_SMART_POINTER_HEADER
