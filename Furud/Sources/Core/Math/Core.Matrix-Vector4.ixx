//
// Core.Matrix-Vector4.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Definitions of 4D vector.
//
module;

#include <Furud.hpp>
#include <stdint.h>
#include <assert.h>



export module Furud.Core.Matrix:Vector4;

export import :Vector3;
export import Furud.Numbers;
import Furud.Platform.Math;
import Furud.Platform.SIMD;
import Furud.Core.Math;



/** Forward declaration. */
export namespace Furud
{
	struct Vector4i;
	furud_inline extern Vector4i Clamp(Vector4i inVal, int32_t const& inMin, int32_t const& inMax) noexcept;


	struct Vector4f;
	furud_inline extern Vector4f Clamp(Vector4f inVal, float const& inMin, float const& inMax) noexcept;
	furud_inline extern void SinCos(const Vector4f& inAngles, Vector4f& outSineResult, Vector4f& outCosineResult) noexcept;

	struct Vector4d;
	furud_inline extern Vector4d Clamp(Vector4d inVal, double const& inMin, double const& inMax) noexcept;
}



/** 4D 32-bit integer vector. */
export namespace Furud
{
	/**
	 * @brief  4D 32-bit integer vector.
	 * @details  三维整型向量。
	 */
	struct alignas(16) Vector4i
	{
		int32_t x, y, z, w;


	public:
		static const Vector4i MIN;
		static const Vector4i MAX;
		static const Vector4i ZERO;
		static const Vector4i ONE;


	public:
		constexpr Vector4i() noexcept {}
		constexpr Vector4i(ForceInitFlag) noexcept : x(0), y(0), z(0), w(0) {}
		constexpr Vector4i(int32_t const& v) noexcept : x(v), y(v), z(v), w(v) {}
		constexpr Vector4i(int32_t const& sx, int32_t const& sy, int32_t const& sz, int32_t const& sw) noexcept : x(sx), y(sy), z(sz), w(sw) {}
		constexpr Vector4i(Vector2i const& xy, int32_t const& sz = 0, int32_t const& sw = 0) noexcept : x(xy.x), y(xy.y), z(sz), w(sw) {}
		constexpr Vector4i(int32_t const& sx, int32_t const& sy, Vector2i const& zw) noexcept : x(sx), y(sy), z(zw.x), w(zw.y) {}
		constexpr Vector4i(int32_t const& sx, Vector2i const& yz, int32_t const& sw) noexcept : x(sx), y(yz.x), z(yz.y), w(sw) {}
		constexpr Vector4i(Vector2i const& xy, Vector2i const& zw) noexcept : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}
		constexpr Vector4i(int32_t const& sx, Vector3i const& yzw) noexcept : x(sx), y(yzw.x), z(yzw.y), w(yzw.z) {}
		constexpr Vector4i(Vector3i const& xyz, int32_t const& sw = 0) noexcept : x(xyz.x), y(xyz.y), z(xyz.z), w(sw) {}
		constexpr Vector4i& operator = (int32_t const& v) noexcept { x = y = z = w = v; return *this; }
		constexpr explicit Vector4i(struct Vector4f const& v) noexcept;
		constexpr explicit Vector4i(struct Vector4d const& v) noexcept;


	public:
		Vector4i(const Vector4i&) = default;
		Vector4i& operator = (const Vector4i&) = default;
		Vector4i(Vector4i&&) = default;
		Vector4i& operator = (Vector4i&&) = default;
		~Vector4i() noexcept {}


	public:
		Vector2i XY() const noexcept { return { x, y }; }
		Vector2i YZ() const noexcept { return { y, z }; }
		Vector2i ZW() const noexcept { return { z, w }; }
		Vector3i XYZ() const noexcept { return { x, y, z }; }
		Vector3i YZW() const noexcept { return { y, z, w }; }
		Vector2i& XYRef() const noexcept { return *(Vector2i*)(&x); }
		Vector2i& YZRef() const noexcept { return *(Vector2i*)(&y); }
		Vector2i& ZWRef() const noexcept { return *(Vector2i*)(&z); }
		Vector3i& XYZRef() const noexcept { return *(Vector3i*)(&x); }
		Vector3i& YZWRef() const noexcept { return *(Vector3i*)(&y); }


	public:
		furud_nodiscard furud_inline Vec4i ToVec4() const noexcept
		{
			Vec4i r;
			r.Load4(this);
			return r;
		}

