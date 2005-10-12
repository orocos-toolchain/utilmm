/* -*- C++ -*-
 * $Id$
 */
#ifndef UTILMM_DEMANGLE_DEMANGLE_HEADER
# define UTILMM_DEMANGLE_DEMANGLE_HEADER

# include <string>
# include <typeinfo>

namespace utilmm {

  /** @brief name demangling function
   *
   * This function demangle C++ type names to a human-readable form.
   * It convert the name given by @c typeid().name() in the classical
   * C++ form. 
   *
   * @param name An internal C++ type name
   * @return the real name of this type
   *
   * @bug this function is not currently fully implemented.
   * It is based on g++ ABI and uses g++ internals. Finally
   * there's no error treatment at this time. 
   *
   * @author Frédéric Py <fpy@laas.fr>
   */
  std::string demangle(char const *name);

  /** @brief name demangling function
   *
   * This function demangle C++ type names to a human-readable form.
   * It convert the name given by @c typeid().name() in the classical
   * C++ form.
   * 
   * @param name An internal C++ type name
   * @return the real name of this type
   *
   * @author Frédéric Py <fpy@laas.fr>
   */
  inline std::string demangle(std::string const &name) {
    return demangle(name.c_str());
  }

  /** @brief demangled name function
   *
   * This function demangle C++ type names to a human-readable form.
   * It convert the name given by @c typeid().name() in the classical
   * C++ form.
   * 
   * @param type A C++ type descriptor
   * @return the real name of this type
   *
   * @author Frédéric Py <fpy@laas.fr>
   */
  inline std::string demangle(std::type_info const &type) {
    return demangle(type.name());
  }

  /** @brief demangled name function
   *
   * This function demangle C++ type names to a human-readable form.
   * It convert the name given by @c typeid().name() in the classical
   * C++ form.
   * 
   * @param Ty A C++ type
   * @return the real name of this type
   *
   * @author Frédéric Py <fpy@laas.fr>
   */
  template<typename Ty>
  std::string real_name_of() {
    return demangle(typeid(Ty));
  }
 
}

#endif // UTILMM_DEMANGLE_DEMANGLE_HEADER 
