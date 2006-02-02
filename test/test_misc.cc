#include "testsuite.hh"
#include <utilmm/auto_flag.hh>
using namespace utilmm;

class TC_Misc
{
public:
    TC_Misc()
    {
    }

    ~TC_Misc() { }

    void test_autoflag()
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
};

void test_misc(test_suite* ts) {
    boost::shared_ptr<TC_Misc> instance( new TC_Misc );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_Misc::test_autoflag, instance ) );
}

