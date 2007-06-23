#include <boost/test/auto_unit_test.hpp>

#include "testsuite.hh"
#include <utilmm/system/process.hh>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

using namespace boost::filesystem;
using namespace utilmm;
using std::string;

string get_file_contents(int fd)
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

void check_var(process& proc, std::string const& varname, std::string const& expected)
{
    tempfile tmpfile("bla");
    proc.clear();
    proc.redirect_to(process::Stdout, fileno(tmpfile.handle()), false);
    proc << "printenv" << varname;

    proc.start();
    proc.wait();
    BOOST_REQUIRE(proc.exit_normal());
    BOOST_REQUIRE(!proc.exit_status());

    int read_fd = open(tmpfile.path().native_file_string().c_str(), O_RDONLY);
    string var_value = get_file_contents(read_fd);
    close(read_fd);
    BOOST_REQUIRE_EQUAL(var_value, expected + "\n");
}

BOOST_AUTO_TEST_CASE( test_environment )
{
    process get_var;
    get_var.set_environment("TESTVAR", "my_value");
    get_var.set_environment("TESTVAR2", "another_value");
    check_var(get_var, "TESTVAR2", "another_value");
    check_var(get_var, "TESTVAR", "my_value");
}

BOOST_AUTO_TEST_CASE( test_run )
{
    path testdir = path(__FILE__).branch_path();
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
BOOST_AUTO_TEST_CASE( test_nonblocking )
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
BOOST_AUTO_TEST_CASE( test_redirect )
{
    path testdir = path(__FILE__).branch_path();
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

