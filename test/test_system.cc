#include "testsuite.hh"
#include <utilmm/system/system.hh>
using namespace utilmm;

class TC_System
{
public:
    TC_System()
    {
    }

    ~TC_System() { }

    void test_filetools()
    {
        // Open a file and test auto_close
        int files[2];
        pipe(files);
        { auto_close close_read(files[0]); }
        BOOST_REQUIRE( close(files[0]) == -1 && errno == EBADF );

        {
            auto_close close_write(files[1]);
            BOOST_REQUIRE(close_write.handle<int>() == files[1]);
            BOOST_REQUIRE_THROW(close_write.handle<FILE*>(), std::bad_cast);
            close_write.detach();
        }
        BOOST_REQUIRE( close(files[1]) == 0 );
    }
};

void test_system(test_suite* ts) {
    boost::shared_ptr<TC_System> instance( new TC_System );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_System::test_filetools, instance ) );
}

