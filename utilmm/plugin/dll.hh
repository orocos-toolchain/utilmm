// Copyright Vladimir Prus 2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DLL_HPP_VP_2004_08_24
#define BOOST_DLL_HPP_VP_2004_08_24

#include <string>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/type_traits/remove_pointer.hpp>

// FIXME: that's for Linux only
#include <dlfcn.h> 

#include <iostream>

namespace utilmm { namespace plugin {

    struct killer 
    {
        killer(void* h) : h(h) {}
        template<class T>
        void operator()(T)
        {
            std::cout << "Killing DLL\n";
            dlclose(h);
        }
        void* h;
    };

    class dll {
    public:
        dll() {} // TODO: should remove this or make non-public
        dll(const std::string& name) : m_name(name) {}

        template<typename SymbolType>
        boost::shared_ptr<
            typename remove_pointer<SymbolType>::type> 
        get(const std::string& symbol_name) const
        {
            // TODO: static assert that SymbolType is a pointer.
            typedef typename remove_pointer<SymbolType>::type PointedType;
            
            // Open the library. Yes, we do it on every access to 
            // a symbol, see the design discussion in the documentation.
            void* handle = dlopen(m_name.c_str(), RTLD_LAZY|RTLD_GLOBAL);
            if (!handle) {
                throw std::logic_error("Could not open DLL");
            }
            // Clear the error state.
            dlerror();
            void* address = dlsym(handle, symbol_name.c_str());
            char* error = dlerror();
            if (error) {
                throw std::logic_error(error);
            }
            // Cast the to right type.
            SymbolType s = (SymbolType)(address);

            boost::shared_ptr<PointedType> result(s, 
                                                  //killer(handle));
                                                  bind(dlclose, handle));
            
            return result;            
        }        

    private:
        std::string m_name;
    };

}}

#endif
