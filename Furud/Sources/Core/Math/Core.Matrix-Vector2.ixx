//
// Core.Matrix-Vector2.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Definitions of 2D vector.
//
module;

#include <Furud.hpp>
#include <stdint.h>



export module Furud.Core.Matrix:Vector2;

export import Furud.Numbers;
import Furud.Platform.Math;
import Furud.Core.Math;



/** Forward declaration. */
export namespace Furud
{
	struct Vector2i;
	FURUD_INLINE extern Vector2i Clamp(const Vector2i& inVal, int32_t const& inMin, int32_t const& inMax) noexcept;


	struct Vector2f;
	FURUD_INLINE extern Vector2f Clamp(const Vector2f& inVal, float const& inMin, float const& inMax) noexcept;


	struct Vector2d;
	FURUD_INLINE extern Vector2d Clamp(const Vector2d& inVal, double const& inMin, double const& inMax) noexcept;
}



/** 2D 32-bit integer vector. */
export namespace Furud
{
	/**
	 * @brief  2D 32-bit integer vector.
	 * @details  二维整型向量。
	 */
	struct alignas(8) Vector2i
	{
		int32_t x, y;


	public:
		static const Vector2i MIN;
		static const Vector2i MAX;
		static const Vector2i ZERO;
		static const Vector2i ONE;


	public:
		constexpr Vector2i() noexcept {}
		constexpr Vector2i(ForceInitFlag) noexcept : x(0), y(0) {}
		constexpr Vector2i(int32_t const& v) noexcept : x(v), y(v) {}
		constexpr Vector2i(int32_t const& sx, int32_t const& sy) noexcept : x(sx), y(sy) {}
		constexpr Vector2i& operator = (int32_t const& v) noexcept { x = y = v; return *this; }
		constexpr explicit Vector2i(struct Vector2f const& v) noexcept;
		constexpr explicit Vector2i(struct Vector2d const& v) noexcept;


	public:
		Vector2i(Vector2i const&) = default;
		Vector2i& operator = (Vector2i const&) = default;
		Vector2i(Vector2i&&) = default;
		Vector2i& operator = (Vector2i&&) = default;
		~Vector2i() noexcept {}


	public:
		bool operator == (Vector2i const& rhs) const noexcept { return x == rhs.x && y == rhs.y; }
		bool operator != (Vector2i const& rhs) const noexcept { return x != rhs.x || y != rhs.y; }


	public:
		Vector2i operator - () const noexcept { return { -x, -y }; }

		Vector2i operator + (Vector2i const& rhs) const noexcept { return { x + rhs.x, y + rhs.y }; }
		Vector2i operator - (Vector2i const& rhs) const noexcept { return { x - rhs.x, y - rhs.y }; }
		Vector2i operator * (Vector2i const& rhs) const noexcept { return { x * rhs.x, y * rhs.y }; }
		Vector2i operator / (Vector2i const& rhs) const noexcept { return { x / rhs.x, y / rhs.y }; }
		
		Vector2i const& operator += (Vector2i const& rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }
		Vector2i const& operator -= (Vector2i const& rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }
		Vector2i const& operator *= (Vector2i const& rhs) noexcept { x *= rhs.x; y *= rhs.y; return *this; }
		Vector2i const& operator /= (Vector2i const& rhs) noexcept { x /= rhs.x; y /= rhs.y; return *this; }
		
		Vector2i operator + (int32_t const& rhs) const noexcept { return { x + rhs, y + rhs }; }
		Vector2i operator - (int32_t const& rhs) const noexcept { return { x - rhs, y - rhs }; }
		Vector2i operator * (int32_t const& rhs) const noexcept { return { x * rhs, y * rhs }; }
		Vector2i operator / (int32_t const& rhs) const noexcept { return { x / rhs, y / rhs }; }
		
		Vector2i const& operator += (int32_t const& rhs) noexcept { x += rhs; y += rhs; return *this; }
		Vector2i const& operator -= (int32_t const& rhs) noexcept { x -= rhs; y -= rhs; return *this; }
		Vector2i const& operator *= (int32_t const& rhs) noexcept { x *= rhs; y *= rhs; return *this; }
		Vector2i const& operator /= (int32_t const& rhs) noexcept { x /= rhs; y /= rhs; return *this; }

