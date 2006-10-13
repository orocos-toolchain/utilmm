#include "testsuite.hh"
#include <utilmm/system/system.hh>
#include <utilmm/system/socket.hh>
#include <utilmm/system/endian.hh>
#include <boost/filesystem/operations.hpp>
#include <iostream>
#include <errno.h>
using namespace utilmm;

class TC_System
{
public:
    TC_System()
    {
    }

    ~TC_System() { }

    void test_swap_endian()
    {
        BOOST_REQUIRE_EQUAL(0x10, swap_endian<int8_t>(0x10));
        BOOST_REQUIRE_EQUAL(0x1032, swap_endian<int16_t>(0x3210));
        BOOST_REQUIRE_EQUAL(0x10325476, swap_endian<int32_t>(0x76543210));
        BOOST_REQUIRE_EQUAL(0x1032547698badcfeLL,swap_endian<int64_t>(0xfedcba9876543210LL));
    }

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

    void test_socket()
    {
	// First create a IP server socket
	server_socket server(server_socket::Inet, server_socket::Stream, "0.0.0.0:4672");
	// This should throw since the port is already used
	BOOST_REQUIRE_THROW(server_socket another_server(server_socket::Inet, server_socket::Stream, "0.0.0.0:4672"), unix_error);

	// Create a socket and connect it to the server
	socket client(socket::Inet, socket::Stream, "localhost:4672");
	// This should throw since there is nothing on the port
	BOOST_REQUIRE_THROW(socket another_client(socket::Inet, socket::Stream, "127.0.0.1:6472"), unix_error);

	server.wait();
	BOOST_REQUIRE(server.try_wait());
	std::auto_ptr<socket> accepted(server.accept());

	// Should not have an incoming connection
	BOOST_REQUIRE(!server.try_wait());

	// write on the connection and check that 
	// we get data on the other end
	accepted->write("blabla", 7);
	char buffer[7];

	//accepted->wait(socket::WaitWrite);
	client.wait(socket::WaitRead);
	//BOOST_REQUIRE(client.try_wait(socket::WaitRead));
	client.read(buffer, 7);
	BOOST_REQUIRE(!client.try_wait(socket::WaitRead));
	BOOST_REQUIRE_EQUAL(std::string(buffer), std::string("blabla"));
    }
};

void test_system(test_suite* ts) {
    boost::shared_ptr<TC_System> instance( new TC_System );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_System::test_filetools, instance ) );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_System::test_tempfile, instance ) );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_System::test_socket, instance ) );
    ts->add( BOOST_CLASS_TEST_CASE( &TC_System::test_swap_endian, instance ) );
}

