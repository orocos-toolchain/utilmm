#include "utilmm/configfile/commandline.hh"

#include "utilmm/configfile/configset.hh"
#include "utilmm/stringtools.hh"
#include <boost/regex.hpp>

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

}

namespace utilmm
{
    /** Builds an option using the option description syntax as in @c command_line 
     * @arg option the option description, see the command_line for its syntax 
     * If @c option is not a valid description string, the constructor returns and
     * isValid() will return false */
    cmdline_option::cmdline_option(const std::string& description)
	: m_multiple(false), m_required(false), m_argument_flags(None)
    {
	static regex rx_valid_identifier("[\\w-]+");

	size_t current = 0;
	if (description[current] == '!')
	{
	    ++current;
	    m_required = true;
	}
	if (description[current] == '*')
	{
	    ++current;
	    m_multiple = true;
	}

	if (!advance(description, ":", current, m_config))
	    throw bad_syntax(description, "expected ':'");

	char delim = advance(description, ",=?:", ++current, m_long);
	if (! regex_match(m_long, rx_valid_identifier))
	    throw bad_syntax(description, "invalid identifier");
	if (!delim) return;

	std::string text;

	if (delim == ',')
	{
	    delim = advance(description, "=?:", ++current, text);

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
		m_argument_flags |= Optional;

	    delim = advance(description, ",:", ++current, text);

	    if (text == "int")
		m_argument_flags |= IntArgument;
	    else if (text == "string")
		m_argument_flags |= StringArgument;
	    else if (text == "bool")
		m_argument_flags |= BoolArgument;
	    else
		throw bad_syntax(description, "invalid option type");

	    if (delim == ',')
	    {
		delim = advance(description, ":", ++current, text);
		if (isRequired() && !isArgumentOptional())
		    throw bad_syntax(description, "it is meaningless to have a default value for a required argument of a require option");

		if (!checkArgument(text))
		    throw bad_syntax(description, "default value for " + getLong() + " is not a valid value");

		m_argument_flags |= DefaultValue;
		m_default = text;
	    }
	    else if (m_argument_flags & Optional)
		throw bad_syntax(description, "options with optional arguments should have a default value");

	    if (!delim) return;
	}

	m_help = string(description, current + 1);
    }

    cmdline_option::~cmdline_option() { }

    bool        cmdline_option::isMultiple() const            { return m_multiple; }
    std::string cmdline_option::getConfigKey() const          
    { 
	if (!m_config.empty())
	    return m_config;
	return getLong(); 
    }
    std::string cmdline_option::getLong()   const             { return m_long; }
    std::string cmdline_option::getShort()  const             { return m_short; }
    std::string cmdline_option::getHelp()   const             { return m_help; }

    bool cmdline_option::isRequired() const		      { return m_required; }

    int  cmdline_option::getArgumentFlags() const	      { return m_argument_flags; }
    bool cmdline_option::hasArgument() const                  { return m_argument_flags; }
    bool cmdline_option::isArgumentOptional() const           { return m_argument_flags & Optional; }

    bool cmdline_option::hasDefaultValue() const		  { return m_argument_flags & DefaultValue; }
    std::string cmdline_option::getDefaultValue() const	  
    { 
	if (hasDefaultValue())
	    return m_default;
	return "true";
    }

    /** Checks that @c value is a valid string according
     * to the argument type (int, bool or string)
     */
    bool cmdline_option::checkArgument(const std::string& value) const
    {
	static const regex 
	    rx_int("[0-9]+"),
	    rx_bool("1|0|false|true");


	if (m_argument_flags & IntArgument)
	    return regex_match(value, rx_int);
	else if (m_argument_flags & BoolArgument)
	    return regex_match(value, rx_bool);
	else return true;
    }

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
	if (! optdesc.checkArgument(value))
	    throw commandline_error("invalid value for --" + optdesc.getLong());

        if (optdesc.isMultiple())
            config.insert(optdesc.getConfigKey(), value);
        else
            config.set(optdesc.getConfigKey(), value);
    }

    int command_line::option_match(config_set& config, cmdline_option const& opt, int argc, char const* const* argv, int i)
    {
	if (argv[i] == "-" + opt.getShort())
	{
	    // check if it looks like we have an argument
	    bool has_argument = (i + 1 < argc && argv[i + 1][0] != '-');

	    if (!opt.hasArgument() || (opt.isArgumentOptional() && !has_argument))
		add_argument(config, opt, opt.getDefaultValue());
	    else if (has_argument)
	    {
		add_argument(config, opt, argv[i + 1]);
		return i + 2;
	    }
	    else
		throw commandline_error("missing argument to -" + opt.getShort());
	}
	else if (argv[i] == "--" + opt.getLong())
	{
	    if (opt.hasArgument() && !opt.isArgumentOptional())
		throw commandline_error("missing argument to --" + opt.getLong());

	    add_argument(config, opt, opt.getDefaultValue());
	}
	else if (starts_with(argv[i], "--" + opt.getLong() + "="))
	{
	    if (!opt.hasArgument())
		throw commandline_error("argument provided to --" + opt.getLong());

	    int seed_size = opt.getLong().length() + 3;
	    add_argument(config, opt, std::string(argv[i]).substr(seed_size));
	}
	else return i;
	return i + 1;
    }

    void command_line::parse(int argc, char const* const argv[], config_set& config)
    {
        std::list<string> remains;
	
	// Parse options from the command line
        for (int i = 0; i < argc; )
        {
	    if (argv[i][0] != '-')
	    {
                remains.push_back(argv[i]);
		++i;
		continue;
	    }

            // Get the option description object (if this is an option)
	    Options::iterator opt;
            for (opt = m_options.begin(); opt != m_options.end(); ++opt)
	    {
		int new_index = option_match(config, *opt, argc, argv, i);
		if (new_index != i) 
		{
		    i = new_index;
		    break;
		}
	    }
	    if (opt == m_options.end())
		throw commandline_error("unknown argument " + string(argv[i]));
        }

	// Set default values for options that have one
	for (Options::iterator opt = m_options.begin(); opt != m_options.end(); ++opt)
	{
	    if (config.exists(opt->getConfigKey()))
		continue;

	    if (opt->isRequired())
		throw commandline_error("required option --" + opt->getLong() + " is missing");
	    else if (opt->hasDefaultValue())
		config.set(opt->getConfigKey(), opt->getDefaultValue());
	}

        m_remaining = remains;
    }

    list<string> command_line::remaining() const { return m_remaining; }
    void command_line::setBanner(std::string const& str) { m_banner = str; }
    void command_line::usage(std::ostream& out) const
    {
	if (! m_banner.empty())
	    out << m_banner << "\n";

        for(Options::const_iterator it = m_options.begin(); it != m_options.end(); ++it)
	{
	    string longopt = it->getLong(), shortopt = it->getShort();
	    bool hasarg = it->hasArgument();
	    bool optarg = it->isArgumentOptional();
            string helpstring = it->getHelp();

	    out << "  ";
	    if (!shortopt.empty())
		out << "-" << shortopt << "\t";
	    out << "--" << longopt;
	    if (hasarg)
	    {
		if (optarg) out << "[=VALUE]";
		else out << "=VALUE";
	    }
	    out << "\t" << helpstring << "\n";
	}

    }
}


