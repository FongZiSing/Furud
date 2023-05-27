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
	private:
		/** The maximum length of name. */
		static constexpr uint32_t capacity = 50;


	public:
		/** The string of name. */
		wchar_t data[capacity];

		/** The prefix of name. */
		const wchar_t* prefix;

		/** The length of prefix. */
		const uint32_t prelen;


	public:
		explicit IName(wchar_t const* inPrefix) noexcept
			: prefix(inPrefix)
			, prelen(std::min(capacity - 1, (uint32_t)__builtin_wcslen(inPrefix)))
		{
			::wmemcpy(data, prefix, prelen);
			data[prelen] = L'\0';
		}

		void Construct(wchar_t const* inName)
		{
			// Copy the real name.
			if (capacity - 1 > prelen)
			{
				const auto suitNameSize = capacity - prelen - 1;
				const auto wantNameSize = (uint32_t)__builtin_wcslen(inName);
				const auto usedNameSize = std::min(suitNameSize, wantNameSize);
				::wmemcpy(data + prelen, inName, usedNameSize);
				data[prelen + usedNameSize] = L'\0';
			}
		}
	};
}