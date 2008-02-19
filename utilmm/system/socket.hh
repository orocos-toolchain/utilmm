#ifndef UTILMM_SOCKET_HH
#define UTILMM_SOCKET_HH

#include <boost/noncopyable.hpp>
#include <string>
#include <vector>

namespace utilmm
{
    /** Base class for sockets
     *
     * @ingroup system
     */
    class base_socket : boost::noncopyable
    {
    public:
	enum Domain { Unix, Inet };
	enum Type { Stream, Datagram };
	enum Wait { WaitRead = 1, WaitWrite = 2, WaitException = 4};

    private:
	static int to_unix(Domain d);
	static int to_unix(Type t);

	struct bad_address : public std::exception { };

	int	m_fd;
	Domain	m_domain;
	Type	m_type;

	int wait(int what, timeval* tv) const;

    protected:
	std::vector<uint8_t> to_sockaddr(std::string const& to) const;

    public:
	base_socket(int fd);
	base_socket(Domain domain, Type type);

	/** Closes the socket */
	~base_socket();

	/** Get the socket file descriptor */
	int fd() const;

	/** Non-blocking version of \c wait
	 *
	 * @param what an OR-ed field of values in the Wait enum
	 * @return true if the event was present and false otherwise
	 */
	bool try_wait(int what) const;

	/** Blocks until one of the specified events is detected on this socket
	 *
	 * @param what an OR-ed field of values in the Wait enum
	 */
	void wait(int what) const;

	/** Flush all buffers related to this socket */
	void flush() const;
    };

    /** A socket connected to a remote peer
     *
     * @ingroup system
     */
    class socket : public base_socket
    {
	friend class server_socket;

    public:
	/** Creates a socket object from an already existing file descriptor */
	socket(int fd);

	/** Opens a new socket in the specified domain and of the specified type,
	 * and connect it to \c connect_to. \c connect_to is either an IP address
	 * of the form \c a.b.c.d:port if \c domain is Inet, or a Unix socket name
	 * otherwise.
	 */
	socket(Domain domain, Type type, std::string const& connect_to);

	/** Connects or reconnects this socket to the specified peer. \c
	 * to is either an IP address of the form \c a.b.c.d:port if \c
	 * domain is Inet, or a Unix socket name otherwise.	 
	 */
	void connect(std::string const& to);

	/** Reads at most \c size bytes in the specified buffer and returns the
	 * count of bytes actually read.
	 *
	 * @exception unix_error raised if an error occured
	 */
	int read(void* buf, size_t size) const;

	/** Writes at most \c size bytes in the specified buffer and returns
	 * the count of bytes actually written.
	 *
	 * @exception unix_error raised if an error occured
	 */
	int write(void const* buf, size_t size) const;
    };

    /** A socket class holding servers (i.e. listening sockets)
     *
     * @ingroup system
     */
    class server_socket : public base_socket
    {
    public:
	server_socket(Domain domain, Type type, std::string const& bind_to, int backlog = 256);

	void bind(std::string const& to);

	/** Waits for an incoming connection 
	 * If block is true, wait for a connection. If it is false, do not block 
	 * and return NULL if there is no connection available
	 */ 
	socket* accept() const;

	/** Checks if there is an incoming connection in the connection queue. 
	 * @return true if it is the case (i.e. accept() will return a socket)
	 * and false otherwise */
	bool try_wait() const;

	/** Waits for an incoming connection */
	void wait() const;
    };
}

#endif

