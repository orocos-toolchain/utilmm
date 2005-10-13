// Copyright Vladimir Prus 2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_EXPORT_PLUGIN_VP_2004_08_25
#define BOOST_EXPORT_PLUGIN_VP_2004_08_25

#include <boost/any.hpp>
#include <string>
#include <map>

#include <utilmm/plugin/concrete_factory.hh>

#define BOOST_EXPORT_PLUGIN(BaseType, ActualType, name) \
    extern "C" std::map<std::string, boost::any>& boost_exported_plugins_list();  \
    namespace {                                                               \
        struct boost_plugin_exporter1 {                                       \
            boost_plugin_exporter1() {                                        \
              static utilmm::plugin::concrete_factory<BaseType, ActualType> cf;  \
              utilmm::plugin::abstract_factory<BaseType>* w = &cf;              \
                boost_exported_plugins_list().insert(std::make_pair(name, w)); \
            }                                                                 \
        } boost_plugin_exporter_instance1;                                    \
    }

#define BOOST_EXPORT_PLUGIN_LIST() \
extern "C" std::map<std::string, boost::any>& boost_exported_plugins_list() \
{                                                                           \
    static std::map<std::string, boost::any> r;                             \
    return r;                                                               \
}


    




#endif
