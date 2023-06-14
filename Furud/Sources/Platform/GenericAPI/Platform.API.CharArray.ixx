//
// Platform.API.CharArray.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Character array.
//
module;

#include <Furud.hpp>
#include <type_traits>
#include <stdlib.h>
#include <ctype.h>
#include <utility>



export module Furud.Platform.API.CharArray;

/** Forward declaration. */
namespace Furud::Internal
{
	template <typename C, typename S>
	class TCharArray;

	template <typename C, typename S>
	class TCharArrayView;
}



/** Template char builder. */
namespace Furud::Internal
{
	template <typename TChar, typename TSize>
	struct TStringBuilder
	{
		// Invalid(Non) position.
		// -1, 表示无效下标
		static constexpr TSize npos { static_cast<TSize>(-1) };


		/**
		 * @brief    Determines whether an char represents a space character.
		 * @returns  True if `ch` is a white-space character (0x09 - 0x0D or 0x20).
		 * @details  判断是不是空格符号。(支持常量求值)
		 */
		furud_nodiscard static constexpr bool IsWhitespace(const TChar& ch) noexcept
		{
			if (std::is_constant_evaluated())
			{
				// horizontal tab  (0x09, '\t').
				// line feed       (0x0a, '\n').
				// vertical tab    (0x0b, '\v').
				// form feed       (0x0c, '\f')
				// carriage return (0x0d, '\r')
				if (static_cast<uint16_t>(ch) >= 0x09ui16 && static_cast<uint16_t>(ch) <= 0x0Dui16)
				{
					return true;
				}

				// space  (0x20, ' ').
				if (static_cast<uint16_t>(ch) == 0x20ui16)
				{
					return true;
				}

				return false;
			}
			else
			{
				if constexpr (std::same_as<TChar, char>)
				{
					return ::isspace(static_cast<uint32_t>(ch));
				}
				else
				{
					return ::iswspace(ch);
				}
			}
		}


		/**
		 * @brief    Convert the character to uppercase.
		 * @details  转换成大写。(支持常量求值)
		 */
		furud_nodiscard static constexpr TChar ToUpper(const TChar& ch) noexcept
		{
			if (std::is_constant_evaluated())
			{
				return (TChar)(std::make_unsigned_t<TChar>(ch) - ((uint32_t(ch) - 'a' < 26u) << 5));
			}
			else
			{
				if constexpr (std::same_as<TChar, char>)
				{
					return (TChar)::toupper(ch);
				}
				else
				{
					return (TChar)::towupper(ch);
				}
			}
		}


		/**
		 * @brief    Convert the character to lowercase.
		 * @details  转换成小写。(支持常量求值)
		 */
		furud_nodiscard static constexpr TChar ToLower(const TChar& ch) noexcept
		{
			if (std::is_constant_evaluated())
			{
				return (TChar)(std::make_unsigned_t<TChar>(ch) + ((uint32_t(ch) - 'A' < 26u) << 5));
			}
			else
			{
				if constexpr (std::same_as<TChar, char>)
				{
					return (TChar)::tolower(ch);
				}
				else
				{
					return (TChar)::towlower(ch);
				}
			}
		}


		/**
		 * @brief    Copies bytes between strings.
		 * @details  字符串拷贝。(支持常量求值)
		 */
		static constexpr void Copy
			( TChar* furud_restrict dst
			, TSize dstCount
			, const TChar* furud_restrict src
			, TSize srcCount
		) noexcept
		{
			if (std::is_constant_evaluated())
			{
				for (TSize i = 0; i != dstCount < srcCount ? dstCount : srcCount; ++i)
				{
					dst[i] = src[i];
				}
			}
			else
			{
				if constexpr (std::same_as<TChar, char>)
				{
					::memcpy(dst, src, dstCount < srcCount ? dstCount : srcCount);
				}
				else
				{
					::wmemcpy(dst, src, dstCount < srcCount ? dstCount : srcCount);
				}
			}
		}


