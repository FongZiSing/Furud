//
// VIEW.h
//
//       Copyright (c) Furud IPC Library. All rights reserved.
//       @Author FongZiSing
//
// Implemention of the view of name, which is a compile-time string.
//
#pragma once



namespace Furud::IPC::Concept
{
	/**
	 * @brief    A native compile-time string.
	 * @details  编译期字符串。
	 */
	class INameView
	{
	protected:
		const char* data;
		const unsigned long long size;


	protected:
		constexpr unsigned long long CaculateSize(const char* inData) noexcept
		{
			return __builtin_strlen(inData);
		}

		constexpr unsigned long long CaculateSize(const unsigned long long& inSize) noexcept
		{
			return inSize != unsigned long long(-1) ? inSize : 0;
		}

	public:
		constexpr INameView() noexcept
			: data(nullptr)
			, size(0)
		{}

		constexpr INameView(const char* inData) noexcept
			: data(inData)
			, size(CaculateSize(inData))
		{}

		constexpr INameView(const char* inData, const unsigned long long& inSize) noexcept
			: data(inData)
			, size(CaculateSize(inSize))
		{}

		constexpr INameView(const char* inFirst, const char* inLast) noexcept
			: data(inFirst)
			, size(CaculateSize(static_cast<unsigned long long>(inLast - inFirst)))
		{}


	public:
		constexpr const char* Data() const noexcept
		{
			return data;
		}

		constexpr unsigned long long Size() const noexcept
		{
			return size;
		}

		constexpr unsigned long long Find(const INameView& arr) const noexcept
		{
			const char* str = data;
			const char* search = arr.data;
			const unsigned long long& strSize = size;
			const unsigned long long& searchSize = arr.size;

			if (searchSize > strSize)
			{
				return -1;
			}

			if (searchSize == 0 || !search)
			{
				return -1;
			}

			const auto tryEnd = str + (strSize - searchSize) + 1;
			for (auto tryPtr = str; ; ++tryPtr)
			{
				tryPtr = __builtin_char_memchr(tryPtr, search[0], static_cast<unsigned long long>(tryEnd - tryPtr));

				if (!tryPtr)
				{
					return -1;
				}

				if (__builtin_memcmp(tryPtr, search, searchSize) == 0)
				{
					return static_cast<unsigned long long>(tryPtr - str);
				}
			}
		}


	public:
#ifdef _IOSTREAM_
		friend std::ostream& operator << (std::ostream& os, const INameView& view)
		{
			for (unsigned long long i = 0; i < view.size; ++i)
			{
				os << view.data[i];
			}
			return os;
		}
#endif
	};
}
