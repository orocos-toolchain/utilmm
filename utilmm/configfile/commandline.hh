#ifndef UTILMM_COMMANDLINE_HH
#define UTILMM_COMMANDLINE_HH

#include <string>
#include <vector>
#include <list>
#include <getopt.h>

namespace { class cmdline_option; }
namespace utilmm
{
    class config_set;
   
    class bad_syntax : public std::exception
    {
    public:
        ~bad_syntax() throw() {}

        std::string source, error;
        bad_syntax(std::string const& source, std::string const& error = "")
            : source(source), error(error) {}
    };

    /** command_line handling based on getopt_long
     *
     * <h2> Description </h2>
     * 
     * The command_line class allows you to parse user-provided command line
     * options and fill a Config object with them.
     *
     * For each command line option, you must provide a long option (--option_text),
     * may provide a short one (-option_character), and each option may have one 
     * (optional) argument. Eventually, you can give a help string (not used yet).
     *
     * During the parsing, a key/value pair is added to a Config object for each
     * option encountered. The value of the entry is either the option's argument or
     * a boolean value of true if no option is specified.
     *
     * <h2> Usage </h2>
     * 
     * The full syntax is
     * 
     * @code
     *    [*separator][config_key]:long_name[,short_name][=value_type|?value_type][|help]
     * @endcode
     *
     * where value_type is one of: int, bool, string
     *
     * When an option is found, an entry is added to a Config object with 
     * the @c config_key key. The value associated depends on the option:
     * <ul>
     *    <li> if the option has an argument it is this argument
     *    <li> if the option has no argument, or if the argument is optional and none
     *    was provided, it is set to a boolean value of true
     * </ul>
     *
     * If the option has a mandatory argument, add @c =value_type after the option 
     * names. If it is optional, use the @c ?value_type syntax. The '@c int' and '@c bool' value types
     * are checked by the command_line object and an error is generated if the user-provided
     * value dos not match.
     * 
     * <h2> Multiple option on the command line </h2>
     * When the same option is provided more than once on the command line, the normal 
     * behaviour is to use the value of the latest. However, you can also get all the values
     * by adding <tt>*separator_char</tt> at the front of the description line. In that case,
     * the config value will be all the values given by the user separated by the provided
     * separator character.
     *
     * For instance, an option like the @c -I option of @c gcc will be described using <br>
     * <tt>*::include,I=string|include path</tt>
     *
     * In which case, the command_line class will build a config value of '/a/path:/another/path' under
     * the key 'include' when it encounters the command line  <br>
     * <tt>gcc -I /a/path -I=/another/path</tt>
     * 
     * <h2> Examples </h2>
     * 
     * The classical --help option will be given using <tt> :help|display this help and exit </tt>
     *
     * The @c -r and @c --recursive options of grep are described using
     * <tt> :recursive,r|equivalent to --directories=recurse </tt>
     *
     * The @c -m and @c --max-count options of grep are described using
     * <tt> :max-count,m=int|stop after NUM matches</tt>
     */
    class command_line
    {
    private:
        typedef std::vector<cmdline_option> Options;

    public:
        /** Builds an object with a null-terminated string list
         * @param options the option list, null-terminated
         */
        command_line(const char* options[]);

        /** @overload */
        command_line(const std::list<std::string>& description);

        ~command_line();

        /** Parses the command line option
         * @param argc the argument count
         * @param argv the argument value
         * @param config the Config object the option values will be written to
         * @return true on success, false on failure
         */
        bool parse(int argc, char* const argv[], config_set& config);

        /** Remaining command line options
         * After all options are matched, and if no error has occured,
         * this function will return all non-option arguments (input
         * files for instance)
         */
        std::list<std::string> remaining() const;

    private:
        void add_argument(config_set& config, cmdline_option const& optdesc, std::string const& value);

        Options    m_options;
        std::list<std::string> m_remaining;
    };
}

#endif

