// Copyright Vladimir Prus 2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ABSTRACT_FACTORY_VP_2004_08_25
#define BOOST_ABSTRACT_FACTORY_VP_2004_08_25

#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/list.hpp>
#include <boost/shared_ptr.hpp>

#include <utilmm/plugin/virtual_constructors.hh>

namespace utilmm { namespace plugin {

    struct empty_abstract_factory_item {
        void create(int*******);
    };

    /** A template class, which is given the base type of plugin and a set
        of constructor parameter types and defines the appropriate virtual
        'create' function.
    */
    template<class BasePlugin, class Parameters> 
    struct abstract_factory_item_N {    
    };

    template<class BasePlugin>
    struct abstract_factory_item_N<BasePlugin, boost::mpl::list<> > {
        virtual BasePlugin* create(dll_handle dll) = 0;
    };

    template<class BasePlugin, class A1>
    struct abstract_factory_item_N<BasePlugin, boost::mpl::list<A1> > {
        virtual BasePlugin* create(dll_handle dll, A1 a1) = 0;
    };

    template<class BasePlugin, class A1, class A2>
    struct abstract_factory_item_N<BasePlugin, boost::mpl::list<A1, A2> > {
        virtual BasePlugin* create(dll_handle dll, A1 a1, A2 a2) = 0;
    };
    

    template<class BasePlugin, class Base, class Parameters> 
    struct abstract_factory_item : 
        public Base, 
        public abstract_factory_item_N<BasePlugin, Parameters> 
    {
        using Base::create;
        using abstract_factory_item_N<BasePlugin, Parameters>::create;
    };

    using namespace boost::mpl::placeholders;

    template<class BasePlugin>
    struct abstract_factory :
        public boost::mpl::inherit_linearly<
        typename virtual_constructors<BasePlugin>::type,
        abstract_factory_item<BasePlugin, _, _>,
        empty_abstract_factory_item>::type
    {
    };    

}}

#endif
