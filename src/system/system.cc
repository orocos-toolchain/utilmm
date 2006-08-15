#include <utilmm/system/system.hh>

#include <exception> // for std::exception
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>  
#include <string.h>  // for strerror
#include <unistd.h>  // for close
#include <boost/noncopyable.hpp>
#include <boost/filesystem/operations.hpp>
#include <vector>

using namespace utilmm;

void unix_error::init_description(std::string const& desc)
{
    size_t desc_len = desc.length();
    strncpy(m_desc, desc.c_str(), 500);
    m_desc[500] = 0;
    strcat(m_desc, ": ");
    // I wanted to use strerror_r, but no way to force the use
    // of the POSIX compliant version
    strncat(m_desc, strerror(m_error), 500 - desc_len);
}
unix_error::unix_error(std::string const& desc, int error_ = errno)
    : m_error(error_) { init_description(desc); }
unix_error::unix_error(std::string const& desc)
    : m_error(errno) { init_description(desc); }
unix_error::unix_error(int error_)
    : m_error(error_) { init_description(""); }
unix_error::unix_error()
    : m_error(errno) { init_description(""); }
unix_error::~unix_error() throw () {}

int unix_error::error() const { return m_error; }
char const* unix_error::what() const throw()
{ return m_desc; }

auto_close::auto_close()
    : m_fileno(-1), m_stream(0) {}
auto_close::auto_close(int fileno)
    : m_fileno(fileno), m_stream(0) {}
auto_close::auto_close(FILE* stream)
    : m_fileno(-1), m_stream(stream) {}
auto_close::~auto_close() { close(); }

void auto_close::close()
{
    int ret = 0;
    if (m_stream)
        ret = fclose(m_stream);
    else if (m_fileno != -1)
        ret = ::close(m_fileno);

    detach();

    if (ret == -1)
        throw unix_error();
}

void  auto_close::detach() 
{ m_fileno = -1; m_stream = 0; }

template<> 
FILE* auto_close::handle<FILE*>() const
{
    if (m_stream ==  0) throw std::bad_cast();
    return m_stream; 
}

template<> 
int   auto_close::handle<int>()   const
{
    if (m_fileno == -1) throw std::bad_cast();
    return m_fileno; 
}

void auto_close::reset(int fileno)
{
    close();
    m_fileno = fileno;
}
void auto_close::reset(FILE* stream)
{
    close();
    m_stream = stream;
}





FILE* tempfile::create()
{ 
    FILE* fd = tmpfile(); 
    if (!fd)
        throw unix_error();
    return fd;
}

FILE* tempfile::mkstemp(std::string const& base, boost::filesystem::path& path)
{
    int const base_length(base.length());

    std::vector<char> value;
    value.resize( base_length + 6 + 1 );
    strncpy(&value[0], base.c_str(), base_length);
    memset(&value[base_length], 'X', 6);
    value[base_length + 6] = '\0';

    int handle = ::mkstemp(&value[0]);
    if (handle < 0)
        throw unix_error();

    path = std::string(&value[0]);
    return fdopen(handle, "w");
}

tempfile::tempfile(std::string const& base)
    : m_path(), m_guard(mkstemp(base, m_path))
{
    if (! handle())
        throw unix_error();
}

tempfile::tempfile()
    : m_guard(create()) {}

tempfile::~tempfile() 
{
    if (! m_path.empty())
    {
        try {
            m_guard.close();
            boost::filesystem::remove(m_path);
        } catch(...) {}
    }
}

FILE* tempfile::handle() const { 
    if (m_path.empty())
	return 0;
    else
	return m_guard.handle<FILE*>(); 
}
FILE* tempfile::detach()
{
    FILE* h = handle();
    m_guard.detach();
    m_path = boost::filesystem::path();
    return h;
}

boost::filesystem::path tempfile::path() const { return m_path; }

