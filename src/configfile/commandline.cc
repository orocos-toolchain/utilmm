#include "utilmm/configfile/commandline.hh"

#include "utilmm/configfile/configset.hh"
#include <boost/regex.hpp>

#include <stdlib.h>

using namespace boost;
using namespace std;


namespace {

    char advance(std::string const& source, std::string const& allowed, size_t& current, std::string& text)
    {
        size_t next = source.find_first_of(allowed, current);
        text = string(source, current, next - current);
        current = next;

        return (next == string::npos) ? 0 : source[current];
    }
    
    using namespace utilmm;

    // [*][config key]:long[,short][=value_type|?value_type][|help]
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
            : m_multiple(false), m_argument(None)
        {
            static regex rx_valid_identifier("[\\w-]*");

            size_t current = 0;
            if (description[current] == '*')
            {
                ++current;
                m_multiple = true;
            }

            if (!advance(description, ":", current, m_config))
                throw bad_syntax(description, "expected ':'");

            char delim = advance(description, ",=?|", ++current, m_long);
            if (! regex_match(m_long, rx_valid_identifier))
                throw bad_syntax(description, "invalid identifier");
            if (!delim) return;

            std::string text;

            if (delim == ',')
            {
                delim = advance(description, "=?|", ++current, text);

                if (text.size() != 1)
                    throw bad_syntax(description, "short option string must be only one character");
                m_short = text;
                if (! regex_match(m_short, rx_valid_identifier))
                    throw bad_syntax(description, "invalid short option character");

                if (! delim) return;
            }

            if (delim == '=' || delim == '?')
            {
                if (delim == '?')
                    m_argument |= Optional;

                delim = advance(description, "|", ++current, text);

                if (text == "int")
                    m_argument |= IntArgument;
                else if (text == "string")
                    m_argument |= StringArgument;
                else if (text == "bool")
                    m_argument |= BoolArgument;
                else
                    throw bad_syntax(description, "invalid option type");

                if (!delim) return;
            }

            m_help = string(description, current + 1);
        }

        ~cmdline_option() { }

        bool        isMultiple() const            { return m_multiple; }
        std::string getConfigKey() const          
        { 
            if (!m_config.empty())
                return m_config;
            return getLong(); 
        }
        std::string getLong()   const             { return m_long; }
        std::string getShort()  const             { return m_short; }
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

    private:
        bool m_multiple;
        std::string m_config, m_long, m_short, m_help;
        int  m_argument;
    };
}

namespace utilmm
{
    command_line::command_line(const char* options[])
    {
        for(char const** opt = options; *opt; ++opt)
            m_options.push_back(cmdline_option(*opt));
    }

    command_line::command_line(const std::list<std::string>& description)
    {
        for(list<string>::const_iterator it = description.begin(); it != description.end(); ++it)
            m_options.push_back(cmdline_option(*it));
    }
            
    command_line::~command_line() { }

    void command_line::add_argument(config_set& config, cmdline_option const& optdesc, std::string const& value)
    { 
        if (optdesc.isMultiple())
            config.insert(optdesc.getConfigKey(), value);
        else
            config.set(optdesc.getConfigKey(), value);
    }
    bool command_line::parse(int argc, char* const argv[], config_set& config)
    {
        std::list<string> remains;
        for (int i = 0; i < argc; ++i)
        {
            // Get the option description object (if this is an option)
            Options::iterator opt;
            for (opt = m_options.begin(); opt != m_options.end(); ++opt)
            {
                if (argv[i] == "-" + opt->getShort() || argv[i] == "--" + opt->getLong())
                    break;
            }
            
            if (opt == m_options.end())
            {
                remains.push_back(argv[i]);
                continue;
            }

            if (opt->hasArgument())
            {
                ++i;
                if (i == argc) // no more argument on the command line
                {
                    if (opt->isArgumentOptional())
                        add_argument(config, *opt, "true");
                    else
                        return false;
                }
                else
                {
                    string optarg(argv[i]);
                    if (optarg[0] != '-' && opt->checkArgument(optarg))
                        add_argument(config, *opt, optarg);
                    else if (opt->isArgumentOptional())
                    {
                        --i;
                        add_argument(config, *opt, "true");
                    }
                    else
                        return false;
                }
            }
            else
                add_argument(config, *opt, "true");
        }

        m_remaining = remains;
        return true;
    }

    list<string> command_line::remaining() const { return m_remaining; }
}