		/**
		 * @brief    Copies bytes between strings.
		 * @details  字符串拷贝。空余部分会初始化为0。(支持常量求值)
		 */
		static constexpr void Assign
			( TChar* furud_restrict dst
			, TSize dstCount
			, const TChar* furud_restrict src
			, TSize srcCount
		) noexcept
		{
			if (std::is_constant_evaluated())
			{
				for (TSize i = 0; i != dstCount < srcCount ? dstCount : srcCount; ++i)
				{
					dst[i] = src[i];
				}

				if (dstCount > srcCount)
				{
					for (TSize i = srcCount; i != dstCount; ++i)
					{
						dst[i] = TChar(0);
					}
				}
			}
			else
			{
				if constexpr (std::same_as<TChar, char>)
				{
					::memcpy(dst, src, dstCount < srcCount ? dstCount : srcCount);
					if (dstCount > srcCount)
					{
						::memset(dst + srcCount, 0, dstCount - srcCount);
					}
				}
				else
				{
					::wmemcpy(dst, src, dstCount < srcCount ? dstCount : srcCount);
					if (dstCount > srcCount)
					{
						::wmemset(dst + srcCount, 0, dstCount - srcCount);
					}
				}
			}
		}


		/**
		 * @brief    Sets a buffer to a specified character.
		 * @details  字符串赋值。(支持常量求值)
		 */
		static constexpr void Assign
			( TChar* const furud_restrict str
			, const TChar& ch
			, TSize count
		) noexcept
		{
			if (std::is_constant_evaluated())
			{
				for (TSize i = 0; i != count; ++i)
				{
					str[i] = ch;
				}
			}
			else
			{
				if constexpr (std::same_as<TChar, char>)
				{
					::memset(str, ch, count);
				}
				else
				{
					::wmemset(str, ch, count);
				}
			}
		}


		/**
		 * @brief    Gets the length of a string.
		 * @returns  Zero if string is nullptr.
		 * @details  计算字符串长度。(支持常量求值)
		 */
		furud_nodiscard static constexpr TSize Length(const TChar* furud_restrict str) noexcept
		{
			if constexpr (std::same_as<TChar, char>)
			{
				return str == nullptr ? 0ull : static_cast<TSize>(__builtin_strlen(str));
			}
			else
			{
				return str == nullptr ? 0ull : static_cast<TSize>(__builtin_wcslen(str));
			}
		}


		/**
		 * @brief    Compares up to the specified count of characters of two strings.
		 * @returns  < 0  `str1` less than `str2`.
		 *           = 0  `str1` equals `str2`.
		 *           > 0  `str1` greater than `str2`.
		 * @details  字符串比较。(支持常量求值)
		 */
		furud_nodiscard static constexpr int32_t Compare
			( const TChar* furud_restrict str1
			, const TChar* furud_restrict str2
			, TSize count
		) noexcept
		{
			if constexpr (std::same_as<TChar, char>)
			{
				return __builtin_memcmp(str1, str2, count);
			}
			else
			{
				return __builtin_wmemcmp(str1, str2, count);
			}
		}

		/**
		 * @brief    Locate the character in a buffer.
		 * @returns  Pointer to the first location of `ch` if successful,
		 *           otherwise it returns nullptr.
		 * @details  字符串查找。(支持常量求值)
		 */
		furud_nodiscard static constexpr TChar* Locate
			( TChar* furud_restrict str
			, const TChar& ch
			, const TSize size
		) noexcept
		{
			if constexpr (std::same_as<TChar, char>)
			{
				return __builtin_char_memchr(str, ch, size);
			}
			else
			{
				return __builtin_wmemchr(str, ch, size);
			}
		}


		/**
		 * @brief    Locate the character in a buffer.
		 * @returns  Pointer to the first location of `ch` if successful,
		 *           otherwise it returns nullptr.
		 * @details  字符串查找。(支持常量求值)
		 */
		furud_nodiscard static constexpr const TChar* Locate
			( const TChar* furud_restrict str
			, const TChar& ch
			, const TSize size
		) noexcept
		{
			if constexpr (std::same_as<TChar, char>)
			{
				return __builtin_char_memchr(str, ch, size);
			}
			else
			{
				return __builtin_wmemchr(str, ch, size);
			}
		}


		/**
		 * @brief    Find the character in a buffer.
		 * @returns  Index of the first location of `ch` if successful,
		 *           otherwise it returns `npos`.
		 * @details  字符串查找。(支持常量求值)
		 */
		furud_nodiscard static constexpr TSize Find
			( const TChar* furud_restrict str
			, const TChar& ch
			, const TSize size
			, const TSize from
		) noexcept
		{
			if (from < size)
			{
				const TChar* found = Locate(str + from, ch, size - from);
				if (found)
				{
					return static_cast<TSize>(found - str);
				}
			}

			return npos;
		}


