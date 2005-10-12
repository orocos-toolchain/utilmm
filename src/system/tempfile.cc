#include "utilmm/system/tempfile.hh"

#include <cstdlib>
#include <cstring>
#include <vector>

using namespace std;

int utilmm::tempname(std::string const& base, boost::filesystem::path& file)
{
    int const base_length(base.length());

    std::vector<char> value;
    value.resize( base_length + 6 + 1 );
    strncpy(&value[0], base.c_str(), base_length);
    memset(&value[base_length], 'X', 6);
    value[base_length + 6] = '\0';

    int handle = mkstemp(&value[0]);
    if (handle < 0)
        return -1;
    
    file = &value[0];
    return handle;
}

using namespace utilmm;
tempfile::tempfile(std::string const& basename)
    : m_fd(-1) { m_fd = tempname(basename, m_path); }
tempfile::~tempfile() 
{
    close(m_fd);
    unlink(m_path.native_file_string().c_str());
}

boost::filesystem::path tempfile::path() const { return m_path; }
int tempfile::fd() const { return m_fd; }

