// Copyright Vladimir Prus 2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CONCRETE_FACTORY_VP_2004_08_25
#define BOOST_CONCRETE_FACTORY_VP_2004_08_25

#include <utilmm/plugin/abstract_factory.hh>
#include <utilmm/plugin/plugin_wrapper.hh>

#include <iostream>

namespace utilmm { namespace plugin {

    template<class BasePlugin, class Concrete, class Base, class Parameters>
    struct concrete_factory_item {                
    };

    template<class BasePlugin, class Concrete, class Base>
    struct concrete_factory_item<BasePlugin, Concrete, Base, boost::mpl::list<> > 
        : public Base
    {                
        BasePlugin* create(dll_handle dll)
        {
            std::cout << "CF: 0 args\n";
            return new plugin_wrapper<Concrete, boost::mpl::list<> >(dll);
        }
    };

    template<class BasePlugin, class Concrete, class Base, class A1>
    struct concrete_factory_item<BasePlugin, Concrete, Base, boost::mpl::list<A1> > 
        : public Base
    {                
        BasePlugin* create(dll_handle dll, A1 a1)
        {
            std::cout << "CF: 1 args\n";
            return new plugin_wrapper<Concrete, boost::mpl::list<A1> >(dll, a1);
        }
    };

    template<class BasePlugin, class Concrete, class Base, class A1, class A2>
    struct concrete_factory_item<BasePlugin, Concrete, Base, boost::mpl::list<A1, A2> > 
        : public Base
    {                
        BasePlugin* create(dll_handle dll, A1 a1, A2 a2)
        {
            std::cout << "CF: 2 args\n";
            return new plugin_wrapper<Concrete, boost::mpl::list<A1, A2> >(dll, a1, a2);
        }
    };
    
    using namespace boost::mpl::placeholders;

    template<class BasePlugin, class Concrete>
    struct concrete_factory :
        public boost::mpl::inherit_linearly<
        typename virtual_constructors<BasePlugin>::type,
        concrete_factory_item<BasePlugin, Concrete, _, _>,
        abstract_factory<BasePlugin> >::type
    {
    };    

    

    


}}

#endif
