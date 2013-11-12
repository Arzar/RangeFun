#pragma once

#include "chunkby.hpp"
#include "day.hpp"

namespace boost
{

	namespace range_detail
	{
		struct ByMonth
		{
			greg::gregorian_calendar::month_type current_month;

			ByMonth() : current_month(1){}
			ByMonth(const greg::date& d) : current_month(d.month()){}

			bool operator()(const greg::date& d) const
			{
				return d.month() == current_month;
			}
		};

		struct ByWeek
		{
			greg::date m_start_week;
			ByWeek(){}
			ByWeek(const greg::date& d) :m_start_week(d){}
			bool operator()(const greg::date& d) const
			{
				if (d == m_start_week)
					return true;
				else
					return d.day_of_week().as_enum() != greg::Monday;
			}
		};

		struct bymonth_t{};
		struct byweek_t{};
		template< class T >
		struct join_holder : holder<T>
		{
			join_holder(T r) : holder<T>(r)
			{ }
		};


		template <class InputRng>
		chunkby_range<typename InputRng::iterator, ByMonth>
		operator|(InputRng& rng, const bymonth_t& f)
		{
			return doChunkBy(rng, ByMonth());
		}

		template <class InputRng>
		chunkby_range<typename InputRng::iterator, ByWeek>
		operator|(InputRng& rng, const byweek_t& f)
		{
			return doChunkBy(rng, ByWeek());
		}

		template <class InputRng, class Value>
		typename range_value <InputRng>::type 
		operator|(InputRng& rng, const join_holder<Value>& jh)
		{
			// Define working types
			typedef typename range_value<InputRng>::type ResultT;
			typedef typename range_const_iterator<InputRng>::type InputIteratorT;

			// Parse input
			InputIteratorT itBegin = ::boost::begin(rng);
			InputIteratorT itEnd = ::boost::end(rng);

			// Construct container to hold the result
			ResultT result;

			// Append first element
			if (itBegin != itEnd)
			{
				result += *itBegin;
				++itBegin;
			}

			for (; itBegin != itEnd; ++itBegin)
			{
				// Add separator
				result += jh.val;
				// Add element
				result += *itBegin;
			}

			return result;
		}

		const bymonth_t bymonth = {};
		const byweek_t byweek = {};

	}

	namespace adaptors
	{
		using ::boost::range_detail::bymonth;
		using ::boost::range_detail::byweek;

		namespace
		{
			const range_detail::forwarder<range_detail::join_holder>
				join =
				range_detail::forwarder<range_detail::join_holder>();
		}
	}

}