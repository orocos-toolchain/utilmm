#include <boost/test/auto_unit_test.hpp>

#include "testsuite.hh"
#include <utilmm/configfile/shell_expand.hh>

#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>

#include <string>

static std::string get_home()
{
    // Get the home directory for the current user
    uid_t uid = getuid();
    passwd* pwd;
    while((pwd = getpwent()))
    {
        if(pwd->pw_uid == uid)
            break;
    }

    endpwent();
    if (pwd)
        return pwd->pw_dir;
    else
        return std::string();
        
}

BOOST_AUTO_TEST_CASE( test_expansion )
{
    std::string home = get_home();
    std::string expanded = utilmm::shell_expand("$HOME");
    BOOST_REQUIRE(home == expanded);
}

