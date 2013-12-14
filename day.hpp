#pragma once

#include <iterator>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/iterator/iterator_facade.hpp>

namespace greg = boost::gregorian;

class day_iterator
	: public boost::iterator_facade <
	day_iterator
	, greg::date
	, boost::forward_traversal_tag
	>
{
public:

	using iterator_category = std::forward_iterator_tag;

	day_iterator()
		: m_date()
	{}

	day_iterator(greg::date d)
		: m_date(d)
	{}

private:
	friend class boost::iterator_core_access;

	void increment() { m_date = m_date + greg::date_duration(1); }

	bool equal(const day_iterator& other) const
	{
		return this->m_date == other.m_date;
	}

	greg::date& dereference() const
	{
		//return greg::date(this->m_date);
		return const_cast<day_iterator*>(this)->m_date;
	}

	greg::date m_date;
};


#include <boost/range/adaptor/argument_fwd.hpp>
#include <boost/range/iterator_range.hpp>




struct day_range : public boost::iterator_range<day_iterator>
{
	day_range(const greg::date& begin, const greg::date& end) :
		boost::iterator_range<day_iterator>(day_iterator(begin), day_iterator(end))
	{}

	day_range(day_iterator begin, day_iterator end) :
		boost::iterator_range<day_iterator>(begin, end)
	{}
};

