//
// Platform.SIMD-Vec8.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Vectorized 8 number structure.
//
module;

#include <Furud.hpp>
#include <immintrin.h>
#include <bit>


export module Furud.Platform.SIMD:Vec8;

import Furud.Numbers;



namespace Furud::Internal
{
	struct IVec8fBuilder
	{
		static consteval __m256 Construct(uint32_t x)
		{
			__m256 result;
			result.m256_f32[0] = std::bit_cast<float>(x);
			result.m256_f32[1] = std::bit_cast<float>(x);
			result.m256_f32[2] = std::bit_cast<float>(x);
			result.m256_f32[3] = std::bit_cast<float>(x);
			result.m256_f32[4] = std::bit_cast<float>(x);
			result.m256_f32[5] = std::bit_cast<float>(x);
			result.m256_f32[6] = std::bit_cast<float>(x);
			result.m256_f32[7] = std::bit_cast<float>(x);
			return result;
		}

		static consteval __m256 Construct(float x)
		{
			__m256 result;
			result.m256_f32[0] = x;
			result.m256_f32[1] = x;
			result.m256_f32[2] = x;
			result.m256_f32[3] = x;
			result.m256_f32[4] = x;
			result.m256_f32[5] = x;
			result.m256_f32[6] = x;
			result.m256_f32[7] = x;
			return result;
		}
	};
}



/** Forward declaration. */
export namespace Furud
{
	class Vec8f;
	FURUD_INLINE extern Vec8f FURUD_VECTORAPI Equal(const Vec8f& lhs, const Vec8f& rhs) noexcept;
	FURUD_INLINE extern Vec8f FURUD_VECTORAPI NotEqual(const Vec8f& lhs, const Vec8f& rhs) noexcept;
	FURUD_INLINE extern Vec8f FURUD_VECTORAPI GreaterThan(const Vec8f& lhs, const Vec8f& rhs) noexcept;
	FURUD_INLINE extern Vec8f FURUD_VECTORAPI LessThan(const Vec8f& lhs, const Vec8f& rhs) noexcept;
	FURUD_INLINE extern Vec8f FURUD_VECTORAPI GreaterThanOrEqual(const Vec8f& lhs, const Vec8f& rhs) noexcept;
	FURUD_INLINE extern Vec8f FURUD_VECTORAPI LessThanOrEqual(const Vec8f& lhs, const Vec8f& rhs) noexcept;
	FURUD_INLINE extern Vec8f FURUD_VECTORAPI operator + (const __m256& lhs, const Vec8f& rhs) noexcept;
	FURUD_INLINE extern Vec8f FURUD_VECTORAPI operator - (const __m256& lhs, const Vec8f& rhs) noexcept;
	FURUD_INLINE extern Vec8f FURUD_VECTORAPI operator * (const __m256& lhs, const Vec8f& rhs) noexcept;
	FURUD_INLINE extern Vec8f FURUD_VECTORAPI operator / (const __m256& lhs, const Vec8f& rhs) noexcept;
	FURUD_INLINE extern Vec8f FURUD_VECTORAPI Min(const Vec8f& lhs, const Vec8f& rhs) noexcept;
	FURUD_INLINE extern Vec8f FURUD_VECTORAPI Max(const Vec8f& lhs, const Vec8f& rhs) noexcept;
	FURUD_INLINE extern Vec8f FURUD_VECTORAPI And(const Vec8f& lhs, const Vec8f& rhs) noexcept;
	FURUD_INLINE extern Vec8f FURUD_VECTORAPI AndNot(const Vec8f& lhs, const Vec8f& rhs) noexcept;
	FURUD_INLINE extern Vec8f FURUD_VECTORAPI Or(const Vec8f& lhs, const Vec8f& rhs) noexcept;
	FURUD_INLINE extern Vec8f FURUD_VECTORAPI Xor(const Vec8f& lhs, const Vec8f& rhs) noexcept;
	FURUD_INLINE extern Vec8f FURUD_VECTORAPI Copysign(const Vec8f& value, const Vec8f& sign) noexcept;
}



/** 8D 32-bit single floating-point vector. */
export namespace Furud
{
	/**
	 * @brief    8D 32-bit single floating-point vector.
	 * @details  SIMD 八维单精度浮点向量。
	 */
	class Vec8f final
	{
	private:
		using IBuilder = Internal::IVec8fBuilder;
		__m256 reg;


	public:

		static constexpr __m256 SIGN_BIT   { IBuilder::Construct(0x80000000ui32) };
		static constexpr __m256 SIGN_MASK  { IBuilder::Construct(0x7fffffffui32) };

