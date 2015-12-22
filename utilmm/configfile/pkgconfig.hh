#ifndef UTILMM_CONFIGFILE_HH
#define UTILMM_CONFIGFILE_HH

#include <string>
#include <list>
#include <utilmm/configfile/exceptions.hh>

namespace utilmm
{
    struct pkgconfig_error : public std::exception {};

    class process;
    
    class pkgconfig
    {
        typedef std::string string;
      
    public:
        /** Creates a package description file.
         * @throws not_found(name) if the package is not available */
        pkgconfig(string const& name);

        ~pkgconfig();

        /** The package name */
        string name() const;
        /** The package version */
        string version() const;

        /** Checks if the given package is available */
        static bool exists(string const& name);

        /** Get a variable defined in this package description */
        string get(string const& var, string const& defval = string()) const;
        
        enum Modes {
            All, Path, Other,
            Static, Libraries
        };
        /** Get compile flags for the given mode */
        string compiler(Modes mode = pkgconfig::All) const;
        /** Get link flags for the given mode */
        string linker(Modes mode = pkgconfig::All) const;

        static std::list<string> packages();
        
    private:
        /** The package name */
        string m_name;

        /** Run the given process object and returns its standard output */
        static string run(process& prs);
        /** Get the value returned by pkgconfig with the given argument applied to the current package*/
        string run(string const& argument) const;
     };
}

#endif

