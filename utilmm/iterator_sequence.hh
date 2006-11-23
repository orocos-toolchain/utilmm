#ifndef UTILMM_ITERATOR_SEQUENCE_HPP
#define UTILMM_ITERATOR_SEQUENCE_HPP
#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/iterator_traits.hpp>


namespace utilmm {
    template<typename It1, typename It2>
    class iterator_sequence : 
	public boost::iterator_facade<
	    iterator_sequence<It1, It2>,
	    typename It1::value_type,
	    typename boost::iterator_traversal<It1>::type>
    {
	typedef iterator_sequence<It1, It2> Self;

    public:
	iterator_sequence(
		It1 first, It1 first_end,
		It2 second_begin, It2 second)
	    : m_first(first), m_first_end(first_end)
	    , m_second_begin(second_begin), m_second(second) {}
	iterator_sequence() {}

    private:
	friend class boost::iterator_core_access;

	It1 m_first, m_first_end;
	It2 m_second_begin, m_second;

	typename Self::reference dereference() const
	{
	    if (m_first == m_first_end)
		return *m_second;
	    else
		return *m_first;
	}
	bool equal(Self x) const
	{ return (x.m_first == m_first && x.m_second == m_second); }
	void increment()
	{
	    if (m_first == m_first_end)
		++m_second;
	    else
		++m_first;
	}
	void decrement()
	{
	    if (m_second == m_second_begin)
		--m_first;
	    else
		--m_second;
	}
	void advance(typename Self::difference_type i)
	{
	    if (i > 0)
	    {
		typename Self::difference_type remains = m_first_end - m_first;
		if (remains > i)
		    m_first += i;
		else
		{
		    m_first = m_first_end;
		    m_second += i - remains;
		}
	    }
	    else
	    {
		typename Self::difference_type remains = m_second_begin - m_second;
		if (remains < i)
		    m_second += i;
		else
		{
		    m_second = m_second_begin;
		    m_first += i - remains;
		}
	    }
	}

	typename Self::difference_type distance_to(Self x) const
	{ return (x.m_first - m_first) + (x.m_second - m_second); }
    };
}
#endif