		/**
		 * @brief    Find characters in a buffer.
		 * @returns  Index of the first location of `ch` if successful,
		 *           otherwise it returns `npos`.
		 * @details  字符串查找。(支持常量求值)
		 */
		furud_nodiscard static constexpr TSize Find
			( const TChar* furud_restrict str
			, const TChar* furud_restrict search
			, TSize strSize
			, TSize searchSize
			, TSize from
		) noexcept
		{
			if (searchSize > strSize || from > strSize - searchSize)
			{
				return npos;
			}

			if (searchSize == 0 || !search)
			{
				return from;
			}

			const auto tryEnd = str + (strSize - searchSize) + 1;
			for (auto tryPtr = str + from; ; ++tryPtr)
			{
				tryPtr = Locate(tryPtr, search[0], static_cast<TSize>(tryEnd - tryPtr));

				if (!tryPtr)
				{
					return npos;
				}

				if (Compare(tryPtr, search, searchSize) == 0)
				{
					return static_cast<TSize>(tryPtr - str);
				}
			}
		}


		/**
		 * @brief    Replaces characters in a buffer.
		 * @details  字符串替换。(支持常量求值)
		 */
		static constexpr void Replace
			( TChar* furud_restrict str
			, const TChar* furud_restrict search
			, const TChar* furud_restrict replacement
			, TSize strSize
			, TSize searchSize
		) noexcept
		{
			if (searchSize > strSize || !search || !replacement)
			{
				return;
			}

			if (searchSize == 0 || *search == TChar(0) || *replacement = TChar(0))
			{
				return;
			}

			TChar* tryPtr = search;
			const TChar* tryEnd = str + (strSize - searchSize) + 1;
			while (tryPtr < tryEnd)
			{
				tryPtr = Locate(tryPtr, search[0], static_cast<TSize>(tryEnd - tryPtr));

				if (!tryPtr)
				{
					return;
				}

				if (Compare(tryPtr, search, searchSize) == 0)
				{
					for (TSize i = 0; i < searchSize; ++i)
					{
						tryPtr[i] = replacement[i];
					}
					tryPtr += searchSize;
				}
				else
				{
					++tryPtr;
				}
			}
		}


		/**
		 * @brief    Converts a string to 64-bit floating point (double).
		 * @returns  0 if failure.
		 * @details  字符串转浮点数。
		 */
		furud_nodiscard furud_inline static double ToDouble(const TChar* str) noexcept
		{
			if constexpr (std::same_as<TChar, char>)
			{
				return ::atof(str);
			}
			else
			{
				return ::_wtof(str);
			}
		}


		/**
		 * @brief    Converts a string to 32-bit floating point (float).
		 * @returns  0 if failure.
		 * @details  字符串转浮点数。
		 */
		furud_nodiscard furud_inline static float ToFloat(const TChar* str) noexcept
		{
			return static_cast<float>(ToDouble(str));
		}


		/**
		 * @brief    Converts a string to a 64-bit integer.
		 * @returns  0 if failure.
		 *           LLONG_MIN/LLONG_MAX if overflow.
		 * @details  字符串转整型。
		 */
		furud_nodiscard furud_inline static int64_t ToInt64(const TChar* str) noexcept
		{
			if constexpr (std::same_as<TChar, char>)
			{
				return ::_atoi64(str);
			}
			else
			{
				return ::_wtoi64(str);
			}
		}


		/**
		 * @brief    Converts a string to an unsigned 64-bit integer.
		 * @returns  0 if failure.
		 *           ULLONG_MAX if overflow.
		 * @details  字符串转整型。
		 */
		furud_nodiscard furud_inline static uint64_t ToUint64(const TChar* str) noexcept
		{
			if constexpr (std::same_as<TChar, char>)
			{
				return ::_strtoui64(str, nullptr, 10);
			}
			else
			{
				return ::_wcstoui64(str, nullptr, 10);
			}
		}


		/**
		 * @brief    Converts a string to a 32-bit integer.
		 * @returns  0 if failure.
		 *           LONG_MIN/LONG_MAX if overflow.
		 * @details  字符串转整型。
		 */
		furud_nodiscard furud_inline static int32_t ToInt32(const TChar* str) noexcept
		{
			if constexpr (std::same_as<TChar, char>)
			{
				return ::atoi(str);
			}
			else
			{
				return ::_wtoi(str);
			}
		}


		/**
		 * @brief    Converts a string to an unsigned 32-bit integer.
		 * @returns  0 if failure.
		 *           ULONG_MAX if overflow.
		 * @details  字符串转整型。
		 */
		furud_nodiscard furud_inline static uint32_t ToUint32(const TChar* str) noexcept
		{
			if constexpr (std::same_as<TChar, char>)
			{
				return ::strtoul(str, nullptr, 10);
			}
			else
			{
				return ::wcstoul(str, nullptr, 10);
			}
		}
	};
}



