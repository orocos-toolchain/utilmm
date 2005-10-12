#ifndef UTILMM_PROCESS_H
#define UTILMM_PROCESS_H
#include "utilmm/config/config.h"
#include <boost/noncopyable.hpp>

#include <string>
#include <map>
#include <list>
#include <signal.h>

namespace utilmm
{
    class process
    {
        typedef std::list<process*>::iterator ProcessHandle;
        ProcessHandle m_handle;

        std::string m_wdir;
        typedef std::list<std::string> Arguments;
        Arguments m_arguments;

        typedef std::map<std::string, std::string> Env;
        Env m_env;

        struct output_file : boost::noncopyable
        {
            static const int InvalidHandle = -1;
            std::string m_path;
            int m_handle;
            bool m_close;

            output_file();
            ~output_file();
            void set ( std::string const& path, int handle = InvalidHandle, bool close = false);
            void close_on_exit();
            bool is_null() const;
            void redirect(FILE* stream);
        };
        output_file m_stdout, m_stderr;

        bool  m_running;
        pid_t m_pid;
        bool  m_normalexit;
        int   m_status;

        bool wait(bool hang);

    public:
        class already_running : public std::exception {};
    
        enum Stream
        { Stdout = 1, Stderr = 2 };
        process();
        ~process();

        void set_workdir(const std::string& dir);
        std::string workdir() const;
        
        std::list<std::string> arguments() const;
        void push(const std::string& arg);
        process& operator << (std::string const& newarg);
        void clear();

        /** Start the process 
         * @return true. There is no attempt to check that the process
         * start went well. */
        void start();
        /** Kill the process if it is running 
         * @return true if the signal was sent successfully or if the process
         * was not running, false otherwise.
         */
        bool kill(int signo = SIGTERM);
        /** Wait for the process to terminate 
         * @return true if the wait() was successful. This return value does not
         * reflect the actual exit status of the process. Use exit_normal and
         * exit_status for that */
        bool wait();

        /** Detach from a running process */
        void detach();

        /** Gets the file name to which \c stream is redirected, or a
         * null string if no redirection is in effect */
        std::string redirection(Stream stream) const;
        /** Remove a redirection on \c stream */
        void erase_redirection(Stream stream);
        /** Sets a redirection on \c stream
         * @arg stream the output stream
         * @arg file   the file name to redirect to
         * @arg handle the file handle to redirect to if is has already been opened 
         * (by a call to utilmm::tempfile() for instance)
         * @arg auto_close if the file should be closed 
         * when the process object is destroyed
         */
        void redirect_to( Stream stream, const std::string& file
                , int handle = output_file::InvalidHandle, bool auto_close = true);

        /** Sets an environment var */
        void set_environment(const std::string& key, const std::string& value);
        /** Gets an environment var */
        std::string environment(const std::string& key) const;
        /** Remove any environment variable */
        void clear_environment();

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

