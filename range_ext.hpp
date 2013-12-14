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
		operator|(const InputRng& rng, const bymonth_t& f)
		{
			return doChunkBy(rng, ByMonth());
		}

		template <class InputRng>
		chunkby_range<typename InputRng::iterator, ByWeek>
		operator|(const InputRng& rng, const byweek_t& f)
		{
			return doChunkBy(rng, ByWeek());
		}

		template <class InputRng, class Value>
		typename range_value <InputRng>::type
		operator|(const InputRng& rng, const join_holder<Value>& jh)
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



// Boost.Range 2.0 Extension library
// via PStade Oven Library
//
// Copyright Akira Takahashi 2011-2013.
// Copyright Shunsuke Sogame 2005-2007.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_RANGE_ACCESS_FRONT_INCLUDE
#define BOOST_RANGE_ACCESS_FRONT_INCLUDE

#include <boost/config.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/empty.hpp>
#include <boost/range/reference.hpp>
#include <boost/range/value_type.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/assert.hpp>
#include <boost/optional.hpp>

namespace boost {

    namespace range_detail {

        struct front_t {
            template <class Signature>
            struct result;

            template <class F, class SinglePassRange>
            struct result<F(SinglePassRange&)> :
                range_reference<
                    typename remove_reference<SinglePassRange>::type
                > {};

            template <class SinglePassRange>
            typename range_reference<SinglePassRange>::type
                operator()(SinglePassRange& rng) const
            {
                BOOST_ASSERT(!boost::empty(rng));
                return *boost::begin(rng);
            }

            template <class SinglePassRange>
            typename range_reference<const SinglePassRange>::type
                operator()(const SinglePassRange& rng) const
            {
                BOOST_ASSERT(!boost::empty(rng));
                return *boost::begin(rng);
            }
        };

        const front_t front = {};

        template <class SinglePassRange>
        typename range_reference<SinglePassRange>::type
            operator|(SinglePassRange& rng, const front_t& f)
        {
            return f(rng);
        }

        template <class SinglePassRange>
        typename range_reference<const SinglePassRange>::type
            operator|(const SinglePassRange& rng, const front_t& f)
        {
            return f(rng);
        }
    } // namespace range_detail

    namespace range { namespace access {
        using ::boost::range_detail::front;
    }} // namespace range::access
}

#endif // BOOST_RANGE_ACCESS_FRONT_INCLUDE





