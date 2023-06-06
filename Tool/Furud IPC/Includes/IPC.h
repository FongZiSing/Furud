#pragma once
#include "GUID.h"
#include <vector>
#include <tuple>
#include <utility>
#include <iostream>
#include <string_view>
#include "VIEW.h"
#include <vector>
#include <concepts>


namespace Furud::IPC::Concept
{
	template <typename T>
	struct IRegisterMessage
	{
		constexpr static INameView StaticName() noexcept
		{
			using INameSize = unsigned long long;

			constexpr INameView functionCTName { __FUNCTION__ };
			constexpr INameView functionSuffix { ">::StaticName" };
			constexpr INameSize npos = INameSize(-1);
			
			// Struct cases.
			constexpr INameView searchPrefix1 { "<struct " };
			constexpr INameSize searchOffset1 = functionCTName.Find(searchPrefix1);
			if constexpr (searchOffset1 != npos)
			{
				constexpr INameSize offset = searchOffset1 + searchPrefix1.Size();
				constexpr INameSize length = functionCTName.Size() - functionSuffix.Size() - offset;
				return { functionCTName.Data() + offset, length };
			}

			// Class cases.
			constexpr INameView searchPrefix2 { "<class " };
			constexpr INameSize searchOffset2 = functionCTName.Find(searchPrefix2);
			if constexpr (searchOffset2 != npos)
			{
				constexpr INameSize offset = searchOffset2 + searchOffset2.Size();
				constexpr INameSize length = functionCTName.Size() - functionSuffix.Size() - offset;
				return { functionCTName.Data() + offset, length };
			}

			//// Universal cases.
			constexpr INameView searchPrefix3 { "IRegisterMessage<" };
			constexpr INameSize searchOffset3 = functionCTName.Find(searchPrefix3);
			if constexpr (searchOffset3 != npos)
			{
				constexpr INameSize offset = searchOffset3 + searchPrefix3.Size();
				constexpr INameSize length = functionCTName.Size() - functionSuffix.Size() - offset;
				return { functionCTName.Data() + offset, length };
			}

			return {};
		}
	};

	struct UniversalType
	{
		template <typename T>
		operator T();
	};


	template <typename U>
	struct InheritType
	{
		template <typename T> requires std::is_base_of_v<T, U>
		operator T();
	};


	template <typename T>
	concept is_container = requires(T t)
	{
		typename T::value_type;
		typename T::size_type;
		std::is_arithmetic_v<typename T::value_type>;
		t.data();
		t.size();
		t.push_back(typename T::value_type{});
		{ t.empty() } -> std::same_as<bool>;
		t.clear();
		t.reserve(typename T::size_type{});
	};

	struct ContainerType
	{
		template <typename T> requires is_container<T>
		operator T();
	};



	template <typename T>
	concept is_aggregate = std::is_aggregate_v<T>;

	struct AggregateType
	{
		template <typename T> requires is_aggregate<T>
		operator T();
	};



	template <typename T>
	concept is_plainolddata =  std::is_trivial_v<T> && std::is_standard_layout_v<T>;

	struct PlainOldDataType
	{
		template <typename T> requires is_plainolddata<T>
		operator T();
	};


	template <typename T, typename... Args>
	constexpr int CountFieldMember()
	{
		if constexpr (requires { T { Args{}..., InheritType<T>{} }; })
		{
			return CountFieldMember<T, InheritType<T>>();
		}
		if constexpr (requires { T { Args{}..., AggregateType{} }; })
		{
			if constexpr (requires { T { Args{}..., PlainOldDataType{} }; })
			{
				return 1 + CountFieldMember<T, Args..., PlainOldDataType>();
			}
			else
			{
				return 0 + CountFieldMember<T, Args..., UniversalType>();
			}
		}
		else if constexpr (requires { T { Args{}..., ContainerType{} }; })
		{
			return 1 + CountFieldMember<T, Args..., ContainerType>();
		}
		else if constexpr (requires { T { Args{}..., PlainOldDataType{} }; })
		{
			return 1 + CountFieldMember<T, Args..., PlainOldDataType>();
		}
		else if constexpr (requires { T { Args{}..., UniversalType{} }; })
		{
			return 0 + CountFieldMember<T, Args..., UniversalType>();
		}

		return 0;
	}