/** Template string allocator. */
namespace Furud::Internal
{
	template <typename TChar, typename TSize>
	struct TStringAllocator
	{
		/**
		 * @brief    Calculate the capacity growth.
		 * @details  计算内存空间增长因子。
		 */
		static constexpr TSize CalculateCapacityGrowth(TSize capacity) noexcept
		{
			// Default minimal capacity.
			TSize growth = 4;

			// Allocate capacity for the array proportional to its size.
			growth += capacity + ((capacity + 1) >> 1);

			// Be careful to overflow here.
			TSize newCapacity = growth > capacity ? growth : capacity;

			return newCapacity;
		}


		/**
		 * @brief   Release the storage.
		 * @detals  释放内存。
		 */
		furud_inline static void Free(TChar* oldData)
		{
			if (oldData != nullptr) delete[] oldData;
		}


		/**
		 * @brief   Allocate a piece of uninitialized storage.
		 * @detals  申请内存。不会初始化这段内存。
		 */
		furud_inline static void Alloc(const TSize& inStorageSize, TChar** furud_restrict outData, TSize* furud_restrict outCapacity)
		{
			*outData = new TChar[inStorageSize];
			*outCapacity = inStorageSize;
		}


		/**
		 * @brief   Reallocate storage and initializes it.
		 * @detals  申请并初始化内存。
		 */
		furud_inline static void Realloc
			( TChar** furud_restrict inOutData
			, TSize* furud_restrict outCapacity
			, TSize newCapacity
			, TSize oldReservedSize
		)
		{
			TChar* oldData = *inOutData;
			TChar* newData = nullptr;
			if (newCapacity != 0)
			{
				newData = new TChar[newCapacity];
				if (oldReservedSize != 0)
				{
					TStringBuilder<TChar, TSize>::Assign(newData, newCapacity, oldData, oldReservedSize);
				}
			}
			Free(oldData);
			*inOutData = newData;
			*outCapacity = newCapacity;
		}
	};
}



/** Template char array view. */
namespace Furud::Internal
{
	/**
	 * @brief    This template describes the minimal common interface necessary to read character than std::string_view.
	 *           It provides const access to the underlying data. Please ensure that
	 *           the underlying data is valid for the lifetime of this template.
	 *           This template supports manifestly constant-evaluated expressions.
	 *           In particular, it is noniterable.
	 * @tparam   C  -  Char type.
	 * @tparam   S  -  Size type.
	 * @details  一个更简易通用的字符视图，可替代std::string_view。有更常用的接口，且完全支持常量求值。
	 */
	template <typename C, typename S>
	class TCharArrayView
	{
	public:
		using TChar = C;
		using TSize = S;
		struct Pair { TCharArrayView<C, S> left, right; };
		static constexpr TSize npos = TSize(-1);


	protected:
		using IStringBuilder = TStringBuilder<TChar, TSize>;


	protected:
		const TChar* data;
		const TSize size;


	public:
		constexpr TCharArrayView() noexcept
			: data(nullptr)
			, size(0)
		{}

		constexpr TCharArrayView(const TChar* inData) noexcept
			: data(inData)
			, size((TSize)IStringBuilder::Length(inData))
		{}

		constexpr TCharArrayView(const TChar* inData, const TSize& inSize) noexcept
			: data(inData)
			, size(std::max<TSize>(inSize, 0))
		{}

		constexpr TCharArrayView(const TChar* inFirst, const TChar* inLast) noexcept
			: data(inFirst)
			, size(std::max<TSize>(TSize(inLast - inFirst), 0))
		{}

		constexpr TCharArrayView(const TCharArray<C, S>& inArr) noexcept;

		constexpr TCharArrayView(const TCharArrayView&) noexcept = default;

		constexpr TCharArrayView& operator = (const TCharArrayView&) noexcept = delete;


	public:
		furud_nodiscard constexpr const TChar* Data() const noexcept
		{
			return data;
		}

		furud_nodiscard constexpr TSize Size() const noexcept
		{
			return size;
		}

		furud_nodiscard constexpr bool IsEmpty() const noexcept
		{
			return size == 0;
		}

		constexpr const TChar& operator[] (const TSize& index) noexcept
		{
			return data[index];
		}

