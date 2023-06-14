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
	namespace IInteger
	{
		constexpr  int8_t I8_MIN = 0x80i8;  // -128
		constexpr  int8_t I8_MAX = 0x7fi8;  //  127
		constexpr uint8_t U8_MAX = 0xffui8; //  255

		constexpr  int16_t I16_MIN = 0x8000i16;  // -32768
		constexpr  int16_t I16_MAX = 0x7fffi16;  //  32767
		constexpr uint16_t U16_MAX = 0xffffui16; //  65535

		constexpr  int32_t I32_MIN = 0x80000000i32;  // -2147483648
		constexpr  int32_t I32_MAX = 0x7fffffffi32;  //  2147483647
		constexpr uint32_t U32_MAX = 0xffffffffui32; //  4294967295

		constexpr  int64_t I64_MIN = 0x8000000000000000i64;  // -9223372036854775808
		constexpr  int64_t I64_MAX = 0x7fffffffffffffffi64;  //  9223372036854775807
		constexpr uint64_t U64_MAX = 0xffffffffffffffffui64; // 18446744073709551615
	};



	namespace IFloat
	{
		constexpr float E       = 2.71828182845904523536f; // e
		constexpr float LOG2E   = 1.44269504088896340736f; // log2(e)
		constexpr float LOG10E  = 0.43429448190325182765f; // log10(e)
		constexpr float LN2     = 0.69314718055994530942f; // ln(2)
		constexpr float LN10    = 2.30258509299404568402f; // ln(10)
		constexpr float PI      = 3.14159265358979323846f; // pi
		constexpr float HALF_PI = 1.57079632679489661923f; // pi/2
		constexpr float INV_PI  = 0.31830988618379067154f; // 1/pi
		constexpr float SQRT2   = 1.41421356237309504880f; // sqrt(2)
		constexpr float SQRT3   = 1.73205080756887729353f; // sqrt(3)
		
		constexpr float DEG_TO_RAD  = PI / 180.f;
		constexpr float RAD_TO_DEG  = 180.f / PI;

		constexpr float NaN       = __builtin_nanf("0");
		constexpr float NEG_NaN   = __builtin_nansf("1");
		constexpr float INF       = __builtin_huge_valf();
		constexpr float NEG_INF   = -__builtin_huge_valf();
		constexpr float MIN       = -3.402823466e+38f;
		constexpr float MAX       = 3.402823466e+38f;
		constexpr float SMALL     = 1e-4f;
		constexpr float SMALLER   = 1e-8f;
		constexpr float SMALLEST  = 1.175494351e-38f;
	};



	namespace IDouble
	{
		constexpr double E        = 2.71828182845904523536; // e
		constexpr double LOG2E    = 1.44269504088896340736; // log2(e)
		constexpr double LOG10E   = 0.43429448190325182765; // log10(e)
		constexpr double LN2      = 0.69314718055994530942; // ln(2)
		constexpr double LN10     = 2.30258509299404568402; // ln(10)
		constexpr double PI       = 3.14159265358979323846; // pi
		constexpr double HALF_PI  = 1.57079632679489661923; // pi/2
		constexpr double INV_PI   = 0.31830988618379067154; // 1/pi
		constexpr double SQRT2    = 1.41421356237309504880; // sqrt(2)
		constexpr double SQRT3    = 1.73205080756887729353; // sqrt(3)

		constexpr double DEG_TO_RAD  = PI / 180.f;
		constexpr double RAD_TO_DEG  = 180.f / PI;

		constexpr double NAN       = __builtin_nan("0");
		constexpr double NEG_NAN   = __builtin_nans("1");
		constexpr double INF       = __builtin_huge_val();
		constexpr double NEG_INF   = -__builtin_huge_val();
		constexpr double MIN       = -1.7976931348623158e+308;
		constexpr double MAX       = 1.7976931348623158e+308;
		constexpr double SMALL     = 1e-4;
		constexpr double SMALLER   = 1e-8;
		constexpr double SMALLEST  = 2.2250738585072014e-308;
	};
}
