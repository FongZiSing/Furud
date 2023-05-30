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



export module Furud.Platform.Math;



#pragma warning (disable:4244)

export namespace Furud
{
	namespace IMath
	{
		furud_nodiscard furud_inline float Floor(const float& value) noexcept { return floorf(value); }
		furud_nodiscard furud_inline double Floor(const double& value) noexcept { return floor(value); }
		template <typename T>
		furud_nodiscard furud_inline float Floor(const T& value) { return floorf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Ceil(const float& value) noexcept { return ceilf(value); }
		furud_nodiscard furud_inline double Ceil(const double& value) noexcept { return ceil(value); }
		template <typename T>
		furud_nodiscard furud_inline float Ceil(const T& value) { return ceilf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Abs(const float& value) noexcept { return fabsf(value); }
		furud_nodiscard furud_inline double Abs(const double& value) noexcept { return fabs(value); }
		template <typename T>
		furud_nodiscard furud_inline decltype(auto) Abs(const T& value) requires std::is_signed_v<std::decay_t<T>> { return value > 0 ? value : -value; }


		furud_nodiscard furud_inline float Copysign(const float& value, const float& sign) noexcept { return __copysignf(value, sign); }
		furud_nodiscard furud_inline double Copysign(const double& value, const double& sign) noexcept { return __copysign(value, sign); }


		furud_nodiscard furud_inline bool IsNaN(const float& value) noexcept { return _isnanf(value) != 0; }
		furud_nodiscard furud_inline bool IsNaN(const double& value) noexcept { return _isnan(value) != 0; }


		furud_nodiscard furud_inline bool IsFinite(const float& value) noexcept { return _finitef(value) != 0; }
		furud_nodiscard furud_inline bool IsFinite(const double& value) noexcept { return _finite(value) != 0; }


		furud_nodiscard furud_inline bool IsInfinite(const float& value) noexcept { return std::isinf(value) != 0; }
		furud_nodiscard furud_inline bool IsInfinite(const double& value) noexcept { return std::isinf(value) != 0; }


		furud_nodiscard furud_inline float Sin(const float& value) noexcept { return sinf(value); }
		furud_nodiscard furud_inline double Sin(const double& value) noexcept { return sin(value); }
		template <typename T>
		furud_nodiscard furud_inline float Sin(const T& value) { return sinf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Asin(const float& value) noexcept { return asinf(value); }
		furud_nodiscard furud_inline double Asin(const double& value) noexcept { return asin(value); }
		template <typename T>
		furud_nodiscard furud_inline float Asin(const T& value) { return asinf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Sinh(const float& value) noexcept { return sinhf(value); }
		furud_nodiscard furud_inline double Sinh(const double& value) noexcept { return sinh(value); }
		template <typename T>
		furud_nodiscard furud_inline float Sinh(const T& value) { return sinhf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Cos(const float& value) noexcept { return cosf(value); }
		furud_nodiscard furud_inline double Cos(const double& value) noexcept { return cos(value); }
		template <typename T>
		furud_nodiscard furud_inline float Cos(const T& value) { return cosf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Acos(const float& value) noexcept { return acosf(value); }
		furud_nodiscard furud_inline double Acos(const double& value) noexcept { return acos(value); }
		template <typename T>
		furud_nodiscard furud_inline float Acos(const T& value) { return acosf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Tan(const float& value) noexcept { return tanf(value); }
		furud_nodiscard furud_inline double Tan(const double& value) noexcept { return tan(value); }
		template <typename T>
		furud_nodiscard furud_inline float Tan(const T& value) { return tanf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Atan(const float& value) noexcept { return atanf(value); }
		furud_nodiscard furud_inline double Atan(const double& value) noexcept { return atan(value); }
		template <typename T>
		furud_nodiscard furud_inline float Atan(const T& value) { return atanf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Sqrt(const float& value) noexcept { return sqrtf(value); }
		furud_nodiscard furud_inline double Sqrt(const double& value) noexcept { return sqrt(value); }
		template <typename T>
		furud_nodiscard furud_inline float Sqrt(const T& value) { return sqrtf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Pow(const float& base, const float& value) noexcept { return powf(base, value); }
		furud_nodiscard furud_inline double Pow(const double& base, const double& value) noexcept { return pow(base, value); }
		template <typename T, typename U>
		furud_nodiscard furud_inline float Pow(const T& base, const U& value) { return powf(static_cast<float>(base), static_cast<float>(value)); }


		furud_nodiscard furud_inline float Exp(const float& value) noexcept { return expf(value); }
		furud_nodiscard furud_inline double Exp(const double& value) noexcept { return exp(value); }
		template <typename T>
		furud_nodiscard furud_inline float Exp(const T& value) { return expf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Exp2(const float& value) noexcept { return powf(2.f, value); }
		furud_nodiscard furud_inline double Exp2(const double& value) noexcept { return pow(2.0, value); }
		template <typename T>
		furud_nodiscard furud_inline float Exp2(const T& value) { return powf(2.f, static_cast<float>(value)); }


		furud_nodiscard furud_inline float Loge(const float& value) noexcept { return logf(value); }
		furud_nodiscard furud_inline double Loge(const double& value) noexcept { return log(value); }
		template <typename T>
		furud_nodiscard furud_inline float Loge(const T& value) { return logf(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Log10(const float& value) noexcept { return log10f(value); }
		furud_nodiscard furud_inline double Log10(const double& value) noexcept { return log10(value); }
		template <typename T>
		furud_nodiscard furud_inline float Log10(const T& value) { return log10f(static_cast<float>(value)); }


		furud_nodiscard furud_inline float Log2(const float& value) noexcept { return logf(value) * 1.4426950f; }
		furud_nodiscard furud_inline double Log2(const double& value) noexcept { return log(value) * 1.442695040888963387; }
		template <typename T>
		furud_nodiscard furud_inline float Log2(const T& value) { return logf(static_cast<float>(value)) * 1.4426950f; }


		furud_nodiscard furud_inline float LogX(const float& base, const float& value) noexcept { return logf(value) / logf(base); }
		furud_nodiscard furud_inline double LogX(const double& base, const double& value) noexcept { return log(value) / log(base); }
		template <typename T, typename U>
		furud_nodiscard furud_inline float LogX(const T& base, const U& value) { return logf(static_cast<float>(value)) / logf(static_cast<float>(base)); }


		furud_nodiscard furud_inline float RecipSqrt(const float& value) noexcept
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

		furud_nodiscard furud_inline double RecipSqrt(const double& value) noexcept
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