	public:
		const Vec8f() noexcept {}
		constexpr Vec8f(__m256&& value) noexcept : reg(std::move(value)) {}
		constexpr Vec8f(const __m256& value) noexcept : reg(value) {}


	public:
		FURUD_INLINE Vec8f(const float& value) noexcept
			: reg(_mm256_set1_ps(value))
		{}

		FURUD_INLINE Vec8f(
			const float& x0, const float& y0,
			const float& z0, const float& w0,
			const float& x1, const float& y1,
			const float& z1, const float& w1
		) noexcept
			: reg(_mm256_setr_ps(x0, y0, z0, w0, x1, y1, z1, w1))
		{}

		FURUD_INLINE operator __m256() noexcept { return reg; }

		FURUD_INLINE operator const __m256() const noexcept { return reg; }

		Vec8f* operator & () = delete;
		
		const Vec8f* operator & () const = delete;


	public:
		//****************************************************************
		// @brief    Loads/Stores a Vec4d from/to aligned memory.
		// @details  加载/存储向量。
		//****************************************************************

		FURUD_INLINE Vec8f& Load(const void* data) noexcept
		{
			reg = _mm256_load_ps(static_cast<const float*>(data));
			return *this;
		}

		FURUD_INLINE void Store(void* data) const noexcept
		{
			_mm256_store_ps(static_cast<float*>(data), reg);
		}

		FURUD_INLINE static void Copy(const void* src, void* dst) noexcept
		{
			_mm256_store_ps(static_cast<float*>(dst), _mm256_load_ps(static_cast<const float*>(src)));
		}


	public:
		//****************************************************************
		// @brief    Creates a four-part mask through elements comparison from each Vec8f.
		// @return   Vec8f( x opr rhs.x ? 0xFFFFFFFF : 0, same for yzw )
		// @details  比较运算。
		//****************************************************************

		FURUD_INLINE friend Vec8f FURUD_VECTORAPI Equal(const Vec8f& lhs, const Vec8f& rhs) noexcept
		{
			return _mm256_cmp_ps(lhs.reg, rhs.reg, _CMP_EQ_OQ);
		}

		FURUD_INLINE friend Vec8f FURUD_VECTORAPI NotEqual(const Vec8f& lhs, const Vec8f& rhs) noexcept
		{
			return _mm256_cmp_ps(lhs.reg, rhs.reg, _CMP_NEQ_OQ);
		}

		FURUD_INLINE friend Vec8f FURUD_VECTORAPI GreaterThan(const Vec8f& lhs, const Vec8f& rhs) noexcept
		{
			return _mm256_cmp_ps(lhs.reg, rhs.reg, _CMP_GT_OQ);
		}

		FURUD_INLINE friend Vec8f FURUD_VECTORAPI LessThan(const Vec8f& lhs, const Vec8f& rhs) noexcept
		{
			return _mm256_cmp_ps(lhs.reg, rhs.reg, _CMP_LT_OQ);
		}

		FURUD_INLINE friend Vec8f FURUD_VECTORAPI GreaterThanOrEqual(const Vec8f& lhs, const Vec8f& rhs) noexcept
		{
			return _mm256_cmp_ps(lhs.reg, rhs.reg, _CMP_GE_OQ);
		}

		FURUD_INLINE friend Vec8f FURUD_VECTORAPI LessThanOrEqual(const Vec8f& lhs, const Vec8f& rhs) noexcept
		{
			return _mm256_cmp_ps(lhs.reg, rhs.reg, _CMP_LE_OQ);
		}


	public:
		//****************************************************************
		// @brief    Basic numerical operations.
		// @return   Vec8f( x opr rhs.x, same for yzw )
		// @details  加减乘除。
		//****************************************************************

		FURUD_INLINE Vec8f FURUD_VECTORAPI operator - () const noexcept { return _mm256_sub_ps(_mm256_set1_ps(0), reg); }

		FURUD_INLINE Vec8f FURUD_VECTORAPI operator + (const Vec8f& rhs) const noexcept { return _mm256_add_ps(reg, rhs.reg); }
		FURUD_INLINE Vec8f FURUD_VECTORAPI operator - (const Vec8f& rhs) const noexcept { return _mm256_sub_ps(reg, rhs.reg); }
		FURUD_INLINE Vec8f FURUD_VECTORAPI operator * (const Vec8f& rhs) const noexcept { return _mm256_mul_ps(reg, rhs.reg); }
		FURUD_INLINE Vec8f FURUD_VECTORAPI operator / (const Vec8f& rhs) const noexcept { return _mm256_div_ps(reg, rhs.reg); }

