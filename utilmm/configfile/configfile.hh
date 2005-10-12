#ifndef UTILMM_CONFIG_FILE_HH
#define UTILMM_CONFIG_FILE_HH

#include "utilmm/configfile/configset.hh"
#include <string>

namespace utilmm
{
    /** A configuration file
     * Reads a configuration file using \c read. Once
     * it has been read, this object is to be used
     * as a top-level scope (ConfigSet) */
    class config_file : public config_set
    {
        std::string m_error;
    public:

        /** Read a configuration file
         * @arg name the file path
         * @return true on success, false on failure. See getError
         * to get a exhaustive error string */
        bool read(const std::string& name);

        /** Get the last error that happened in \c read */
        std::string get_error() const;
    };
}

#endif

