//
// Core.Matrix-Matrix.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Definitions of matrix.
//
module;

#include <Furud.hpp>



export module Furud.Core.Matrix:Matrix;

export import :Vector4;
export import Furud.Numbers;
import Furud.Platform.SIMD;



/** 4x4 32-bit single floating-point matrix. */
export namespace Furud
{
	/**
	 * @brief  4x4 32-bit single floating-point vector matrix, column-major matrix.
	 * @details  四乘四单精度浮点、列主序矩阵。
	 */
	struct Matrix44f
	{
		alignas(32) float m[4][4];


	public:
		static const Matrix44f IDENTITY;


	public:
		constexpr Matrix44f() noexcept
		{}
		
		constexpr Matrix44f(float const& v) noexcept
		{
			m[0][0] = v;  m[0][1] = 0;  m[0][2] = 0;  m[0][3] = 0;
			m[1][0] = 0;  m[1][1] = v;  m[1][2] = 0;  m[1][3] = 0;
			m[2][0] = 0;  m[2][1] = 0;  m[2][2] = v;  m[2][3] = 0;
			m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 1;
		}

		constexpr Matrix44f& operator = (float const& v) noexcept
		{
			m[0][0] = v;  m[0][1] = 0;  m[0][2] = 0;  m[0][3] = 0;
			m[1][0] = 0;  m[1][1] = v;  m[1][2] = 0;  m[1][3] = 0;
			m[2][0] = 0;  m[2][1] = 0;  m[2][2] = v;  m[2][3] = 0;
			m[3][0] = 0;  m[3][1] = 0;  m[3][2] = 0;  m[3][3] = 1;
			return *this;
		}

		constexpr Matrix44f(Vector4f const& row0, Vector4f const& row1, Vector4f const& row2, Vector4f const& row3) noexcept
		{
			m[0][0] = row0.x;  m[0][1] = row0.y;  m[0][2] = row0.z;  m[0][3] = row0.w;
			m[1][0] = row1.x;  m[1][1] = row1.y;  m[1][2] = row1.z;  m[1][3] = row1.w;
			m[2][0] = row2.x;  m[2][1] = row2.y;  m[2][2] = row2.z;  m[2][3] = row2.w;
			m[3][0] = row3.x;  m[3][1] = row3.y;  m[3][2] = row3.z;  m[3][3] = row3.w;
		}
		
		furud_inline Matrix44f(Matrix44f const& v) noexcept
		{
			Vec8f::Copy(&v.m[0][0], &m[0][0]);
			Vec8f::Copy(&v.m[2][0], &m[2][0]);
		}

		furud_inline Matrix44f& operator = (Matrix44f const& v) noexcept
		{
			Vec8f::Copy(&v.m[0][0], &m[0][0]);
			Vec8f::Copy(&v.m[2][0], &m[2][0]);
		}

		furud_inline Matrix44f(Matrix44f&& v) noexcept
		{
			m[0][0] = v.m[0][0];  m[0][1] = v.m[0][1];  m[0][2] = v.m[0][2];  m[0][3] = v.m[0][3];
			m[1][0] = v.m[1][0];  m[1][1] = v.m[1][1];  m[1][2] = v.m[1][2];  m[1][3] = v.m[1][3];
			m[2][0] = v.m[2][0];  m[2][1] = v.m[2][1];  m[2][2] = v.m[2][2];  m[2][3] = v.m[2][3];
			m[3][0] = v.m[3][0];  m[3][1] = v.m[3][1];  m[3][2] = v.m[3][2];  m[3][3] = v.m[3][3];
		}

		furud_inline constexpr Matrix44f& operator = (Matrix44f&& v) noexcept
		{
			m[0][0] = v.m[0][0];  m[0][1] = v.m[0][1];  m[0][2] = v.m[0][2];  m[0][3] = v.m[0][3];
			m[1][0] = v.m[1][0];  m[1][1] = v.m[1][1];  m[1][2] = v.m[1][2];  m[1][3] = v.m[1][3];
			m[2][0] = v.m[2][0];  m[2][1] = v.m[2][1];  m[2][2] = v.m[2][2];  m[2][3] = v.m[2][3];
			m[3][0] = v.m[3][0];  m[3][1] = v.m[3][1];  m[3][2] = v.m[3][2];  m[3][3] = v.m[3][3];
			return *this;
		}

		~Matrix44f() noexcept {}
		

	public:
		FURUD_NODISCARD furud_inline Mat44f ToMat4() const noexcept
		{
			Mat44f r;
			r.Load(this);
			return r;
		}

		FURUD_NODISCARD furud_inline Mat44f& AsMat4() noexcept
		{
			return *(Mat44f*)(this);
		}

		FURUD_NODISCARD furud_inline const Mat44f& AsMat4() const noexcept
		{
			return *(Mat44f*)(this);
		}


	public:
		furud_inline bool operator == (Matrix44f const& rhs) const noexcept
		{
			const float* furud_restrict p = &m[0][0];
			const float* furud_restrict q = &rhs.m[0][0];

			for (int32_t i = 0; i < 16; i++, p++, q++)
				if (*q != *p) return false;
			return true;
		}

