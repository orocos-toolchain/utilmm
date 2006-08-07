#include "testsuite.hh"
#include <utilmm/system/system.hh>
#include <boost/filesystem/operations.hpp>
#include <iostream>
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

    void test_tempfile()
    {
        boost::filesystem::path tmppath;
        {
            tempfile file("bla");
            tmppath = file.path();
            BOOST_REQUIRE(boost::filesystem::exists(tmppath));
        }
        BOOST_REQUIRE(!boost::filesystem::exists(tmppath));

        {
            tempfile file("blo");
            tmppath = file.path();
            std::cout << tmppath.native_file_string() << std::endl;
            BOOST_REQUIRE(boost::filesystem::exists(tmppath));
            file.detach();
            BOOST_REQUIRE(boost::filesystem::exists(tmppath));
	    BOOST_REQUIRE_EQUAL(file.handle(), (FILE*) 0);
        }
        BOOST_REQUIRE(boost::filesystem::exists(tmppath));
        boost::filesystem::remove(tmppath);
    }
};

void test_system(test_suite* ts) {
    boost::shared_ptr<TC_System> instance( new TC_System );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_System::test_filetools, instance ) );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_System::test_tempfile, instance ) );
}