		constexpr const TChar& operator[] (const TSize& index) const noexcept
		{
			return data[index];
		}


	public:
		furud_nodiscard constexpr bool IsEquals(const TChar* rhs) const noexcept
		{
			if (size == 0) return rhs == nullptr;
			return IStringBuilder::Compare(data, rhs, size) == 0;
		}

		furud_nodiscard constexpr bool IsEquals(const TCharArrayView& rhs) const noexcept
		{
			if (size == 0)        return rhs.size == 0;
			if (data == rhs.data) return size == rhs.size;
			return IStringBuilder::Compare(data, rhs.data, size) == 0;
		}

		furud_nodiscard constexpr bool operator == (const TChar* rhs) const noexcept
		{
			return IsEquals(rhs);
		}

		furud_nodiscard friend constexpr bool operator == (const TChar* lhs, const TCharArrayView& rhs) noexcept
		{
			return rhs.IsEquals(lhs);
		}

		furud_nodiscard constexpr bool operator == (const TCharArrayView& rhs) const noexcept
		{
			return IsEquals(rhs);
		}


	public:
		/**
		 * @brief    Removes whitespace character from the start of this array.
		 * @details  删除开头位置的空格符号。
		 */
		furud_nodiscard constexpr TCharArrayView TrimLeft() const noexcept
		{
			TSize spaceNum = 0;
			for (TSize i = 0; i < size; ++i)
			{
				if (!IStringBuilder::IsWhitespace(data[i]))
				{
					break;
				}
				++spaceNum;
			}
			return { data + spaceNum, size - spaceNum };
		};

		/**
		 * @brief    Removes whitespace character from the end of this array.
		 * @details  删除结尾位置的空格符号。
		 */
		furud_nodiscard constexpr TCharArrayView TrimRight() const noexcept
		{
			TSize newSize = size;
			while (newSize > 0 && IStringBuilder::IsWhitespace(data[newSize - 1]))
			{
				--newSize;
			}
			return { data, newSize };
		};

		/**
		 * @brief    Removes whitespace character from the start and end of this array.
		 * @details  删除开头和结尾位置的空格符号。
		 */
		furud_nodiscard constexpr TCharArrayView Trim() const noexcept
		{
			return TrimRight().TrimLeft();
		};


	public:
		/**
		 * @brief    Test whether starts with given character.
		 * @details  测试开头字符。
		 */
		furud_nodiscard constexpr bool StartWith(const TChar& prefix) const noexcept
		{
			return size >= 1 && data[0] == prefix;
		}

		/**
		 * @brief    Test whether ends with given character.
		 * @details  测试结尾字符。
		 */
		furud_nodiscard constexpr bool EndWith(const TChar& suffix) const noexcept
		{
			return size >= 1 && data[size - 1] = suffix;
		}

		/**
		 * @brief    Test whether ends with given characters.
		 * @details  测试开头字符。
		 */
		furud_nodiscard constexpr bool StartWith(const TCharArrayView& prefix) const noexcept
		{
			if (prefix.size <= 0 || prefix.size > size)
			{
				return false;
			}

			TCharArrayView sub{ data, prefix.size };
			return prefix == sub;
		}

		/**
		 * @brief    Test whether ends with given characters.
		 * @details  测试结尾字符。
		 */
		furud_nodiscard constexpr bool EndWith(const TCharArrayView& suffix) const noexcept
		{
			if (suffix.size <= 0 || suffix.size > size)
			{
				return false;
			}

			TSize newSize = size - suffix.size;
			TCharArrayView sub{ data + newSize, newSize };
			return suffix == sub;
		}


	public:
		/**
		 * @brief    Finds the first occurrence of the given character in this array, return it's index.
		 * @returns  `npos` if failure.
		 * @returns  查找字符，返回下标。
		 */
		furud_nodiscard constexpr TSize Find(const TChar& c, const TSize& from = 0) const noexcept
		{
			return (TSize)IStringBuilder::Find(data, c, size, from);
		}

		/**
		 * @brief    Searches the given character in this array, and returns index.
		 * @returns  `npos` if failure.
		 * @returns  查找字符，返回下标。
		 */
		furud_nodiscard constexpr TSize Find(const TCharArrayView& arr, const TSize& from = 0) const noexcept
		{
			return (TSize)IStringBuilder::Find(
				data, arr.data,
				size, arr.size,
				from
			);
		}


	public:
		/**
		 * @brief    Converts to double number.
		 * @details  转换成双精度浮点数。
		 */
		furud_nodiscard constexpr double ToDouble() const noexcept
		{
			return IStringBuilder::ToDouble(data);
		}

