#include <boost/test/auto_unit_test.hpp>

#include "testsuite.hh"
#include <utilmm/auto_flag.hh>
#include <utilmm/stringtools.hh>
using namespace utilmm;

BOOST_AUTO_TEST_CASE( test_autoflag )
{
    bool flag = false;
    
    { auto_flag<bool> guard(flag, true, true);
	BOOST_REQUIRE( flag );
    }
    BOOST_REQUIRE( !flag );

    { auto_flag<bool> guard(flag, false, true);
	BOOST_REQUIRE( !flag );
    }
    BOOST_REQUIRE( !flag );

    { auto_flag<bool> guard(flag, false, false);
	BOOST_REQUIRE( !flag );
    }
    BOOST_REQUIRE( flag );

    int field = 1;
    { auto_flag<int> guard(field, 2, true, true);
	BOOST_REQUIRE(field & 2 == 2);
    }
    BOOST_REQUIRE(field == 1);

    { auto_flag<int> guard(field, 1, false, true);
	BOOST_REQUIRE(field == 0);
    }
    BOOST_REQUIRE(field == 1);

    { auto_flag<int> guard(field, 2, false, false);
	BOOST_REQUIRE(field == 1);
    }
    BOOST_REQUIRE(field == 3);
}

BOOST_AUTO_TEST_CASE( test_stringlist )
{
    using std::string;
    
    {
	string a = "a b  c";
	stringlist l = split(a);

	BOOST_REQUIRE_EQUAL(3U, l.size());
	BOOST_REQUIRE_EQUAL("a", l.front());
	BOOST_REQUIRE_EQUAL("c", l.back());
	BOOST_REQUIRE_EQUAL("a b c", join(l));
    }

    {
	string id = "a_b_c";
	stringlist l = split(id, "_");
	BOOST_REQUIRE_EQUAL(3U, l.size());
	BOOST_REQUIRE_EQUAL("abc", join(l, ""));
    }

    { 
	string id = "a_b c";
	BOOST_REQUIRE_EQUAL("A_B C", upcase(id));
    }

    { 
	string match = "do begins with bla";
	string nomatch = "does not begins with bla";
	string seed = "do begins";
	BOOST_REQUIRE(starts_with(match, seed));
	BOOST_REQUIRE(!starts_with(nomatch, seed));
    }
}

BOOST_AUTO_TEST_CASE( test_clean_path )
{
    using std::string;
    using boost::filesystem::path;
    path p = clean_path("a/../b");
    BOOST_REQUIRE_EQUAL("b", path_to_string(p));
    p = clean_path("a//b");
    BOOST_REQUIRE_EQUAL("a/b", path_to_string(p));
    p = clean_path("a/./b");
    BOOST_REQUIRE_EQUAL("a/b", path_to_string(p));
    p = clean_path("//a/./b//");
    BOOST_REQUIRE_EQUAL("/a/b/.", path_to_string(p));
}
