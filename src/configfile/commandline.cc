#include "utilmm/configfile/commandline.hh"

#include "utilmm/configfile/configset.hh"
#include <boost/regex.hpp>

#include <iostream>
#include <stdlib.h>

using namespace boost;
using namespace std;


namespace {
    // [*:][config key]:long[,short][=value_type|?value_type][|help]
    /** Each option description is parsed and transformed in
     * a cmdline_option object. The command_line code then
     * uses these objects
     */
    class cmdline_option
    {
    public:
        /** Argument types 
         * Each option may have zero or one option
         * the option is described using a or-ed 
         * int of this enum
         */
        enum ArgumentType
        {
            None = 0,         /// no arguments
            Optional = 1,     /// the argument may be ommitted

            IntArgument = 2,    /// the argument is an integer
            BoolArgument = 4,   /// the argument is a boolean, that is 0, 1, 'false' or 'true'
            StringArgument = 8  /// the argument is a string
        };

    public:
        /** Builds an option using the option description syntax as in @c command_line 
         * @arg option the option description, see the command_line for its syntax 
         * If @c option is not a valid description string, the constructor returns and
         * isValid() will return false */
        cmdline_option(const std::string& description)
            : m_valid(false), m_multiple(false), m_separator(0), m_short(0), m_argument(None), m_long_cstr(0)
        {
            static regex rx_scandesc("(\\*.)?([\\w-]*):([\\w-]+)(?:,([\\w-]))?(=[\\w-]+|\\?[\\w-]+)?(?:\\|(.*))?");

            smatch match;
            if (! regex_match( description, match, rx_scandesc))
            {
                cerr << "Invalid command line description " << description << endl;
                return;
            }

            if (match[1].matched)
            {
                m_multiple = true;
                m_separator = match[1].str()[1];
            }
            
            m_config = match[2];
            m_long   = match[3];
            if (match[4].matched)
                m_short  = match[4].str()[0];

            if (! match[5].str().empty())
            {
                std::string option_type = match[5];
                if (*option_type.begin() == '?')
                    m_argument |= Optional;

                option_type = std::string(option_type, 1);
                if (option_type == "int")
                    m_argument |= IntArgument;
                else if (option_type == "string")
                    m_argument |= StringArgument;
                else if (option_type == "bool")
                    m_argument |= BoolArgument;
                else
                {
                    cerr << "Invalid option type " << option_type << " in " << description << endl;
                    return;
                }
            }

            m_help = match[6];

            m_long_cstr = new char[m_long.size() + 1];
            strncpy(m_long_cstr, m_long.c_str(), m_long.size() + 1);

            m_valid = true;
        }

        ~cmdline_option()
        { delete m_long_cstr; }

        bool        isValid() const               { return m_valid; }
        bool        isMultiple() const            { return m_multiple; }
        char        getMultipleSeparator() const  { return m_separator; }
        std::string getConfigKey() const          { return m_config; }
        std::string getLong()   const             { return m_long; }
        char        getShort()  const             { return m_short; }
        std::string getHelp()   const             { return m_help; }

        bool hasArgument() const                  { return m_argument; }
        bool isArgumentOptional() const           { return m_argument & Optional; }

        /** Checks that @c value is a valid string according
         * to the argument type (int, bool or string)
         */
        bool checkArgument(const std::string& value) const
        {
            static const regex 
                rx_int("[0-9]+"),
                rx_bool("1|0|false|true");

            
            if (m_argument & IntArgument)
                return regex_match(value, rx_int);
            else if (m_argument & BoolArgument)
                return regex_match(value, rx_bool);
            else return true;
        }

        /** Gets the short option string as needed by
         * getopt_long
         */
        std::string getGetoptShort() const
        {
            const char as_string[2] = { m_short, 0 };
            if (! m_short) return "";
            if (m_argument == None)    return as_string;
            if (m_argument & Optional) return string(as_string) + "::";
            return string(as_string) + ":";
        }

