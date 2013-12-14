#pragma once

#include "day.hpp"


template <typename Iterator, typename Predicate>
struct Chunker
{
	Chunker(Iterator begin, Iterator end) :m_subrange(begin, begin), m_end(end)
	{
		move_to_next_subrange();
	}

	day_range& subrange()
	{
		return m_subrange;
	}

	bool move_to_next_subrange()
	{
		if (m_subrange.end() == m_end)
			return false;

		Iterator begin_new_subrange = m_subrange.end();
		Iterator end_new_subrange = find_next_subrange(begin_new_subrange, Predicate(*begin_new_subrange));

		m_subrange = day_range(begin_new_subrange, end_new_subrange);
		return true;
	}
private:
	Iterator find_next_subrange(Iterator it, Predicate pred)
	{
		while (it != m_end && pred(*it))
		{
			it++;
		}

		return it;
	}

	day_range m_subrange;
	Iterator m_end;
};

template <typename Iterator, typename Predicate>
class chunkby_iterator
	: public boost::iterator_facade <
	chunkby_iterator<Iterator, Predicate>
	, day_range
	, boost::forward_traversal_tag
	>
{

public:

	chunkby_iterator() :m_chunker(nullptr){}
	chunkby_iterator(Chunker<Iterator, Predicate>* chuncker)
		: m_chunker(chuncker)
	{
	}

private:
	friend class boost::iterator_core_access;

	void increment()
	{
		bool can_continue = m_chunker->move_to_next_subrange();
		if (can_continue == false)
			m_chunker = nullptr; // we hit the end of the range
	}

	bool equal(const chunkby_iterator& other) const
	{
		return this->m_chunker == other.m_chunker;
	}

	day_range& dereference() const
	{
		return m_chunker->subrange();
	}

	Chunker<Iterator, Predicate>* m_chunker;
};


namespace boost
{
	namespace range_detail
	{
		template <typename Iterator, typename Predicate>
		using chunkby_range_base = boost::iterator_range<chunkby_iterator<Iterator, Predicate>>;

		template <typename Iterator, typename Predicate>
		struct chunkby_range : public boost::iterator_range<chunkby_iterator<Iterator, Predicate>>
		{
			using iterator = chunkby_iterator<Iterator, Predicate>;

			Chunker<Iterator, Predicate> m_chuncker;

			chunkby_range(Iterator begin, Iterator end) :
				boost::iterator_range<chunkby_iterator<Iterator, Predicate>>(iterator(&m_chuncker), iterator()),
				m_chuncker(begin, end)
			{}
		};

		template <typename Range, typename Predicate>
		chunkby_range<typename Range::iterator, Predicate> doChunkBy(const Range& r, Predicate p)
		{
			return chunkby_range<typename Range::iterator, Predicate>(r.begin(), r.end());
		}

		template< class T >
		struct chunkby_holder : holder<T>
		{
			chunkby_holder(T r) : holder<T>(r)
			{ }
		};

		template< class InputRng, class Predicate >
		inline chunkby_range<typename InputRng::iterator, Predicate> operator| (InputRng& r, const chunkby_holder<Predicate>& f)
		{
			return doChunkBy(r, f.val);
		}
	}


	using range_detail::chunkby_range;

	namespace adaptors
	{
		namespace
		{
			const range_detail::forwarder<range_detail::chunkby_holder>
				chunkBy = range_detail::forwarder<range_detail::chunkby_holder>();
		}
	}
}
