#include <boost/test/auto_unit_test.hpp>

#include "testsuite.hh"
#include <boost/filesystem/path.hpp>
#include <utilmm/configfile/pkgconfig.hh>
#include <iostream>

using namespace utilmm;
using namespace boost::filesystem;

BOOST_AUTO_TEST_CASE( test_exists )
{
    path testdir = path(__FILE__).branch_path();
#if BOOST_VERSION >= 104600
    setenv("PKG_CONFIG_PATH", testdir.string().c_str(), 1);
#else
    setenv("PKG_CONFIG_PATH", testdir.native_file_string().c_str(), 1);
#endif

    // Check for a non-existent pc file
    BOOST_REQUIRE_THROW(new pkgconfig("doesnotexist"), not_found);
    // Check for an existent pc file
    pkgconfig pc("test_pkgconfig");

    // Check the data
    BOOST_REQUIRE( pc.name() == "test_pkgconfig" );
    BOOST_REQUIRE( pc.version() == "0.1" );
    BOOST_REQUIRE( pc.get("prefix", "") == "/opt" );
    BOOST_REQUIRE( pc.get("bla", "") == "");
    BOOST_REQUIRE( pc.compiler() == "-DB21R -I/opt/include/test" );
    BOOST_REQUIRE( pc.compiler(pkgconfig::Path) == "-I/opt/include/test" );
    BOOST_REQUIRE( pc.compiler(pkgconfig::Other) == "-DB21R" );
    BOOST_REQUIRE( pc.linker() == "-L/opt/i386-linux/lib -lpkgconfig_test" );
    BOOST_REQUIRE( pc.linker(pkgconfig::Libraries) == "-lpkgconfig_test" );
}

