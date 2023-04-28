//
// Platform.Thread.Parallel.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Parallel interface.
//
module;

#include <Furud.hpp>
#include <ppl.h>



export module Furud.Platform.Thread.Parallel;

namespace Furud::Internal
{
	template <typename F>
	concept is_callable = requires(F const& function) { function(0); };
}



export namespace Furud
{
	/**
	 * @brief    Concurrency utility.
	 * @details  并发工具类。
	 */
	namespace IParallel
	{
		/**
		 * @brief    Iterates over a range of indices and executes a
		 *           user-supplied function at each iteration, in parallel.
		 * @param    start  -  the first index to be included in the iteration.
		 * @param    end    -  the last index (excluded).
		 * @details  并行 for。
		 */
		void For(int32_t start, int32_t end, Internal::is_callable auto const& function)
		{
			Concurrency::parallel_for(start, end, function, Concurrency::auto_partitioner{});
		}


		/**
		 * @brief    Iterates over a range of indices and executes a
		 *           user-supplied function at each iteration, in parallel.
		 * @param    num  -  the total number of iteration.
		 * @details  并行 for。
		 */
		void For(int32_t num, Internal::is_callable auto const& function)
		{
			Concurrency::parallel_for(0, num, function, Concurrency::auto_partitioner{});
		}


		/**
		 * @brief    Sorts the specified range in parallel.
		 * @tparam   T  -  Data type.
		 * @details  并行排序。
		 */
		template <typename T>
		void Sort(T* begin, T* end)
		{
			Concurrency::parallel_sort(begin, end);
		}


		/**
		 * @brief    Sorts the specified range in parallel.
		 * @tparam   T  -  Data type.
		 * @tparam   F  -  Compare function type.
		 * @details  并行排序。
		 */
		template <typename T, typename F>
		void Sort(T* begin, T* end, const F& comparator)
		{
			Concurrency::parallel_sort(begin, end, comparator);
		}
	};
}