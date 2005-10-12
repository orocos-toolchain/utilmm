#include <utilmm/system/process.hh>
#include <utilmm/system/system.hh>

#include "boost/thread/mutex.hpp"

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#include <iostream>

using std::list;
using std::string;
using std::cout;
using std::cerr;
using std::endl;


namespace utilmm
{
    process::output_file::output_file()
        : m_path(), m_handle(InvalidHandle), m_close(true) {}
    process::output_file::~output_file()
    { set("", InvalidHandle); }

    void process::output_file::close_on_exit()
    {
        if (m_close && m_handle != InvalidHandle)
            close(m_handle);
        m_handle = InvalidHandle;
    }
    void process::output_file::set(std::string const& path, int handle, bool close)
    {
        close_on_exit();

        m_path = path;
        m_handle = handle;
        m_close = close;
    }
    bool process::output_file::is_null() const { return m_path.empty(); }

    void process::output_file::redirect(FILE* stream)
    {
        if (is_null())
            return;

        if (m_handle == InvalidHandle)
        {
            m_handle = open(m_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (m_handle == InvalidHandle)
                throw unix_error();
        }

        if (dup2(m_handle, fileno(stream)) < 0)
            throw unix_error();
    }
}

namespace
{
    char* const* stringlist_to_execlist(list<string>& strlist)
    {
        char** ret = new char* [ strlist.size() + 1 ];
        int i = 0;
        for (list<string>::iterator it = strlist.begin(); it != strlist.end(); ++it)
            ret[i++] = const_cast<char*>(it->c_str());
        ret[ i ] = 0;
        return ret;
    }

    using utilmm::process;
    using boost::mutex;
    
    boost::mutex mtx_processes;
    typedef std::list<process *> ProcessList;
    typedef ProcessList::iterator       ProcessHandle;
    std::list<process*> processes;
    
    ProcessHandle register_process(process* process)
    { mutex::scoped_lock locker(mtx_processes);
        processes.push_front( process );
        return processes.begin();
    }
    
    void deregister_process(ProcessHandle it)
    { mutex::scoped_lock locker(mtx_processes);
        processes.erase(it);
    }

    RETSIGTYPE (*old_sigint_handler)(int) = 0;
    RETSIGTYPE sigint_handler(int signum)
    {
        for (ProcessList::iterator it = processes.begin(); it != processes.end(); ++it)
            (*it)->kill();

        if (old_sigint_handler)
            old_sigint_handler(signum);

        exit(-1);
    }

    void setup_sigint_handler()
    {
        static bool setup = false;
        if (setup)
            return;

        struct sigaction action;
        memset(&action, 0, sizeof(struct sigaction));
        action.sa_handler = sigint_handler;

        struct sigaction old_action;
        sigaction(SIGINT, &action, &old_action);
        old_sigint_handler = old_action.sa_handler;
        
        setup = true;
    }
}

using namespace utilmm;

process::process()
    : m_handle(register_process(this))
    , m_running(false), m_pid(0), m_normalexit(true), m_status(0)
{
    setup_sigint_handler();
}
process::~process()
{ 
    deregister_process(m_handle);
    kill(); 
    wait(true);
}

std::string process::workdir() const { return m_wdir; }
void        process::set_workdir(const std::string& wdir)
{ m_wdir = wdir; }

list<string> process::arguments() const { return m_arguments; }
void         process::push(const std::string& argument) { m_arguments.push_back(argument); }
process&     process::operator << (std::string const& newarg)
{ 
    push(newarg); 
    return *this;
}
void         process::clear() { m_arguments.clear(); }

std::string process::redirection(Stream stream) const
{
    switch(stream)
    {
        case Stdout: return m_stdout.m_path;
        case Stderr: return m_stderr.m_path;
    };
    return "";
}
void process::erase_redirection(Stream stream) { redirect_to(stream, ""); }
void process::redirect_to( Stream stream, std::string const& file, int fileno, bool close_on_quit )
{
    switch(stream)
    {
        case Stdout: 
          m_stdout.set(file, fileno, close_on_quit); 
          break;
        case Stderr: 
          m_stderr.set(file, fileno, close_on_quit);
          break;
    };
}

void process::start()
{
    if (running())
        throw already_running();

    list<string> argv = m_arguments;
    Env          env  = m_env;

    int pc_comm[2];
    pipe(pc_comm);
    auto_close read_guard(pc_comm[0]);
    auto_close write_guard(pc_comm[1]);
    
    pid_t child_pid = fork();
    if (child_pid == -1)
        throw unix_error(errno);
    else if (child_pid)
    {
        // if close_on_exit is active, we should close
        // on the parent side. The fd's are still open on the child side
        m_stdout.close_on_exit();
        m_stderr.close_on_exit();

        int start_status;
        close(pc_comm[1]);
        if (read(pc_comm[0], &start_status, sizeof(start_status)) != 0)
        { // a problem occured while exec'ing
            throw unix_error(errno);
        }

        m_pid = child_pid;
        m_running = true;
        return;
    }
    else
    {
        // in the child
        char* const* exec_argv = stringlist_to_execlist(argv);
        char* prog = exec_argv[0];

        m_stdout.redirect(stdout);
        m_stderr.redirect(stderr);

        for (Env::const_iterator it = env.begin(); it != env.end(); ++it)
        {
            std::string putenv_arg = (it->first + "=" + it->second);
            putenv( const_cast<char*>(putenv_arg.c_str()) );
        }

        execvp(prog, exec_argv);

        // Error if reached
        int error = errno;
        write(pc_comm[1], &error, sizeof(error));
        exit(1);
    }
}

void process::detach()
{
    m_running = false;
    m_pid = 0;
}
bool process::kill(int signo)
{ 
    if (running())
        return (::kill(m_pid, signo) == 0); 
    return true;
}

bool process::wait()
{ return wait(true); }
bool process::wait(bool hang)
{
    int status;

    pid_t wait_ret;
    do
    { wait_ret = waitpid(m_pid, &status, (hang ? 0 : WNOHANG) ); }
    while (wait_ret == -1 && errno == EINTR);
    
    if (wait_ret == -1)
        return false;

    m_running = false;
    m_normalexit = WIFEXITED(status);
    if (m_normalexit)
        m_status = WEXITSTATUS(status);
    else m_status = 0;

    return true;
}

bool  process::exit_normal() const { return m_normalexit; }
int   process::exit_status() const { return m_status; }
bool  process::running()
{
    if (! m_running)
        return false;

    wait(false);
    return m_running; 
}
pid_t process::pid() const { return m_pid; }

void process::set_environment(const std::string& key, const std::string& value)
{ m_env[key] = value; }
std::string process::environment(const std::string& key) const
{ 
    Env::const_iterator it = m_env.find(key);
    if (it == m_env.end()) return string();
    return it->second;
}
void process::clear_environment() { m_env.clear(); }

