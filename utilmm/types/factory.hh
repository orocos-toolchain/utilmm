/* -*- C++ -*-
 * $Id: factory.hh 957 2005-03-07 16:01:20Z sjoyeux $
 */
#ifndef UTILMM_TYPES_FACTORY_HEADER
# define UTILMM_TYPES_FACTORY_HEADER
#include "utilmm/config/config.h"

#include "boost/utility.hpp"

#include "utilmm/singleton/wrapper_fwd.hh"
#include "utilmm/functional/arg_traits.hh"

#include "utilmm/types/factory_fwd.hh"
#include "utilmm/types/bits/factory_error.hh"

namespace utilmm {

  /** @brief Generic Factory implementation
   *
   * This class offers a generic implementation of thze factory design pattern.
   *
   * A factory is a producer of classes of a special base type. The production
   * will depend on the request.
   *
   * @param AbstractProduct Base type for the produced instances.
   * @param IdentifierType Type of the production method identifiers.   
   * @param Result Type of the result (@c AbstractProduct*).
   * @param ProductCreator Type of the creation methods (@c Result(*)()). 
   * @param FactoryErrorPolicy Error management policy
   * (@c utilmm::factory_toolbox::error)
   * @param OrderId An order between @a IdentifierType
   * (@c std::less<IdentifierType>)
   *
   * @note this class is a singleton
   *
   * @sa utilmm::singleton::use
   *
   * @author Frédéric Py <fpy@laas.fr>
   *
   * @ingroup factory
   */
  template< class AbstractProduct, typename IdentifierType, typename Result,
	    typename ProductCreator,
	    template<typename, class, typename> class FactoryErrorPolicy,
	    class OrderId >
  class factory
    :public FactoryErrorPolicy<IdentifierType, AbstractProduct, Result>,
     public boost::noncopyable {
  private:
    typedef typename arg_traits<IdentifierType>::type id_param;
    typedef typename arg_traits<ProductCreator>::type creator_param;

    typedef std::map<IdentifierType, ProductCreator, OrderId> factory_db;
    
    factory_db associations;

  public:
    /** @brief Add a new production method 
     *
     * This method add a new production method to the factory.
     * @param id Identifier for the production method
     * @param creator Production method.
     * 
     * @retval true if the new creation method was added
     * @retval false if there was already a creation method with @a id
     *
     * After this operation client can request for product having @a id
     * as identifeir
     */
    bool add(id_param id, creator_param creator) {
      typename factory_db::value_type to_add(id, creator);

      return associations.insert(to_add).second;
    }
    /** @brief Remove a production method
     *
     * This method allow user to remove a creation method from the factory
     *
     * @param id The identifier of the creation method to remove
     *
     * @retval true if the creation method was removed.
     * @retval false if there was no creation method attached to @a id
     */
    bool remove(id_param id) {
      return associations.erase(id)==1;
    }

    /** @brief Alias creation
     *
     * Make an alias identifer for a creation method
     *
     * @param from The alias identifier
     * @param to an identifer
     *
     * This method will try to create an alias to cration method named @a to
     *
     * @retval true if the alias was created
     * @retval false else
     */
    bool make_alias(id_param from, id_param to) {
      typename factory_db::iterator i = associations.find(to);
      
      return associations.end()==i || add(from, i->second);
    }

    /** @brief Creation method
     * @param id an identifer
     *
     * Create a new product using the cration method attched to @a id
     *
     * @return The created product
     *
     * @note If there's no creation method attched to @a id this function
     * will call @c FactoryErrorPolicy::on_unknown_id
     */
    Result create(id_param id) {
      typename factory_db::iterator i = associations.find(id);
      
      if( associations.end()==i )
	return this->on_unknown_id(id);
      else
	return (i->second)();
    }

    /** @brief Check for entry
     * @param id an identifer
     *
     * This function test if there's a creation method attched to @a id
     *
     * @retval true If thered's a creation method attched to @a id
     * @retval false else
     */
    bool check_entry(id_param id) const {
      return associations.find(id)!=associations.end();
    }

    /** @brief Creation method access
     *
     * @param id An identifier
     * 
     * This function get the creation mathod attched to @a id
     *
     * @return A creation method
     *
     * @note In case there's no crezation method attached to @a id this
     * function will call @c FactoryErrorPolicy::on_unknown_type
     */
    ProductCreator const &get_creator(id_param id) const {
      typename factory_db::const_iterator i = associations.find(id);
      
      if( associations.end()==i )
	return this->on_unknown_type(id);
      else
	return i->second;
    }
    
  private:
    factory() {}
    ~factory() {}

    template<class Ty>
    friend class singleton::wrapper;
  }; // class utilmm::factory<>
            
} // namespace utilmm

# define IN_UTILMM_TYPES_FACTORY_HEADER
#include "utilmm/types/bits/factory.tcc"
# undef IN_UTILMM_TYPES_FACTORY_HEADER
#endif // UTILMM_TYPES_FACTORY_HEADER
/** @file types/factory.hh
 * @brief Defintion of utilmm::factory
 *
 * This header defines The @c utilmm::factory class
 *
 * @author Frédéric Py <fpy@laas.fr>
 * @ingroup factory
 */
/** @defgroup factory Factory design pattern
 *
 * @brief Factory design pattern implementation.
 *
 * This module include all the classes used to implement the factory
 * design pattern.
 */