		/**
		 * @brief    Converts to float number.
		 * @details  转换成单精度浮点数。
		 */
		furud_nodiscard constexpr float ToFloat() const noexcept
		{
			return IStringBuilder::ToFloat(data);
		}

		/**
		 * @brief    Converts to 64-bit signed integer number.
		 * @details  转换成64位有符号整数。
		 */
		furud_nodiscard constexpr int64_t ToInt64() const noexcept
		{
			return IStringBuilder::ToInt64(data);
		}

		/**
		 * @brief    Converts to 32-bit signed integer number.
		 * @details  转换成64位有符号整数。
		 */
		furud_nodiscard constexpr int32_t ToInt32() const noexcept
		{
			return IStringBuilder::ToInt32(data);
		}


	public:
		/**
		 * @brief    Splits this array at the index of given `delim` character.
		 * @details  拆分字符。
		 */
		constexpr Pair Split(const TChar& delim) const noexcept
		{
			TSize index = (TSize)IStringBuilder::Find(data, delim, size, 0);
			if (index == npos)
			{
				return Pair{};
			}

			TSize offset = index + 1;
			return Pair
			{
				TCharArrayView { data, index },
				TCharArrayView { offset >= size ? nullptr : data + offset, offset >= size ? TSize(0) : size - offset }
			};
		}
	};
}



/** Template char array. */
namespace Furud::Internal
{
	/**
	 * @brief    This template describes the minimal common interface necessary to read character than std::string.
	 * 	         In particular, it is noniterable and noncopyable. But it can be duplicated by Clone() function.
	 * @tparam   C  -  Char type.
	 * @tparam   S  -  Size type.
	 * @details  一个更简易通用的字符串实现，可替代std::string。有更常用的接口。此外
	 *              -  禁止placement new，避免不良编程方式导致的内存泄露。
	 *              -  禁止拷贝复制行为，从而保证任何地方只能通过指针或引用使用，除非使用显示的Clone函数。
	 */
	template <typename C, typename S>
	class TCharArray
	{
	public:
		using TChar = C;
		using TSize = S;
		static constexpr TSize npos = TSize(-1);


	protected:
		using IStringBuilder = TStringBuilder<TChar, TSize>;
		using ICharAllocator = TStringAllocator<TChar, TSize>;


	protected:
		TChar* data;
		TSize size;
		TSize capacity;


	public:
		/** Noncopyable. */
		TCharArray(const TCharArray&) = delete;

		/** Noncopyable. */
		TCharArray& operator = (const TCharArray&) = delete;

		/** Disable placement new. */
		void* operator new (size_t, void*) = delete;
		void* operator new (size_t) { return ::new TCharArray; }

		constexpr TCharArray() noexcept
			: data(nullptr)
			, size(0)
			, capacity(0)
		{}

		virtual ~TCharArray()
		{
			ICharAllocator::Free(data);
			data = nullptr;
			size = capacity = 0;
		}

		furud_inline TCharArray(const TChar* inData)
		{
			size = (TSize)IStringBuilder::Length(inData);
			ICharAllocator::Alloc(size + 1, &data, &capacity);
			IStringBuilder::Assign(data, size + 1, inData, size);
		}

		furud_inline TCharArray(const TSize& inSize)
		{
			size = inSize;
			ICharAllocator::Alloc(size + 1, &data, &capacity);
			data[0] = TSize(0);
		}

		furud_inline TCharArray(const TCharArrayView<TChar, TSize>& inData)
		{
			size = inData.Size();
			ICharAllocator::Alloc(size + 1, &data, &capacity);
			IStringBuilder::Assign(data, size + 1, inData, size);
		}

		furud_inline TCharArray(const TSize& inSize, const TChar& ch)
		{
			size = inSize;
			ICharAllocator::Alloc(size + 1, &data, &capacity);
			IStringBuilder::Assign(data, ch, size);
			data[size] = TSize(0);
		}

		furud_inline TCharArray(TCharArray&& inArr) noexcept
		{
			std::swap(data, inArr.data);
			std::swap(size, inArr.size);
			std::swap(capacity, inArr.capacity);
			inArr.~TCharArray();
		}

		furud_inline TCharArray& operator = (TCharArray&& inArr) noexcept
		{
			Swap(inArr);
			inArr.~TCharArray();
			return *this;
		}