        /** Fills a option structure as needed by getopt_long
         * @arg str the structure to fill
         * @arg index the value to put into str->val
         */
        bool getGetoptLong(struct option* str, int index) const
        {
            if (m_long.empty())
                return false;

            str->name = m_long_cstr;
            if (! hasArgument())
                str->has_arg = no_argument;
            else if (isArgumentOptional())
                str->has_arg = optional_argument;
            else str->has_arg = required_argument;
            
            str->flag = 0;
            str->val  = index;

            return true;
        }

    private:
        bool m_valid;

        bool m_multiple;
        char m_separator;
        
        std::string 
             m_config,
             m_long;
        char m_short;
        std::string m_help;

        int  m_argument;

        char* m_long_cstr;
    };
}

namespace utilmm
{
    command_line::command_line(const char* options[])
        : m_getopt_short(0), m_getopt_long(0)
    {
        list<string> strlist;
        for(; *options; ++options)
            strlist.push_back(*options);
        init(strlist);
    }

    command_line::command_line(const list<string>& description)
        : m_getopt_short(0), m_getopt_long(0)
    {
        init(description);
    }
            
        
        
    bool command_line::init(const list<string>& description)
    {
        int long_options = 0;
        for(list<string>::const_iterator opt_it = description.begin(); opt_it != description.end(); ++opt_it)
        {
            cmdline_option* option = new cmdline_option(*opt_it);
            if (! option -> isValid())
            {
                delete option;
                return false; 
            }

            m_options.push_back(option);
            long_options += (option -> getLong().empty()) ? 0 : 1;
        }

        m_getopt_long = new option[long_options + 1];
        memset(&m_getopt_long[long_options], 0, sizeof(option));

        int         option_index = 0;
        int         option_long_index = 0;
        std::string option_short(":");
        for (Options::const_iterator it = m_options.begin(); it != m_options.end(); ++it)
        {
            option_short += (*it) -> getGetoptShort();
            
            if ((*it)->getGetoptLong(m_getopt_long + option_long_index, 256 + option_index))
                ++option_long_index;

            ++option_index;
        }

        m_getopt_short = new char[option_short.size() + 1];
        strncpy(m_getopt_short, option_short.c_str(), option_short.size() + 1);

        return true;
    }

    command_line::~command_line()
    {
        delete[] m_getopt_long;
        delete   m_getopt_short;

        for (Options::iterator it = m_options.begin(); it != m_options.end(); ++it)
            delete *it;
    }


    bool command_line::parse(int argc, char* const argv[], config_set* config)
    {
        if (! m_getopt_short)
            return false;

        opterr = 1;

        int retval;
        while ( (retval = getopt_long(argc, argv, m_getopt_short, m_getopt_long, 0)) != -1)
        {
            if (retval == '?')
            {
                cerr << "Unknown option " << (char)optopt << endl;
                return false;
            }
            if (retval == ':')
            {
                cerr << "Missing parameter for option " << (char)optopt << endl;
                return false;
            }

            // find the option object
            cmdline_option* option_object = 0;
            if (retval >= 256)
                option_object = m_options[retval - 256];
            else 
            {
                for (Options::const_iterator it = m_options.begin(); it != m_options.end(); ++it)
                {
                    if ((*it) -> getShort() == retval)
                    {
                        option_object = *it;
                        break;
                    }
                }
            }

            // validates the argument if it has one
            if (optarg && ! option_object->checkArgument(optarg))
                return false;

            // set the config variable, or use long option name if no config key has been 
            // specified
            std::string config_key(option_object->getConfigKey());
            if (config_key.empty())
                config_key = option_object->getLong();
            std::string config_value(optarg ? optarg : "true");

            if (option_object -> isMultiple())
            {
                std::string value (config->get<std::string>(config_key, ""));
                if (! value.empty())
                    value += option_object->getMultipleSeparator();
                value += config_value;

                config_value = value;
            }

            config->insert(config_key, config_value);
        }

        for (int remaining = optind; remaining < argc; ++remaining)
            m_remaining.push_back(argv[remaining]);

        return true;
    }

    list<string> command_line::remaining() const { return m_remaining; }
}


