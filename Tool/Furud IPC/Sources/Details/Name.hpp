//
// Name.hpp
//
//       Copyright (c) Furud IPC Library. All rights reserved.
//       @Author FongZiSing
//
// Implemention of name string.
//
#pragma once

#include <wchar.h>
#include <utility>



namespace Furud::IPC
{
	struct IName
	{
	public:
		/** The maximum length of name. */
		static constexpr unsigned long long capacity = 50;

		/** The string of name. */
		wchar_t data[capacity];


	public:
		explicit IName(wchar_t const* inPrefix, wchar_t const* inName) noexcept
		{
			// Copy the prefix name.
			const auto suitPrefixSize = capacity - 1;
			const auto wantPrefixSize = __builtin_wcslen(inPrefix);
			const auto usedPrefixSize = std::min(suitPrefixSize, wantPrefixSize);
			::wmemcpy(data, inPrefix, wantPrefixSize);

			// Copy the real name.
			if (suitPrefixSize > usedPrefixSize)
			{
				const auto suitNameSize = suitPrefixSize - usedPrefixSize;
				const auto wantNameSize = __builtin_wcslen(inName);
				const auto usedNameSize = std::min(suitNameSize, wantNameSize);
				::wmemcpy(data + usedPrefixSize, inName, usedNameSize);
				data[usedPrefixSize + usedNameSize] = L'\0';
			}
			else
			{
				data[suitPrefixSize] = L'\0';
			}
		}
	};
}