		furud_inline TCharArray Clone() noexcept
		{
			TCharArray arr;
			arr.size = size;
			ICharAllocator::Alloc(arr.size + 1, &arr.data, &arr.capacity);
			IStringBuilder::Assign(arr.data, arr.size + 1, data, size);
			return arr;
		}

		furud_inline TCharArray Clone() const noexcept
		{
			TCharArray arr;
			arr.size = size;
			ICharAllocator::Alloc(arr.size + 1, &arr.data, &arr.capacity);
			IStringBuilder::Assign(arr.data, arr.size + 1, data, size);
			return arr;
		}


	public:
		furud_nodiscard constexpr TChar* Data() noexcept
		{
			return data;
		}

		furud_nodiscard constexpr const TChar* Data() const noexcept
		{
			return data;
		}

		furud_nodiscard constexpr TSize Size() const noexcept
		{
			return size;
		}

		furud_nodiscard constexpr TSize Capacity() const noexcept
		{
			return capacity;
		}

		furud_nodiscard constexpr bool IsEmpty() const noexcept
		{
			return size == 0;
		}

		constexpr TChar& operator[] (const TSize& index) noexcept
		{
			return data[index];
		}

		constexpr const TChar& operator[] (const TSize& index) const noexcept
		{
			return data[index];
		}


	public:
		/**
		 * @brief    Clear the array. It does not reallocate the memory storage.
		 * @details  清空数组。保持内存大小不变。
		 */
		furud_inline void Clear()
		{
			size = 0;

			if (capacity)
			{
				data[size] = TChar(0);
			}
		}

		/**
		 * @brief    Empties the array. And supports reallocation of memory storage.
		 * @details  清空数组。支持重新分配更大的内存。
		 */
		furud_inline void Empty(const TSize& inCapacity = 0)
		{
			size = 0;

			if (capacity < inCapacity)
			{
				ICharAllocator::Realloc(&data, &capacity, inCapacity, 0);
			}

			if (capacity)
			{
				data[size] = TChar(0);
			}
		}

		/**
		 * @brief    Reserves the array. Then it can contain at least `inCapacity` character.
		 * @details  扩大数组内存。
		 */
		furud_inline void Reserve(const TSize& inCapacity)
		{
			// Requested the new capacity is not larger than current capacity, ignore.
			if (capacity >= inCapacity)
			{
				return;
			}

			ICharAllocator::Realloc(&data, &capacity, inCapacity, size);
		}

		/**
		 * @brief    Shrinks the array. Then it only uses smallest memory storage as possible.
		 * @details  缩紧数组内存。
		 */
		furud_inline void Shrink()
		{
			if (size + 1 != capacity)
			{
				ICharAllocator::Realloc(&data, &capacity, size + 1, size);
			}
		}

		/**
		 * @brief    Swap the array's used memory.
		 * @details  互换内存。
		 */
		constexpr void Swap(TCharArray& rhs) noexcept
		{
			std::swap(data, rhs.data);
			std::swap(size, rhs.size);
			std::swap(capacity, rhs.capacity);
		}

		/**
		 * @brief    Pushes a new character to the end of the array.
		 *           Possibly reallocating the array to fit.
		 * @details  在尾部插入一个新的字符。
		 */
		furud_inline void Push(const TChar& ch)
		{
			if (size + 1 >= capacity)
			{
				ICharAllocator::Realloc
					( &data
					, &capacity
					, ICharAllocator::CalculateCapacityGrowth(capacity)
					, size
				);
			}

			data[size++] = ch;
		}

		/**
		 * @brief    Pushes a new character to the end of the array.
		 *           Please ensure memory safety manually.
		 * @details  在尾部插入一个新的字符。
		 */
		constexpr void PushUnsafe(const TChar& ch) noexcept
		{
			data[size++] = ch;
		}

		/**
		 * @brief    Pops the character at the end of the array.
		 * @returns  0 if the array is empty.
		 * @details  弹出尾部的一个字符。
		 */
		furud_inline TChar Pop() noexcept
		{
			if (size > 0)
			{
				--size;
				TChar ch = data[size];
				data[size] = TChar(0);
				return ch;
			}

			return TChar(0);
		}

		/**
		 * @brief    Pops the character at the end of the array.
		 *           Please ensure memory safety manually.
		 * @details  弹出尾部的一个字符。
		 */
		furud_inline TChar PopUnsafe() noexcept
		{
			--size;
			TChar ch = data[size];
			data[size] = TChar(0);
			return ch;
		}

