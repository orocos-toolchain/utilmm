#include "testsuite.hh"
#include <utilmm/configfile/configfile.hh>
#include <utilmm/configfile/commandline.hh>
#include <boost/filesystem/path.hpp>
#include <utilmm/stringtools.hh>
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

    void test_basic_properties()
    {
	BOOST_REQUIRE(! m_config->empty());
	config_set empty_set;
	BOOST_REQUIRE( empty_set.empty() );
    }

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

	config_set const& only_child = m_config->child("child");
	BOOST_REQUIRE_EQUAL(child, &only_child);

	config_set const& not_a_child = m_config->child("not_a_child");
	BOOST_REQUIRE(not_a_child.empty());
    }

    void test_cmdline_option_parsing()
    {
	{ cmdline_option opt("*:include,I=string:include path");
	    BOOST_REQUIRE(!opt.isRequired());
	    BOOST_REQUIRE(opt.isMultiple());
	    BOOST_REQUIRE_EQUAL("include", opt.getLong());
	    BOOST_REQUIRE_EQUAL("include", opt.getConfigKey());
	    BOOST_REQUIRE_EQUAL("I", opt.getShort());
	    BOOST_REQUIRE(opt.hasArgument());
	    BOOST_REQUIRE(!opt.isArgumentOptional());
	    BOOST_REQUIRE(!opt.hasDefaultValue());
	    BOOST_REQUIRE(opt.getArgumentFlags() & cmdline_option::StringArgument);
	    BOOST_REQUIRE_EQUAL("include path", opt.getHelp());
	}
	{ cmdline_option opt("!:required=bool:is a required argument");
	    BOOST_REQUIRE(opt.isRequired());
	    BOOST_REQUIRE(!opt.isMultiple());
	    BOOST_REQUIRE_EQUAL("required", opt.getLong());
	    BOOST_REQUIRE_EQUAL("required", opt.getConfigKey());
	    BOOST_REQUIRE_EQUAL("", opt.getShort());
	    BOOST_REQUIRE(opt.hasArgument());
	    BOOST_REQUIRE(opt.getArgumentFlags() & cmdline_option::BoolArgument);
	    BOOST_REQUIRE_EQUAL("is a required argument", opt.getHelp());
	}
	{ cmdline_option opt("vkey:verbose,v?int,1:include path");
	    BOOST_REQUIRE(!opt.isRequired());
	    BOOST_REQUIRE(!opt.isMultiple());
	    BOOST_REQUIRE_EQUAL("vkey", opt.getConfigKey());
	    BOOST_REQUIRE_EQUAL("verbose", opt.getLong());
	    BOOST_REQUIRE_EQUAL("v", opt.getShort());
	    BOOST_REQUIRE(opt.hasArgument());
	    BOOST_REQUIRE(opt.isArgumentOptional());
	    BOOST_REQUIRE(opt.hasDefaultValue());
	    BOOST_REQUIRE(opt.getArgumentFlags() & cmdline_option::IntArgument);
	    BOOST_REQUIRE_EQUAL("1", opt.getDefaultValue());
	    BOOST_REQUIRE_EQUAL("include path", opt.getHelp());
	}
	{ cmdline_option opt(":quiet");
	    BOOST_REQUIRE(!opt.isRequired());
	    BOOST_REQUIRE(!opt.isMultiple());
	    BOOST_REQUIRE_EQUAL("quiet", opt.getConfigKey());
	    BOOST_REQUIRE_EQUAL("quiet", opt.getLong());
	    BOOST_REQUIRE_EQUAL("", opt.getShort());
	    BOOST_REQUIRE(!opt.hasArgument());
	    BOOST_REQUIRE_EQUAL("", opt.getHelp());
	}

	// no long option
        BOOST_REQUIRE_THROW( cmdline_option option("*::bla"), bad_syntax );
	// invalid long option
        BOOST_REQUIRE_THROW( cmdline_option option("*:invalid key:bla"), bad_syntax );
	// invalid data type
        BOOST_REQUIRE_THROW( cmdline_option option(":blo=badtype"), bad_syntax );
	// optional argument and no default value
        BOOST_REQUIRE_THROW( cmdline_option option(":foo?int"), bad_syntax );
	// required option, required argument and default value
	BOOST_REQUIRE_THROW( cmdline_option opt("!:required=bool,true:is a required argument"), bad_syntax );
	// required option, optional argument and default value
	BOOST_REQUIRE_NO_THROW( cmdline_option opt("!:required?bool,true:is a required argument") );
    }

    void test_commandline()
    {
	char const* valid_spec[] = { 
	    "*:include,I=string:include path",
	    "!:required=bool:is a required argument",
	    "vkey:verbose,v?int,1:include path",
	    "defval:default-value=int,10:should be set to default value",
	    ":quiet",
	    0
	};

        {
            list<string> strlist_spec;
	    for (char const** specline = valid_spec; *specline; ++specline)
		strlist_spec.push_back(*specline);

            command_line cmdline(valid_spec);
            check_cmdline_properties(cmdline);
        }

	command_line cmdline(valid_spec);
	check_cmdline_properties(cmdline);

	config_set config;

	char* missing_required[] = { "--quiet" };
	BOOST_REQUIRE_THROW( cmdline.parse(1, missing_required, config), commandline_error );

	char* missing_argument[] = { "--required=true", "--include" };
	BOOST_REQUIRE_THROW( cmdline.parse(2, missing_argument, config), commandline_error );

	char* missing_argument_short[] = { "--required=true", "-I", "--quiet" };
	BOOST_REQUIRE_THROW( cmdline.parse(3, missing_argument_short, config), commandline_error );

	char* invalid_argument_type[] = { "--required=true", "--verbose=bla" };
	BOOST_REQUIRE_THROW( cmdline.parse(2, invalid_argument_type, config), commandline_error );

	char* overriding_default_value[] = { "--required=true", "--default-value=20" };
	BOOST_REQUIRE_NO_THROW( cmdline.parse(2, overriding_default_value, config) );
	BOOST_REQUIRE_EQUAL(20, config.get<int>("defval"));
    }

    void check_cmdline_properties(command_line& cmdline)
    {
        config_set config;
        char* valid_argv[] = { "--required=false", "-I", "bla", "--include=test", "--quiet", "--verbose", "bla.cpp" };
        
        cmdline.parse(7, valid_argv, config);
        BOOST_REQUIRE_EQUAL(1UL, cmdline.remaining().size() );
        BOOST_REQUIRE_EQUAL("bla.cpp", cmdline.remaining().front() );

        list<string> includes = config.get< list<string> >("include");
        BOOST_REQUIRE_EQUAL(2UL, includes.size());
        BOOST_REQUIRE( "bla" ==  includes.back() || "bla" == includes.front() );
        BOOST_REQUIRE( "test" ==  includes.back() || "test" == includes.front() );

        BOOST_REQUIRE_EQUAL( config.get<bool>("quiet"), true );
        BOOST_REQUIRE_EQUAL( config.get<bool>("vkey"), 1 );
        BOOST_REQUIRE_EQUAL( config.get<int>("defval"), 10 );
    }

private:
    path testdir;
    config_file* m_config;
};


void test_configfile(test_suite* ts) {
    boost::shared_ptr<TC_Configfile> instance( new TC_Configfile );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_Configfile::test_basic_properties, instance ) );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_Configfile::test_scalar, instance ) );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_Configfile::test_list, instance ) );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_Configfile::test_child, instance ) );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_Configfile::test_cmdline_option_parsing, instance ) );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_Configfile::test_commandline, instance ) );
}