		furud_nodiscard furud_intrinsic Vec4i& AsVec4() noexcept
		{
			return *(Vec4i*)(this);
		}

		furud_nodiscard furud_intrinsic const Vec4i& AsVec4() const noexcept
		{
			return *(Vec4i*)(this);
		}


	public:
		bool operator == (const Vector4i& rhs) const noexcept { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
		bool operator != (const Vector4i& rhs) const noexcept { return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w; }


	public:
		Vector4i operator - () const noexcept { Vector4i v; (-AsVec4()).Store4(&v); return v; }

		Vector4i operator + (const Vector4i& rhs) const noexcept { Vector4i v; v.AsVec4() = AsVec4() + rhs.AsVec4(); return v; }
		Vector4i operator - (const Vector4i& rhs) const noexcept { Vector4i v; v.AsVec4() = AsVec4() - rhs.AsVec4(); return v; }
		Vector4i operator * (const Vector4i& rhs) const noexcept { Vector4i v; v.AsVec4() = AsVec4() * rhs.AsVec4(); return v; }
		Vector4i operator / (const Vector4i& rhs) const noexcept { Vector4i v; v.AsVec4() = AsVec4() / rhs.AsVec4(); return v; }

		const Vector4i& operator += (const Vector4i& rhs) noexcept { AsVec4() = AsVec4() + rhs.AsVec4(); }
		const Vector4i& operator -= (const Vector4i& rhs) noexcept { AsVec4() = AsVec4() - rhs.AsVec4(); }
		const Vector4i& operator *= (const Vector4i& rhs) noexcept { AsVec4() = AsVec4() * rhs.AsVec4(); }
		const Vector4i& operator /= (const Vector4i& rhs) noexcept { AsVec4() = AsVec4() / rhs.AsVec4(); }

		Vector4i operator + (int32_t const& rhs) const noexcept { Vector4i v; v.AsVec4() = AsVec4() + Vec4i(rhs); return v; }
		Vector4i operator - (int32_t const& rhs) const noexcept { Vector4i v; v.AsVec4() = AsVec4() - Vec4i(rhs); return v; }
		Vector4i operator * (int32_t const& rhs) const noexcept { Vector4i v; v.AsVec4() = AsVec4() * Vec4i(rhs); return v; }
		Vector4i operator / (int32_t const& rhs) const noexcept { Vector4i v; v.AsVec4() = AsVec4() / Vec4i(rhs); return v; }

		const Vector4i& operator += (int32_t const& rhs) noexcept { AsVec4() = AsVec4() + Vec4i(rhs); }
		const Vector4i& operator -= (int32_t const& rhs) noexcept { AsVec4() = AsVec4() - Vec4i(rhs); }
		const Vector4i& operator *= (int32_t const& rhs) noexcept { AsVec4() = AsVec4() * Vec4i(rhs); }
		const Vector4i& operator /= (int32_t const& rhs) noexcept { AsVec4() = AsVec4() / Vec4i(rhs); }


	public:
		furud_nodiscard furud_inline friend Vector4i Clamp(Vector4i inVal, int32_t const& inMin, int32_t const& inMax) noexcept
		{
			Vec4i& vval = inVal.AsVec4();
			Vec4i vmin = inMin;
			Vec4i vmax = inMax;
			vval = Max(vmin, Min(vval, vmax));
			return inVal;
		}
	};


	const Vector4i Vector4i::MIN = IInteger::I32_MIN;
	const Vector4i Vector4i::MAX = IInteger::I32_MAX;
	const Vector4i Vector4i::ZERO = 0;
	const Vector4i Vector4i::ONE = 1;
}



/** 4D 32-bit single floating-point vector. */
export namespace Furud
{
	/**
	 * @brief  4D 32-bit single floating-point vector.
	 * @details  三维单精度浮点向量。
	 */
	struct alignas(16) Vector4f
	{
		float x, y, z, w;


	public:
		static const Vector4f MIN;
		static const Vector4f MAX;
		static const Vector4f ZERO;
		static const Vector4f ONE;


