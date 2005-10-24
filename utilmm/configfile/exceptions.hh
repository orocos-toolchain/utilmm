#ifndef UTILMM_CONFIGFILE_EXCEPTIONS_HH
#define UTILMM_CONFIGFILE_EXCEPTIONS_HH

#include <exception>
#include <string>

namespace utilmm
{
    struct not_found : public std::exception 
    {
        std::string const file;

        not_found(std::string const& file)
            : file(file) {}
        ~not_found() throw() {}

        char const* what() throw() { return file.c_str(); }
    };
}

#endif

