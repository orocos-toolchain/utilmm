#ifndef TEST_SUITE_HH
#define TEST_SUITE_HH

#include <boost/version.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_log.hpp>
#include <boost/filesystem/path.hpp>

using namespace boost::unit_test;

#ifndef BOOST_REQUIRE_THROW
#  define BOOST_REQUIRE_THROW(s,c) BOOST_CHECK_THROW(s,c)
#  define BOOST_REQUIRE_NO_THROW(s) BOOST_CHECK_NO_THROW(s)
#  define BOOST_REQUIRE_EQUAL(s,c) BOOST_CHECK_EQUAL(s,c)
#endif

#if BOOST_VERSION < 103300
inline void set_log_threshold_level(log_level lev)
{ unit_test_log::instance().set_log_threshold_level(lev); }
#else
inline void set_log_threshold_level(log_level lev)
{ unit_test_log.set_threshold_level(lev); }
#endif

#endif

#if BOOST_VERSION >= 104600
inline std::string path_to_string(boost::filesystem::path p)
{ return p.string(); }
#else
inline std::string path_to_string(boost::filesystem::path p)
{ return p.native_file_string(); }
#endif

