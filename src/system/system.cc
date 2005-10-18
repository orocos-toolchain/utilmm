#include <utilmm/system/system.hh>
#include <vector>

using namespace utilmm;

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

boost::filesystem::path tempfile::name(std::string const& base)
{
    int const base_length(base.length());

    std::vector<char> value;
    value.resize( base_length + 6 + 1 );
    strncpy(&value[0], base.c_str(), base_length);
    memset(&value[base_length], 'X', 6);
    value[base_length + 6] = '\0';

    int handle = mkstemp(&value[0]);
    if (handle < 0)
        throw unix_error();
    ::close(handle);
    return &value[0];
}


tempfile::tempfile()
    : auto_close(create()) {}

FILE* tempfile::detach()
{
    FILE* h = handle<FILE*>();
    auto_close::detach();
    return h;
}

