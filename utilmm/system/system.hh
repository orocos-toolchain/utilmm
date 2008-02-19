#ifndef UTILMM_SYSTEM_HH
#define UTILMM_SYSTEM_HH

#include <boost/noncopyable.hpp>
#include <boost/filesystem/path.hpp>

namespace utilmm
{
    /** Exception raised when a standard C call returns an error. It is based
     * on the errno error report mechanism.
     *
     * @ingroup system
     */
    class unix_error : public std::exception
    {
	char m_desc[512];
	void init_description(std::string const& desc);

    public:
        explicit unix_error(std::string const& desc, int error);
        explicit unix_error(std::string const& desc);
        explicit unix_error(int error);
        explicit unix_error();
        ~unix_error() throw ();
        
        int error() const;
        char const* what() const throw();

    private:
        int m_error;
    };
    
    /** The C++ guard mechanism applied to file descriptors: when destroyed, an
     * auto_close object releases the file descriptor it has been given at
     * construction.
     *
     * @ingroup system
     */
    class auto_close : boost::noncopyable
    {
    public:
        auto_close();
        explicit auto_close(int fileno);
        explicit auto_close(FILE* stream);
        ~auto_close();
        
	/** Access to the file descriptor. 
	 *
	 * @exception std::bad_cast Raised if the required type is not the
	 * right file descriptor type (i.e. if handle<FILE> is calledd on a
	 * auto_close object created with an 'int' fd)
	 */
        template<typename T> T handle()  const;
            
	/** Close the file descriptor right away */
        void  close();

	/** First, close the current defined file descriptor and then sets it
	 * to a new one. Calling reset(-1) is equivalent to calling close()
	 * directly.
	 */
        void  reset(int fd);

	/** First, close the current defined file descriptor and then sets it
	 * to a new one. Calling reset(-1) is equivalent to calling close()
	 * directly.
	 */
        void  reset(FILE* stream);

	/** Disassociates this object and the file descriptor associated with
	 * it \b without closing the file descriptor
	 */
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
     * removed on destruction 
     *
     * @ingroup system
     */
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
         * \return      the file descriptor to the open file
         * \exception   unix_error  an error occured
         */
        static FILE* create();
    };
}

#endif

/** @defgroup system OS-related tools
 * 
 * This system defines OS-related tools: endianness changing operators,
 * temporary files, sockets. These tools are tested on Linux but should be
 * portable to all POSIX-compliant OSes.
 */ 

