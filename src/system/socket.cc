#include <utilmm/system/socket.hh>
#include <utilmm/system/system.hh>

#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <algorithm>

#include <sys/socket.h> 
#include <netdb.h>
#include <sys/un.h> 
#include <netinet/in.h> 
#include <netinet/ip.h> 
#include <sys/time.h>
#include <sys/select.h>

#include <iostream>

using namespace std;
using namespace boost;

namespace utilmm
{
    int base_socket::to_unix(Domain d)
    {
	switch(d)
	{
	    case Unix: return PF_UNIX;
	    case Inet: return PF_INET;
	}
	return 0;
    };

    int base_socket::to_unix(Type t)
    {
	switch(t)
	{
	    case Stream:	return SOCK_STREAM;
	    case Datagram:	return SOCK_DGRAM;
	}
	return 0;
    }

    base_socket::base_socket(int _fd)
	: m_fd(_fd) {}
    base_socket::base_socket(Domain domain, Type type)
	: m_fd(-1)
    {
	m_fd = ::socket(base_socket::to_unix(domain), base_socket::to_unix(type), 0);
	if (m_fd == -1)
	    throw unix_error("cannot open the socket");
	m_domain = domain;
	m_type = type;
    }

    base_socket::~base_socket()
    {
	if (m_fd != -1)
	    close(m_fd);
    }

    vector<uint8_t> base_socket::to_sockaddr(std::string const& to) const
    {
	vector<uint8_t> ret;
	if (m_domain == Unix)
	{
#if __APPLE__
	    static const unsigned int UNIX_MAX_PATH = 104; // check /usr/include/sys/un.h on Mac
#else
	    static const unsigned int UNIX_MAX_PATH = 108;
#endif
	    if (to.size() >= UNIX_MAX_PATH)
		throw bad_address();

	    sockaddr_un addr;
	    addr.sun_family = AF_UNIX;
	    strncpy(addr.sun_path, to.c_str(), UNIX_MAX_PATH - 1);
	    addr.sun_path[UNIX_MAX_PATH - 1] = 0;


	    copy(
		    reinterpret_cast<uint8_t*>(&addr), 
		    reinterpret_cast<uint8_t*>(&addr) + sizeof(addr), 
		    back_inserter(ret));
	}
	else if (m_domain == Inet)
	{
	    static regex rx_ip("^(.+):(\\d+)$");
	    smatch match;
	    if (!regex_match(to, match, rx_ip))
		throw bad_address();

	    std::string hostname = string(match[1].first, match[1].second);
	    uint16_t port = lexical_cast<uint16_t>( string(match[2].first, match[2].second) );

	    struct hostent* host = gethostbyname(hostname.c_str());
	    if (!host)
		throw unix_error("cannot get host address");

	    if (host->h_addrtype == AF_INET)
	    {
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		memcpy(&addr.sin_addr, host->h_addr_list[0], host->h_length);
		copy(
		    reinterpret_cast<uint8_t*>(&addr), 
		    reinterpret_cast<uint8_t*>(&addr) + sizeof(addr), 
		    back_inserter(ret));
	    }
	    else
	    {
		sockaddr_in6 addr;
		addr.sin6_family = AF_INET6;
		addr.sin6_port = htons(port);
		addr.sin6_flowinfo = 0; // don't know what this is
		memcpy(&addr.sin6_addr, &host->h_addr_list[0], host->h_length);
		copy(
		    reinterpret_cast<uint8_t*>(&addr), 
		    reinterpret_cast<uint8_t*>(&addr) + sizeof(addr), 
		    back_inserter(ret));
	    }
	}
	return ret;
    }

    int base_socket::fd() const { return m_fd; }
    bool base_socket::try_wait(int what) const
    {
	timeval tv = { 0, 0 };
	return wait(what, &tv) > 0;
    }
    void base_socket::wait(int what) const { wait(what, 0); }
    int base_socket::wait(int what, timeval* tv) const
    {
	fd_set rd_set, wr_set, exc_set;
	fd_set *rd_p = 0, *wr_p = 0, *exc_p = 0;
	if (what & WaitRead)
	{
	    FD_ZERO(&rd_set);
	    FD_SET(fd(), &rd_set);
	    rd_p = &rd_set;
	}
	if (what & WaitWrite)
	{
	    FD_ZERO(&wr_set);
	    FD_SET(fd(), &wr_set);
	    wr_p = &wr_set;
	}
	if (what & WaitException)
	{
	    FD_ZERO(&exc_set);
	    FD_SET(fd(), &exc_set);
	    exc_p = &exc_set;
	}

	int ret = select(m_fd + 1, rd_p, wr_p, exc_p, tv);
	if (ret == -1)
	    throw unix_error("error while waiting for socket");
	return ret;
    }

    void base_socket::flush() const
    { fsync(fd()); }


    socket::socket(Domain domain, Type type, std::string const& connect_to)
	: base_socket(domain, type)
    { connect(connect_to); }
    socket::socket(int _fd)
	: base_socket(_fd) {}

    void socket::connect(std::string const& to)
    {
	vector<uint8_t> addr = to_sockaddr(to);
	if (::connect(fd(), reinterpret_cast<sockaddr*>(&addr[0]), addr.size()) == -1)
	    throw unix_error("cannot connect to " + to);
    }
    int socket::read(void* buf, size_t size) const
    {
	int read_bytes = ::read(fd(), buf, size);
	if (read_bytes == -1)
	    throw unix_error("cannot read on socket");
	return read_bytes;
    }
    int socket::write(void const* buf, size_t size) const
    {
	int written = ::write(fd(), buf, size);
	if (written == -1)
	    throw unix_error("cannot write on socket");
	return written;
    }





    server_socket::server_socket(Domain domain, Type type, std::string const& bind_to, int backlog)
	: base_socket(domain, type)
    { 
	bind(bind_to); 
	if (listen(fd(), backlog) == -1)
	    throw unix_error("cannot listen to " + bind_to);
    }

    void server_socket::bind(std::string const& to)
    {
	vector<uint8_t> addr = to_sockaddr(to);
	if (::bind(fd(), reinterpret_cast<sockaddr*>(&addr[0]), addr.size()) == -1)
	    throw unix_error("cannot bind to " + to);
    }

    void server_socket::wait() const
    { return base_socket::wait(base_socket::WaitRead); }
    bool server_socket::try_wait() const
    { return base_socket::try_wait(base_socket::WaitRead); }

    socket* server_socket::accept() const
    {
	int sock_fd = ::accept(fd(), NULL, NULL);
	if (sock_fd == -1)
	    throw unix_error("failed in accept()");

	return new socket(sock_fd);
    }
}