	constexpr decltype(auto) VisitFieldMember(auto&& message, auto&& visitor) noexcept
	{
		constexpr auto count = CountFieldMember<std::remove_reference_t<decltype(message)>>();

		if constexpr (count == 0)
		{
			visitor();
		}
		else if constexpr (count == 1)
		{
			auto&& [a1] = message;
			return visitor(a1);
		}
		else if constexpr (count == 2)
		{
			auto&& [a1, a2] = message;
			return visitor(a1, a2);
		}
		else if constexpr (count == 3)
		{
			auto&& [a1, a2, a3] = message;
			return visitor(a1, a2, a3);
		}
		else if constexpr (count == 4)
		{
			auto&& [a1, a2, a3, a4] = message;
			return visitor(a1, a2, a3, a4);
		}
		else if constexpr (count == 5)
		{
			auto&& [a1, a2, a3, a4, a5] = message;
			return visitor(a1, a2, a3, a4, a5);
		}
		else if constexpr (count == 6)
		{
			auto&& [a1, a2, a3, a4, a5, a6] = message;
			return visitor(a1, a2, a3, a4, a5, a6);
		}
		else if constexpr (count == 7)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7);
		}
		else if constexpr (count == 8)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8);
		}
		else if constexpr (count == 9)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9);
		}
		else if constexpr (count == 10)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
		}
		else if constexpr (count == 11)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
		}
		else if constexpr (count == 12)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
		}
		else if constexpr (count == 13)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
		}
		else if constexpr (count == 14)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);
		}
		else if constexpr (count == 15)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15);
		}
		else if constexpr (count == 16)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16);
		}
		else if constexpr (count == 17)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17);
		}
		else if constexpr (count == 18)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18);
		}
		else if constexpr (count == 19)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19);
		}
		else if constexpr (count == 20)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20);
		}
		else if constexpr (count == 21)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21);
		}
		else if constexpr (count == 22)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22);
		}
		else if constexpr (count == 23)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23);
		}
		else if constexpr (count == 24)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24);
		}
		else if constexpr (count == 25)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25);
		}
		else if constexpr (count == 26)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26);
		}
		else if constexpr (count == 27)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27);
		}
		else if constexpr (count == 28)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28);
		}
		else if constexpr (count == 29)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29);
		}
		else if constexpr (count == 30)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30);
		}
		else if constexpr (count == 31)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31);
		}
		else if constexpr (count == 32)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32);
		}
		else if constexpr (count == 33)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33);
		}
		else if constexpr (count == 34)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34);
		}
		else if constexpr (count == 35)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35);
		}
		else if constexpr (count == 36)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36);
		}
		else if constexpr (count == 37)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37);
		}
		else if constexpr (count == 38)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38);
		}
		else if constexpr (count == 39)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39);
		}
		else if constexpr (count == 40)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40);
		}
		else if constexpr (count == 41)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41);
		}
		else if constexpr (count == 42)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42);
		}
		else if constexpr (count == 43)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43);
		}
		else if constexpr (count == 44)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44);
		}
		else if constexpr (count == 45)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45);
		}
		else if constexpr (count == 46)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46);
		}
		else if constexpr (count == 47)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47);
		}
		else if constexpr (count == 48)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48);
		}
		else if constexpr (count == 49)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49);
		}
		else if constexpr (count == 50)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50);
		}
		else if constexpr (count == 51)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51);
		}
		else if constexpr (count == 52)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52);
		}
		else if constexpr (count == 53)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53);
		}
		else if constexpr (count == 54)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53, a54] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53, a54);
		}
		else if constexpr (count == 55)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53, a54, a55] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53, a54, a55);
		}
		else if constexpr (count == 56)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53, a54, a55, a56] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53, a54, a55, a56);
		}
		else if constexpr (count == 57)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53, a54, a55, a56, a57] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53, a54, a55, a56, a57);
		}
		else if constexpr (count == 58)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53, a54, a55, a56, a57, a58] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53, a54, a55, a56, a57, a58);
		}
		else if constexpr (count == 59)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59);
		}
		else if constexpr (count == 60)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60);
		}
		else if constexpr (count == 61)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61);
		}
		else if constexpr (count == 62)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62);
		}
		else if constexpr (count == 63)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63);
		}
		else if constexpr (count == 64)
		{
			auto&& [a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63, a64] = message;
			return visitor(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16,
					a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31, a32,
					a33, a34, a35, a36, a37, a38, a39, a40, a41, a42, a43, a44, a45, a46, a47, a48,
					a49, a50, a51, a52, a53, a54, a55, a56, a57, a58, a59, a60, a61, a62, a63, a64);
		}
		else
		{
			static_assert(count <= 64, "[Furud] Message is too long, should not exceed 64!");
		}
	}

	constexpr int FlattenFieldMember(auto&& object);

	template <typename T>
	constexpr int GetField()
	{
		//return 0;
		if constexpr (is_aggregate<T>)
		{
			if constexpr (is_plainolddata<T>)
			{
				return FlattenFieldMember(T{});
				//return 0;
			}
			else
			{
				return 0;
			}
		}
		else if constexpr (is_plainolddata<T>)
		{
			return  IRegisterMessage<T>::StaticName().Size();
		}
		else if constexpr (is_container<T>)
		{
			return IRegisterMessage<T>::StaticName().Size();
		}
		else
		{
			return 0;
		}
		//return 0;
	}

	template <typename... Args>
	constexpr int GetFieldMember()
	{
		return (GetField<std::remove_reference_t<Args>>() + ...);
	}


	constexpr int FlattenFieldMember(auto&& object)
	{
		return VisitFieldMember(
			std::forward<decltype(object)>(object),
			[](auto&& ...args) constexpr noexcept -> int
			{
				return GetFieldMember<decltype(args)...>();
			});
	}

	//constexpr GUID CalculateGUID(auto&& message)
	//{
	//	constexpr INameView messageName = IRegisterMessage<decltype(message)>::StaticName();
	//	constexpr auto fieldCount = CountFieldMember<decltype(message)>();
	//	constexpr auto bytes = messageName.Size() + fieldCount;

	//	char s[bytes];
	//	for (auto i = 0; i < messageName.Size(); ++i)
	//	{
	//		s[i] = messageName.Data()[i];
	//	}
	//	s[bytes] = '\0';
	//}
}