		FURUD_INLINE Vec8f FURUD_VECTORAPI operator + (const __m256& rhs) const noexcept { return _mm256_add_ps(reg, rhs); }
		FURUD_INLINE Vec8f FURUD_VECTORAPI operator - (const __m256& rhs) const noexcept { return _mm256_sub_ps(reg, rhs); }
		FURUD_INLINE Vec8f FURUD_VECTORAPI operator * (const __m256& rhs) const noexcept { return _mm256_mul_ps(reg, rhs); }
		FURUD_INLINE Vec8f FURUD_VECTORAPI operator / (const __m256& rhs) const noexcept { return _mm256_div_ps(reg, rhs); }

		FURUD_INLINE friend Vec8f FURUD_VECTORAPI operator + (const __m256& lhs, const Vec8f& rhs) noexcept { return _mm256_add_ps(lhs, rhs.reg); }
		FURUD_INLINE friend Vec8f FURUD_VECTORAPI operator - (const __m256& lhs, const Vec8f& rhs) noexcept { return _mm256_sub_ps(lhs, rhs.reg); }
		FURUD_INLINE friend Vec8f FURUD_VECTORAPI operator * (const __m256& lhs, const Vec8f& rhs) noexcept { return _mm256_mul_ps(lhs, rhs.reg); }
		FURUD_INLINE friend Vec8f FURUD_VECTORAPI operator / (const __m256& lhs, const Vec8f& rhs) noexcept { return _mm256_div_ps(lhs, rhs.reg); }

		FURUD_INLINE const Vec8f& operator += (const Vec8f& rhs) noexcept { reg = _mm256_add_ps(reg, rhs.reg); return *this; }
		FURUD_INLINE const Vec8f& operator -= (const Vec8f& rhs) noexcept { reg = _mm256_sub_ps(reg, rhs.reg); return *this; }
		FURUD_INLINE const Vec8f& operator *= (const Vec8f& rhs) noexcept { reg = _mm256_mul_ps(reg, rhs.reg); return *this; }
		FURUD_INLINE const Vec8f& operator /= (const Vec8f& rhs) noexcept { reg = _mm256_div_ps(reg, rhs.reg); return *this; }

		FURUD_INLINE const Vec8f& operator += (const __m256& rhs) noexcept { reg = _mm256_add_ps(reg, rhs); return *this; }
		FURUD_INLINE const Vec8f& operator -= (const __m256& rhs) noexcept { reg = _mm256_sub_ps(reg, rhs); return *this; }
		FURUD_INLINE const Vec8f& operator *= (const __m256& rhs) noexcept { reg = _mm256_mul_ps(reg, rhs); return *this; }
		FURUD_INLINE const Vec8f& operator /= (const __m256& rhs) noexcept { reg = _mm256_div_ps(reg, rhs); return *this; }


	public:
		//****************************************************************
		// @brief    Basic numerical operations.
		// @return   Vec8f( x opr rhs.x, same for yzw )
		// @details  加减乘除。
		//****************************************************************

		FURUD_INLINE Vec8f FURUD_VECTORAPI operator + (const float& rhs) const noexcept { return _mm256_add_ps(reg, _mm256_set1_ps(rhs)); }
		FURUD_INLINE Vec8f FURUD_VECTORAPI operator - (const float& rhs) const noexcept { return _mm256_sub_ps(reg, _mm256_set1_ps(rhs)); }
		FURUD_INLINE Vec8f FURUD_VECTORAPI operator * (const float& rhs) const noexcept { return _mm256_mul_ps(reg, _mm256_set1_ps(rhs)); }
		FURUD_INLINE Vec8f FURUD_VECTORAPI operator / (const float& rhs) const noexcept { return _mm256_div_ps(reg, _mm256_set1_ps(rhs)); }

		FURUD_INLINE const Vec8f& operator += (const float& rhs) noexcept { reg = _mm256_add_ps(reg, _mm256_set1_ps(rhs)); return *this; }
		FURUD_INLINE const Vec8f& operator -= (const float& rhs) noexcept { reg = _mm256_sub_ps(reg, _mm256_set1_ps(rhs)); return *this; }
		FURUD_INLINE const Vec8f& operator *= (const float& rhs) noexcept { reg = _mm256_mul_ps(reg, _mm256_set1_ps(rhs)); return *this; }
		FURUD_INLINE const Vec8f& operator /= (const float& rhs) noexcept { reg = _mm256_div_ps(reg, _mm256_set1_ps(rhs)); return *this; }