		friend Vector2i operator + (int32_t const& lhs, Vector2i const& rhs) noexcept { return { lhs + rhs.x, lhs + rhs.y }; }
		friend Vector2i operator - (int32_t const& lhs, Vector2i const& rhs) noexcept { return { lhs - rhs.x, lhs - rhs.y }; }
		friend Vector2i operator * (int32_t const& lhs, Vector2i const& rhs) noexcept { return { lhs * rhs.x, lhs * rhs.y }; }
		friend Vector2i operator / (int32_t const& lhs, Vector2i const& rhs) noexcept { return { lhs / rhs.x, lhs / rhs.y }; }


	public:
		FURUD_NODISCARD FURUD_INLINE friend Vector2i Clamp(Vector2i const& inVal, int32_t const& inMin, int32_t const& inMax) noexcept
		{
			return
			{
				IMath::Clamp(inVal.x, inMin, inMax),
				IMath::Clamp(inVal.y, inMin, inMax)
			};
		}
	};


	const Vector2i Vector2i::MIN  = IInteger::I32_MIN;
	const Vector2i Vector2i::MAX  = IInteger::I32_MAX;
	const Vector2i Vector2i::ZERO = 0;
	const Vector2i Vector2i::ONE  = 1;
}



/** 2D 32-bit single floating-point vector. */
export namespace Furud
{
	/**
	 * @brief  2D 32-bit single floating-point vector.
	 * @details  二维单精度浮点向量。
	 */
	struct alignas(8) Vector2f
	{
		float x, y;


	public:
		static const Vector2f MIN;
		static const Vector2f MAX;
		static const Vector2f ZERO;
		static const Vector2f ONE;


	public:
		constexpr Vector2f() noexcept {}
		constexpr Vector2f(ForceInitFlag) noexcept : x(0), y(0) {}
		constexpr Vector2f(float const& v) noexcept : x(v), y(v) {}
		constexpr Vector2f(float const& sx, float const& sy) noexcept : x(sx), y(sy) {}
		constexpr Vector2f& operator = (float const& v) noexcept { x = y = v; return *this; }
		constexpr explicit Vector2f(struct Vector2i const& v) noexcept;
		constexpr explicit Vector2f(struct Vector2d const& v) noexcept;


	public:
		Vector2f(Vector2f const&) = default;
		Vector2f& operator = (Vector2f const&) = default;
		Vector2f(Vector2f&&) = default;
		Vector2f& operator = (Vector2f&&) = default;
		~Vector2f() noexcept {}


	public:
		bool operator == (Vector2f const& rhs) const noexcept { return x == rhs.x && y == rhs.y; }
		bool operator != (Vector2f const& rhs) const noexcept { return x != rhs.x || y != rhs.y; }


	public:
		Vector2f operator - () const noexcept { return { -x, -y }; }
		
		Vector2f operator + (Vector2f const& rhs) const noexcept { return { x + rhs.x, y + rhs.y }; }
		Vector2f operator - (Vector2f const& rhs) const noexcept { return { x - rhs.x, y - rhs.y }; }
		Vector2f operator * (Vector2f const& rhs) const noexcept { return { x * rhs.x, y * rhs.y }; }
		Vector2f operator / (Vector2f const& rhs) const noexcept { return { x / rhs.x, y / rhs.y }; }
		
		Vector2f const& operator += (Vector2f const& rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }
		Vector2f const& operator -= (Vector2f const& rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }
		Vector2f const& operator *= (Vector2f const& rhs) noexcept { x *= rhs.x; y *= rhs.y; return *this; }
		Vector2f const& operator /= (Vector2f const& rhs) noexcept { x /= rhs.x; y /= rhs.y; return *this; }

