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
        explicit unix_error(int error_ = errno)
            : m_error(error_) {}
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
    class tempfile : boost::noncopyable
    {
        boost::filesystem::path m_path;
        auto_close              m_guard;
        
    public:
        /** Create a temporary file.
         * If you use this constructor, you cannot have the file name.
         * You'll have to use only the file handle.
         *
         * @throw unix_error
         */
        tempfile();

        /** Close and destroy the file */
        ~tempfile();

        /** Create a temporary file using a base name
         * The file name is available using path()
         *
         * The tempfile() version is more secure. Use only this 
         * constructor if you need the file path
         */
        tempfile(std::string const& basename);

        /** Disassociates this object from the temporary file
         * After a call to disassociate(), path() and fd() are
         * no longer available
         *
         * @return the file path 
         */
        FILE* detach();

        /** Wrapper around mkstemp
         * Do not use this function to create a temporary file
         * Use a tempfile object instead
         */
        static FILE* mkstemp(std::string const& base, boost::filesystem::path& path);

        /** Get the file path
         * The file path is only available if the <tt>tempfile(std::string)</tt>
         * constructor has been used.
         * @return the file path or an empty path if it is not available
         */
        boost::filesystem::path path() const;

        /** Get the underlying file handle */
        FILE* handle() const;
        
        /** Creates a temporary file
         * \param[out]  path        the file path
         * \return      the file descriptor to the open file
         * \exception   unix_error  an error occured
         */
        static FILE* create();
    };
}

#endif

