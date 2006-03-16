#include <string>
#include <list>
#include <ctype.h>

namespace utilmm {
    typedef std::list<std::string> stringlist;

    stringlist split(std::string const& s, std::string const& sep = " ", bool ignore_empty = true)
    {
        using std::string;

        stringlist l;
        string::size_type sep_length = sep.length();

        string::size_type from = 0;
        for(;;)
        {
            string::size_type to = s.find(sep, from);
            if (to == string::npos)
            {
                if (from < s.length() || !ignore_empty)
                    l.push_back(string(s, from));
                return l;
            }

            if (to > from || !ignore_empty)
                l.push_back(string(s, from, to - from));
            from = to + sep_length;
        }
    }

    std::string join(stringlist const& l, std::string const& sep = " ")
    {
        using std::string;

        if (l.empty())
            return "";

        string s = l.front();

        stringlist::const_iterator const end = l.end();
        stringlist::const_iterator it = l.begin();
        for (++it; it != l.end(); ++it)
            s += sep + *it;
        return s;
    }

    std::string upcase(std::string const& s)
    {
        using std::string;
        string ret(s);
        string::iterator it, end = ret.end();
        for (it = ret.begin(); it != ret.end(); ++it)
            *it = toupper(*it);

        return ret;
    }
}


