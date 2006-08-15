#include <boost/noncopyable.hpp>
#include <string>
#include <vector>

namespace utilmm
{
    class base_socket : boost::noncopyable
    {
    public:
	enum Domain { Unix, Inet };
	enum Type { Stream, Datagram };

    private:
	static int to_unix(Domain d);
	static int to_unix(Type t);

	struct bad_address : public std::exception { };

	int	m_fd;
	Domain	m_domain;
	Type	m_type;

    protected:
	std::vector<uint8_t> to_sockaddr(std::string const& to) const;
	base_socket(int fd);

    public:
	/** Creates a socket and connects it to \c connect_to. Connect_to
	 * is either a UNIX socket name if domain is Unix, or a IP address
	 * of the form "a.b.c.d:port"
	 */
	base_socket(Domain domain, Type type);
	~base_socket();

	int fd() const;
    };

    class socket : public base_socket
    {
	friend class server_socket;
	socket(int fd);

    public:
	socket(Domain domain, Type type, std::string const& connect_to);

	void connect(std::string const& to);
	int read(void* buf, size_t size) const;
	int write(void const* buf, size_t size) const;
    };

    class server_socket : public base_socket
    {
    public:
	server_socket(Domain domain, Type type, std::string const& bind_to, int backlog = 256);

	void bind(std::string const& to);
	/** Waits for an incoming connection 
	 * If block is true, wait for a connection.
	 * If it is false, do not block and return
	 * NULL if there is no connection available
	 */ 
	socket* wait(bool block = true) const;

	/** Checks if there is an incoming connection in 
	 * the connection queue. Returns true if it is
	 * the case (i.e. wait() will return a socket)
	 * and false otherwise */
	bool try_wait() const;
    };
}

