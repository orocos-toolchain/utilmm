#include "utilmm/configfile/configfile.hh"
#include "utilmm/configfile/exceptions.hh"

#include <boost/regex.hpp>

#include <fstream>
#include <sstream>

using std::fstream;
using std::string;
using std::stringbuf;
using namespace utilmm;

using boost::regex;
using boost::smatch;

parse_error::parse_error(int line_, std::string const& message_)
    : line(line_), message(message_) {}

config_file::config_file(std::string const& name)
{
    try { read(name); }
    catch(...) {
        clear();
        throw;
    }
}
    
void config_file::read(const std::string& name)
{
    clear();

    fstream file;
    file.open(name.c_str(), fstream::in);
    if (! file.is_open()) 
        throw not_found(name);

    static const string
        regexp_header("^[[:blank:]]*"),
        regexp_trail("[[:blank:]]*(?:#.*)?$");

    static regex
        rx_empty(regexp_header + regexp_trail),
        rx_name(regexp_header + "([[:alnum:]_-]+)([^#]*)" + regexp_trail),
        rx_attribute(regexp_header + ":[[:blank:]]*([^#]+)" + regexp_trail),
        rx_open_bracket(regexp_header + "\\{" + regexp_trail),
        rx_close_bracket(regexp_header + "\\}" + regexp_trail);

    enum { Normal, FindBracket } mode;
    mode = Normal;

    config_set* cur_set = this;
    
    int line_number = 1;
    stringbuf linebuf;
    const string empty_string;
    while (! file.eof())
    {
        while (file.peek() == '\n')
        {
            ++line_number;
            file.ignore();
        }

        file.get(linebuf);
        const string line(linebuf.str());
        linebuf.str(empty_string);
        
        smatch result;
        if (regex_match(line, rx_empty)) continue;

        if (mode == FindBracket)
        {
            if (! regex_match(line, rx_open_bracket))
                throw parse_error(line_number, "expected '{', found " + line);

            mode = Normal;
        }
        else if (regex_match(line, result, rx_name))
        {
            string key = result[1];
            string value = result[2];

            smatch attribute_value;

            if (value.empty() 
                    || regex_match(value, rx_open_bracket))
            {
                mode = value.empty() ? FindBracket : Normal;

                config_set* new_set = new config_set(cur_set);
                cur_set->insert( key, new_set );
                cur_set = new_set;
            }
            else if (regex_match(value, attribute_value, rx_attribute)) 
                cur_set->insert( key, attribute_value[1] );
            else
                throw parse_error(line_number, "expected '" + key + ": value', found " + key + value);
        }
        else if (regex_match(line, rx_close_bracket))
        {
            if (cur_set -> parent() == 0)
                throw parse_error(line_number, "unmatched bracket");
            cur_set = cur_set->parent();
        }
        else
            throw parse_error(line_number, "expected \"key: value\", found " + line);
    }

    if (mode == FindBracket)
        throw parse_error(line_number, "expected '{', got end of file");
    else if (cur_set -> parent())
        throw parse_error(line_number, "expected '}' before end of file");
}

