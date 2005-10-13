// Copyright Vladimir Prus 2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PLUGIN_FACTORY_VP_2004_08_25
#define BOOST_PLUGIN_FACTORY_VP_2004_08_25

#include <utilmm/plugin/virtual_constructors.hh>
#include <utilmm/plugin/dll.hh>
#include <utilmm/plugin/abstract_factory.hh>
#include <utility>
#include <stdexcept>
#include <string>
#include <utility>

namespace utilmm { namespace plugin {

    namespace {
        typedef std::map<std::string, boost::any> exported_plugins_t;
        typedef exported_plugins_t& (*get_plugins_list_t)();
        typedef exported_plugins_t& (get_plugins_list_np)();

        template<class BasePlugin>
        std::pair<abstract_factory<BasePlugin>*,
                  dll_handle >
        get_abstract_factory(const dll& d, const std::string& klass)
        {
            boost::shared_ptr<get_plugins_list_np> f; 
            f = d.template get<exported_plugins_t& (*)()>("boost_exported_plugins_list");    
            exported_plugins_t& e = (*f)();
            
            if (e.count(klass)) {

                abstract_factory<BasePlugin>** xw = 
                    boost::any_cast<abstract_factory<BasePlugin>*>(&(e[klass]));

                if (!xw) {
                    throw std::logic_error("Can't cast to the right factor type\n");                    
                }
                abstract_factory<BasePlugin>* w = *xw;
                return make_pair(w, f);
            } else {
                throw std::logic_error("Class of the specified name is not found");
            }
        }

    }



    struct empty_plugin_factory_item {
        void create(int****);
    protected:
        dll m_dll;
    };

    template<class BasePlugin, class Base, class Parameters>
    struct plugin_factory_item {                
    };

    template<class BasePlugin, class Base>
    struct plugin_factory_item<BasePlugin, Base, boost::mpl::list<> > : public Base { 
        using Base::create;
        BasePlugin* create(const std::string& name)
        {
            std::pair<abstract_factory<BasePlugin>*,
                dll_handle > r = get_abstract_factory<BasePlugin>(this->m_dll, name);
            return r.first->create(r.second);            
        }
    };

    template<class BasePlugin, class Base, class A1>
    struct plugin_factory_item<BasePlugin, Base, boost::mpl::list<A1> > : public Base {                
        using Base::create;
        BasePlugin* create(const std::string& name, A1 a1)
        {
            std::pair<abstract_factory<BasePlugin>*,
                dll_handle > r = get_abstract_factory<BasePlugin>(this->m_dll, name);
            return r.first->create(r.second, a1);            
        }
    };

    template<class BasePlugin, class Base, class A1, class A2>
    struct plugin_factory_item<BasePlugin, Base, boost::mpl::list<A1, A2> > : public Base {                
        using Base::create;
        BasePlugin* create(const std::string& name, A1 a1, A2 a2)
        {
            std::pair<abstract_factory<BasePlugin>*,
                dll_handle > r = get_abstract_factory<BasePlugin>(this->m_dll, name);
            return r.first->create(r.second, a1, a2);            
        }
    };

#if 0

    template<class BasePlugin, class Base, class Parameters>
    struct plugin_factory_item 
        : public Base,
          public plugin_factory_item_N<BasePlugin, Parameters>

    {
        using Base::create;
        using plugin_factory_item_N<BasePlugin, Parameters>::create;
    };
#endif

    using namespace boost::mpl::placeholders;

    template<class BasePlugin>
    struct plugin_factory : 
        public boost::mpl::inherit_linearly<
        typename virtual_constructors<BasePlugin>::type,
        plugin_factory_item<BasePlugin, _, _>,
        empty_plugin_factory_item>::type 
    {        
        plugin_factory(const dll& d)
        {
            this->m_dll = d;
        }
    };

}}

#endif
