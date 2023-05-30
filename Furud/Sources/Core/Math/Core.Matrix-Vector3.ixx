//
// Core.Matrix-Vector3.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Definitions of 3D vector.
//
module;

#include <Furud.hpp>
#include <stdint.h>



export module Furud.Core.Matrix:Vector3;

export import :Vector2;
export import Furud.Numbers;
import Furud.Platform.Math;
import Furud.Platform.SIMD;
import Furud.Core.Math;



/** Forward declaration. */
export namespace Furud
{
	struct Vector3i;
	furud_inline extern Vector3i Clamp(const Vector3i& inVal, int32_t const& inMin, int32_t const& inMax) noexcept;


	struct Vector3f;
	furud_inline extern Vector3f Clamp(const Vector3f& inVal, float const& inMin, float const& inMax) noexcept;


	struct Vector3d;
	furud_inline extern Vector3d Clamp(const Vector3d& inVal, double const& inMin, double const& inMax) noexcept;
}



/** 3D 32-bit integer vector. */
export namespace Furud
{
	/**
	 * @brief  3D 32-bit integer vector.
	 * @details  三维整型向量。
	 */
	struct Vector3i
	{
		int32_t x, y, z;


	public:
		static const Vector3i MIN;
		static const Vector3i MAX;
		static const Vector3i ZERO;
		static const Vector3i ONE;


	public:
		constexpr Vector3i() noexcept {}
		constexpr Vector3i(ForceInitFlag) noexcept : x(0), y(0), z(0) {}
		constexpr Vector3i(int32_t const& v) noexcept : x(v), y(v), z(v) {}
		constexpr Vector3i(int32_t const& sx, int32_t const& sy, int32_t const& sz) noexcept : x(sx), y(sy), z(sz) {}
		constexpr Vector3i(Vector2i const& xy, int32_t const& sz = 0) noexcept : x(xy.x), y(xy.y), z(sz) {}
		constexpr Vector3i(int32_t const& sx, Vector2i const& yz) noexcept : x(sx), y(yz.x), z(yz.y) {}
		constexpr Vector3i& operator = (int32_t const& v) noexcept { x = y = z = v; return *this; }
		constexpr explicit Vector3i(struct Vector3f const& v) noexcept;
		constexpr explicit Vector3i(struct Vector3d const& v) noexcept;


	public:
		Vector3i(const Vector3i&) = default;
		Vector3i& operator = (const Vector3i&) = default;
		Vector3i(Vector3i&&) = default;
		Vector3i& operator = (Vector3i&&) = default;
		~Vector3i() noexcept {}


	public:
		Vector2i XY() const noexcept { return { x, y }; }
		Vector2i XZ() const noexcept { return { x, z }; }
		Vector2i YZ() const noexcept { return { y, z }; }
		Vector2i& XYRef() const noexcept { return *(Vector2i*)(&x); }
		Vector2i& YZRef() const noexcept { return *(Vector2i*)(&y); }


	public:
		furud_nodiscard furud_inline Vec4i ToVec4() const noexcept
		{
			Vec4i r;
			r.Load3(this);
			return r;
		}


	public:
		bool operator == (const Vector3i& rhs) const noexcept { return x == rhs.x && y == rhs.y && z == rhs.z; }
		bool operator != (const Vector3i& rhs) const noexcept { return x != rhs.x || y != rhs.y || z != rhs.z; }


	public:
		Vector3i operator - () const noexcept { return { -x, -y, -z }; }