		Vector2f operator + (float const& rhs) const noexcept { return { x + rhs, y + rhs }; }
		Vector2f operator - (float const& rhs) const noexcept { return { x - rhs, y - rhs }; }
		Vector2f operator * (float const& rhs) const noexcept { return { x * rhs, y * rhs }; }
		Vector2f operator / (float const& rhs) const noexcept { return { x / rhs, y / rhs }; }
		
		Vector2f const& operator += (float const& rhs) noexcept { x += rhs; y += rhs; return *this; }
		Vector2f const& operator -= (float const& rhs) noexcept { x -= rhs; y -= rhs; return *this; }
		Vector2f const& operator *= (float const& rhs) noexcept { x *= rhs; y *= rhs; return *this; }
		Vector2f const& operator /= (float const& rhs) noexcept { x /= rhs; y /= rhs; return *this; }

		friend Vector2f operator + (float const& lhs, Vector2f const& rhs) noexcept { return { lhs + rhs.x, lhs + rhs.y }; }
		friend Vector2f operator - (float const& lhs, Vector2f const& rhs) noexcept { return { lhs - rhs.x, lhs - rhs.y }; }
		friend Vector2f operator * (float const& lhs, Vector2f const& rhs) noexcept { return { lhs * rhs.x, lhs * rhs.y }; }
		friend Vector2f operator / (float const& lhs, Vector2f const& rhs) noexcept { return { lhs / rhs.x, lhs / rhs.y }; }


	public:
		FURUD_NODISCARD FURUD_INLINE float operator ^ (Vector2f const& rhs) const noexcept
		{
			return x * rhs.y - y * rhs.x;
		}

		FURUD_NODISCARD FURUD_INLINE float CrossProduct(Vector2f const& rhs) const noexcept
		{
			return (*this) ^ rhs;
		}

		FURUD_NODISCARD FURUD_INLINE float operator | (Vector2f const& rhs) const noexcept
		{
			return x * rhs.x + y * rhs.y;
		}

		FURUD_NODISCARD FURUD_INLINE float DotProduct(Vector2f const& rhs) const noexcept
		{
			return (*this) | rhs;
		}

		FURUD_NODISCARD FURUD_INLINE float LengthSquared() const noexcept
		{
			return x * x + y * y;
		}

		FURUD_NODISCARD FURUD_INLINE float Length() const noexcept
		{
			return IMath::Sqrt(x * x + y * y);
		}

		FURUD_NODISCARD FURUD_INLINE Vector2f Normalize(float const& tolerance = IFloat::SMALLER) const noexcept
		{
			const float squareSum = x * x + y * y;
			if (squareSum > tolerance)
			{
				const float scale = IMath::RecipSqrt(squareSum);
				return { x * scale, y * scale };
			}
			return *this;
		}

		FURUD_NODISCARD FURUD_INLINE bool IsNormalized(float const& tolerance = IFloat::SMALL) const noexcept
		{
			const float squareSum = x * x + y * y;
			return (IMath::Abs(1.f - squareSum) < tolerance);
		}

		FURUD_NODISCARD FURUD_INLINE friend Vector2f Clamp(Vector2f const& inVal, float const& inMin, float const& inMax) noexcept
		{
			return
			{
				IMath::Clamp(inVal.x, inMin, inMax),
				IMath::Clamp(inVal.y, inMin, inMax)
			};
		}

		FURUD_NODISCARD FURUD_INLINE bool AnyNaN() const noexcept
		{
			return IMath::IsNaN(x) || IMath::IsNaN(y);
		}

		FURUD_NODISCARD FURUD_INLINE bool AllNaN() const noexcept
		{
			return IMath::IsNaN(x) && IMath::IsNaN(y);
		}

		FURUD_NODISCARD FURUD_INLINE bool AnyFinite() const noexcept
		{
			return IMath::IsFinite(x) || IMath::IsFinite(y);
		}

		FURUD_NODISCARD FURUD_INLINE bool AllFinite() const noexcept
		{
			return IMath::IsFinite(x) && IMath::IsFinite(y);
		}


