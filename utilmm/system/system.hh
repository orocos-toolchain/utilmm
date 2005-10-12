#ifndef UTILMM_SYSTEM_HH
#define UTILMM_SYSTEM_HH

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>  
#include <exception> // for std::exception
#include <string.h>  // for strerror
#include <unistd.h>  // for close

namespace utilmm
{
    class unix_error : public std::exception
    {
    public:
        unix_error(int error = errno)
            : m_error(error) {}
        ~unix_error() throw () {}
        
        int error() const { return m_error; }
        char const* what() const throw()
        { return strerror(m_error); }

    private:
        int m_error;
    };
    
    class auto_close
    {
    public:
        auto_close(int fd)
            : m_fd(fd), m_file(0) {}
        auto_close(FILE* file)
            : m_fd(-1), m_file(file) {}
        ~auto_close()
        {
            if (m_file)
                fclose(m_file);
            else
                close(m_fd);
        }

    private:
        int m_fd;
        FILE* m_file;
    };
}

#endif

