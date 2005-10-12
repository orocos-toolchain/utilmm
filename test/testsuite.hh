#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_log.hpp>

using namespace boost::unit_test;

#if BOOST_VERSION < 103300
#define BOOST_REQUIRE_THROW(s,c) BOOST_CHECK_THROW(s,c)
void set_log_threshold_level(log_level lev)
{ unit_test_log::instance().set_log_threshold_level(lev); }
#else
void set_log_threshold_level(log_level lev)
{ unit_test_log.set_threshold_level(lev); }
#endif

void test_pkgconfig( test_suite*);
void test_shellexpand( test_suite* );
void test_process( test_suite* );