	public:
		//FURUD_NODISCARD FURUD_INLINE BasicMatrix<float> ToMatrix() const noexcept;
		//FURUD_NODISCARD FURUD_INLINE BasicMatrix<float> ToInvMatrix() const noexcept;
	};


	const Vector2f Vector2f::MIN  = IFloat::MIN;
	const Vector2f Vector2f::MAX  = IFloat::MAX;
	const Vector2f Vector2f::ZERO = 0;
	const Vector2f Vector2f::ONE  = 1;
}



/** 2D 64-bit double floating-point vector. */
export namespace Furud
{
	/**
	 * @brief  2D 64-bit double floating-point vector.
	 * @details  二维单精度浮点向量。
	 */
	struct alignas(16) Vector2d
	{
		double x, y;


	public:
		static const Vector2d MIN;
		static const Vector2d MAX;
		static const Vector2d ZERO;
		static const Vector2d ONE;


	public:
		constexpr Vector2d() noexcept {}
		constexpr Vector2d(ForceInitFlag) noexcept : x(0), y(0) {}
		constexpr Vector2d(double const& v) noexcept : x(v), y(v) {}
		constexpr Vector2d(double const& sx, double const& sy) noexcept : x(sx), y(sy) {}
		constexpr Vector2d& operator = (double const& v) noexcept { x = y = v; return *this; }
		constexpr explicit Vector2d(struct Vector2i const& v) noexcept;
		constexpr explicit Vector2d(struct Vector2f const& v) noexcept;


	public:
		Vector2d(Vector2d const&) = default;
		Vector2d& operator = (Vector2d const&) = default;
		Vector2d(Vector2d&&) = default;
		Vector2d& operator = (Vector2d&&) = default;
		~Vector2d() noexcept {}


	public:
		bool operator == (Vector2d const& rhs) const noexcept { return x == rhs.x && y == rhs.y; }
		bool operator != (Vector2d const& rhs) const noexcept { return x != rhs.x || y != rhs.y; }


	public:
		Vector2d operator - () const noexcept { return { -x, -y }; }

		Vector2d operator + (Vector2d const& rhs) const noexcept { return { x + rhs.x, y + rhs.y }; }
		Vector2d operator - (Vector2d const& rhs) const noexcept { return { x - rhs.x, y - rhs.y }; }
		Vector2d operator * (Vector2d const& rhs) const noexcept { return { x * rhs.x, y * rhs.y }; }
		Vector2d operator / (Vector2d const& rhs) const noexcept { return { x / rhs.x, y / rhs.y }; }

		Vector2d const& operator += (Vector2d const& rhs) noexcept { x += rhs.x; y += rhs.y; return *this; }
		Vector2d const& operator -= (Vector2d const& rhs) noexcept { x -= rhs.x; y -= rhs.y; return *this; }
		Vector2d const& operator *= (Vector2d const& rhs) noexcept { x *= rhs.x; y *= rhs.y; return *this; }
		Vector2d const& operator /= (Vector2d const& rhs) noexcept { x /= rhs.x; y /= rhs.y; return *this; }

		Vector2d operator + (double const& rhs) const noexcept { return { x + rhs, y + rhs }; }
		Vector2d operator - (double const& rhs) const noexcept { return { x - rhs, y - rhs }; }
		Vector2d operator * (double const& rhs) const noexcept { return { x * rhs, y * rhs }; }
		Vector2d operator / (double const& rhs) const noexcept { return { x / rhs, y / rhs }; }

		Vector2d const& operator += (double const& rhs) noexcept { x += rhs; y += rhs; return *this; }
		Vector2d const& operator -= (double const& rhs) noexcept { x -= rhs; y -= rhs; return *this; }
		Vector2d const& operator *= (double const& rhs) noexcept { x *= rhs; y *= rhs; return *this; }
		Vector2d const& operator /= (double const& rhs) noexcept { x /= rhs; y /= rhs; return *this; }

