/* -*- C++ -*-
 * $Id: factory_fwd.hh 957 2005-03-07 16:01:20Z sjoyeux $
 */
#ifndef UTILMM_TYPES_FACTORY_FWD
# define UTILMM_TYPES_FACTORY_FWD

# include <functional>

#include "utilmm/types/bits/factory_error_fwd.hh" 

namespace utilmm {
  
  template< class AbstractProduct, typename IdentifierType, 
	    typename Result = AbstractProduct *,
	    typename ProductCreator = Result (*)(),
	    template<typename, class, typename> 
            class FactoryErrorPolicy = factory_toolbox::error,
	    class OrderId = std::less<IdentifierType> >
  class factory;

} // namespace utilmm

#endif // UTILMM_TYPES_FACTORY_FWD
/** @file types/factory_fwd.hh
 * @brief Forward declaration of utilmm::factory
 *
 * @author Frédéric Py <fpy@laas.fr>
 *
 * @ingroup factory
 */
