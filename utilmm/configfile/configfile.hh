#ifndef UTILMM_CONFIG_FILE_HH
#define UTILMM_CONFIG_FILE_HH

#include <utilmm/configfile/configset.hh>
#include <string>

namespace utilmm
{
    struct parse_error : public std::exception
    {
        int const         line;
        std::string const message;

        explicit parse_error(int line, std::string const& message);
        ~parse_error() throw() {}
    };

    /** A configuration file
     * Reads a configuration file using \c read. Once
     * it has been read, this object is to be used
     * as a top-level scope (ConfigSet) */
    class config_file : public config_set
    {
    private:
        void read(const std::string& name);

    public:

        /** Read a configuration file
         * @arg name the file path
         * @throw parse_error
         */
        config_file(const std::string& name);
    };
}

#endif

