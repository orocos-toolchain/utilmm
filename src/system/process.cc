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
#include <cstring>
#include <stdio.h>

#include <iostream>
#include <boost/filesystem/exception.hpp>

using std::list;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

using namespace boost::filesystem;


namespace utilmm
{
    bool process::output_file::is_null() const
    {
        try { handle<int>(); }
        catch(std::bad_cast) { return true; }
        return false;
    }

    void process::output_file::redirect(FILE* stream)
    {
        if (is_null())
            return;

        if (dup2(handle<int>(), fileno(stream)) < 0)
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
    typedef std::list<process*> ProcessList;
    typedef ProcessList::iterator       ProcessHandle;
    std::list<process*> managed_processes;
    
    void register_process(process* process)
    { 
        sigset_t set, old_set;
        sigemptyset(&set);
        sigaddset(&set, SIGINT);
        
        sigprocmask(SIG_BLOCK, &set, &old_set);
        managed_processes.push_back(process);
        sigprocmask(SIG_SETMASK, &old_set, 0);
    }
    
    void deregister_process(process* process)
    {
        sigset_t set, old_set;
        sigemptyset(&set);
        sigaddset(&set, SIGINT);
        
        sigprocmask(SIG_BLOCK, &set, &old_set);
        managed_processes.remove(process);
        sigprocmask(SIG_SETMASK, &old_set, 0);
    }

    RETSIGTYPE (*old_sigint_handler)(int) = 0;
    RETSIGTYPE sigint_handler(int signum)
    {
        if (old_sigint_handler)
            old_sigint_handler(signum);
        else
            abort();
    }

}

using namespace utilmm;

process::process()
    : m_running(false), m_pid(0), m_normalexit(true), m_status(0), m_do_setpgid(false)
{
    register_process(this);
}
process::~process()
{ 
    deregister_process(this);
    signal(); 
    wait(true);
}

boost::filesystem::path process::workdir() const { return m_wdir; }
void        process::set_workdir(boost::filesystem::path const& wdir)
{ m_wdir = wdir; }

list<string> process::cmdline() const { return m_cmdline; }
void         process::push(const std::string& argument) { m_cmdline.push_back(argument); }
process&     process::operator << (std::string const& newarg)
{ 
    push(newarg); 
    return *this;
}
void         process::clear() { m_cmdline.clear(); }



void process::install_sigint_handler()
{
    static bool setup = false;
    if (setup)
        return;

    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = sigint_handler;

    struct sigaction old_action;
    sigaction(SIGINT, &action, &old_action);
    if (old_action.sa_flags & SA_SIGINFO)
        std::cerr << "WARNING: overriding previous SIGINT signal handler" << std::endl;
    else if (old_action.sa_handler != SIG_IGN && old_action.sa_handler != SIG_DFL)
        old_sigint_handler = old_action.sa_handler;

    setup = true;
}

void process::killall()
{
    for (ProcessList::iterator it = managed_processes.begin(); it != managed_processes.end(); ++it)
        kill((*it)->pid(), SIGINT);
}
void process::erase_redirection(Stream stream) { redirect_to(stream, ""); }
void process::redirect_to( Stream stream, boost::filesystem::path const& file)
{
    int handle = open(file.native_file_string().c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (handle == -1)
        throw unix_error();

    redirect_to(stream, handle, true);
}

void process::redirect_to( Stream stream, FILE* handle, bool close_on_exit)
{
    redirect_to(stream, dup(fileno(handle)), close_on_exit);
    if (close_on_exit)
        fclose(handle);
}

void process::redirect_to( Stream stream, int handle, bool close_on_exit)
{
    if (!close_on_exit)
    {
        handle = dup(handle);
        if (handle == -1)
            throw unix_error();
    }
    get_stream(stream).reset(handle);
}

process::output_file& process::get_stream(Stream stream)
{
    switch(stream)
    {
        case Stdout: return m_stdout;
        case Stderr: return m_stderr;
    }

    // Never reached
    assert(false);
    return m_stdout;
}

void process::set_pgid(pid_t pgid)
{
    m_do_setpgid = true;
    m_pgid = pgid;
}

static const int CHDIR_ERROR = 0;
static const int EXEC_ERROR  = 1;
void process::start()
{
    if (running())
        throw already_running();

    list<string> argv = m_cmdline;
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
        // close fds on the parent side
        m_stdout.close();
        m_stderr.close();
        write_guard.close();

        // wait for the exec() to happen
        process_child_error(pc_comm[0]);

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
        read_guard.close();

	if (m_do_setpgid)
	    setpgid(0, m_pgid);

        // must use execvp bec. we want exec to search for prog for us
        // so, should set environment variables ourselves
        for (Env::const_iterator it = env.begin(); it != env.end(); ++it)
        {
            std::string putenv_arg = (it->first + "=" + it->second);
            putenv( strdup(putenv_arg.c_str()) );
        }

        if (!m_wdir.empty())
        {
            if (chdir(m_wdir.native_file_string().c_str()) == -1)
                send_child_error(pc_comm[1], CHDIR_ERROR);
        }
            
        fcntl(pc_comm[1], F_SETFD, FD_CLOEXEC);
        execvp(prog, exec_argv);

        // Error if reached
        send_child_error(pc_comm[1], EXEC_ERROR);
    }
}

void process::process_child_error(int fd)
{
    int error_type;
    int error;
    if (read(fd, &error_type, sizeof(error_type)) == 0)
        return;
    read(fd, &error, sizeof(error));

    throw unix_error(error);
}

void process::send_child_error(int fd, int error_type)
{
    int error = errno;
    write(fd, &error_type, sizeof(error_type));
    write(fd, &error, sizeof(error));
    exit(1);
}

void process::detach()
{
    m_running = false;
    m_pid = 0;
}
void process::signal(int signo)
{ 
    if (!running())
        return;

    if (::kill(m_pid, signo) == 0)
        return;

    if (errno != ESRCH)
        throw unix_error();
}

void process::wait() { wait(true); }
bool process::wait(bool hang)
{
    int status;

    pid_t wait_ret;
    do
    { wait_ret = waitpid(m_pid, &status, (hang ? 0 : WNOHANG) ); }
    while (wait_ret == -1 && errno == EINTR);
    
    if (!hang && wait_ret == 0)
        return false;

    // We consider that ECHILD means the process has terminated
    // EINTR is taken care of
    // EINVAL is an internal error

    m_running = false;

    if (wait_ret != -1) // no status information if wait_ret == -1
    {
        m_normalexit = WIFEXITED(status);
        if (m_normalexit)
            m_status = WEXITSTATUS(status);
        else m_status = 0;
    }
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