namespace Furud::IPC
{
	template <typename F>
	struct MessageField
	{
		using Field = F;
		Field field;
		constexpr operator Field() noexcept { return field; }
		constexpr operator Field&() noexcept { return field; }
		constexpr operator const Field& () const noexcept { return field; }
	};

	template <typename T>
	struct RegisterMessage
	{
	};

	struct AnyType
	{
		template <typename U>
		operator U();
	};

	template <typename T>
	constexpr size_t member_count(auto&&... args)
	{

		if constexpr (requires { T { args... }; })
		{
			if constexpr (!requires { T { args..., AnyType{} }; })
			{
				return sizeof...(args) -1;
			}
			else
			{
				return member_count<T>(args..., AnyType{});
			}
		}

		return 0;
	}

	template <typename T>
	constexpr size_t caculate_one_size(const T& item)
	{
		//if constexpr (std::is_fundamental_v<T> || std::is_enum_v<T>)
		{
			return sizeof(T);
		}
		//else
		//{
		//	static_assert(!sizeof(T), typeid(T).name());
		//}
	}

	constexpr size_t caculate_playload_size() { return 0; }

	template <typename T, typename... Args>
	constexpr size_t caculate_playload_size(const T& item, const Args& ...args)
	{
		if constexpr (sizeof...(args))
		{
			return caculate_one_size(item) + caculate_playload_size(args...);
		}
		else
		{
			return caculate_one_size(item);
		}
	}

	constexpr void visit_members(auto&& message, auto&& visitor)
	{
		constexpr auto count = member_count<decltype(message)>();
		if constexpr (count == 0)
		{
			visitor();
		}
		else if constexpr (count == 1)
		{
			auto&& [m1] = message;
			visitor(m1);
		}
		else if constexpr (count == 2)
		{
			auto&& [m1, m2] = message;
			visitor(m1, m2);
		}
		else
		{
			static_assert(count == 0, "error");
		}
	}

	template <typename T>
	constexpr size_t visit_members_size(auto&& visitor)
	{
		auto message = T {};
		constexpr auto count = member_count<decltype(message)>();
		if constexpr (count == 0)
		{
			return visitor();
		}
		else if constexpr (count == 1)
		{
			auto&& [m1] = message;
			return visitor(m1);
		}
		else if constexpr (count == 2)
		{
			auto&& [m1, m2] = message;
			return visitor(m1, m2);
		}
		else
		{
			static_assert(count == 0, "error");
		}
	}

	struct LayoutInfo
	{
		size_t offset;
	};

	struct MyMessage : public Concept::IRegisterMessage<MyMessage>
	{
		struct ccccc
		{
			//std::string name;
			float b;
			float c;
		} c;
		//std::string s;
		char hello;
		char world;
		//std::string s;
	};

	template <size_t n>
	struct MsgName
	{
		char m[32 * n + 1];

		template<typename... Args>
		constexpr MsgName(Args&&... args) noexcept
			: m{ args... }
		{}
	};



	static size_t Serialize(const MyMessage& message)
	{
		const size_t fieldNum = Concept::FlattenFieldMember(MyMessage{});
		constexpr MsgName<fieldNum> name { };
		constexpr auto visitor = [](auto &&...items) constexpr noexcept
		{
			return 0;// caculate_playload_size(items...);
			//std::cout << playloadsize << "\n";
			//((std::cout << items << " "), ...);
		};
		constexpr auto str = MyMessage::StaticName();
		std::cout << str << "\n";
		//constexpr size_t s = visit_members_size<MyMessage>(visitor);
		return fieldNum;
	}
}

void TestIPC(int argc, char** argv);