	public:
		constexpr Vector4f() noexcept {}
		constexpr Vector4f(ForceInitFlag) noexcept : x(0), y(0), z(0), w(0) {}
		constexpr Vector4f(float const& v) noexcept : x(v), y(v), z(v), w(v) {}
		constexpr Vector4f(float const& sx, float const& sy, float const& sz, float const& sw) noexcept : x(sx), y(sy), z(sz), w(sw) {}
		constexpr Vector4f(Vector2f const& xy, float const& sz = 0, float const& sw = 0) noexcept : x(xy.x), y(xy.y), z(sz), w(sw) {}
		constexpr Vector4f(float const& sx, float const& sy, Vector2f const& zw) noexcept : x(sx), y(sy), z(zw.x), w(zw.y) {}
		constexpr Vector4f(float const& sx, Vector2f const& yz, float const& sw) noexcept : x(sx), y(yz.x), z(yz.y), w(sw) {}
		constexpr Vector4f(Vector2f const& xy, Vector2f const& zw) noexcept : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}
		constexpr Vector4f(float const& sx, Vector3f const& yzw) noexcept : x(sx), y(yzw.x), z(yzw.y), w(yzw.z) {}
		constexpr Vector4f(Vector3f const& xyz, float const& sw = 0) noexcept : x(xyz.x), y(xyz.y), z(xyz.z), w(sw) {}
		constexpr Vector4f& operator = (float const& v) noexcept { x = y = z = w = v; return *this; }
		constexpr explicit Vector4f(struct Vector4i const& v) noexcept;
		constexpr explicit Vector4f(struct Vector4d const& v) noexcept;


	public:
		Vector4f(const Vector4f&) = default;
		Vector4f& operator = (const Vector4f&) = default;
		Vector4f(Vector4f&&) = default;
		Vector4f& operator = (Vector4f&&) = default;
		~Vector4f() noexcept {}


	public:
		Vector2f XY() const noexcept { return { x, y }; }
		Vector2f YZ() const noexcept { return { y, z }; }
		Vector2f ZW() const noexcept { return { z, w }; }
		Vector3f XYZ() const noexcept { return { x, y, z }; }
		Vector3f YZW() const noexcept { return { y, z, w }; }
		Vector2f& XYRef() const noexcept { return *(Vector2f*)(&x); }
		Vector2f& YZRef() const noexcept { return *(Vector2f*)(&y); }
		Vector2f& ZWRef() const noexcept { return *(Vector2f*)(&z); }
		Vector3f& XYZRef() const noexcept { return *(Vector3f*)(&x); }
		Vector3f& YZWRef() const noexcept { return *(Vector3f*)(&y); }


	public:
		furud_nodiscard furud_inline Vec4f ToVec4() const noexcept
		{
			Vec4f r;
			r.Load4(this);
			return r;
		}

		furud_nodiscard furud_intrinsic Vec4f& AsVec4() noexcept
		{
			return *(Vec4f*)(this);
		}

		furud_nodiscard furud_intrinsic const Vec4f& AsVec4() const noexcept
		{
			return *(Vec4f*)(this);
		}

	public:
		constexpr float operator [](const int32_t& i) const noexcept
		{
			assert(i >=0 && i <= 3);
			return (&x)[i];
		}