		/**
		 * @brief    Appends the specified character array to this array.
		 *           Possibly reallocating the array to fit.
		 * @details  尾部追加数组。
		 */
		furud_inline void Append(const TCharArrayView<TChar, TSize>& inArr)
		{
			if (inArr.Size() == 0)
			{
				return;
			}

			TSize newSize = size + inArr.Size();
			if (newSize >= capacity)
			{
				TSize newCapacity = 0;
				TChar* newData = nullptr;
				ICharAllocator::Alloc(ICharAllocator::CalculateCapacityGrowth(newSize), &newData, &newCapacity);
				IStringBuilder::Copy(newData, newCapacity, data, size);
				IStringBuilder::Copy(newData + size, newCapacity - size, inArr.Data(), inArr.Size());
				ICharAllocator::Free(data);
				newData[newSize] = TChar(0);
				data = newData;
				size = newSize;
				capacity = newCapacity;
			}
			else
			{
				IStringBuilder::Copy(data + size, capacity - size, inArr.Data(), inArr.Size());
				data[newSize] = TChar(0);
				size = newSize;
			}
		}

		/**
		 * @brief    Finds the first character.
		 * @returns  `npos` if failure.
		 * @returns  查找字符，返回下标。
		 */
		constexpr TSize IndexOf(const TChar& ch) noexcept
		{
			return (TSize)IStringBuilder::Find(data, ch, size, 0);
		}

		/**
		 * @brief    Converts the character to uppercase.
		 * @details  转换成大写。
		 */
		furud_inline void ToUpper() noexcept
		{
			for (TSize i = 0; i != size; ++i)
			{
				data[i] = IStringBuilder::ToUpper(data[i]);
			}
		}

		/**
		 * @brief    Converts the character to lowercase.
		 * @details  转换成小写。
		 */
		furud_inline void ToLower() noexcept
		{
			for (TSize i = 0; i != size; ++i)
			{
				data[i] = IStringBuilder::ToLower(data[i]);
			}
		}

		/**
		 * @brief    Test whether this array starts with given character.
		 * @details  测试开头字符。
		 */
		furud_nodiscard constexpr bool StartWith(const TChar& prefix) const noexcept
		{
			return size >= 1 && data[0] == prefix;
		}

		/**
		 * @brief    Test whether this array ends with given character.
		 * @details  测试结尾字符。
		 */
		furud_nodiscard constexpr bool EndWith(const TChar& suffix) const noexcept
		{
			return size >= 1 && data[size - 1] = suffix;
		}

		/**
		 * @brief    Test whether this array starts with given characters.
		 * @details  测试开头字符。
		 */
		furud_nodiscard constexpr bool StartWith(const TCharArrayView<TChar, TSize>& prefix) const noexcept
		{
			return TCharArrayView<TChar, TSize>(*this).StartWith(prefix);
		}

		/**
		 * @brief    Test whether this array ends with given characters.
		 * @details  测试结尾字符。
		 */
		furud_nodiscard constexpr bool EndWith(const TCharArrayView<TChar, TSize>& suffix) const noexcept
		{
			return TCharArrayView<TChar, TSize>(*this).EndWith(suffix);
		}

		/**
		 * @brief    Returns the sub-array from the given position for count characters.
		 * @details  获取子字符串。
		 */
		furud_nodiscard constexpr TCharArrayView<TChar, TSize> Sub(const TSize& from, const TSize& count) const noexcept
		{
			return TCharArrayView<TChar, TSize>
			{
				from < size ? data + from : nullptr,
					std::min(size - from, count)
			};
		}

		/**
		 * @brief    Replaces characters in a buffer.
		 * @details  字符串替换。(支持常量求值)
		 */
		void Replace(const TCharArrayView<TChar, TSize>& search, const TCharArrayView<TChar, TSize>& replacement) noexcept
		{
			if (search.Size() == replacement.Size())
			{
				IStringBuilder::Replace(data, search.Data(), replacement, Data(), size, search.Size());
			}
		}
	};



	template <typename C, typename S>
	constexpr TCharArrayView<C, S>::TCharArrayView(const TCharArray<C, S>& inArr) noexcept
		: data(inArr.Data())
		, size(inArr.Size())
	{}
}



export namespace Furud
{
	using AnsicharArrayView  = Internal::TCharArrayView<char, size_t>;
	using WidecharArrayView  = Internal::TCharArrayView<wchar_t, size_t>;
	using AnsicharArray      = Internal::TCharArray<char, size_t>;
	using WidecharArray      = Internal::TCharArray<wchar_t, size_t>;
}