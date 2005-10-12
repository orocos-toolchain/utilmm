#include "utilmm/configfile/configset.hh"
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace utilmm;

config_set::config_set(config_set* parent)
    : m_parent(parent) {}

config_set::~config_set()
{ 
    clear(); 
}
    
void config_set::clear() 
{
    m_values.clear();
    for (ChildMap::iterator it = m_children.begin(); it != m_children.end(); ++it)
        delete it -> second;
    m_children.clear();
}
    
const config_set* config_set::get_parent() const { return m_parent; } 
config_set* config_set::get_parent() { return m_parent; } 
list<const config_set*> config_set::get_children(const string& name) const
{
    typedef list<const config_set *> SetList;
    pair<ChildMap::const_iterator, ChildMap::const_iterator>
        range = m_children.equal_range(name);

    ChildMap::const_iterator
        it = range.first,
        end = range.second;
    
    SetList ret;
    while (it != end)
    {
        ret.push_back(it -> second);
        ++it;
    }
    return ret;
}

bool config_set::exists(const string& name) const
{
    return 
        m_values.find(name) != m_values.end()
        || m_children.find(name) != m_children.end();
}

void config_set::insert(string const& name, string const& value)
{ m_values.insert( make_pair(name, value) ); }
void config_set::insert(string const& name, config_set const* child)
{ m_children.insert( make_pair(name, child) ); }



namespace
{
    template<typename T> T convert(const std::string& value)
    { return boost::lexical_cast<T>(value); }
    template<> bool convert<bool>(const std::string& value)
    {
        if (value == "true" || value == "1")
            return true;
        if (value == "false" || value == "0")
            return false;

        throw boost::bad_lexical_cast();
    }
   
    template<typename T>
    list<T> convert_list(const list<string>& values)
    {
        try
        {
            list<T> ret;
            for (list<string>::const_iterator it = values.begin(); it != values.end(); ++it)
            {
                T new_val = convert<T>(*it);
                ret.push_back(new_val);
            }
            return ret;
        }
        catch(boost::bad_lexical_cast&)
        { return list<T>(); }
    }

    template<typename T>
    T  get_value(const std::string& name, T defval, const config_set* config)
    { 
        list<string> strlist = config -> get_list_string(name);
        if (strlist.empty())
            return defval;
        list<T> Tlist = convert_list<T>(strlist);
        if (Tlist.empty())
            return defval;

        return Tlist.front();
    }
}

list<string> config_set::get_list_string(const string& name) const
{
    list<string> values;
    for(ValueMap::const_iterator it = m_values.find(name); it != m_values.end() && it->first == name; ++it)
        values.push_back(it->second);
    return values;
}
list<int>    config_set::get_list_int(const string& name) const
{ return convert_list<int> (get_list_string(name)); }
list<bool>    config_set::get_list_bool(const string& name) const
{ return convert_list<bool>(get_list_string(name)); }


string config_set::get_string(const string& name, const string& defval) const
{ return get_value<std::string>(name, defval, this); }
int config_set::get_int(const string& name, int defval) const
{ return get_value<int>(name, defval, this); }
bool config_set::get_bool(const string& name, bool defval) const
{ return get_value<bool>(name, defval, this); }

