#include "utilmm/configfile/shell_expand.hh"

#include <boost/regex.hpp>
#include <iterator>
#include <sstream>

using namespace std;
using boost::regex;
using boost::smatch;


#include <iostream>

string utilmm::shell_expand(string const& text)
{
    static regex const 
        rx_shellvalue("\\$(\\w+)");

    smatch result;
    if (! regex_search(text, result, rx_shellvalue))
        return text;
    
    string::const_iterator last_match;
    string::const_iterator const end = text.end();

    ostringstream expanded;
    do
    {
        expanded << result.prefix();

        std::string env_key = result[1];
        char const* env_value = getenv(env_key.c_str());
        if (env_value)
            expanded << env_value;

        last_match = result.suffix().first;
    } while(regex_search(last_match, end, result, rx_shellvalue));

    copy(last_match, end, ostream_iterator<string::value_type>(expanded));
    return expanded.str();
}

