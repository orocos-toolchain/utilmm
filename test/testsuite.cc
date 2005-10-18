#include "testsuite.hh"
#include <boost/test/unit_test_log.hpp>
#include <iostream>

test_suite*
init_unit_test_suite( int argc, char * argv[] ) {
    set_log_threshold_level(log_successful_tests);
    test_suite* ts = BOOST_TEST_SUITE( "Testing Utilmm" );

    // Check system first, it is used by configfile
    test_suite* ts_sys = BOOST_TEST_SUITE( "Testing utilmm/system" );
    test_system(ts_sys);
    test_process(ts_sys);
    ts->add(ts_sys);

    test_suite* ts_config = BOOST_TEST_SUITE( "Testing utilmm/configfile" );
    test_pkgconfig(ts_config);
    test_shellexpand(ts_config);
    ts->add(ts_config);

    return ts;
}

