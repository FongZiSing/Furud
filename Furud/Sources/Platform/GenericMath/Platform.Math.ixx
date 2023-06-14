//
// Platform.Math.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Common Math Library.
//
module;

#include <Furud.hpp>
#include <cmath>
#include <immintrin.h>
#include <concepts>



export module Furud.Platform.Math;

namespace Furud::inline Internal
{
	template <typename T>
	concept convertible = std::convertible_to<T, float>;

	template <typename T>
	concept is_signed = std::is_signed_v<T>;
}



#pragma warning (disable:4244)

export namespace Furud
{
	namespace IMath
	{
		furud_nodiscard furud_inline float Floor(float value) noexcept { return floorf(value); }
		furud_nodiscard furud_inline double Floor(double value) noexcept { return floor(value); }
		furud_nodiscard furud_inline float Floor(convertible auto value) { return floorf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Ceil(float value) noexcept { return ceilf(value); }
		furud_nodiscard furud_inline double Ceil(double value) noexcept { return ceil(value); }
		furud_nodiscard furud_inline float Ceil(convertible auto value) { return ceilf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Abs(float value) noexcept { return fabsf(value); }
		furud_nodiscard furud_inline double Abs(double value) noexcept { return fabs(value); }
		furud_nodiscard furud_inline decltype(auto) Abs(is_signed auto value) { return value > 0 ? value : -value; }


		furud_nodiscard furud_inline float Copysign(float value, float sign) noexcept { return __copysignf(value, sign); }
		furud_nodiscard furud_inline double Copysign(double value, double sign) noexcept { return __copysign(value, sign); }


		furud_nodiscard furud_inline bool IsNaN(float value) noexcept { return _isnanf(value) != 0; }
		furud_nodiscard furud_inline bool IsNaN(double value) noexcept { return _isnan(value) != 0; }


		furud_nodiscard furud_inline bool IsFinite(float value) noexcept { return _finitef(value) != 0; }
		furud_nodiscard furud_inline bool IsFinite(double value) noexcept { return _finite(value) != 0; }


		furud_nodiscard furud_inline bool IsInfinite(float value) noexcept { return std::isinf(value) != 0; }
		furud_nodiscard furud_inline bool IsInfinite(double value) noexcept { return std::isinf(value) != 0; }


		furud_nodiscard furud_inline float Sin(float value) noexcept { return sinf(value); }
		furud_nodiscard furud_inline double Sin(double value) noexcept { return sin(value); }
		furud_nodiscard furud_inline float Sin(convertible auto value) { return sinf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Asin(float value) noexcept { return asinf(value); }
		furud_nodiscard furud_inline double Asin(double value) noexcept { return asin(value); }
		furud_nodiscard furud_inline float Asin(convertible auto value) { return asinf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Sinh(float value) noexcept { return sinhf(value); }
		furud_nodiscard furud_inline double Sinh(double value) noexcept { return sinh(value); }
		furud_nodiscard furud_inline float Sinh(convertible auto value) { return sinhf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Cos(float value) noexcept { return cosf(value); }
		furud_nodiscard furud_inline double Cos(double value) noexcept { return cos(value); }
		furud_nodiscard furud_inline float Cos(convertible auto value) { return cosf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Acos(float value) noexcept { return acosf(value); }
		furud_nodiscard furud_inline double Acos(double value) noexcept { return acos(value); }
		furud_nodiscard furud_inline float Acos(convertible auto value) { return acosf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Tan(float value) noexcept { return tanf(value); }
		furud_nodiscard furud_inline double Tan(double value) noexcept { return tan(value); }
		furud_nodiscard furud_inline float Tan(convertible auto value) { return tanf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Atan(float value) noexcept { return atanf(value); }
		furud_nodiscard furud_inline double Atan(double value) noexcept { return atan(value); }
		furud_nodiscard furud_inline float Atan(convertible auto value) { return atanf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Sqrt(float value) noexcept { return sqrtf(value); }
		furud_nodiscard furud_inline double Sqrt(double value) noexcept { return sqrt(value); }
		furud_nodiscard furud_inline float Sqrt(convertible auto value) { return sqrtf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Pow(float base, float value) noexcept { return powf(base, value); }
		furud_nodiscard furud_inline double Pow(double base, double value) noexcept { return pow(base, value); }
		furud_nodiscard furud_inline float Pow(convertible auto base, convertible auto value) { return powf(static_cast<float>(base), static_cast<float>(value)); }


		furud_nodiscard furud_inline float Exp(float value) noexcept { return expf(value); }
		furud_nodiscard furud_inline double Exp(double value) noexcept { return exp(value); }
		furud_nodiscard furud_inline float Exp(convertible auto value) { return expf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Exp2(float value) noexcept { return powf(2.f, value); }
		furud_nodiscard furud_inline double Exp2(double value) noexcept { return pow(2.0, value); }
		furud_nodiscard furud_inline float Exp2(convertible auto value) { return powf(2.f, static_cast<float>(value)); }


		furud_nodiscard furud_inline float Loge(float value) noexcept { return logf(value); }
		furud_nodiscard furud_inline double Loge(double value) noexcept { return log(value); }
		furud_nodiscard furud_inline float Loge(convertible auto value) { return logf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Log10(float value) noexcept { return log10f(value); }
		furud_nodiscard furud_inline double Log10(double value) noexcept { return log10(value); }
		furud_nodiscard furud_inline float Log10(convertible auto value) { return log10f(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Log2(float value) noexcept { return logf(value) * 1.4426950f; }
		furud_nodiscard furud_inline double Log2(double value) noexcept { return log(value) * 1.442695040888963387; }
		furud_nodiscard furud_inline float Log2(convertible auto value) { return logf(static_cast<float>(value)) * 1.4426950f; }


		furud_nodiscard furud_inline float LogX(float base, float value) noexcept { return logf(value) / logf(base); }
		furud_nodiscard furud_inline double LogX(double base, double value) noexcept { return log(value) / log(base); }
		furud_nodiscard furud_inline float LogX(convertible auto base, convertible auto value) { return logf(static_cast<float>(value)) / logf(static_cast<float>(base)); }


		furud_nodiscard furud_inline float RecipSqrt(float value) noexcept
		{
			const __m128 HALF = _mm_set_ss(0.5f);
			__m128 x0, y0, x1, x2, fOver2;

			y0 = _mm_set_ss(value);
			fOver2 = _mm_mul_ss(y0, HALF);

			// 1/sqrt estimate (12 bits)
			x0 = _mm_rsqrt_ss(y0);

			// 1st Newton-Raphson iteration
			x1 = _mm_mul_ss(x0, x0);
			x1 = _mm_fnmadd_ss(fOver2, x1, HALF);
			x1 = _mm_fmadd_ss(x0, x1, x0);

			// 2nd Newton-Raphson iteration
			x2 = _mm_mul_ss(x1, x1);
			x2 = _mm_fnmadd_ss(fOver2, x2, HALF);
			x2 = _mm_fmadd_ss(x1, x2, x1);

			return _mm_cvtss_f32(x2);
		}

		furud_nodiscard furud_inline double RecipSqrt(double value) noexcept
		{
			const __m128d ONE = _mm_set_sd(1.0);
			__m128d v;

			v = _mm_set_sd(value);
			v = _mm_sqrt_sd(ONE, v);
			v = _mm_div_sd(ONE, v);

			return _mm_cvtsd_f64(v);
		}
	};
}

#pragma warning (default:4244)