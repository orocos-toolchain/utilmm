#include "testsuite.hh"
#include <utilmm/configfile/configfile.hh>
#include <utilmm/configfile/commandline.hh>
#include <iostream>
#include <boost/filesystem/path.hpp>
#include <algorithm>

using namespace utilmm;
using namespace boost::filesystem;
using namespace std;

class TC_Configfile
{
public:
    TC_Configfile()
        : m_config(0)
    {
        testdir = path(__FILE__).branch_path();
        m_config = new config_file( (testdir / "test_configfile.config").native_file_string().c_str() );
    }

    ~TC_Configfile() 
    { delete m_config; }

    template<typename T>
    void test_get_scalar(string const& name, T expected)
    { 
        T val = m_config -> get<T>(name);
        BOOST_REQUIRE_EQUAL(val, expected);
    }
    void test_scalar()
    {
        BOOST_REQUIRE_EQUAL(m_config->get<string>("unknown_key", "defval"), "defval");
        BOOST_REQUIRE_EQUAL(m_config->get<string> ("str"), "a string");
        BOOST_REQUIRE_EQUAL(m_config->get<int>("int_one"), 1);
        BOOST_REQUIRE_EQUAL(m_config->get<bool>("bool_true"), true);
        BOOST_REQUIRE_EQUAL(m_config->get<bool>("bool_false"), false);
        BOOST_REQUIRE_THROW(m_config->get<bool>("bool_invalid"), boost::bad_lexical_cast);
    }

    void test_list()
    {
        list<int> values = m_config->get< list<int> >("list");
        BOOST_REQUIRE(!values.empty());
        
        vector<int> expected(10, 0);
        for (list<int>::const_iterator it = values.begin(); it != values.end(); ++it)
            expected[*it] = 1;

        BOOST_REQUIRE(find(expected.begin(), expected.end(), 0) == expected.end());
        
    }

    void test_child()
    {
        std::list<config_set const*> children = m_config->children("child");
        BOOST_REQUIRE_EQUAL(children.size(), 1UL);
        
        config_set const* child = children.front();
        BOOST_REQUIRE_EQUAL(child->get<string>("str"), "another string");
    }

    void test_commandline()
    {
        {
            char const* valid_spec[] = { 
                "*:include,I=string|include path",
                "vkey:verbose,v?int|include path",
                ":quiet",
                0
            };

            command_line cmdline(valid_spec);
            check_cmdline_properties(cmdline);
        }

        char const* invalid_specs[] = {
            "*::bla", 0,
            ":blo=badtype", 0
        };
        BOOST_REQUIRE_THROW( command_line cmdline(invalid_specs), bad_syntax );
        BOOST_REQUIRE_THROW( command_line cmdline(invalid_specs + 2), bad_syntax );

        {
            list<string> valid_spec;
            valid_spec.push_back("*:include,I=string|include path");
            valid_spec.push_back("vkey:verbose,v?int|include path");
            valid_spec.push_back(":quiet");
            command_line cmdline(valid_spec);
            check_cmdline_properties(cmdline);
        }
    }

    void check_cmdline_properties(command_line& cmdline)
    {
        config_set config;
        char* valid_argv[] = { "-I", "bla", "--include", "test", "--quiet", "--verbose", "bla.cpp" };
        
        BOOST_REQUIRE(( cmdline.parse(7, valid_argv, config) ));
        BOOST_REQUIRE_EQUAL(1UL, cmdline.remaining().size() );
        BOOST_REQUIRE_EQUAL("bla.cpp", cmdline.remaining().front() );

        list<string> includes = config.get< list<string> >("include");
        BOOST_REQUIRE_EQUAL(2UL, includes.size());
        BOOST_REQUIRE( "bla" ==  includes.back() || "bla" == includes.front() );
        BOOST_REQUIRE( "test" ==  includes.back() || "test" == includes.front() );
        BOOST_REQUIRE_EQUAL( config.get<bool>("quiet"), true );
        BOOST_REQUIRE_EQUAL( config.get<bool>("vkey"), true );
    }

private:
    path testdir;
    config_file* m_config;
};


void test_configfile(test_suite* ts) {
    boost::shared_ptr<TC_Configfile> instance( new TC_Configfile );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_Configfile::test_scalar, instance ) );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_Configfile::test_list, instance ) );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_Configfile::test_child, instance ) );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_Configfile::test_commandline, instance ) );
}

