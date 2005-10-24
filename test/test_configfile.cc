#include "testsuite.hh"
#include <utilmm/configfile/configfile.hh>
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
        BOOST_REQUIRE(val == expected);
    }
    void test_scalar()
    {
        BOOST_REQUIRE(m_config->get<string>("unknown_key", "defval") == "defval");
        BOOST_REQUIRE(m_config->get<string> ("str") == "a string");
        BOOST_REQUIRE(m_config->get<int>("int_one") == 1);
        BOOST_REQUIRE(m_config->get<bool>("bool_true") == true);
        BOOST_REQUIRE(m_config->get<bool>("bool_false") == false);
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
        BOOST_REQUIRE(children.size() == 1);
        
        config_set const* child = children.front();
        BOOST_REQUIRE(child->get<string>("str") == "another string");
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
}

