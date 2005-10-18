#include <utilmm/system/system.hh>
#include <utilmm/system/process.hh>
#include <utilmm/configfile/pkgconfig.hh>

using namespace utilmm;
using std::string;

pkgconfig::pkgconfig(string const& name)
    : m_name(name)
{
    if(!exists(name))
        throw not_found();
}

pkgconfig::~pkgconfig() {}

string pkgconfig::name() const { return m_name; }
string pkgconfig::version() const { return run("--modversion"); }

bool pkgconfig::exists(string const& name)
{
    process prs;
    prs << "pkg-config" << "--exists" << name;

    try { run(prs); return true; } 
    catch(not_found) { return false; }
}

string pkgconfig::get(string const& varname, string const& defval) const
{
    try { return run("--variable=" + varname); }
    catch(pkgconfig_error) { return defval; } // pkg-config 0.19 crashes when varname does not exist ...
    catch(not_found) { return defval; }
}

static char* compiler_flags[] = { "--cflags", "--cflags-only-I", "--cflags-only-other" };
string pkgconfig::compiler(Modes mode) const
{ return run(compiler_flags[mode]); }

static char* linker_flags[] = { "--libs", "--libs-only-L", "--libs-only-other", "--static", "--libs-only-l",  };
string pkgconfig::linker(Modes mode) const
{ return run(linker_flags[mode]); }




#include <iostream>
string pkgconfig::run(string const& argument) const
{
    process prs;
    prs << "pkg-config" << argument << m_name;
    string output = run(prs);
    // strip output
    string::size_type first = output.find_first_not_of(" \t\n");
    if (first == string::npos) return string();
    string::size_type last  = output.find_last_not_of(" \t\n");
    if (last == string::npos) return string(output, first);
    return string(output, first, last - first + 1);
}

string pkgconfig::run(process& prs)
{
    int pipeno[2];
    pipe(pipeno);

    prs.redirect_to(process::Stdout, pipeno[1]);
    prs.redirect_to(process::Stderr, "/dev/null");
    prs.start();

    string output;
    while(true) 
    {
        char buffer[256];
        int read_count = read(pipeno[0], buffer, 256);
        if (read_count == -1)
            throw unix_error();
        if (read_count == 0)  break;
        output += string(buffer, read_count);
    }
    prs.wait();

    if (!prs.exit_normal()) throw pkgconfig_error();
    if (prs.exit_status())  throw not_found();

    return output;
}