		Vector3i operator + (const Vector3i& rhs) const noexcept { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
		Vector3i operator - (const Vector3i& rhs) const noexcept { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
		Vector3i operator * (const Vector3i& rhs) const noexcept { return { x * rhs.x, y * rhs.y, z * rhs.z }; }
		Vector3i operator / (const Vector3i& rhs) const noexcept { return { x / rhs.x, y / rhs.y, z / rhs.z }; }

		const Vector3i& operator += (const Vector3i& rhs) noexcept { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
		const Vector3i& operator -= (const Vector3i& rhs) noexcept { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
		const Vector3i& operator *= (const Vector3i& rhs) noexcept { x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this; }
		const Vector3i& operator /= (const Vector3i& rhs) noexcept { x /= rhs.x; y /= rhs.y; z /= rhs.z; return *this; }

		Vector3i operator + (int32_t const& rhs) const noexcept { return { x + rhs, y + rhs, z + rhs }; }
		Vector3i operator - (int32_t const& rhs) const noexcept { return { x - rhs, y - rhs, z - rhs }; }
		Vector3i operator * (int32_t const& rhs) const noexcept { return { x * rhs, y * rhs, z * rhs }; }
		Vector3i operator / (int32_t const& rhs) const noexcept { return { x / rhs, y / rhs, z / rhs }; }

		const Vector3i& operator += (int32_t const& rhs) noexcept { x += rhs; y += rhs; z += rhs; return *this; }
		const Vector3i& operator -= (int32_t const& rhs) noexcept { x -= rhs; y -= rhs; z -= rhs; return *this; }
		const Vector3i& operator *= (int32_t const& rhs) noexcept { x *= rhs; y *= rhs; z *= rhs; return *this; }
		const Vector3i& operator /= (int32_t const& rhs) noexcept { x /= rhs; y /= rhs; z /= rhs; return *this; }

		friend Vector3i operator + (int32_t const& lhs, const Vector3i& rhs) noexcept { return { lhs + rhs.x, lhs + rhs.y, lhs + rhs.z }; }
		friend Vector3i operator - (int32_t const& lhs, const Vector3i& rhs) noexcept { return { lhs - rhs.x, lhs - rhs.y, lhs - rhs.z }; }
		friend Vector3i operator * (int32_t const& lhs, const Vector3i& rhs) noexcept { return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.z }; }
		friend Vector3i operator / (int32_t const& lhs, const Vector3i& rhs) noexcept { return { lhs / rhs.x, lhs / rhs.y, lhs / rhs.z }; }


	public:
		furud_nodiscard furud_inline friend Vector3i Clamp(const Vector3i& inVal, int32_t const& inMin, int32_t const& inMax) noexcept
		{
			return
			{
				IMath::Clamp(inVal.x, inMin, inMax),
				IMath::Clamp(inVal.y, inMin, inMax),
				IMath::Clamp(inVal.z, inMin, inMax)
			};
		}


	public:
		//furud_nodiscard furud_inline BasicMatrix<int32_t> ToMatrix() const noexcept;
		//furud_nodiscard furud_inline BasicMatrix<int32_t> ToInvMatrix() const noexcept;
	};


	const Vector3i Vector3i::MIN = IInteger::I32_MIN;
	const Vector3i Vector3i::MAX = IInteger::I32_MAX;
	const Vector3i Vector3i::ZERO = 0;
	const Vector3i Vector3i::ONE = 1;
}



/** 3D 32-bit single floating-point vector. */
export namespace Furud
{
	/**
	 * @brief  3D 32-bit single floating-point vector.
	 * @details  三维单精度浮点向量。
	 */
	struct Vector3f
	{
		float x, y, z;


	public:
		static const Vector3f MIN;
		static const Vector3f MAX;
		static const Vector3f ZERO;
		static const Vector3f ONE;


	public:
		constexpr Vector3f() noexcept {}		
		constexpr Vector3f(ForceInitFlag) noexcept: x(0), y(0), z(0) {}		
		constexpr Vector3f(float const& v) noexcept : x(v), y(v), z(v) {}
		constexpr Vector3f(float const& sx, float const& sy, float const& sz) noexcept : x(sx), y(sy), z(sz) {}
		constexpr Vector3f(Vector2f const& xy, float const& sz = 0) noexcept : x(xy.x), y(xy.y), z(sz) {}
		constexpr Vector3f(float const& sx, Vector2f const& yz) noexcept : x(sx), y(yz.x), z(yz.y) {}
		constexpr Vector3f& operator = (float const& v) noexcept { x = y = z = v; return *this; }
		constexpr explicit Vector3f(struct Vector3i const& v) noexcept;
		constexpr explicit Vector3f(struct Vector3d const& v) noexcept;


	public:
		Vector3f(const Vector3f&) = default;
		Vector3f& operator = (const Vector3f&) = default;
		Vector3f(Vector3f&&) = default;
		Vector3f& operator = (Vector3f&&) = default;
		~Vector3f() noexcept {}


	public:
		Vector2f XY() const noexcept { return { x, y }; }
		Vector2f XZ() const noexcept { return { x, z }; }
		Vector2f YZ() const noexcept { return { y, z }; }
		Vector2f& XYRef() const noexcept { return *(Vector2f*)(&x); }
		Vector2f& YZRef() const noexcept { return *(Vector2f*)(&y); }


	public:
		furud_nodiscard furud_inline Vec4f ToVec4() const noexcept
		{
			Vec4f r;
			r.Load3(this);
			return r;
		}


	public:
		bool operator == (const Vector3f& rhs) const noexcept { return x == rhs.x && y == rhs.y && z == rhs.z; }
		bool operator != (const Vector3f& rhs) const noexcept { return x != rhs.x || y != rhs.y || z != rhs.z; }


	public:
		Vector3f operator - () const noexcept { return { -x, -y, -z }; }
		
		Vector3f operator + (const Vector3f& rhs) const noexcept { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
		Vector3f operator - (const Vector3f& rhs) const noexcept { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
		Vector3f operator * (const Vector3f& rhs) const noexcept { return { x * rhs.x, y * rhs.y, z * rhs.z }; }
		Vector3f operator / (const Vector3f& rhs) const noexcept { return { x / rhs.x, y / rhs.y, z / rhs.z }; }
		
		const Vector3f& operator += (const Vector3f& rhs) noexcept { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
		const Vector3f& operator -= (const Vector3f& rhs) noexcept { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
		const Vector3f& operator *= (const Vector3f& rhs) noexcept { x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this; }
		const Vector3f& operator /= (const Vector3f& rhs) noexcept { x /= rhs.x; y /= rhs.y; z /= rhs.z; return *this; }

		Vector3f operator + (float const& rhs) const noexcept { return { x + rhs, y + rhs, z + rhs }; }
		Vector3f operator - (float const& rhs) const noexcept { return { x - rhs, y - rhs, z - rhs }; }
		Vector3f operator * (float const& rhs) const noexcept { return { x * rhs, y * rhs, z * rhs }; }
		Vector3f operator / (float const& rhs) const noexcept { return { x / rhs, y / rhs, z / rhs }; }
		
		const Vector3f& operator += (float const& rhs) noexcept { x += rhs; y += rhs; z += rhs; return *this; }
		const Vector3f& operator -= (float const& rhs) noexcept { x -= rhs; y -= rhs; z -= rhs; return *this; }
		const Vector3f& operator *= (float const& rhs) noexcept { x *= rhs; y *= rhs; z *= rhs; return *this; }
		const Vector3f& operator /= (float const& rhs) noexcept { x /= rhs; y /= rhs; z /= rhs; return *this; }

		friend Vector3f operator + (float const& lhs, const Vector3f& rhs) noexcept { return { lhs + rhs.x, lhs + rhs.y, lhs + rhs.z }; }
		friend Vector3f operator - (float const& lhs, const Vector3f& rhs) noexcept { return { lhs - rhs.x, lhs - rhs.y, lhs - rhs.z }; }
		friend Vector3f operator * (float const& lhs, const Vector3f& rhs) noexcept { return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.z }; }
		friend Vector3f operator / (float const& lhs, const Vector3f& rhs) noexcept { return { lhs / rhs.x, lhs / rhs.y, lhs / rhs.z }; }


	public:
		furud_nodiscard furud_inline Vector3f operator ^ (const Vector3f& rhs) const noexcept
		{
			return
			{
				y * rhs.z - z * rhs.y,
				z * rhs.x - x * rhs.z,
				x * rhs.y - y * rhs.x
			};
		}

		furud_nodiscard furud_inline Vector3f CrossProduct(const Vector3f& rhs) const noexcept
		{
			return (*this) ^ rhs;
		}

		furud_nodiscard furud_inline float operator | (const Vector3f& rhs) const noexcept
		{
			return x * rhs.x + y * rhs.y + z * rhs.z;
		}

		furud_nodiscard furud_inline float DotProduct(const Vector3f& rhs) const noexcept
		{
			return (*this) | rhs;
		}

		furud_nodiscard furud_inline float LengthSquared() const noexcept
		{
			return x * x + y * y + z * z;
		}

		furud_nodiscard furud_inline float Length() const noexcept
		{
			return IMath::Sqrt(x * x + y * y + z * z);
		}

		furud_nodiscard furud_inline Vector3f Normalize(float const& tolerance = IFloat::SMALLER) const noexcept
		{
			Vector3f result;
			Vec4f r;
			r.Load3(this).Normalize(tolerance).Store3(&result);
			return result;
		}

		furud_nodiscard furud_inline bool IsNormalized(float const& tolerance = IFloat::SMALL) const noexcept
		{
			Vec4f r;
			r.Load3(this);
			const float squareSum = Furud::DotProduct(r, r);
			return (IMath::Abs(1.f - squareSum) < tolerance);
		}

		furud_nodiscard furud_inline friend Vector3f Clamp(const Vector3f& inVal, float const& inMin, float const& inMax) noexcept
		{
			return
			{
				IMath::Clamp(inVal.x, inMin, inMax),
				IMath::Clamp(inVal.y, inMin, inMax),
				IMath::Clamp(inVal.z, inMin, inMax)
			};
		}

		furud_nodiscard furud_inline bool AnyNaN() const noexcept
		{
			return IMath::IsNaN(x) || IMath::IsNaN(y) || IMath::IsNaN(z);
		}

		furud_nodiscard furud_inline bool AllNaN() const noexcept
		{
			return IMath::IsNaN(x) && IMath::IsNaN(y) && IMath::IsNaN(z);
		}

		furud_nodiscard furud_inline bool AnyFinite() const noexcept
		{
			return IMath::IsFinite(x) || IMath::IsFinite(y) || IMath::IsFinite(z);
		}

		furud_nodiscard furud_inline bool AllFinite() const noexcept
		{
			return IMath::IsFinite(x) && IMath::IsFinite(y) && IMath::IsFinite(z);
		}


	public:
		//furud_nodiscard furud_inline BasicMatrix<float> ToMatrix() const noexcept;
		//furud_nodiscard furud_inline BasicMatrix<float> ToInvMatrix() const noexcept;
	};


	const Vector3f Vector3f::MIN = IFloat::MIN;
	const Vector3f Vector3f::MAX = IFloat::MAX;
	const Vector3f Vector3f::ZERO = 0;
	const Vector3f Vector3f::ONE = 1;
}



/** 3D 64-bit double floating-point vector. */
export namespace Furud
{
	/**
	 * @brief  3D 64-bit double floating-point vector.
	 * @details  三维单精度浮点向量。
	 */
	struct Vector3d
	{
		double x, y, z;


	public:
		static const Vector3d MIN;
		static const Vector3d MAX;
		static const Vector3d ZERO;
		static const Vector3d ONE;


	public:
		constexpr Vector3d() noexcept {}
		constexpr Vector3d(ForceInitFlag) noexcept : x(0), y(0), z(0) {}
		constexpr Vector3d(double const& v) noexcept : x(v), y(v), z(v) {}
		constexpr Vector3d(double const& sx, double const& sy, double const& sz) noexcept : x(sx), y(sy), z(sz) {}
		constexpr Vector3d(Vector2d const& xy, double const& sz = 0) noexcept : x(xy.x), y(xy.y), z(sz) {}
		constexpr Vector3d(double const& sx, Vector2d const& yz) noexcept : x(sx), y(yz.x), z(yz.y) {}
		constexpr Vector3d& operator = (double const& v) noexcept { x = y = z = v; return *this; }
		constexpr explicit Vector3d(struct Vector3i const& v) noexcept;
		constexpr explicit Vector3d(struct Vector3f const& v) noexcept;


	public:
		Vector3d(const Vector3d&) = default;
		Vector3d& operator = (const Vector3d&) = default;
		Vector3d(Vector3d&&) = default;
		Vector3d& operator = (Vector3d&&) = default;
		~Vector3d() noexcept {}


	public:
		Vector2d XY() const noexcept { return { x, y }; }
		Vector2d XZ() const noexcept { return { x, z }; }
		Vector2d YZ() const noexcept { return { y, z }; }
		Vector2d& XYRef() const noexcept { return *(Vector2d*)(&x); }
		Vector2d& YZRef() const noexcept { return *(Vector2d*)(&y); }


	public:
		furud_nodiscard furud_inline Vec4f ToVec4() const noexcept
		{
			Vec4f r;
			r.Load3(this);
			return r;
		}


	public:
		bool operator == (const Vector3d& rhs) const noexcept { return x == rhs.x && y == rhs.y && z == rhs.z; }
		bool operator != (const Vector3d& rhs) const noexcept { return x != rhs.x || y != rhs.y || z != rhs.z; }


	public:
		Vector3d operator - () const noexcept { return { -x, -y, -z }; }

		Vector3d operator + (const Vector3d& rhs) const noexcept { return { x + rhs.x, y + rhs.y, z + rhs.z }; }
		Vector3d operator - (const Vector3d& rhs) const noexcept { return { x - rhs.x, y - rhs.y, z - rhs.z }; }
		Vector3d operator * (const Vector3d& rhs) const noexcept { return { x * rhs.x, y * rhs.y, z * rhs.z }; }
		Vector3d operator / (const Vector3d& rhs) const noexcept { return { x / rhs.x, y / rhs.y, z / rhs.z }; }

		const Vector3d& operator += (const Vector3d& rhs) noexcept { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
		const Vector3d& operator -= (const Vector3d& rhs) noexcept { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
		const Vector3d& operator *= (const Vector3d& rhs) noexcept { x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this; }
		const Vector3d& operator /= (const Vector3d& rhs) noexcept { x /= rhs.x; y /= rhs.y; z /= rhs.z; return *this; }

		Vector3d operator + (double const& rhs) const noexcept { return { x + rhs, y + rhs, z + rhs }; }
		Vector3d operator - (double const& rhs) const noexcept { return { x - rhs, y - rhs, z - rhs }; }
		Vector3d operator * (double const& rhs) const noexcept { return { x * rhs, y * rhs, z * rhs }; }
		Vector3d operator / (double const& rhs) const noexcept { return { x / rhs, y / rhs, z / rhs }; }

		const Vector3d& operator += (double const& rhs) noexcept { x += rhs; y += rhs; z += rhs; return *this; }
		const Vector3d& operator -= (double const& rhs) noexcept { x -= rhs; y -= rhs; z -= rhs; return *this; }
		const Vector3d& operator *= (double const& rhs) noexcept { x *= rhs; y *= rhs; z *= rhs; return *this; }
		const Vector3d& operator /= (double const& rhs) noexcept { x /= rhs; y /= rhs; z /= rhs; return *this; }

		friend Vector3d operator + (double const& lhs, const Vector3d& rhs) noexcept { return { lhs + rhs.x, lhs + rhs.y, lhs + rhs.z }; }
		friend Vector3d operator - (double const& lhs, const Vector3d& rhs) noexcept { return { lhs - rhs.x, lhs - rhs.y, lhs - rhs.z }; }
		friend Vector3d operator * (double const& lhs, const Vector3d& rhs) noexcept { return { lhs * rhs.x, lhs * rhs.y, lhs * rhs.z }; }
		friend Vector3d operator / (double const& lhs, const Vector3d& rhs) noexcept { return { lhs / rhs.x, lhs / rhs.y, lhs / rhs.z }; }


	public:
		furud_nodiscard furud_inline Vector3d operator ^ (const Vector3d& rhs) const noexcept
		{
			return
			{
				y * rhs.z - z * rhs.y,
				z * rhs.x - x * rhs.z,
				x * rhs.y - y * rhs.x
			};
		}

		furud_nodiscard furud_inline Vector3d CrossProduct(const Vector3d& rhs) const noexcept
		{
			return (*this) ^ rhs;
		}

		furud_nodiscard furud_inline double operator | (const Vector3d& rhs) const noexcept
		{
			return x * rhs.x + y * rhs.y + z * rhs.z;
		}

		furud_nodiscard furud_inline double DotProduct(const Vector3d& rhs) const noexcept
		{
			return (*this) | rhs;
		}

		furud_nodiscard furud_inline double LengthSquared() const noexcept
		{
			return x * x + y * y + z * z;
		}

		furud_nodiscard furud_inline double Length() const noexcept
		{
			return IMath::Sqrt(x * x + y * y + z * z);
		}

		furud_nodiscard furud_inline Vector3d Normalize(double const& tolerance = IDouble::SMALLER) const noexcept
		{
			Vector3d result = *this;
			const double squareSum = result.LengthSquared();
			if (squareSum > tolerance)
			{
				const float scale = IMath::RecipSqrt(squareSum);
				result.x *= scale;
				result.y *= scale;
				result.z *= scale;
			}
			return result;
		}

		furud_nodiscard furud_inline bool IsNormalized(double const& tolerance = IDouble::SMALL) const noexcept
		{
			const double squareSum = LengthSquared();
			return (IMath::Abs(1.f - squareSum) < tolerance);
		}

		furud_nodiscard furud_inline friend Vector3d Clamp(const Vector3d& inVal, double const& inMin, double const& inMax) noexcept
		{
			return
			{
				IMath::Clamp(inVal.x, inMin, inMax),
				IMath::Clamp(inVal.y, inMin, inMax),
				IMath::Clamp(inVal.z, inMin, inMax)
			};
		}

		furud_nodiscard furud_inline bool AnyNaN() const noexcept
		{
			return IMath::IsNaN(x) || IMath::IsNaN(y) || IMath::IsNaN(z);
		}

		furud_nodiscard furud_inline bool AllNaN() const noexcept
		{
			return IMath::IsNaN(x) && IMath::IsNaN(y) && IMath::IsNaN(z);
		}

		furud_nodiscard furud_inline bool AnyFinite() const noexcept
		{
			return IMath::IsFinite(x) || IMath::IsFinite(y) || IMath::IsFinite(z);
		}

		furud_nodiscard furud_inline bool AllFinite() const noexcept
		{
			return IMath::IsFinite(x) && IMath::IsFinite(y) && IMath::IsFinite(z);
		}


	public:
		//furud_nodiscard furud_inline BasicMatrix<double> ToMatrix() const noexcept;
		//furud_nodiscard furud_inline BasicMatrix<double> ToInvMatrix() const noexcept;
	};


	const Vector3d Vector3d::MIN = IDouble::MIN;
	const Vector3d Vector3d::MAX = IDouble::MAX;
	const Vector3d Vector3d::ZERO = 0;
	const Vector3d Vector3d::ONE = 1;
}



namespace Furud
{
	constexpr Vector3i::Vector3i(Vector3f const& v) noexcept : x((int32_t)v.x), y((int32_t)v.y), z((int32_t)v.z) {}
	constexpr Vector3i::Vector3i(Vector3d const& v) noexcept : x((int32_t)v.x), y((int32_t)v.y), z((int32_t)v.z) {}
	constexpr Vector3f::Vector3f(Vector3i const& v) noexcept : x((float)v.x), y((float)v.y), z((float)v.z) {}
	constexpr Vector3f::Vector3f(Vector3d const& v) noexcept : x((float)v.x), y((float)v.y), z((float)v.z) {}
	constexpr Vector3d::Vector3d(Vector3i const& v) noexcept : x((double)v.x), y((double)v.y), z((double)v.z) {}
	constexpr Vector3d::Vector3d(Vector3f const& v) noexcept : x((double)v.x), y((double)v.y), z((double)v.z) {}
}