	public:
		bool operator == (const Vector4f& rhs) const noexcept { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
		bool operator != (const Vector4f& rhs) const noexcept { return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w; }


	public:
		Vector4f operator + (const Vector4f& rhs) const noexcept { Vector4f v; v.AsVec4() = AsVec4() + rhs.AsVec4(); return v; }
		Vector4f operator - (const Vector4f& rhs) const noexcept { Vector4f v; v.AsVec4() = AsVec4() - rhs.AsVec4(); return v; }
		Vector4f operator * (const Vector4f& rhs) const noexcept { Vector4f v; v.AsVec4() = AsVec4() * rhs.AsVec4(); return v; }
		Vector4f operator / (const Vector4f& rhs) const noexcept { Vector4f v; v.AsVec4() = AsVec4() / rhs.AsVec4(); return v; }
		const Vector4f& operator += (const Vector4f& rhs) noexcept { AsVec4() = AsVec4() + rhs.AsVec4(); }
		const Vector4f& operator -= (const Vector4f& rhs) noexcept { AsVec4() = AsVec4() - rhs.AsVec4(); }
		const Vector4f& operator *= (const Vector4f& rhs) noexcept { AsVec4() = AsVec4() * rhs.AsVec4(); }
		const Vector4f& operator /= (const Vector4f& rhs) noexcept { AsVec4() = AsVec4() / rhs.AsVec4(); }

		Vector4f operator - () const noexcept { Vector4f v; (-AsVec4()).Store4(&v); return v; }
		Vector4f operator + (float const& rhs) const noexcept { Vector4f v; v.AsVec4() = AsVec4() + Vec4f(rhs); return v; }
		Vector4f operator - (float const& rhs) const noexcept { Vector4f v; v.AsVec4() = AsVec4() - Vec4f(rhs); return v; }
		Vector4f operator * (float const& rhs) const noexcept { Vector4f v; v.AsVec4() = AsVec4() * Vec4f(rhs); return v; }
		Vector4f operator / (float const& rhs) const noexcept { Vector4f v; v.AsVec4() = AsVec4() / Vec4f(rhs); return v; }
		const Vector4f& operator += (float const& rhs) noexcept { AsVec4() = AsVec4() + Vec4f(rhs); }
		const Vector4f& operator -= (float const& rhs) noexcept { AsVec4() = AsVec4() - Vec4f(rhs); }
		const Vector4f& operator *= (float const& rhs) noexcept { AsVec4() = AsVec4() * Vec4f(rhs); }
		const Vector4f& operator /= (float const& rhs) noexcept { AsVec4() = AsVec4() / Vec4f(rhs); }


	public:
		furud_nodiscard furud_inline float operator | (const Vector4f& rhs) const noexcept
		{
			return Furud::DotProduct(AsVec4(), rhs.AsVec4());
		}

		furud_nodiscard furud_inline float DotProduct(const Vector4f& rhs) const noexcept
		{
			return *this | rhs;
		}

		furud_nodiscard furud_inline float LengthSquared() const noexcept
		{
			return AsVec4().Squares().Sum();
		}

		furud_nodiscard furud_inline float Length() const noexcept
		{
			return Furud::DotProduct(AsVec4(), AsVec4());
		}

		furud_nodiscard furud_inline Vector4f Normalize(float const& tolerance = IFloat::SMALL) const noexcept
		{
			Vector4f v;
			AsVec4().Normalize().Store4(&v);
			return v;
		}

		furud_nodiscard furud_inline bool IsNormalized(float const& tolerance = IFloat::SMALL) const noexcept
		{
			const float squareSum = AsVec4().Squares().Sum();
			return (IMath::Abs(1.f - squareSum) < tolerance);
		}

		furud_nodiscard furud_inline Vector4f Homogenize() const noexcept
		{
			Vector4f v;
			AsVec4().Homogenize().Store4(&v);
			return v;
		}

		furud_nodiscard furud_inline friend Vector4f Clamp(Vector4f inVal, float const& inMin, float const& inMax) noexcept
		{
			Vec4f& vval = inVal.AsVec4();
			Vec4f vmin = inMin;
			Vec4f vmax = inMax;
			vval = Max(vmin, Min(vval, vmax));
			return inVal;
		}


	public:
		furud_inline friend void SinCos(const Vector4f& inAngles, Vector4f& outSineResult, Vector4f& outCosineResult) noexcept
		{
			const Vec4f& angles = inAngles.AsVec4();
			const Vec4f RECIP_PI2 { 1.f / (2.f * IFloat::PI) };
			
			// Map to [-pi, pi]
			// X = a - 2pi * round(a/2pi)
			// Note the round(), not truncate(). In this case round() can round halfway cases using round-to-nearest-even OR round-to-nearest.
			// quotient = round(a/2pi)
			Vec4f quotient = angles * RECIP_PI2;
			// round to nearest even is the default rounding mode but that's fine here.
			quotient = quotient.Round();
			// X = a - 2pi * quotient
			Vec4f x = angles - Vec4f::PI2 * quotient;

			// Map in [-pi/2,pi/2]
			Vec4f sign = And(x, Vec4f::SIGN_BIT);
			Vec4f siPI = Or(Vec4f::PI, sign);  // pi when x >= 0, -pi when x < 0
			Vec4f absX = And(x, Vec4f::SIGN_MASK);
			Vec4f rflX = siPI - x;
			Vec4f mask = GreaterThan(absX, Vec4f::HALF_PI);
			x = Select(mask, rflX, x);
			sign = Select(mask, Vec4f::NEG_ONE, Vec4f::ONE);

			const Vec4f squaredX = x.Squares();

			// 11-degree minimax approximation
			// outSineResult = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;
			const Vec4f sinCoeff0{ 1.0f, -0.16666667f, 0.0083333310f, -0.00019840874f };
			const Vec4f sinCoeff1{ 2.7525562e-06f };
			const Vec4f sinCoeff2{ -2.3889859e-08f };

			Vec4f s = sinCoeff2;
			s = MultiplyAdd(squaredX, s, sinCoeff1);
			s = MultiplyAdd(squaredX, s, sinCoeff0.ReplicateW());
			s = MultiplyAdd(squaredX, s, sinCoeff0.ReplicateZ());
			s = MultiplyAdd(squaredX, s, sinCoeff0.ReplicateY());
			s = MultiplyAdd(squaredX, s, sinCoeff0.ReplicateX());
			outSineResult.AsVec4() = s * x;

			// 10-degree minimax approximation
			// outCosineResult = sign * (((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f);
			const Vec4f cosCoeff0{ 1.0f, -0.5f, 0.041666638f, -0.0013888378f };
			const Vec4f cosCoeff1{ 2.4760495e-05f };
			const Vec4f cosCoeff2{ -2.6051615e-07f };

			Vec4f c = cosCoeff2;
			c = MultiplyAdd(squaredX, c, cosCoeff1);
			c = MultiplyAdd(squaredX, c, cosCoeff0.ReplicateW());
			c = MultiplyAdd(squaredX, c, cosCoeff0.ReplicateZ());
			c = MultiplyAdd(squaredX, c, cosCoeff0.ReplicateY());
			c = MultiplyAdd(squaredX, c, cosCoeff0.ReplicateX());
			outCosineResult.AsVec4() = c * sign;
		}
	};


