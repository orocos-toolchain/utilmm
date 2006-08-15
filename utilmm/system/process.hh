#ifndef UTILMM_PROCESS_H
#define UTILMM_PROCESS_H
#include "utilmm/config/config.h"
#include <utilmm/system/system.hh>
#include <boost/noncopyable.hpp>
#include <boost/filesystem/path.hpp>

#include <string>
#include <map>
#include <list>
#include <signal.h>

namespace utilmm
{
    /** An external process
     *
     * @ingroup system
     * @author Sylvain Joyeux <sylvain.joyeux@laas.fr>
     */
    class process
    {
    public:
        static const int InvalidHandle = -1;
        /** Definition of the streams we can redirect to */
        enum Stream { Stdout = 1, Stderr = 2 };

    private:
        typedef std::list<process*>::iterator ProcessHandle;
        ProcessHandle m_handle;

        boost::filesystem::path m_wdir;
        typedef std::list<std::string> CommandLine;
        CommandLine m_cmdline;

        typedef std::map<std::string, std::string> Env;
        Env m_env;

        struct output_file : public utilmm::auto_close
        {
            bool is_null() const;
            void redirect(FILE* stream);
        };
        output_file m_stdout, m_stderr;
        output_file& get_stream(Stream stream);

        bool  m_running;
        pid_t m_pid;
        bool  m_normalexit;
        int   m_status;

	bool m_do_setpgid;
	pid_t m_pgid;

        bool wait(bool hang);
        void send_child_error(int fd, int error_type);
        void process_child_error(int fd);

    public:
        class already_running : public std::exception {};
    
        process();
        ~process();

        /** Set the working directory */
        void set_workdir(boost::filesystem::path const& dir);
        /** Get the working directory 
         * \return the value set by set_workdir, or an empty string 
         */
        boost::filesystem::path workdir() const;
        
        /** Get the list of elements in the command line.
         * It includes the process name. */
        std::list<std::string> cmdline() const;
        /** Add an element on the command line */
        void push(const std::string& arg);
        /** An alias to push
         * \see push
         */
        process& operator << (std::string const& newarg);
        /** Clear the command line */
        void clear();

        /** Start the process 
         * \exception unix_error        an error occured while starting the process
         * \exception already_running   this process object has already started
         */
        void start();

        /** Send a signal to a running process
         * @arg signo specify the signal to send to the process
         * @return { true if the signal was sent successfully or if the process
         * was not running, false otherwise. }
         * \exception unix_error    an error occured
         */
        void signal(int signo = SIGINT);

        /** Install a SIGINT handler which calls process::killall */
        static void install_sigint_handler();

        /** Kill all processes managed by a process instance 
         * It is safe to call this inside a signal handler */
        static void killall();
        
        /** Wait for the process to terminate 
         * Use running() to check if the process is running or not */
        void wait();

        /** Detach from a running process 
         * Call this to make the process object forget about 
         * a running child.
         *
         * After a call to detach(), the child process won't be 
         * stopped when this object is destroyed 
         */
        void detach();

        /** Removes any redirection for \c stream 
         * \param stream the output stream to consider 
         */
        void erase_redirection(Stream stream);

        /** Redirects the program output to a file or an already opened file descriptor
         *
         * \par[Using temporary files]
         * \code
         *  process prs;
         *  tempfile tmp("basename");
         *  prs.redirect_to(process::Stdout, tmp.fd(), true);
         * \endcode
         *
         * \par[Make process open a file]
         * \code
         *  process prs;
         *  prs.redirect_to(process::Stdout, "my_output_file");
         * \endcode
         * 
         * \param stream the output stream
         * \param name   { a name for the destination. If handle is InvalidHandle, 
         *               then \c name shall be a valid file name, since the file 
         *               will be created in start() }
         * \param handle the file handle to redirect to, or InvalidHandle
         * \param auto_close { if the file descriptor is owned by this process
         *               if auto_close is true, there is no more guarantee as to 
         *               the status of the file descriptor. It can be closed
         *               anytime by process }
         */
        void redirect_to( Stream stream, int   handle, bool auto_close = true);
        void redirect_to( Stream stream, FILE* handle, bool auto_close = true);
        void redirect_to( Stream stream, boost::filesystem::path const& file);


        /** Override an environment variable
         * This function sets or overrides an environment variable for the 
         * subprocess. 
         * \param key the variable name
         * \param value the variable value 
         */
        void set_environment(const std::string& key, const std::string& value);
        /** Gets an overriden environment variable
         * \param key the variable name
         * \return the variable value if \c key has previously been overriden by a call to set_environment
         * */
        std::string environment(const std::string& key) const;
        /** Remove any overriden environment variable */
        void clear_environment();

	/** Set the process group ID at startup. See setpgid(3) */
	void set_pgid(pid_t pid);

        /** Check if the last running process exited normally */
        bool exit_normal() const;
        /** Get the exit status of the last running process */
        int exit_status() const;

        /** Get the PID of the last running process */
        pid_t pid() const;
        /** Check if the process is running */
        bool  running();
    };
}

#endif