		friend Vector2d operator + (float const& lhs, Vector2d const& rhs) noexcept { return { lhs + rhs.x, lhs + rhs.y }; }
		friend Vector2d operator - (float const& lhs, Vector2d const& rhs) noexcept { return { lhs - rhs.x, lhs - rhs.y }; }
		friend Vector2d operator * (float const& lhs, Vector2d const& rhs) noexcept { return { lhs * rhs.x, lhs * rhs.y }; }
		friend Vector2d operator / (float const& lhs, Vector2d const& rhs) noexcept { return { lhs / rhs.x, lhs / rhs.y }; }


	public:
		FURUD_NODISCARD FURUD_INLINE double operator ^ (Vector2d const& rhs) const noexcept
		{
			return x * rhs.y - y * rhs.x;
		}

		FURUD_NODISCARD FURUD_INLINE double CrossProduct(Vector2d const& rhs) const noexcept
		{
			return (*this) ^ rhs;
		}

		FURUD_NODISCARD FURUD_INLINE double operator | (Vector2d const& rhs) const noexcept
		{
			return x * rhs.x + y * rhs.y;
		}

		FURUD_NODISCARD FURUD_INLINE double DotProduct(Vector2d const& rhs) const noexcept
		{
			return (*this) | rhs;
		}

		FURUD_NODISCARD FURUD_INLINE double LengthSquared() const noexcept
		{
			return x * x + y * y;
		}

		FURUD_NODISCARD FURUD_INLINE double Length() const noexcept
		{
			return IMath::Sqrt(x * x + y * y);
		}

		FURUD_NODISCARD FURUD_INLINE Vector2d Normalize(double const& tolerance = IDouble::SMALLER) const noexcept
		{
			const double squareSum = x * x + y * y;
			if (squareSum > tolerance)
			{
				const double scale = IMath::RecipSqrt(squareSum);
				return { x * scale, y * scale };
			}
			return *this;
		}

		FURUD_NODISCARD FURUD_INLINE bool IsNormalized(double const& tolerance = IDouble::SMALL) const noexcept
		{
			const double squareSum = x * x + y * y;
			return (IMath::Abs(1.f - squareSum) < tolerance);
		}

		FURUD_NODISCARD FURUD_INLINE friend Vector2d Clamp(Vector2d const& inVal, double const& inMin, double const& inMax) noexcept
		{
			return
			{
				IMath::Clamp(inVal.x, inMin, inMax),
				IMath::Clamp(inVal.y, inMin, inMax)
			};
		}

		FURUD_NODISCARD FURUD_INLINE bool AnyNaN() const noexcept
		{
			return IMath::IsNaN(x) || IMath::IsNaN(y);
		}

		FURUD_NODISCARD FURUD_INLINE bool AllNaN() const noexcept
		{
			return IMath::IsNaN(x) && IMath::IsNaN(y);
		}

		FURUD_NODISCARD FURUD_INLINE bool AnyFinite() const noexcept
		{
			return IMath::IsFinite(x) || IMath::IsFinite(y);
		}

		FURUD_NODISCARD FURUD_INLINE bool AllFinite() const noexcept
		{
			return IMath::IsFinite(x) && IMath::IsFinite(y);
		}


	public:

	};


	const Vector2d Vector2d::MIN  = IDouble::MIN;
	const Vector2d Vector2d::MAX  = IDouble::MAX;
	const Vector2d Vector2d::ZERO = 0;
	const Vector2d Vector2d::ONE  = 1;
}



namespace Furud
{
	constexpr Vector2i::Vector2i(Vector2f const& v) noexcept : x((int32_t)v.x), y((int32_t)v.y) {}
	constexpr Vector2i::Vector2i(Vector2d const& v) noexcept : x((int32_t)v.x), y((int32_t)v.y) {}
	constexpr Vector2f::Vector2f(Vector2i const& v) noexcept : x((float)v.x), y((float)v.y) {}
	constexpr Vector2f::Vector2f(Vector2d const& v) noexcept : x((float)v.x), y((float)v.y) {}
	constexpr Vector2d::Vector2d(Vector2i const& v) noexcept : x((double)v.x), y((double)v.y) {}
	constexpr Vector2d::Vector2d(Vector2f const& v) noexcept : x((double)v.x), y((double)v.y) {}
}