#include "utilmm/configfile/configset.hh"
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace utilmm;

config_set::config_set(config_set* parent_)
    : m_parent(parent_) {}

config_set::~config_set()
{ 
    clear(); 
}
    
void config_set::clear() 
{
    m_values.clear();
    while (!m_children.empty())
    {
        ChildMap::iterator it = m_children.begin();
        delete it->second;
        m_children.erase(it);
    }
}
    
const config_set* config_set::parent() const { return m_parent; } 
config_set* config_set::parent() { return m_parent; } 
list<const config_set*> config_set::children(const string& name) const
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
void config_set::insert(string const& name, list<string> const& value)
{ 
    list<string>::const_iterator it, end = value.end();
    for (it = value.begin(); it != value.end(); ++it)
        insert(name, *it);
}
void config_set::insert(string const& name, config_set const* child)
{ m_children.insert( make_pair(name, child) ); }
void config_set::set(string const& name, string const& value)
{
    m_values.erase(name);
    insert(name, value);
}
void config_set::set(string const& name, list<string> const& value)
{
    m_values.erase(name);
    insert(name, value);
}



template<> bool config_set::convert(const std::string& value)
{
    if (value == "true" || value == "1")
        return true;
    if (value == "false" || value == "0")
        return false;

    throw boost::bad_lexical_cast();
}

template<>
config_set::stringlist config_set::get(const string& name, stringlist const& defval, 
        boost::enable_if< details::is_list<config_set::stringlist> >::type* dummy) const
{
    list<string> values;
    for(ValueMap::const_iterator it = m_values.find(name); it != m_values.end() && it->first == name; ++it)
        values.push_back(it->second);

    if(values.empty())
        return defval;

    return values;
}

