// Copyright Vladimir Prus 2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PLUGIN_WRAPPER_VP_2004_08_25
#define BOOST_PLUGIN_WRAPPER_VP_2004_08_25

#include <boost/mpl/list.hpp>
#include <boost/shared_ptr.hpp>

#include <iostream>

namespace utilmm { namespace plugin {

    template<class Wrapped, class Parameters>
    struct plugin_wrapper {};

    struct dll_handle_holder {
        dll_handle_holder(dll_handle dll) : m_dll(dll) {}
        ~dll_handle_holder()
        {
            std::cout << "DLL handle is dying\n";
        }
    private:
        dll_handle m_dll;
    };

    template<class Wrapped>
    struct plugin_wrapper<Wrapped, boost::mpl::list<> > 
        : public dll_handle_holder, public Wrapped {        
        plugin_wrapper(dll_handle dll) : dll_handle_holder(dll) {}

    };

    template<class Wrapped, class A1>
    struct plugin_wrapper<Wrapped, boost::mpl::list<A1> > 
        : public dll_handle_holder, Wrapped {        
        plugin_wrapper(dll_handle dll, A1 a1) 
        : dll_handle_holder(dll), Wrapped(a1) {}

    };

    template<class Wrapped, class A1, class A2>
    struct plugin_wrapper<Wrapped, boost::mpl::list<A1, A2> > 
        : public dll_handle_holder, Wrapped {        
        plugin_wrapper(dll_handle dll, A1 a1, A2 a2) : 
        dll_handle_holder(dll), Wrapped(a1, a2) {
            std::cout << "plugin_wrapper: " << a1 << " : " << a2 << "\n";
        }
    };

}}

#endif
