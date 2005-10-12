#ifndef UTILMM_CONFIGFILE_HH
#define UTILMM_CONFIGFILE_HH

#include <string>
#include <exception>

namespace utilmm
{
    class process;
    
    class pkgconfig
    {
        typedef std::string string;
    public:
        struct not_found : public std::exception {};
        struct pkgconfig_error : public std::exception {};
      
    public:
        pkgconfig(string const& name);
        ~pkgconfig();

        string name() const;
        string version() const;

        static bool exists(string const& name);

        string get(string const& var, string const& defval = string()) const;
        
        enum Modes {
            All, Path, Other,
            Static, Libraries
        };
        string compiler(Modes mode = pkgconfig::All) const;
        string linker(Modes mode = pkgconfig::All) const;
        
    private:
        /** The package name */
        string m_name;

        /** Run the given process object and returns its standard output */
        static string run(process& prs);
        /** Get the value returned by pkgconfig with the given argument */
        string run(string const& argument) const;
     };
}

#endif

