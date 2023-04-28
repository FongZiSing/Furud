//
// Platform.Numbers.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Definitions of useful mathematical constants.
//
module;

#include <Furud.hpp>
#include <stdint.h>



export module Furud.Numbers;



export namespace Furud
{
	struct IInteger
	{
		static constexpr  int8_t I8_MIN = 0x80i8;  // -128
		static constexpr  int8_t I8_MAX = 0x7fi8;  //  127
		static constexpr uint8_t U8_MAX = 0xffui8; //  255

		static constexpr  int16_t I16_MIN = 0x8000i16;  // -32768
		static constexpr  int16_t I16_MAX = 0x7fffi16;  //  32767
		static constexpr uint16_t U16_MAX = 0xffffui16; //  65535

		static constexpr  int32_t I32_MIN = 0x80000000i32;  // -2147483648
		static constexpr  int32_t I32_MAX = 0x7fffffffi32;  //  2147483647
		static constexpr uint32_t U32_MAX = 0xffffffffui32; //  4294967295

		static constexpr  int64_t I64_MIN = 0x8000000000000000i64;  // -9223372036854775808
		static constexpr  int64_t I64_MAX = 0x7fffffffffffffffi64;  //  9223372036854775807
		static constexpr uint64_t U64_MAX = 0xffffffffffffffffui64; // 18446744073709551615
	};



	struct IFloat
	{
		static constexpr float E       = 2.71828182845904523536f; // e
		static constexpr float LOG2E   = 1.44269504088896340736f; // log2(e)
		static constexpr float LOG10E  = 0.43429448190325182765f; // log10(e)
		static constexpr float LN2     = 0.69314718055994530942f; // ln(2)
		static constexpr float LN10    = 2.30258509299404568402f; // ln(10)
		static constexpr float PI      = 3.14159265358979323846f; // pi
		static constexpr float HALF_PI = 1.57079632679489661923f; // pi/2
		static constexpr float INV_PI  = 0.31830988618379067154f; // 1/pi
		static constexpr float SQRT2   = 1.41421356237309504880f; // sqrt(2)
		static constexpr float SQRT3   = 1.73205080756887729353f; // sqrt(3)
		
		static constexpr float DEG_TO_RAD  = PI / 180.f;
		static constexpr float RAD_TO_DEG  = 180.f / PI;

		static constexpr float NaN       = __builtin_nanf("0");
		static constexpr float NEG_NaN   = __builtin_nansf("1");
		static constexpr float INF       = __builtin_huge_valf();
		static constexpr float NEG_INF   = -__builtin_huge_valf();
		static constexpr float MIN       = -3.402823466e+38f;
		static constexpr float MAX       = 3.402823466e+38f;
		static constexpr float SMALL     = 1e-4f;
		static constexpr float SMALLER   = 1e-8f;
		static constexpr float SMALLEST  = 1.175494351e-38f;
	};



	struct IDouble
	{
		static constexpr double E        = 2.71828182845904523536; // e
		static constexpr double LOG2E    = 1.44269504088896340736; // log2(e)
		static constexpr double LOG10E   = 0.43429448190325182765; // log10(e)
		static constexpr double LN2      = 0.69314718055994530942; // ln(2)
		static constexpr double LN10     = 2.30258509299404568402; // ln(10)
		static constexpr double PI       = 3.14159265358979323846; // pi
		static constexpr double HALF_PI  = 1.57079632679489661923; // pi/2
		static constexpr double INV_PI   = 0.31830988618379067154; // 1/pi
		static constexpr double SQRT2    = 1.41421356237309504880; // sqrt(2)
		static constexpr double SQRT3    = 1.73205080756887729353; // sqrt(3)

		static constexpr double DEG_TO_RAD  = PI / 180.f;
		static constexpr double RAD_TO_DEG  = 180.f / PI;

		static constexpr double NAN       = __builtin_nan("0");
		static constexpr double NEG_NAN   = __builtin_nans("1");
		static constexpr double INF       = __builtin_huge_val();
		static constexpr double NEG_INF   = -__builtin_huge_val();
		static constexpr double MIN       = -1.7976931348623158e+308;
		static constexpr double MAX       = 1.7976931348623158e+308;
		static constexpr double SMALL     = 1e-4;
		static constexpr double SMALLER   = 1e-8;
		static constexpr double SMALLEST  = 2.2250738585072014e-308;
	};
}