		furud_inline bool operator != (Matrix44f const& rhs) const noexcept
		{
			return !(*this == rhs);
		}


	public:
		FURUD_NODISCARD furud_inline Matrix44f operator - () const noexcept
		{
			Matrix44f result;
			(-AsMat4()).Store(&result);
			return result;
		}

		FURUD_NODISCARD furud_inline Matrix44f operator + (Matrix44f const& rhs) const noexcept
		{
			Matrix44f result;
			result.AsMat4() = AsMat4() + rhs.AsMat4();
			return result;
		}

		FURUD_NODISCARD furud_inline Matrix44f operator - (Matrix44f const& rhs) const noexcept
		{
			Matrix44f result;
			result.AsMat4() = AsMat4() - rhs.AsMat4();
			return result;
		}

		furud_inline Matrix44f const& operator += (Matrix44f const& rhs) noexcept
		{
			AsMat4() = AsMat4() + rhs.AsMat4();
			return *this;
		}

		furud_inline Matrix44f const& operator -= (Matrix44f const& rhs) noexcept
		{
			AsMat4() = AsMat4() - rhs.AsMat4();
			return *this;
		}

		Matrix44f const& operator + (float const& rhs) = delete;

		Matrix44f const& operator - (float const& rhs) = delete;

		Matrix44f const& operator += (float const& rhs) = delete;

		Matrix44f const& operator -= (float const& rhs) = delete;

		FURUD_NODISCARD furud_inline Matrix44f operator * (float const& rhs) const noexcept
		{
			Matrix44f result;
			result.AsMat4() = AsMat4() * rhs;
			return result;
		}

		FURUD_NODISCARD furud_inline Matrix44f operator / (float const& rhs) const noexcept
		{
			Matrix44f result;
			result.AsMat4() = AsMat4() / rhs;
			return result;
		}

		furud_inline Matrix44f const& operator *= (float const& rhs) noexcept
		{
			AsMat4() = AsMat4() * rhs;
			return *this;
		}

		furud_inline Matrix44f const& operator /= (float const& rhs) noexcept
		{
			AsMat4() = AsMat4() / rhs;
			return *this;
		}


	public:
		FURUD_NODISCARD furud_inline Matrix44f Transpose() const noexcept
		{
			Matrix44f result;
			result.AsMat4() = AsMat4().Transpose();
			return result;
		}

		FURUD_NODISCARD furud_inline Matrix44f Inverse() const noexcept
		{
			Matrix44f result;
			result.AsMat4() = AsMat4().Inverse();
			return result;
		}


	public:
		FURUD_NODISCARD furud_inline Vector3f TransformPosition3(Vector3f const& rhs) const noexcept
		{
			const Mat44f& m = AsMat4();
			const Vec4f v { rhs.x, rhs.y, rhs.z, 1 };

			Vector3f result;
			(v * m).Homogenize().Store3(&result);
			return result;

		}

		FURUD_NODISCARD furud_inline Vector3f TransformVector3(Vector3f const& rhs) const noexcept
		{
			const Mat44f& m = AsMat4();
			const Vec4f v { rhs.x, rhs.y, rhs.z, 0 };
	
			Vector3f result;
			(v * m).Store3(&result);
			return result;
		}

		FURUD_NODISCARD furud_inline Vector3f TransformPosition3(Vector4f const& rhs) const noexcept
		{
			const Mat44f& m = AsMat4();
			const Vec4f& v = rhs.AsVec4();

			Vector3f result;
			(v * m).Homogenize().Store3(&result);
			return result;
		}

		FURUD_NODISCARD furud_inline Vector3f TransformVector3(Vector4f const& rhs) const noexcept
		{
			const Mat44f& m = AsMat4();
			const Vec4f& v = rhs.AsVec4();
			
			Vector3f result;
			(v * m).Store3(&result);
			return result;
		}

		FURUD_NODISCARD furud_inline Vector4f TransformPosition4(Vector3f const& rhs) const noexcept
		{
			const Mat44f& m = AsMat4();
			const Vec4f v { rhs.x, rhs.y, rhs.z, 1 };

			Vector4f result;
			(v * m).Homogenize().Store4(&result);
			return result;
		}
		FURUD_NODISCARD furud_inline Vector4f TransformVector4(Vector3f const& rhs) const noexcept
		{
			const Mat44f& m = AsMat4();
			const Vec4f v { rhs.x, rhs.y, rhs.z, 0 };

			Vector4f result;
			(v * m).Store4(&result);
			return result;
		}

		FURUD_NODISCARD furud_inline Vector4f TransformPosition4(Vector4f const& rhs) const noexcept
		{
			const Mat44f& m = AsMat4();
			const Vec4f& v = rhs.AsVec4();

			Vector4f result;
			(v * m).Homogenize().Store4(&result);
			return result;
		}

		FURUD_NODISCARD furud_inline Vector4f TransformVector4(Vector4f const& rhs) const noexcept
		{
			const Mat44f& m = AsMat4();
			const Vec4f& v = rhs.AsVec4();

			Vector4f result;
			(v * m).Store4(&result);
			return result;
		}
	};


	const Matrix44f Matrix44f::IDENTITY = 1.f;
}