	const Vector4f Vector4f::MIN = IFloat::MIN;
	const Vector4f Vector4f::MAX = IFloat::MAX;
	const Vector4f Vector4f::ZERO = 0;
	const Vector4f Vector4f::ONE = 1;
}



/** 4D 64-bit double floating-point vector. */
export namespace Furud
{
	/**
	 * @brief  4D 64-bit double floating-point vector.
	 * @details  三维单精度浮点向量。
	 */
	struct alignas(32) Vector4d
	{
		double x, y, z, w;


	public:
		static const Vector4d MIN;
		static const Vector4d MAX;
		static const Vector4d ZERO;
		static const Vector4d ONE;


	public:
		constexpr Vector4d() noexcept {}
		constexpr Vector4d(ForceInitFlag) noexcept : x(0), y(0), z(0), w(0) {}
		constexpr Vector4d(double const& v) noexcept : x(v), y(v), z(v), w(v) {}
		constexpr Vector4d(double const& sx, double const& sy, double const& sz, double const& sw) noexcept : x(sx), y(sy), z(sz), w(sw) {}
		constexpr Vector4d(Vector2d const& xy, double const& sz = 0, double const& sw = 0) noexcept : x(xy.x), y(xy.y), z(sz), w(sw) {}
		constexpr Vector4d(double const& sx, double const& sy, Vector2d const& zw) noexcept : x(sx), y(sy), z(zw.x), w(zw.y) {}
		constexpr Vector4d(double const& sx, Vector2d const& yz, double const& sw) noexcept : x(sx), y(yz.x), z(yz.y), w(sw) {}
		constexpr Vector4d(Vector2d const& xy, Vector2d const& zw) noexcept : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}
		constexpr Vector4d(double const& sx, Vector3d const& yzw) noexcept : x(sx), y(yzw.x), z(yzw.y), w(yzw.z) {}
		constexpr Vector4d(Vector3d const& xyz, double const& sw = 0) noexcept : x(xyz.x), y(xyz.y), z(xyz.z), w(sw) {}
		constexpr Vector4d& operator = (double const& v) noexcept { x = y = z = w = v; return *this; }
		constexpr explicit Vector4d(struct Vector4i const& v) noexcept;
		constexpr explicit Vector4d(struct Vector4f const& v) noexcept;


	public:
		Vector4d(const Vector4d&) = default;
		Vector4d& operator = (const Vector4d&) = default;
		Vector4d(Vector4d&&) = default;
		Vector4d& operator = (Vector4d&&) = default;
		~Vector4d() noexcept {}


