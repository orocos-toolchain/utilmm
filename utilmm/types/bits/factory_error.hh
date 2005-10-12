/* -*- C++ -*-
 * $Id: factory_error.hh 954 2005-02-23 12:38:20Z fpy $
 */
#ifndef UTILMM_TYPES_FACTORY_ERROR_HEADER
# define UTILMM_TYPES_FACTORY_ERROR_HEADER

# include <stdexcept>
# include <sstream>

namespace utilmm {
  /** @brief Toolbox for factory implementation
   *
   * This namespace embeds classes used to implment utilmm::factory
   *
   * @ingroup factory
   * @ingroup intern
   */
  namespace factory_toolbox {
    
    /** @brief default utilmm::factory error policy
     *
     * This class implements the default error policy for utilmm::factory
     * class. This error policy will throw an exception on error
     *
     * @author Frédéric Py <fpy@laas.fr>
     *
     * @ingroup factory
     * @ingroup intern
     */
    template<class AbstractProduct, typename IdentifierType, typename Result>
    struct error {
      
      /** @brief factory error exception
       *
       * This exception wioll be thrown by factory when there's an error
       *
       * @author Frédéric Py <fpy@laas.fr>
       *
       * @ingroup factory
       * @ingroup intern
       *
       * @relates utilmm::factory_toolbox::error
       */
      class exception
	:public std::runtime_error {
      private:
	exception(std::string const &msg)
	  : std::runtime_error(msg) {}

	friend class error;
      }; // class utilmm::factory_toolbox::error<>::exception

      /** @brief unknown id error
       *
       * Thsi function will be called by utilmm::factory if user request
       * for an unknown identifier
       *
       * @param id The identifer
       *
       * @throw utilmm::factory_toolbox::error::exception The identifier is unknown
       */
      template<typename Ty>
      static Ty on_unknown_id(IdentifierType const &id) {
	std::ostringstream msg;
	msg<<"factory error : type "<<id<<" is unknown.";
	throw exception(msg.str());
      }

      /** @brief unknown creation method error
       *
       * Thsi function will be called by utilmm::factory if user request
       * for an unknown creation method
       *
       * @param id The identifier of the creation method
       *
       * @throw utilmm::factory_toolbox::error::exception There's no creation merthod attached to @a id
       */
      static Result on_unknown_type(IdentifierType const &id) {
	return on_unknown_id<Result>(id);
      }
    }; // struct utilmm::factory_toolbox::error<>
    
  } // namespace utilmm::factory_toolbox
} // namespace utilmm

#endif // UTILMM_TYPES_FACTORY_ERROR_HEADER
/** @file types/bits/factory_error.hh
 * @brief Definition of utilmm::factory_toolbox::error
 *
 * @author Frédéric Py <fpy@laas.fr>
 *
 * @ingroup factory
 * @ingroup intern
 */