	public:
		//****************************************************************
		// @brief    Returns the minimum/maximum values of two Vec8f.
		// @return   Vec8f( min( lhs.x, rhs.x )/max( lhs.x, rhs.x ), same for yzw )
		// @details  最大/最小值。
		//****************************************************************

		FURUD_INLINE friend Vec8f FURUD_VECTORAPI Min(const Vec8f& lhs, const Vec8f& rhs) noexcept { return _mm256_min_ps(lhs.reg, rhs.reg); }
		FURUD_INLINE friend Vec8f FURUD_VECTORAPI Max(const Vec8f& lhs, const Vec8f& rhs) noexcept { return _mm256_max_ps(lhs.reg, rhs.reg); }


	public:
		/**
		 * @brief    Computes the square roots.
		 * @return   Vec8f( sqrt(x), same for yzw )
		 * @details  平方根。
		 */
		FURUD_INLINE Vec8f FURUD_VECTORAPI Sqrt() const noexcept { return _mm256_sqrt_ps(reg); }

		/**
		 * @brief    Computes the reciprocal square roots.
		 * @return   Vec8f( sqrt(x), same for yzw )
		 * @details  平方根倒数。
		 */
		FURUD_INLINE Vec8f FURUD_VECTORAPI RecipSqrt() const noexcept { return _mm256_rsqrt_ps(reg); }

		/**
		 * @brief    Computes the squares.
		 * @return   Vec8f( reg1.x * reg1.x, same for yzw )
		 * @details  平方。
		 */
		FURUD_INLINE Vec8f FURUD_VECTORAPI Squares() const noexcept { return _mm256_mul_ps(reg, reg); }

		/*
		 * @brief    Computes the approximations of reciprocals.
		 * @return   Vec8f( 1 / reg1.x, same for yzw )
		 * @details  倒数。
		 */
		FURUD_INLINE Vec8f FURUD_VECTORAPI Reciprocal() const noexcept { return _mm256_rcp_ps(reg); }

		/**
		 * @brief    Returns the absolute value.
		 * @return   Vec8f( abs(reg.x), same for yzw )
		 * @details  绝对值。
		 */
		FURUD_INLINE Vec8f FURUD_VECTORAPI Abs() const noexcept
		{

			return _mm256_and_ps(reg, SIGN_MASK);
		}


	public:
		//****************************************************************
		// @brief    Combines two Vec8f using bitwise logic operation.
		// @return   Vec8f( this.x opr reg2.x, same for yzw )
		// @details  逻辑运算。
		//****************************************************************

		FURUD_INLINE friend Vec8f FURUD_VECTORAPI And(const Vec8f& lhs, const Vec8f& rhs) noexcept { return _mm256_and_ps(lhs.reg, rhs.reg); }
		FURUD_INLINE friend Vec8f FURUD_VECTORAPI AndNot(const Vec8f& lhs, const Vec8f& rhs) noexcept { return _mm256_andnot_ps(lhs.reg, rhs.reg); }
		FURUD_INLINE friend Vec8f FURUD_VECTORAPI Or(const Vec8f& lhs, const Vec8f& rhs) noexcept { return _mm256_or_ps(lhs.reg, rhs.reg); }
		FURUD_INLINE friend Vec8f FURUD_VECTORAPI Xor(const Vec8f& lhs, const Vec8f& rhs) noexcept { return _mm256_xor_ps(lhs.reg, rhs.reg); }


	public:
		/**
		 * @brief    Returns an integer bit-mask (0x00 - 0x0f) based on the sign-bit for each elements in a Vec8f.
		 * @return   Bit 0 = sign(mask.x), Bit 1 = sign(mask.y), Bit 2 = sign(mask.z), Bit 3 = sign(mask.w) ...
		 * @details  掩码。
		 */
		FURUD_INLINE int32_t MaskBits() const noexcept { return _mm256_movemask_ps(reg); }

		/**
		 * @brief   Copy sign.
		 * @return  Vec8f( sign.x > 0 ? abs(value.x) : -abs(value.x), same for yzw )
		 * @details  复制符号位。
		 */
		FURUD_INLINE friend Vec8f FURUD_VECTORAPI Copysign(const Vec8f& value, const Vec8f& sign) noexcept
		{
			return _mm256_or_ps(_mm256_and_ps(SIGN_BIT, sign.reg), _mm256_andnot_ps(SIGN_BIT, value.reg));
		}
	};
}