	public:
		Vector2d XY() const noexcept { return { x, y }; }
		Vector2d YZ() const noexcept { return { y, z }; }
		Vector2d ZW() const noexcept { return { z, w }; }
		Vector3d XYZ() const noexcept { return { x, y, z }; }
		Vector3d YZW() const noexcept { return { y, z, w }; }
		Vector2d& XYRef() const noexcept { return *(Vector2d*)(&x); }
		Vector2d& YZRef() const noexcept { return *(Vector2d*)(&y); }
		Vector2d& ZWRef() const noexcept { return *(Vector2d*)(&z); }
		Vector3d& XYZRef() const noexcept { return *(Vector3d*)(&x); }
		Vector3d& YZWRef() const noexcept { return *(Vector3d*)(&y); }


	public:
		bool operator == (const Vector4d& rhs) const noexcept { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
		bool operator != (const Vector4d& rhs) const noexcept { return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w; }


	public:
		Vector4d operator - () const noexcept { return { -x, -y, -z, -w }; }

		Vector4d operator + (const Vector4d& rhs) const noexcept { return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w }; }
		Vector4d operator - (const Vector4d& rhs) const noexcept { return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w }; }
		Vector4d operator * (const Vector4d& rhs) const noexcept { return { x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w }; }
		Vector4d operator / (const Vector4d& rhs) const noexcept { return { x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w }; }

		const Vector4d& operator += (const Vector4d& rhs) noexcept { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
		const Vector4d& operator -= (const Vector4d& rhs) noexcept { x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this; }
		const Vector4d& operator *= (const Vector4d& rhs) noexcept { x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w; return *this; }
		const Vector4d& operator /= (const Vector4d& rhs) noexcept { x /= rhs.x; y /= rhs.y; z /= rhs.z; w /= rhs.w; return *this; }

		Vector4d operator + (double const& rhs) const noexcept { return { x + rhs, y + rhs, z + rhs, w + rhs }; }
		Vector4d operator - (double const& rhs) const noexcept { return { x - rhs, y - rhs, z - rhs, w - rhs }; }
		Vector4d operator * (double const& rhs) const noexcept { return { x * rhs, y * rhs, z * rhs, w * rhs }; }
		Vector4d operator / (double const& rhs) const noexcept { return { x / rhs, y / rhs, z / rhs, w / rhs }; }

		const Vector4d& operator += (double const& rhs) noexcept { x += rhs; y += rhs; z += rhs; w += rhs; return *this; }
		const Vector4d& operator -= (double const& rhs) noexcept { x -= rhs; y -= rhs; z -= rhs; w -= rhs; return *this; }
		const Vector4d& operator *= (double const& rhs) noexcept { x *= rhs; y *= rhs; z *= rhs; w *= rhs; return *this; }
		const Vector4d& operator /= (double const& rhs) noexcept { x /= rhs; y /= rhs; z /= rhs; w /= rhs; return *this; }


	public:
		furud_nodiscard furud_inline friend Vector4d Clamp(Vector4d inVal, double const& inMin, double const& inMax) noexcept
		{
			return
			{
				IMath::Clamp(inVal.x, inMin, inMax),
				IMath::Clamp(inVal.y, inMin, inMax),
				IMath::Clamp(inVal.z, inMin, inMax),
				IMath::Clamp(inVal.w, inMin, inMax)
			};
		}
	};


	const Vector4d Vector4d::MIN = IDouble::MIN;
	const Vector4d Vector4d::MAX = IDouble::MAX;
	const Vector4d Vector4d::ZERO = 0;
	const Vector4d Vector4d::ONE = 1;
}



namespace Furud
{
	constexpr Vector4i::Vector4i(Vector4f const& v) noexcept : x((int32_t)v.x), y((int32_t)v.y), z((int32_t)v.z), w((int32_t)w) {}
	constexpr Vector4i::Vector4i(Vector4d const& v) noexcept : x((int32_t)v.x), y((int32_t)v.y), z((int32_t)v.z), w((int32_t)w) {}
	constexpr Vector4f::Vector4f(Vector4i const& v) noexcept : x((float)v.x), y((float)v.y), z((float)v.z), w((float)w) {}
	constexpr Vector4f::Vector4f(Vector4d const& v) noexcept : x((float)v.x), y((float)v.y), z((float)v.z), w((float)w) {}
	constexpr Vector4d::Vector4d(Vector4i const& v) noexcept : x((double)v.x), y((double)v.y), z((double)v.z), w((double)w) {}
	constexpr Vector4d::Vector4d(Vector4f const& v) noexcept : x((double)v.x), y((double)v.y), z((double)v.z), w((double)w) {}
}