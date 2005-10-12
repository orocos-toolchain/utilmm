#ifndef UTILMM_SYSTEM_TEMPFILE_HH
#define UTILMM_SYSTEM_TEMPFILE_HH

#include <boost/filesystem/path.hpp>

namespace utilmm
{
    int tempname(std::string const& pattern, boost::filesystem::path& path);

    class tempfile
    {
    public:
        tempfile(std::string const& basename);
        ~tempfile();

        boost::filesystem::path path() const;
        int fd() const;

    private:
        int m_fd;
        boost::filesystem::path m_path;
    };
}

#endif

