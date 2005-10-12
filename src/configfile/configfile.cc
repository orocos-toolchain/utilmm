#include "utilmm/configfile/configfile.hh"

#include <boost/regex.hpp>

#include <fstream>
#include <sstream>

using std::fstream;
using std::string;
using std::stringbuf;
using namespace utilmm;

using boost::regex;
using boost::smatch;

bool config_file::read(const std::string& name)
{
    m_error = "";
    clear();

    fstream file;
    file.open(name.c_str(), fstream::in);
    if (! file.is_open()) 
    {
        m_error = "unable to open " + name + " for reading";
        return false;
    }

    static const string
        regexp_header("^[[:blank:]]*"),
        regexp_trail("(#.*)?$");

    static regex
        rx_empty(regexp_header + regexp_trail),
        rx_name(regexp_header + "([[:alnum:]_-]+)([^#]*)" + regexp_trail),
        rx_attribute(regexp_header + ":[[:blank:]]*([^#]+)" + regexp_trail),
        rx_open_bracket(regexp_header + "\\{" + regexp_trail),
        rx_close_bracket(regexp_header + "\\}" + regexp_trail);

    enum { Normal, FindBracket } mode;
    mode = Normal;

    config_set* cur_set = this;
    
    stringbuf linebuf;
    const string empty_string;
    while (! file.eof())
    {
        while (file.peek() == '\n')
            file.ignore();

        file.get(linebuf);
        const string line(linebuf.str());
        linebuf.str(empty_string);
        
        smatch result;
        if (regex_match(line, rx_empty)) continue;

        if (mode == FindBracket)
        {
            if (! regex_match(line, rx_open_bracket))
            {
                m_error = "Expected '{', found " + line;
                return false;
            }
            mode = Normal;
        }
        else if (regex_match(line, result, rx_name))
        {
            string name = result[1];
            string value = result[2];

            smatch attribute_value;

            if (value.empty() 
                    || regex_match(value, rx_open_bracket))
            {
                mode = value.empty() ? FindBracket : Normal;

                config_set* new_set = new config_set(cur_set);
                cur_set->insert( name, new_set );
                cur_set = new_set;
            }
            else if (regex_match(value, attribute_value, rx_attribute)) 
            {
                cur_set->insert( name, attribute_value[1] );
            }
            else
            {
                m_error = "Syntax error in " + value;
                return false;
            }
        }
        else if (regex_match(line, rx_close_bracket))
        {
            if (cur_set -> get_parent() == 0)
            {
                m_error = "Unmatched bracket";
                return false;
            }
            cur_set = cur_set -> get_parent();
        }
        else
        {
            m_error = "Syntax error in line " + line;
            return false;
        }
    }

    if (cur_set -> get_parent())
    {
        m_error = "Expected closing bracket, found eof";
        return false;
    }

    return true;
}

std::string config_file::get_error() const { return m_error; }

