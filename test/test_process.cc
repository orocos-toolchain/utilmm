#include "testsuite.hh"
#include <utilmm/system/process.hh>

using namespace boost::filesystem;
using namespace utilmm;

class TC_Process
{
public:
    TC_Process()
    {
        testdir = path(__FILE__).branch_path();
    }

    void test_run()
    {
        boost::filesystem::path temp = tempfile::name("bla");
        
        process copy;
        copy << "cp" 
            << (testdir/"test_pkgconfig.pc").native_file_string()
            << temp.native_file_string();

        copy.start();
        copy.wait();
    }

    void assert_closed(int fd)
    {
        BOOST_REQUIRE(close(fd) == -1);
        BOOST_REQUIRE(errno == EBADF);
    }
    void test_redirect()
    {
        int files[2];
        pipe(files);
        auto_close read_guard(files[0]);
        
        process cat;
        cat << "cat" << (testdir/"test_pkgconfig.pc").native_file_string();

        cat.redirect_to(process::Stdout, files[1]);
        cat.start();

        // the write pipe should be closed now. Otherwise
        // we'll never reach EOF on files[0] and the loop
        // will block
        assert_closed(files[1]);
    
        std::string output;
        int read_count;
        do
        {
            char buffer[256];
            read_count = read(files[0], buffer, 256);
            BOOST_REQUIRE(read_count >= 0);
            output += std::string(buffer, read_count);
        } while(read_count);

        cat.wait();
    }

private:
    path testdir;
};

void test_process(test_suite* ts)
{
    boost::shared_ptr<TC_Process> instance( new TC_Process );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_Process::test_run, instance ) );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_Process::test_redirect, instance ) );
}

