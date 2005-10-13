// Copyright Vladimir Prus 2004.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_VIRTUAL_CONSTRUCTORS_VP_2004_08_05
#define BOOST_VIRTUAL_CONSTRUCTORS_VP_2004_08_05

#include <boost/mpl/list.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include <string>
#include <map>

namespace utilmm { namespace plugin {

    typedef boost::shared_ptr<
        std::map<std::string, boost::any>& ()> dll_handle;

    template<class BasePlugin>
    struct virtual_constructors {
        typedef boost::mpl::list<boost::mpl::list<> > type;
    };

}}


#endif
