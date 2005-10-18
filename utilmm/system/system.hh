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
#include <boost/noncopyable.hpp>
#include <boost/filesystem/path.hpp>

namespace utilmm
{
    class unix_error : public std::exception
    {
    public:
        explicit unix_error(int error = errno)
            : m_error(error) {}
        ~unix_error() throw () {}
        
        int error() const { return m_error; }
        char const* what() const throw()
        { return strerror(m_error); }

    private:
        int m_error;
    };
    
    class auto_close : boost::noncopyable
    {
    public:
        auto_close();
        explicit auto_close(int fileno);
        explicit auto_close(FILE* stream);
        ~auto_close();
        
        template<typename T> T handle()  const;
            
        void  close();
        void  reset(int fd);
        void  reset(FILE* stream);

        void  detach();

    private:
        int m_fileno;
        FILE* m_stream;
    };
    template<> 
        FILE* auto_close::handle<FILE*>() const;
    template<> 
        int   auto_close::handle<int>()   const;

    /** A temporary file. It is created on construction and
     * removed on destruction */
    class tempfile : public utilmm::auto_close
    {
    public:
        /** Create a temporary file.
         * Throws unix_error on error
         */
        tempfile();

        /** Disassociates this object from the temporary file
         * After a call to disassociate(), path() and fd() are
         * no longer available
         * @return the file path */
        FILE* detach();

        /** Get a temporary name
         * Do not use this function to create a temporary file
         * Use a tempfile object instead
         */
        static boost::filesystem::path name(std::string const& basename);

        /** Creates a temporary file
         * \param[out]  path        the file path
         * \return      the file descriptor to the open file
         * \exception   unix_error  an error occured
         */
        static FILE* create();
    };
}

#endif

