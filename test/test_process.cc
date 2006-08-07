#include "testsuite.hh"
#include <utilmm/system/process.hh>

using namespace boost::filesystem;
using namespace utilmm;
using std::string;

class TC_Process
{
public:
    TC_Process()
    {
        testdir = path(__FILE__).branch_path();
    }

    string get_file_contents(int fd) const
    {
        std::string output;
        int read_count;
        do
        {
            char buffer[256];
            read_count = read(fd, buffer, 256);
            BOOST_REQUIRE(read_count >= 0);
            output += std::string(buffer, read_count);
        } while(read_count);

	return output;
    }

    void test_environment()
    {
	tempfile tmpfile("bla");
	process get_var;
	get_var << "printenv" << "TESTVAR";
	get_var.set_environment("TESTVAR", "my_value");
	get_var.redirect_to(process::Stdout, fileno(tmpfile.handle()), false);
	get_var.start();
	get_var.wait();
        BOOST_REQUIRE(get_var.exit_normal());
        BOOST_REQUIRE(!get_var.exit_status());

	int read_fd = open(tmpfile.path().native_file_string().c_str(), O_RDONLY);
	string var_value = get_file_contents(read_fd);
	close(read_fd);
	BOOST_REQUIRE_EQUAL(var_value, "my_value\n");
    }

    void test_run()
    {
        tempfile tmpfile("bla");
        
        process copy;
        copy << "cp" 
            << (testdir/"test_pkgconfig.pc").native_file_string()
            << tmpfile.path().native_file_string();

        copy.start();
        copy.wait();
        BOOST_REQUIRE(copy.exit_normal());
        BOOST_REQUIRE(!copy.exit_status());
    }
    void test_nonblocking()
    {
        process blocking;
        blocking << "sleep" << "10";
        blocking.start();
        blocking.signal(SIGKILL);
        blocking.wait();
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

	string output = get_file_contents(files[0]);
	BOOST_REQUIRE(output.size() > 0);
    
        cat.wait();
        BOOST_REQUIRE(cat.exit_normal());
        BOOST_REQUIRE(!cat.exit_status());

	int test_source = open( (testdir/"test_pkgconfig.pc").native_file_string().c_str(), O_RDONLY);
	BOOST_REQUIRE(test_source != -1);
	string source = get_file_contents(test_source);
	close(test_source);

	BOOST_REQUIRE_EQUAL(source, output);
    }

private:
    path testdir;
};

void test_process(test_suite* ts)
{
    boost::shared_ptr<TC_Process> instance( new TC_Process );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_Process::test_run, instance ) );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_Process::test_redirect, instance ) );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_Process::test_nonblocking, instance ), 0, 1 );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_Process::test_environment, instance ) );
}

