//
// Platform.SIMD-Vec4.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Vectorized 4 number structure.
//
module;

#include <Furud.hpp>
#include <stdint.h>
#include <immintrin.h>
#include <utility>



export module Furud.Platform.SIMD:Vec4;

import Furud.Numbers;



namespace Furud::Internal
{
	struct IVec4iBuilder
	{
		static consteval __m128i Construct(uint32_t x)
		{
			__m128i result;
			result.m128i_u32[0] = x;
			result.m128i_u32[1] = x;
			result.m128i_u32[2] = x;
			result.m128i_u32[3] = x;
			return result;
		}

		static consteval __m128i Construct(uint32_t x, uint32_t y, uint32_t z, uint32_t w)
		{
			__m128i result;
			result.m128i_u32[0] = x;
			result.m128i_u32[1] = y;
			result.m128i_u32[2] = z;
			result.m128i_u32[3] = w;
			return result;
		}
	};


	struct IVec4fBuilder
	{
		static consteval __m128 Construct(float x, float y, float z, float w)
		{
			__m128 result;
			result.m128_f32[0] = x;
			result.m128_f32[1] = y;
			result.m128_f32[2] = z;
			result.m128_f32[3] = w;
			return result;
		}

		static consteval __m128 Construct(uint32_t x, uint32_t y, uint32_t z, uint32_t w)
		{
			__m128 result;
			result.m128_u32[0] = x;
			result.m128_u32[1] = y;
			result.m128_u32[2] = z;
			result.m128_u32[3] = w;
			return result;
		}

		static consteval __m128 Construct(float x)
		{
			__m128 result;
			result.m128_f32[0] = x;
			result.m128_f32[1] = x;
			result.m128_f32[2] = x;
			result.m128_f32[3] = x;
			return result;
		}

		static consteval __m128 Construct(uint32_t x)
		{
			__m128 result;
			result.m128_u32[0] = x;
			result.m128_u32[1] = x;
			result.m128_u32[2] = x;
			result.m128_u32[3] = x;
			return result;
		}
	};
}



/** Forward declaration. */
export namespace Furud
{
	class Vec4i;
	furud_inline extern Vec4i furud_vectorapi Equal(const Vec4i& lhs, const Vec4i& rhs) noexcept;
	furud_inline extern Vec4i furud_vectorapi NotEqual(const Vec4i& lhs, const Vec4i& rhs) noexcept;
	furud_inline extern Vec4i furud_vectorapi GreaterThan(const Vec4i& lhs, const Vec4i& rhs) noexcept;
	furud_inline extern Vec4i furud_vectorapi LessThan(const Vec4i& lhs, const Vec4i& rhs) noexcept;
	furud_inline extern Vec4i furud_vectorapi GreaterThanOrEqual(const Vec4i& lhs, const Vec4i& rhs) noexcept;
	furud_inline extern Vec4i furud_vectorapi LessThanOrEqual(const Vec4i& lhs, const Vec4i& rhs) noexcept;
	furud_inline extern Vec4i furud_vectorapi operator + (const __m128i& lhs, const Vec4i& rhs) noexcept;
	furud_inline extern Vec4i furud_vectorapi operator - (const __m128i& lhs, const Vec4i& rhs) noexcept;
	furud_inline extern Vec4i furud_vectorapi operator * (const __m128i& lhs, const Vec4i& rhs) noexcept;
	furud_inline extern Vec4i furud_vectorapi operator / (const __m128i& lhs, const Vec4i& rhs) noexcept;
	furud_inline extern Vec4i furud_vectorapi operator + (const int32_t& lhs, const Vec4i& rhs) noexcept;
	furud_inline extern Vec4i furud_vectorapi operator - (const int32_t& lhs, const Vec4i& rhs) noexcept;
	furud_inline extern Vec4i furud_vectorapi operator * (const int32_t& lhs, const Vec4i& rhs) noexcept;
	furud_inline extern Vec4i furud_vectorapi operator / (const int32_t& lhs, const Vec4i& rhs) noexcept;
	furud_inline extern Vec4i furud_vectorapi Min(const Vec4i& lhs, const Vec4i& rhs) noexcept;
	furud_inline extern Vec4i furud_vectorapi Max(const Vec4i& lhs, const Vec4i& rhs) noexcept;
	furud_inline extern Vec4i furud_vectorapi Min(const Vec4i& lhs, const Vec4i& mhs, const Vec4i& rhs) noexcept;
	furud_inline extern Vec4i furud_vectorapi Max(const Vec4i& lhs, const Vec4i& mhs, const Vec4i& rhs) noexcept;
	furud_inline extern Vec4i furud_vectorapi And(const Vec4i& lhs, const Vec4i& rhs) noexcept;
	furud_inline extern Vec4i furud_vectorapi AndNot(const Vec4i& lhs, const Vec4i& rhs) noexcept;
	furud_inline extern Vec4i furud_vectorapi Or(const Vec4i& lhs, const Vec4i& rhs) noexcept;
	furud_inline extern Vec4i furud_vectorapi Xor(const Vec4i& lhs, const Vec4i& rhs) noexcept;
	furud_inline extern Vec4i furud_vectorapi Copysign(const Vec4i& value, const Vec4i& sign) noexcept;


	class Vec4f;
	furud_inline extern Vec4f furud_vectorapi Equal(const Vec4f& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi NotEqual(const Vec4f& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi GreaterThan(const Vec4f& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi LessThan(const Vec4f& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi GreaterThanOrEqual(const Vec4f& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi LessThanOrEqual(const Vec4f& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi operator + (const __m128& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi operator - (const __m128& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi operator * (const __m128& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi operator / (const __m128& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi MultiplyAdd(const Vec4f& lhs, const Vec4f& mhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi MultiplyAdd(const Vec4f& lhs1, const Vec4f& lhs2, const Vec4f& rhs1, const Vec4f& rhs2) noexcept;
	furud_inline extern Vec4f furud_vectorapi MultiplyAdd(const Vec4f& lhs1, const Vec4f& lhs2, const Vec4f& mhs1, const Vec4f& mhs2, const Vec4f& rhs1, const Vec4f& rhs2) noexcept;
	furud_inline extern Vec4f furud_vectorapi MultiplySub(const Vec4f& lhs, const Vec4f& mhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi NegateMultiplyAdd(const Vec4f& lhs, const Vec4f& mhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi NegateMultiplySub(const Vec4f& lhs, const Vec4f& mhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi operator + (const float& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi operator - (const float& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi operator * (const float& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi operator / (const float& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi Shuffle(const Vec4f& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi Shuffle(const Vec4f& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi Min(const Vec4f& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi Max(const Vec4f& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi Min(const Vec4f& lhs, const Vec4f& mhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi Max(const Vec4f& lhs, const Vec4f& mhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi Select(const Vec4f& mask, const Vec4f& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern float furud_vectorapi DotProduct(const Vec4f& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi Pow(const Vec4f& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi And(const Vec4f& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi AndNot(const Vec4f& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi Or(const Vec4f& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi Xor(const Vec4f& lhs, const Vec4f& rhs) noexcept;
	furud_inline extern Vec4f furud_vectorapi Copysign(const Vec4f& value, const Vec4f& sign) noexcept;


	/** @todo Need AVX512F + AVX512VL */
	/*
	class Vec4d;
	furud_inline extern Vec4d furud_vectorapi Equal(const Vec4d& lhs, const Vec4d& rhs) noexcept;
	furud_inline extern Vec4d furud_vectorapi NotEqual(const Vec4d& lhs, const Vec4d& rhs) noexcept;
	furud_inline extern Vec4d furud_vectorapi GreaterThan(const Vec4d& lhs, const Vec4d& rhs) noexcept;
	furud_inline extern Vec4d furud_vectorapi LessThan(const Vec4d& lhs, const Vec4d& rhs) noexcept;
	furud_inline extern Vec4d furud_vectorapi GreaterThanOrEqual(const Vec4d& lhs, const Vec4d& rhs) noexcept;
	furud_inline extern Vec4d furud_vectorapi LessThanOrEqual(const Vec4d& lhs, const Vec4d& rhs) noexcept;
	furud_inline extern Vec4d furud_vectorapi operator + (const __m256d& lhs, const Vec4d& rhs) noexcept;
	furud_inline extern Vec4d furud_vectorapi operator - (const __m256d& lhs, const Vec4d& rhs) noexcept;
	furud_inline extern Vec4d furud_vectorapi operator * (const __m256d& lhs, const Vec4d& rhs) noexcept;
	furud_inline extern Vec4d furud_vectorapi operator / (const __m256d& lhs, const Vec4d& rhs) noexcept;
	furud_inline extern Vec4d furud_vectorapi operator + (const double& lhs, const Vec4d& rhs) noexcept;
	furud_inline extern Vec4d furud_vectorapi operator - (const double& lhs, const Vec4d& rhs) noexcept;
	furud_inline extern Vec4d furud_vectorapi operator * (const double& lhs, const Vec4d& rhs) noexcept;
	furud_inline extern Vec4d furud_vectorapi operator / (const double& lhs, const Vec4d& rhs) noexcept;
	furud_inline extern Vec4d furud_vectorapi Min(const Vec4d& lhs, const Vec4d& rhs) noexcept;
	furud_inline extern Vec4d furud_vectorapi Max(const Vec4d& lhs, const Vec4d& rhs) noexcept;
	furud_inline extern Vec4d furud_vectorapi Min(const Vec4d& lhs, const Vec4d& mhs, const Vec4d& rhs) noexcept;
	furud_inline extern Vec4d furud_vectorapi Max(const Vec4d& lhs, const Vec4d& mhs, const Vec4d& rhs) noexcept;
	furud_inline extern double furud_vectorapi DotProduct(const Vec4d& lhs, const Vec4d& rhs) noexcept;
	furud_inline extern Vec4d furud_vectorapi And(const Vec4d& lhs, const Vec4d& rhs) noexcept;
	furud_inline extern Vec4d furud_vectorapi AndNot(const Vec4d& lhs, const Vec4d& rhs) noexcept;
	furud_inline extern Vec4d furud_vectorapi Or(const Vec4d& lhs, const Vec4d& rhs) noexcept;
	furud_inline extern Vec4d furud_vectorapi Xor(const Vec4d& lhs, const Vec4d& rhs) noexcept;
	furud_inline extern Vec4d furud_vectorapi Copysign(const Vec4d& value, const Vec4d& sign) noexcept;
	*/
}



/** 4D 32-bit integer vector. */
export namespace Furud
{
	/**
	 * @brief    4D 32-bit integer vector.
	 * @details  SIMD 四维整型向量。
	 */
	class Vec4i final
	{
	private:
		using IBuilder = Internal::IVec4iBuilder;
		__m128i r;


	public:
		static constexpr __m128i X_MASK    { IBuilder::Construct(0xffffffffui32, 0, 0, 0) };
		static constexpr __m128i Y_MASK    { IBuilder::Construct(0, 0xffffffffui32, 0, 0) };
		static constexpr __m128i Z_MASK    { IBuilder::Construct(0, 0, 0xffffffffui32, 0) };
		static constexpr __m128i W_MASK    { IBuilder::Construct(0, 0, 0, 0xffffffffui32) };
		static constexpr __m128i XY_MASK   { IBuilder::Construct(0xffffffffui32, 0xffffffffui32, 0, 0) };
		static constexpr __m128i XZ_MASK   { IBuilder::Construct(0xffffffffui32, 0, 0xffffffffui32, 0) };
		static constexpr __m128i YW_MASK   { IBuilder::Construct(0, 0xffffffffui32, 0, 0xffffffffui32) };
		static constexpr __m128i ZW_MASK   { IBuilder::Construct(0, 0, 0xffffffffui32, 0xffffffffui32) };
		static constexpr __m128i XYZ_MASK  { IBuilder::Construct(0xffffffffui32, 0xffffffffui32, 0xffffffffui32, 0) };
		static constexpr __m128i YZW_MASK  { IBuilder::Construct(0, 0xffffffffui32, 0xffffffffui32, 0xffffffffui32) };
		static constexpr __m128i XYZW_MASK { IBuilder::Construct(0xffffffffui32) };
		static constexpr __m128i SIGN_BIT  { IBuilder::Construct(0x80000000ui32) };
		static constexpr __m128i SIGN_MASK { IBuilder::Construct(0x7fffffffui32) };


	public:
		constexpr Vec4i() noexcept {}
		constexpr Vec4i(__m128i&& value) noexcept : r(std::move(value)) {}
		constexpr Vec4i(const __m128i& value) noexcept : r(value) {}


	public:
		furud_inline Vec4i(const int32_t& value) noexcept
			: r(_mm_set1_epi32(value))
		{}

		furud_inline Vec4i(const int32_t& x, const int32_t& y, const int32_t& z, const int32_t& w = 0.f) noexcept
			: r(_mm_setr_epi32(x, y, z, w))
		{}

		furud_inline operator __m128i() noexcept { return r; }

		furud_inline operator const __m128i() const noexcept { return r; }

		Vec4i* operator & () = delete;
		
		const Vec4i* operator & () const = delete;


	public:
		//****************************************************************
		// @brief    Loads/Stores a Vec4i from/to aligned memory.
		// @details  加载/存储向量。
		//****************************************************************

		furud_inline void Load3(const void* data) noexcept
		{
			const int32_t* furud_restrict p = static_cast<const int32_t*>(data);
			r = _mm_setr_epi32(p[0], p[1], p[2], 0);
		}
	
		furud_inline void Load4(const void* data) noexcept
		{
			r = _mm_load_si128(static_cast<const __m128i*>(data));
		}

		furud_inline void Store3(void* data) noexcept
		{
			alignas(16) int32_t xyzw[4];
			_mm_store_si128(reinterpret_cast<__m128i*>(xyzw), r);
			int32_t* furud_restrict p = static_cast<int32_t*>(data);
			p[0] = xyzw[0];
			p[1] = xyzw[1];
			p[2] = xyzw[2];
		}

		furud_inline void Store4(void* data) const noexcept
		{
			_mm_store_si128(static_cast<__m128i*>(data), r);
		}


	private:
		furud_inline int32_t Extract(int32_t&& index) const noexcept
		{
			return r.m128i_i32[index];
		}

		furud_inline void Insert(int32_t&& index, const int32_t& value) noexcept
		{
			r.m128i_i32[index] = value;
		}


	public:
		//****************************************************************
		// @brief    Visits element.
		// @details  访问向量元素。
		//****************************************************************

		furud_inline int32_t X() const noexcept { return Extract(0); }
		furud_inline int32_t Y() const noexcept { return Extract(1); }
		furud_inline int32_t Z() const noexcept { return Extract(2); }
		furud_inline int32_t W() const noexcept { return Extract(3); }
		furud_inline void SetX(const int32_t& value) noexcept { Insert(0, value); }
		furud_inline void SetY(const int32_t& value) noexcept { Insert(1, value); }
		furud_inline void SetZ(const int32_t& value) noexcept { Insert(2, value); }
		furud_inline void SetW(const int32_t& value) noexcept { Insert(3, value); }
		furud_inline void SetX0() noexcept { r = _mm_and_epi32(r, YZW_MASK); }
		furud_inline void SetW0() noexcept { r = _mm_and_epi32(r, XYZ_MASK); }


	public:
		//****************************************************************
		// @brief    Replicates one element into all four elements and returns a new Vec4i.
		// @return   Vec4i( r[index], same for yzw )
		// @detials  复制向量元素。
		//****************************************************************

		furud_inline Vec4i furud_vectorapi ReplicateX() const noexcept { return _mm_shuffle_epi32(r, 0);    }
		furud_inline Vec4i furud_vectorapi ReplicateY() const noexcept { return _mm_shuffle_epi32(r, 85u);  }
		furud_inline Vec4i furud_vectorapi ReplicateZ() const noexcept { return _mm_shuffle_epi32(r, 170u); }
		furud_inline Vec4i furud_vectorapi ReplicateW() const noexcept { return _mm_shuffle_epi32(r, 255u); }


	public:
		/**
		 * @brief    Converts Vec4i (signed 32-bit integer) to Vec4f (float).
		 * @return   ( (float)r.x, same for yzw )
		 * @details  整数转浮点数。
		 */
		furud_inline Vec4f furud_vectorapi ToFloat() const noexcept;


	public:
		//****************************************************************
		// @brief    Creates a four-part mask through elements comparison from each Vec4i.
		// @return   Vec4i( x opr rhs.x ? 0xFFFFFFFF : 0, same for yzw )
		// @details  比较运算。
		//****************************************************************

		furud_inline friend Vec4i furud_vectorapi Equal(const Vec4i& lhs, const Vec4i& rhs) noexcept
		{
			return _mm_cmpeq_epi32(lhs.r, rhs.r);
		}

		furud_inline friend Vec4i furud_vectorapi NotEqual(const Vec4i& lhs, const Vec4i& rhs) noexcept
		{
			return _mm_xor_si128(_mm_cmpeq_epi32(lhs.r, rhs.r), XYZW_MASK); // Not Equal
		}

		furud_inline friend Vec4i furud_vectorapi GreaterThan(const Vec4i& lhs, const Vec4i& rhs) noexcept
		{
			return _mm_cmpgt_epi32(lhs.r, rhs.r);
		}

		furud_inline friend Vec4i furud_vectorapi LessThan(const Vec4i& lhs, const Vec4i& rhs) noexcept
		{
			return _mm_cmplt_epi32(lhs.r, rhs.r);
		}

		furud_inline friend Vec4i furud_vectorapi GreaterThanOrEqual(const Vec4i& lhs, const Vec4i& rhs) noexcept
		{
			return _mm_xor_si128(_mm_cmplt_epi32(lhs.r, rhs.r), XYZW_MASK); // Not Less-Than
		}

		furud_inline friend Vec4i furud_vectorapi LessThanOrEqual(const Vec4i& lhs, const Vec4i& rhs) noexcept
		{
			return _mm_xor_si128(_mm_cmpgt_epi32(lhs.r, rhs.r), XYZW_MASK); // Not Greater-Than
		}


	public:
		//****************************************************************
		// @brief    Basic numerical operations.
		// @return   Vec4i( x opr rhs.x, same for yzw )
		// @details  加减乘除。
		//****************************************************************

		furud_inline Vec4i furud_vectorapi operator - () const noexcept { return _mm_sub_epi32(_mm_setzero_si128(), r); }

		furud_inline Vec4i furud_vectorapi operator + (const Vec4i& rhs) const noexcept { return _mm_add_epi32(r, rhs.r); }
		furud_inline Vec4i furud_vectorapi operator - (const Vec4i& rhs) const noexcept { return _mm_sub_epi32(r, rhs.r); }
		furud_inline Vec4i furud_vectorapi operator * (const Vec4i& rhs) const noexcept { return _mm_mullo_epi32(r, rhs.r); }
		furud_inline Vec4i furud_vectorapi operator / (const Vec4i& rhs) const noexcept { return _mm_div_epi32(r, rhs.r); }

		furud_inline Vec4i furud_vectorapi operator + (const __m128i& rhs) const noexcept { return _mm_add_epi32(r, rhs); }
		furud_inline Vec4i furud_vectorapi operator - (const __m128i& rhs) const noexcept { return _mm_sub_epi32(r, rhs); }
		furud_inline Vec4i furud_vectorapi operator * (const __m128i& rhs) const noexcept { return _mm_mullo_epi32(r, rhs); }
		furud_inline Vec4i furud_vectorapi operator / (const __m128i& rhs) const noexcept { return _mm_div_epi32(r, rhs); }

		furud_inline friend Vec4i furud_vectorapi operator + (const __m128i& lhs, const Vec4i& rhs) noexcept { return _mm_add_epi32(lhs, rhs.r); }
		furud_inline friend Vec4i furud_vectorapi operator - (const __m128i& lhs, const Vec4i& rhs) noexcept { return _mm_sub_epi32(lhs, rhs.r); }
		furud_inline friend Vec4i furud_vectorapi operator * (const __m128i& lhs, const Vec4i& rhs) noexcept { return _mm_mullo_epi32(lhs, rhs.r); }
		furud_inline friend Vec4i furud_vectorapi operator / (const __m128i& lhs, const Vec4i& rhs) noexcept { return _mm_div_epi32(lhs, rhs.r); }

		furud_inline const Vec4i& operator += (const Vec4i& rhs) noexcept { r = _mm_add_epi32(r, rhs.r); return *this; }
		furud_inline const Vec4i& operator -= (const Vec4i& rhs) noexcept { r = _mm_sub_epi32(r, rhs.r); return *this; }
		furud_inline const Vec4i& operator *= (const Vec4i& rhs) noexcept { r = _mm_mullo_epi32(r, rhs.r); return *this; }
		furud_inline const Vec4i& operator /= (const Vec4i& rhs) noexcept { r = _mm_div_epi32(r, rhs.r); return *this; }

		furud_inline const Vec4i& operator += (const __m128i& rhs) noexcept { r = _mm_add_epi32(r, rhs); return *this; }
		furud_inline const Vec4i& operator -= (const __m128i& rhs) noexcept { r = _mm_sub_epi32(r, rhs); return *this; }
		furud_inline const Vec4i& operator *= (const __m128i& rhs) noexcept { r = _mm_mullo_epi32(r, rhs); return *this; }
		furud_inline const Vec4i& operator /= (const __m128i& rhs) noexcept { r = _mm_div_epi32(r, rhs); return *this; }


	public:
		//****************************************************************
		// @brief    Basic numerical operations.
		// @return   Vec4i( x opr rhs, same for yzw )
		// @details  加减乘除。
		//****************************************************************

		furud_inline Vec4i furud_vectorapi operator + (const int32_t& rhs) const noexcept { return _mm_add_epi32(r, _mm_set1_epi32(rhs)); }
		furud_inline Vec4i furud_vectorapi operator - (const int32_t& rhs) const noexcept { return _mm_sub_epi32(r, _mm_set1_epi32(rhs)); }
		furud_inline Vec4i furud_vectorapi operator * (const int32_t& rhs) const noexcept { return _mm_mullo_epi32(r, _mm_set1_epi32(rhs)); }
		furud_inline Vec4i furud_vectorapi operator / (const int32_t& rhs) const noexcept { return _mm_div_epi32(r, _mm_set1_epi32(rhs)); }

		furud_inline friend Vec4i furud_vectorapi operator + (const int32_t& lhs, const Vec4i& rhs) noexcept { return _mm_add_epi32(_mm_set1_epi32(lhs), rhs.r); }
		furud_inline friend Vec4i furud_vectorapi operator - (const int32_t& lhs, const Vec4i& rhs) noexcept { return _mm_sub_epi32(_mm_set1_epi32(lhs), rhs.r); }
		furud_inline friend Vec4i furud_vectorapi operator * (const int32_t& lhs, const Vec4i& rhs) noexcept { return _mm_mullo_epi32(_mm_set1_epi32(lhs), rhs.r); }
		furud_inline friend Vec4i furud_vectorapi operator / (const int32_t& lhs, const Vec4i& rhs) noexcept { return _mm_div_epi32(_mm_set1_epi32(lhs), rhs.r); }

		furud_inline const Vec4i& operator += (const int32_t& rhs) noexcept { r = _mm_add_epi32(r, _mm_set1_epi32(rhs));   return *this; }
		furud_inline const Vec4i& operator -= (const int32_t& rhs) noexcept { r = _mm_sub_epi32(r, _mm_set1_epi32(rhs));   return *this; }
		furud_inline const Vec4i& operator *= (const int32_t& rhs) noexcept { r = _mm_mullo_epi32(r, _mm_set1_epi32(rhs)); return *this; }
		furud_inline const Vec4i& operator /= (const int32_t& rhs) noexcept { r = _mm_div_epi32(r, _mm_set1_epi32(rhs));   return *this; }


	public:
		/**
		 * @brief    Swizzles the Vec4i.
		 * @return   Vec4i( r[i], r[j], r[k], r[l] )
		 * @details  交换向量元素位置。
		 */
		template<int32_t i, int32_t j, int32_t k, int32_t l>
		furud_inline Vec4i furud_vectorapi Swizzle() const noexcept
		{
			constexpr int32_t v = i | (j << 2) | (k << 4) | (l << 6);
			return _mm_shuffle_epi32(r, v);
		}


	public:
		//****************************************************************
		// @brief    Returns the minimum/maximum values of two Vec4i.
		// @return   Vec4i( min( lhs.x, rhs.x )/max( lhs.x, rhs.x ), same for yzw )
		// @details  最大/最小值。
		//****************************************************************

		furud_inline friend Vec4i furud_vectorapi Min(const Vec4i& lhs, const Vec4i& rhs) noexcept { return _mm_min_epi32(lhs.r, rhs.r); }
		furud_inline friend Vec4i furud_vectorapi Max(const Vec4i& lhs, const Vec4i& rhs) noexcept { return _mm_max_epi32(lhs.r, rhs.r); }

		furud_inline friend Vec4i furud_vectorapi Min(const Vec4i& lhs, const Vec4i& mhs, const Vec4i& rhs) noexcept { return _mm_min_epi32(lhs.r, _mm_min_epi32(mhs.r, rhs.r)); }
		furud_inline friend Vec4i furud_vectorapi Max(const Vec4i& lhs, const Vec4i& mhs, const Vec4i& rhs) noexcept { return _mm_max_epi32(lhs.r, _mm_max_epi32(mhs.r, rhs.r)); }


	public:
		/**
		 * @brief    Returns the absolute Vec4i.
		 * @return   Vec4i( abs(r.x), same for yzw )
		 * @details  绝对值。
		 */
		furud_inline Vec4i furud_vectorapi Abs() const noexcept { return _mm_and_epi32(r, SIGN_MASK); }


	public:
		//****************************************************************
		// @brief    Combines two Vec4i using bitwise logic operation.
		// @return   Vec4i( r.x opr reg2.x, same for yzw )
		// @details  按位逻辑运算。
		//****************************************************************

		furud_inline friend Vec4i furud_vectorapi And(const Vec4i& lhs, const Vec4i& rhs) noexcept { return _mm_and_epi32(lhs.r, rhs.r); }
		furud_inline friend Vec4i furud_vectorapi AndNot(const Vec4i& lhs, const Vec4i& rhs) noexcept { return _mm_andnot_epi32(lhs.r, rhs.r); }
		furud_inline friend Vec4i furud_vectorapi Or(const Vec4i& lhs, const Vec4i& rhs) noexcept { return _mm_or_epi32(lhs.r, rhs.r); }
		furud_inline friend Vec4i furud_vectorapi Xor(const Vec4i& lhs, const Vec4i& rhs) noexcept { return _mm_xor_epi32(lhs.r, rhs.r); }


	public:
		/**
		 * @brief    Copy sign.
		 * @return   Vec4i( sign.x > 0 ? abs(value.x) : -abs(value.x), same for yzw )
		 * @details  复制符号位。
		 */
		furud_inline friend Vec4i furud_vectorapi Copysign(const Vec4i& value, const Vec4i& sign) noexcept
		{
			return _mm_or_epi32(_mm_and_epi32(SIGN_BIT, sign.r), _mm_andnot_epi32(SIGN_BIT, value.r));
		}
	};
}



/** 4D 32-bit single floating-point vector. */
export namespace Furud
{
	/**
	 * @brief    4D 32-bit single floating-point vector.
	 * @details  SIMD 四维单精度浮点向量。
	 */
	class Vec4f final
	{
	private:
		using IBuilder = Internal::IVec4fBuilder;
		friend class Mat44f;
		//friend class Vec4d;
		__m128 r;


	public:
		static constexpr __m128 ZERO            { IBuilder::Construct(0.f) };
		static constexpr __m128 ONE             { IBuilder::Construct(1.f) };
		static constexpr __m128 NEG_ONE         { IBuilder::Construct(-1.f) };
		static constexpr __m128 TWO             { IBuilder::Construct(2.f) };
		static constexpr __m128 NEG_TWO         { IBuilder::Construct(-2.f) };
		static constexpr __m128 HALF            { IBuilder::Construct(0.5f) };
		static constexpr __m128 PI              { IBuilder::Construct(IFloat::PI) };
		static constexpr __m128 PI2             { IBuilder::Construct(2.0f * IFloat::PI) };
		static constexpr __m128 HALF_PI         { IBuilder::Construct(0.5f * IFloat::PI) };
		static constexpr __m128 D180            { IBuilder::Construct(180.f) };
		static constexpr __m128 D360            { IBuilder::Construct(360.f) };
		static constexpr __m128 D255            { IBuilder::Construct(255.f) };
		static constexpr __m128 DEG_TO_RAD      { IBuilder::Construct(IFloat::DEG_TO_RAD) };
		static constexpr __m128 RAD_TO_DEG      { IBuilder::Construct(IFloat::RAD_TO_DEG) };
		static constexpr __m128 SIGN_BIT        { IBuilder::Construct(0x80000000ui32) };
		static constexpr __m128 SIGN_MASK       { IBuilder::Construct(0x7fffffffui32) };
		static constexpr __m128 NON_FRACTIONAL  { IBuilder::Construct(8388608.f) };
		static constexpr __m128 X_MASK          { IBuilder::Construct(0xffffffffui32, 0, 0, 0) };
		static constexpr __m128 Y_MASK          { IBuilder::Construct(0, 0xffffffffui32, 0, 0) };
		static constexpr __m128 Z_MASK          { IBuilder::Construct(0, 0, 0xffffffffui32, 0) };
		static constexpr __m128 W_MASK          { IBuilder::Construct(0, 0, 0, 0xffffffffui32) };
		static constexpr __m128 XY_MASK         { IBuilder::Construct(0xffffffffui32, 0xffffffffui32, 0, 0) };
		static constexpr __m128 XZ_MASK         { IBuilder::Construct(0xffffffffui32, 0, 0xffffffffui32, 0) };
		static constexpr __m128 YW_MASK         { IBuilder::Construct(0, 0xffffffffui32, 0, 0xffffffffui32) };
		static constexpr __m128 ZW_MASK         { IBuilder::Construct(0, 0, 0xffffffffui32, 0xffffffffui32) };
		static constexpr __m128 XYZ_MASK        { IBuilder::Construct(0xffffffffui32, 0xffffffffui32, 0xffffffffui32, 0) };
		static constexpr __m128 YZW_MASK        { IBuilder::Construct(0, 0xffffffffui32, 0xffffffffui32, 0xffffffffui32) };
		static constexpr __m128 XYZW_MASK       { IBuilder::Construct(0xffffffffui32) };


	public:
		constexpr Vec4f() noexcept {}
		constexpr Vec4f(__m128&& value) noexcept : r(std::move(value)) {}
		constexpr Vec4f(const __m128& value) noexcept : r(value) {}
		constexpr Vec4f(float&& x) noexcept : r({ x, x, x, x }) {}
		constexpr Vec4f(float&& x, float&& y, float&& z, float&& w) noexcept : r({ x, y, z, w }) {}


	public:
		furud_inline Vec4f(const float& value) noexcept
			: r(_mm_set_ps1(value))
		{}

		furud_inline Vec4f(const float& x, const float& y, const float& z, const float& w = 0.f) noexcept
			: r(_mm_setr_ps(x, y, z, w))
		{}

		furud_inline operator __m128() noexcept { return r; }

		furud_inline operator const __m128() const noexcept { return r; }

		Vec4f* operator & () = delete;
		
		const Vec4f* operator & () const = delete;


	public:
		//****************************************************************
		// @brief    Loads/Stores a Vec4f from/to aligned memory.
		// @details  加载/存储向量。
		//****************************************************************

		furud_inline Vec4f& Load3(const void* data) noexcept
		{
			const float* furud_restrict p = static_cast<const float*>(data);
			r = _mm_setr_ps(p[0], p[1], p[2], 0.f);
			return *this;
		}

		furud_inline Vec4f& Load4(const void* data) noexcept
		{
			r = _mm_load_ps(static_cast<const float*>(data));
			return *this;
		}

		furud_inline void Store3(void* data) noexcept
		{
			float* furud_restrict p = static_cast<float*>(data);
			_MM_EXTRACT_FLOAT(p[0], r, 0);
			_MM_EXTRACT_FLOAT(p[1], r, 1);
			_MM_EXTRACT_FLOAT(p[2], r, 2);
		}

		furud_inline void Store4(void* data) const noexcept
		{
			_mm_store_ps(static_cast<float*>(data), r);
		}


	private:
		furud_inline void Insert(int32_t&& index, const float& value) noexcept
		{
			r.m128_f32[index] = value;
		}


	public:
		//****************************************************************
		// @brief    Visits element.
		// @details  访问向量元素。
		//****************************************************************

		furud_inline float X() const noexcept { float x; _MM_EXTRACT_FLOAT(x, r, 0); return x; }
		furud_inline float Y() const noexcept { float y; _MM_EXTRACT_FLOAT(y, r, 1); return y; }
		furud_inline float Z() const noexcept { float z; _MM_EXTRACT_FLOAT(z, r, 2); return z; }
		furud_inline float W() const noexcept { float w; _MM_EXTRACT_FLOAT(w, r, 3); return w; }

		furud_inline void SetX(const float& value) noexcept { Insert(0, value); }
		furud_inline void SetY(const float& value) noexcept { Insert(1, value); }
		furud_inline void SetZ(const float& value) noexcept { Insert(2, value); }
		furud_inline void SetW(const float& value) noexcept { Insert(3, value); }

		furud_inline void SetX0() noexcept { r = _mm_and_ps(r, YZW_MASK); }
		furud_inline void SetW0() noexcept { r = _mm_and_ps(r, XYZ_MASK); }


	public:
		//****************************************************************
		// @brief    Replicates one element into all four elements and returns a new Vec4f.
		// @return   Vec4f( r[index], same for yzw )
		// @details  复制向量元素。
		//****************************************************************

		furud_inline Vec4f furud_vectorapi ReplicateX() const noexcept { return _mm_shuffle_ps(r, r, 0);    }
		furud_inline Vec4f furud_vectorapi ReplicateY() const noexcept { return _mm_shuffle_ps(r, r, 85u);  }
		furud_inline Vec4f furud_vectorapi ReplicateZ() const noexcept { return _mm_shuffle_ps(r, r, 170u); }
		furud_inline Vec4f furud_vectorapi ReplicateW() const noexcept { return _mm_shuffle_ps(r, r, 255u); }


	public:
		/**
		 * @brief    Converts Vec4f (float) to Vec4i (signed 32-bit integer).
		 * @return   Vec4i( (int32_t)r.x, same for yzw )
		 * @details  浮点数转整数。
		 */
		furud_inline Vec4i furud_vectorapi ToInteger() const noexcept;


	public:
		//****************************************************************
		// @brief    Creates a four-part mask through elements comparison from each Vec4f.
		// @return   Vec4f( x opr rhs.x ? 0xFFFFFFFF : 0, same for yzw )
		// @details  比较运算。
		//****************************************************************

		furud_inline friend Vec4f furud_vectorapi Equal(const Vec4f& lhs, const Vec4f& rhs) noexcept
		{
			return _mm_cmpeq_ps(lhs.r, rhs.r);
		}

		furud_inline friend Vec4f furud_vectorapi NotEqual(const Vec4f& lhs, const Vec4f& rhs) noexcept
		{
			return _mm_cmpneq_ps(lhs.r, rhs.r);
		}

		furud_inline friend Vec4f furud_vectorapi GreaterThan(const Vec4f& lhs, const Vec4f& rhs) noexcept
		{
			return _mm_cmpgt_ps(lhs.r, rhs.r);
		}

		furud_inline friend Vec4f furud_vectorapi LessThan(const Vec4f& lhs, const Vec4f& rhs) noexcept
		{
			return _mm_cmplt_ps(lhs.r, rhs.r);
		}

		furud_inline friend Vec4f furud_vectorapi GreaterThanOrEqual(const Vec4f& lhs, const Vec4f& rhs) noexcept
		{
			return _mm_cmpge_ps(lhs.r, rhs.r);
		}

		furud_inline friend Vec4f furud_vectorapi LessThanOrEqual(const Vec4f& lhs, const Vec4f& rhs) noexcept
		{
			return _mm_cmple_ps(lhs.r, rhs.r);
		}


	public:
		//****************************************************************
		// @brief    Basic numerical operations.
		// @return   Vec4f( x opr rhs.x, same for yzw )
		// @details  加减乘除，求余。
		//****************************************************************

		furud_inline Vec4f furud_vectorapi operator - () const noexcept { return _mm_sub_ps(_mm_setzero_ps(), r); }

		furud_inline Vec4f furud_vectorapi operator + (const Vec4f& rhs) const noexcept { return _mm_add_ps(r, rhs.r); }
		furud_inline Vec4f furud_vectorapi operator - (const Vec4f& rhs) const noexcept { return _mm_sub_ps(r, rhs.r); }
		furud_inline Vec4f furud_vectorapi operator * (const Vec4f& rhs) const noexcept { return _mm_mul_ps(r, rhs.r); }
		furud_inline Vec4f furud_vectorapi operator / (const Vec4f& rhs) const noexcept { return _mm_div_ps(r, rhs.r); }

		furud_inline Vec4f furud_vectorapi operator + (const __m128& rhs) const noexcept { return _mm_add_ps(r, rhs); }
		furud_inline Vec4f furud_vectorapi operator - (const __m128& rhs) const noexcept { return _mm_sub_ps(r, rhs); }
		furud_inline Vec4f furud_vectorapi operator * (const __m128& rhs) const noexcept { return _mm_mul_ps(r, rhs); }
		furud_inline Vec4f furud_vectorapi operator / (const __m128& rhs) const noexcept { return _mm_div_ps(r, rhs); }

		furud_inline friend Vec4f furud_vectorapi operator + (const __m128& lhs, const Vec4f& rhs) noexcept { return _mm_add_ps(lhs, rhs.r); }
		furud_inline friend Vec4f furud_vectorapi operator - (const __m128& lhs, const Vec4f& rhs) noexcept { return _mm_sub_ps(lhs, rhs.r); }
		furud_inline friend Vec4f furud_vectorapi operator * (const __m128& lhs, const Vec4f& rhs) noexcept { return _mm_mul_ps(lhs, rhs.r); }
		furud_inline friend Vec4f furud_vectorapi operator / (const __m128& lhs, const Vec4f& rhs) noexcept { return _mm_div_ps(lhs, rhs.r); }

		furud_inline const Vec4f& furud_vectorapi operator += (const Vec4f& rhs) noexcept { r = _mm_add_ps(r, rhs.r); return *this; }
		furud_inline const Vec4f& furud_vectorapi operator -= (const Vec4f& rhs) noexcept { r = _mm_sub_ps(r, rhs.r); return *this; }
		furud_inline const Vec4f& furud_vectorapi operator *= (const Vec4f& rhs) noexcept { r = _mm_mul_ps(r, rhs.r); return *this; }
		furud_inline const Vec4f& furud_vectorapi operator /= (const Vec4f& rhs) noexcept { r = _mm_div_ps(r, rhs.r); return *this; }

		furud_inline const Vec4f& furud_vectorapi operator += (const __m128& rhs) noexcept { r = _mm_add_ps(r, rhs); return *this; }
		furud_inline const Vec4f& furud_vectorapi operator -= (const __m128& rhs) noexcept { r = _mm_sub_ps(r, rhs); return *this; }
		furud_inline const Vec4f& furud_vectorapi operator *= (const __m128& rhs) noexcept { r = _mm_mul_ps(r, rhs); return *this; }
		furud_inline const Vec4f& furud_vectorapi operator /= (const __m128& rhs) noexcept { r = _mm_div_ps(r, rhs); return *this; }

		furud_inline const Vec4f& furud_vectorapi operator %= (const Vec4f& rhs) noexcept
		{
			*this = *this % rhs;
			return *this;
		}

		/**
		 * @return  Vec4f( this % rhs )
		 */
		furud_inline Vec4f furud_vectorapi operator % (const Vec4f& rhs) const noexcept
		{
			__m128 div = _mm_div_ps(r, rhs.r);

			// Floats where abs(f) >= 2^23 have no fractional portion, and larger values would overflow RegisterTruncate.
			__m128 noFractionMask = _mm_cmpge_ps(_mm_and_ps(div, SIGN_MASK), NON_FRACTIONAL);
			__m128 temp = _mm_blendv_ps(_mm_cvtepi32_ps(_mm_cvttps_epi32(div)), div, noFractionMask);
			__m128 result = _mm_sub_ps(r, _mm_mul_ps(rhs.r, temp));

			// Clamp to [-abs2, abs2] because of possible failures for very large numbers (>1e10) due to precision loss.
			__m128 abs2 = _mm_and_ps(rhs.r, SIGN_MASK);
			return _mm_max_ps(_mm_sub_ps(_mm_setzero_ps(), abs2), _mm_min_ps(result, abs2));
		}

		/**
		 * @return  Vec4f( lhs * mhs + rhs )
		 */
		furud_inline friend Vec4f furud_vectorapi MultiplyAdd(const Vec4f& lhs, const Vec4f& mhs, const Vec4f& rhs) noexcept
		{
			return _mm_fmadd_ps(lhs.r, mhs.r, rhs.r);
		}

		/**
		 * @return  Vec4f( lhs1 * lhs2 + rhs1 * rhs2 )
		 */
		furud_inline friend Vec4f furud_vectorapi MultiplyAdd(const Vec4f& lhs1, const Vec4f& lhs2, const Vec4f& rhs1, const Vec4f& rhs2) noexcept
		{
			return _mm_fmadd_ps(lhs2.r, lhs1.r, _mm_mul_ps(rhs2.r, rhs1.r));
		}

		/**
		 * @return  Vec4f( lhs1 * lhs2 + mhs1 * mhs2 + rhs1 * rhs2 )
		 */
		furud_inline friend Vec4f furud_vectorapi MultiplyAdd(const Vec4f& lhs1, const Vec4f& lhs2, const Vec4f& mhs1, const Vec4f& mhs2, const Vec4f& rhs1, const Vec4f& rhs2) noexcept
		{
			return _mm_fmadd_ps(lhs2.r, lhs1.r, _mm_fmadd_ps(mhs2.r, mhs1.r, _mm_mul_ps(rhs2.r, rhs1.r)));
		}

		/**
		 * @return  Vec4f( lhs * mhs - rhs )
		 */
		furud_inline friend Vec4f furud_vectorapi MultiplySub(const Vec4f& lhs, const Vec4f& mhs, const Vec4f& rhs) noexcept
		{
			return _mm_fmsub_ps(lhs.r, mhs.r, rhs.r);
		}

		/**
		 * @return  Vec4f( -( mhs * mhs ) + rhs )
		 */
		furud_inline friend Vec4f furud_vectorapi NegateMultiplyAdd(const Vec4f& lhs, const Vec4f& mhs, const Vec4f& rhs) noexcept
		{
			return _mm_fnmadd_ps(lhs.r, mhs.r, rhs.r);
		}

		/**
		 * @return  Vec4f( -( lhs * mhs ) - rhs )
		 */
		furud_inline friend Vec4f furud_vectorapi NegateMultiplySub(const Vec4f& lhs, const Vec4f& mhs, const Vec4f& rhs) noexcept
		{
			return _mm_fnmsub_ps(lhs.r, mhs.r, rhs.r);
		}


	public:
		//****************************************************************
		// @brief    Basic numerical operations.
		// @return   Vec4f( x opr rhs.x, same for yzw )
		// @details  加减乘除。
		//****************************************************************

		furud_inline Vec4f furud_vectorapi operator + (const float& rhs) const noexcept { return _mm_add_ps(r, _mm_set_ps1(rhs)); }
		furud_inline Vec4f furud_vectorapi operator - (const float& rhs) const noexcept { return _mm_sub_ps(r, _mm_set_ps1(rhs)); }
		furud_inline Vec4f furud_vectorapi operator * (const float& rhs) const noexcept { return _mm_mul_ps(r, _mm_set_ps1(rhs)); }
		furud_inline Vec4f furud_vectorapi operator / (const float& rhs) const noexcept { return _mm_div_ps(r, _mm_set_ps1(rhs)); }

		furud_inline friend Vec4f furud_vectorapi operator + (const float& lhs, const Vec4f& rhs) noexcept { return _mm_add_ps(_mm_set_ps1(lhs), rhs.r); }
		furud_inline friend Vec4f furud_vectorapi operator - (const float& lhs, const Vec4f& rhs) noexcept { return _mm_sub_ps(_mm_set_ps1(lhs), rhs.r); }
		furud_inline friend Vec4f furud_vectorapi operator * (const float& lhs, const Vec4f& rhs) noexcept { return _mm_mul_ps(_mm_set_ps1(lhs), rhs.r); }
		furud_inline friend Vec4f furud_vectorapi operator / (const float& lhs, const Vec4f& rhs) noexcept { return _mm_div_ps(_mm_set_ps1(lhs), rhs.r); }

		furud_inline const Vec4f& operator += (const float& rhs) noexcept { r = _mm_add_ps(r, _mm_set_ps1(rhs)); return *this; }
		furud_inline const Vec4f& operator -= (const float& rhs) noexcept { r = _mm_sub_ps(r, _mm_set_ps1(rhs)); return *this; }
		furud_inline const Vec4f& operator *= (const float& rhs) noexcept { r = _mm_mul_ps(r, _mm_set_ps1(rhs)); return *this; }
		furud_inline const Vec4f& operator /= (const float& rhs) noexcept { r = _mm_div_ps(r, _mm_set_ps1(rhs)); return *this; }


	public:
		/**
		 * @brief    Swizzles the Vec4f.
		 * @return   Vec4f( r[i], r[j], r[k], r[l] )
		 * @details  交换向量元素位置。
		 */
		template<uint32_t i, uint32_t j, uint32_t k, uint32_t l>
		furud_inline Vec4f furud_vectorapi Swizzle() const noexcept
		{
			constexpr int32_t v = i | (j << 2) | (k << 4) | (l << 6);
			return _mm_shuffle_ps(r, r, v);
		}

		/**
		 * @brief    Creates a Vec4f through selecting two elements from each Vec4f via a shuffle mask.
		 * @return   Vec4f( lhs.r[x], lhs.r[y], rhs.r[z], rhs.r[w] )
		 * @details  交换向量元素位置。
		 */
		template<uint32_t i, uint32_t j, uint32_t k, uint32_t l>
		furud_inline friend Vec4f furud_vectorapi Shuffle(const Vec4f& lhs, const Vec4f& rhs) noexcept
		{
			constexpr int32_t v = i | (j << 2) | (k << 4) | (l << 6);
			return _mm_shuffle_ps(lhs.r, rhs.r, v);
		}


	public:
		//****************************************************************
		// @brief    Returns the minimum/maximum values of two Vec4f.
		// @return   Vec4f( min( lhs.x, rhs.x )/max( lhs.x, rhs.x ), same for yzw )
		// @details  最大/最小值。
		//****************************************************************

		furud_inline friend Vec4f furud_vectorapi Min(const Vec4f& lhs, const Vec4f& rhs) noexcept { return _mm_min_ps(lhs.r, rhs.r); }
		furud_inline friend Vec4f furud_vectorapi Max(const Vec4f& lhs, const Vec4f& rhs) noexcept { return _mm_max_ps(lhs.r, rhs.r); }

		furud_inline friend Vec4f furud_vectorapi Min(const Vec4f& lhs, const Vec4f& mhs, const Vec4f& rhs) noexcept { return _mm_min_ps(_mm_min_ps(lhs.r, mhs.r), rhs.r); }
		furud_inline friend Vec4f furud_vectorapi Max(const Vec4f& lhs, const Vec4f& mhs, const Vec4f& rhs) noexcept { return _mm_max_ps(_mm_max_ps(lhs.r, mhs.r), rhs.r); }


	public:
		/**
		 * @brief    Computes the square roots.
		 * @return   Vec4f( sqrt(r.x), same for yzw )
		 * @details  平方根。
		 */
		furud_inline Vec4f furud_vectorapi Sqrt() const noexcept { return _mm_sqrt_ps(r); }

		/**
		 * @brief    Computes the reciprocal square roots.
		 * @return   Vec4f( 1/sqrt(r.x), same for yzw )
		 * @details  平方根倒数。
		 */
		furud_inline Vec4f furud_vectorapi RecipSqrt() const noexcept { return _mm_rsqrt_ps(r); }

		/**
		 * @brief    Computes the squares.
		 * @return   Vec4f( r.x * r.x, same for yzw )
		 * @details  平方。
		 */
		furud_inline Vec4f furud_vectorapi Squares() const noexcept { return _mm_mul_ps(r, r); }

		/*
		 * @brief    Computes the approximations of reciprocals.
		 * @return   Vec4f( 1 / r.x, same for yzw )
		 * @details  倒数。
		 */
		furud_inline Vec4f furud_vectorapi Reciprocal() const noexcept { return _mm_rcp_ps(r); }

		/**
		 * @brief    Returns the absolute value.
		 * @return   Vec4f( abs(r.x), same for yzw )
		 * @details  绝对值。
		 */
		furud_inline Vec4f furud_vectorapi Abs() const noexcept { return _mm_and_ps(r, SIGN_MASK); }

		/**
		 * @brief    Rounds A to an integer with truncation towards zero for each elements. (e.g. -1.7 truncated to -1, 2.8 truncated to 2)
		 * @return   Vec4f( truncate(r.x), same for yzw )
		 * @details  截断。
		 */
		furud_inline Vec4f furud_vectorapi Truncate() const noexcept
		{
			return _mm_round_ps(r, _MM_FROUND_TRUNC);
		}

		/**
		 * @brief    Rounds A to an integer for each elements.
		 * @return   Vec4f( round(r.x), same for yzw )
		 * @details  四舍五入。
		 */
		furud_inline Vec4f furud_vectorapi Round() const noexcept
		{
			return _mm_round_ps(r, _MM_FROUND_NINT);
		}

		/**
		 * @brief    Calculates floor() for each elements.
		 * @return   Vec4f( floor(r.x), same for yzw )
		 * @details  向下取整。
		 */
		furud_inline Vec4f furud_vectorapi Floor() const noexcept
		{
			__m128 trunc = _mm_cvtepi32_ps(_mm_cvttps_epi32(r));
			__m128 mask = _mm_cmpge_ps(r, ZERO);
			__m128 sub = _mm_blendv_ps(ONE, ZERO, mask);
			return _mm_sub_ps(trunc, sub);
		}

		/**
		 * @brief    Calculates ceil() for each elements.
		 * @return   Vec4f( ceil(r.x), same for yzw )
		 * @details  向上取整。
		 */
		furud_inline Vec4f furud_vectorapi Ceil() const noexcept
		{
			__m128 trunc = _mm_cvtepi32_ps(_mm_cvttps_epi32(r));
			__m128 mask = _mm_cmpge_ps(r, ZERO);
			__m128 add = _mm_blendv_ps(ZERO, ONE, mask);
			return _mm_add_ps(trunc, add);
		}

		/**
		 * @brief    Returns a sum of four elements.
		 * @return   ( r.x + r.y + r.z + r.w )
		 * @details  向量元素求和。
		 */
		furud_inline float Sum() const noexcept
		{
			__m128 xy_zw = _mm_hadd_ps(r, r);
			__m128 xyzw = _mm_hadd_ps(xy_zw, xy_zw);
			return _mm_cvtss_f32(xyzw);
		}

		/**
		 * @brief    Returns a sum of four elements.
		 * @return   Vec4f( r.x + r.y + r.z + r.w )
		 * @details  向量元素求和。
		 */
		furud_inline Vec4f furud_vectorapi Sum4() const noexcept
		{
			__m128 xy_zw = _mm_hadd_ps(r, r);
			return _mm_hadd_ps(xy_zw, xy_zw);
		}

		/**
		 * @brief    Returns the magnitude.
		 * @return   ( r.x + r.y + r.z + r.w )
		 * @details  模长。
		 */
		furud_inline float Length() const noexcept
		{
			float len = _mm_cvtss_f32(_mm_dp_ps(r, r, 0xf1));
			return sqrtf(len);
		}

		/**
		 * @brief    Returns the magnitude.
		 * @return   Vec4f( r.x + r.y + r.z + r.w )
		 * @details  模长。
		 */
		furud_inline Vec4f Length4() const noexcept
		{
			return _mm_sqrt_ps(_mm_dp_ps(r, r, 0xf1));
		}

		/**
		 * @brief    Normalizes this vec4f.
		 * @return   *this / Length
		 * @details  单位化。
		 */
		furud_inline Vec4f furud_vectorapi Normalize(const float& tolerance = IFloat::SMALLER) const noexcept
		{
			const float squareSum = Squares().Sum();
			if (squareSum > tolerance)
			{
				__m128 x0, y0, x1, x2, fOver2;

				// Fast Inverse Square Root.
				y0 = _mm_set_ps1(squareSum);
				fOver2 = _mm_mul_ps(y0, HALF);

				// 1/sqrt estimate (12 bits)
				x0 = _mm_rsqrt_ps(y0);

				// 1st Newton-Raphson iteration
				x1 = _mm_mul_ps(x0, x0);
				x1 = _mm_fnmadd_ps(fOver2, x1, HALF);
				x1 = _mm_fmadd_ps(x0, x1, x0);

				// 2nd Newton-Raphson iteration
				x2 = _mm_mul_ps(x1, x1);
				x2 = _mm_fnmadd_ps(fOver2, x2, HALF);
				x2 = _mm_fmadd_ps(x1, x2, x1);

				return _mm_mul_ps(r, x2);
			}
			return *this;
		}

		/**
		 * @brief    Homogeneous division.
		 * @return   Vec4f( r.x / r.w, r.y / r.w, r.z / r.w, 1 )
		 * @details  齐次化。
		 */
		furud_inline Vec4f furud_vectorapi Homogenize() const noexcept
		{
			return _mm_div_ps(r, _mm_shuffle_ps(r, r, 255u));
		}

		/**
		 * @brief    Does a bitwise Vec4f selection based on a mask.
		 * @return   Vec4f( (mask.x & 0x80000000) ? lhs.x : rhs.x, same for yzw )
		 * @details  选择向量元素。
		 */
		furud_inline friend Vec4f furud_vectorapi Select(const Vec4f& mask, const Vec4f& lhs, const Vec4f& rhs) noexcept
		{
			return _mm_blendv_ps(rhs.r, lhs.r, mask.r);
		}

		/**
		 * @brief    Dot product two Vec4f.
		 * @return   ( lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w + rhs.w )
		 * @details  点乘。
		 */
		furud_inline friend float furud_vectorapi DotProduct(const Vec4f& lhs, const Vec4f& rhs) noexcept
		{
			return _mm_cvtss_f32(_mm_dp_ps(lhs.r, rhs.r, 0xf1));
		}

		/**
		 * @brief    Compute the exponential value of two Vec4f.
		 * @return   ( Pow(lhs.x, rhs.x), same for yzw )
		 * @details  幂运算。
		 */
		furud_inline friend Vec4f furud_vectorapi Pow(const Vec4f& lhs, const Vec4f& rhs) noexcept
		{
			return _mm_pow_ps(lhs.r, rhs.r);
		}


	public:
		//****************************************************************
		// @brief    Combines two Vec4f using bitwise logic operation.
		// @return   Vec4f( this.x opr reg2.x, same for yzw )
		// @details  按位逻辑运算。
		//****************************************************************

		furud_inline friend Vec4f furud_vectorapi And(const Vec4f& lhs, const Vec4f& rhs) noexcept { return _mm_and_ps(lhs.r, rhs.r); }
		furud_inline friend Vec4f furud_vectorapi AndNot(const Vec4f& lhs, const Vec4f& rhs) noexcept { return _mm_andnot_ps(lhs.r, rhs.r); }
		furud_inline friend Vec4f furud_vectorapi Or(const Vec4f& lhs, const Vec4f& rhs) noexcept { return _mm_or_ps(lhs.r, rhs.r); }
		furud_inline friend Vec4f furud_vectorapi Xor(const Vec4f& lhs, const Vec4f& rhs) noexcept { return _mm_xor_ps(lhs.r, rhs.r); }


	public:
		/**
		 * @brief    Returns an integer bit-mask (0x00 - 0x0f) based on the sign-bit for each elements in a Vec4f.
		 * @return   Bit 0 = sign(mask.x), Bit 1 = sign(mask.y), Bit 2 = sign(mask.z), Bit 3 = sign(mask.w)
		 * @details  掩码。
		 */
		furud_inline int32_t MaskBits() const noexcept { return _mm_movemask_ps(r); }

		/**
		 * @brief    Copy sign.
		 * @return   Vec4f( sign.x > 0 ? abs(value.x) : -abs(value.x), same for yzw )
		 * @details  复制符号位。
		 */
		furud_inline friend Vec4f furud_vectorapi Copysign(const Vec4f& value, const Vec4f& sign) noexcept
		{
			return _mm_or_ps(_mm_and_ps(SIGN_BIT, sign.r), _mm_andnot_ps(SIGN_BIT, value.r));
		}
	};


	furud_inline Vec4f Vec4i::ToFloat() const noexcept { return _mm_cvtepi32_ps(r); }
	furud_inline Vec4i Vec4f::ToInteger() const noexcept { return _mm_cvtps_epi32(r); }
}



/** 4D 64-bit double floating-point vector. */
export namespace Furud
{
	/**
	 * @brief    4D 32-bit double floating-point vector.
	 * @details  SIMD 四维双精度浮点向量。
	 * @todo     Need AVX512F + AVX512VL
	 */
	//class Vec4d final
	//{
	//private:
	//	__m256d r;


	//public:
	//	constexpr Vec4d() noexcept {}
	//	constexpr Vec4d(__m256d&& value) noexcept : r(std::move(value)) {}
	//	constexpr Vec4d(const __m256d& value) noexcept : r(value) {}
	//	constexpr Vec4d(double&& x) noexcept : r({ x, x, x, x }) {}
	//	constexpr Vec4d(double&& x, double&& y, double&& z, double&& w) noexcept : r({ x, y, z, w }) {}


	//public:
	//	furud_inline Vec4d(const double& value) noexcept
	//		: r(_mm256_set1_pd(value))
	//	{}

	//	furud_inline Vec4d(const double& x, const double& y, const double& z, const double& w = 0.0) noexcept
	//		: r(_mm256_setr_pd(x, y, z, w))
	//	{}

	//	furud_inline Vec4d(const Vec4f& value) noexcept
	//		: r(_mm256_cvtps_pd(value.r))
	//	{}

	//	furud_inline Vec4d(const __m128& value) noexcept
	//		: r(_mm256_cvtps_pd(value))
	//	{}

	//	furud_inline operator __m256d() noexcept { return r; }

	//	Vec4d* operator & () = delete;
	//	
	//	const Vec4d* operator & () const = delete;


	//public:
	//	//****************************************************************
	//	// @brief    Loads/Stores a Vec4d from/to memory.
	//	// @details  加载/存储向量。
	//	//****************************************************************

	//	furud_inline Vec4d& Load3(const void* data) noexcept
	//	{
	//		const double* furud_restrict p = static_cast<const double*>(data);
	//		r = _mm256_setr_pd(p[0], p[1], p[2], 0.0);
	//		return *this;
	//	}

	//	furud_inline Vec4d& Load4(const void* data) noexcept
	//	{
	//		r = _mm256_load_pd(static_cast<const double*>(data));
	//		return *this;
	//	}

	//	furud_inline void Store3(void* data) noexcept
	//	{
	//		alignas(32) double xyzw[4];
	//		_mm256_store_pd(xyzw, r);
	//		double* furud_restrict p = static_cast<double*>(data);
	//		p[0] = xyzw[0];
	//		p[1] = xyzw[1];
	//		p[2] = xyzw[2];
	//	}
	//	
	//	furud_inline void Store4(void* data) const noexcept
	//	{
	//		_mm256_store_pd(static_cast<double*>(data), r);
	//	}


	//private:
	//	furud_inline double Extract(int32_t&& index) const noexcept
	//	{
	//		return r.m256d_f64[index];
	//	}

	//	furud_inline void Insert(int32_t&& index, const double& value) noexcept
	//	{
	//		r.m256d_f64[index] = value;
	//	}


	//public:
	//	//****************************************************************
	//	// @brief    Visits element.
	//	// @details  访问向量元素。
	//	//****************************************************************

	//	furud_inline double X() const noexcept { return Extract(0); }
	//	furud_inline double Y() const noexcept { return Extract(1); }
	//	furud_inline double Z() const noexcept { return Extract(2); }
	//	furud_inline double W() const noexcept { return Extract(3); }
	//	furud_inline void SetX(const double& value) noexcept { return Insert(0, value); }
	//	furud_inline void SetY(const double& value) noexcept { return Insert(1, value); }
	//	furud_inline void SetZ(const double& value) noexcept { return Insert(2, value); }
	//	furud_inline void SetW(const double& value) noexcept { return Insert(3, value); }


	//public:
	//	//****************************************************************
	//	// @brief    Creates a four-part mask through elements comparison from each Vec4d.
	//	// @return   Vec4d( x opr rhs.x ? 0xFFFFFFFF : 0, same for yzw )
	//	// @details  比较运算。
	//	//****************************************************************

	//	furud_inline friend Vec4d furud_vectorapi Equal(const Vec4d& lhs, const Vec4d& rhs) noexcept
	//	{
	//		return _mm256_cmp_pd(lhs.r, rhs.r, _CMP_EQ_OQ);
	//	}

	//	furud_inline friend Vec4d furud_vectorapi NotEqual(const Vec4d& lhs, const Vec4d& rhs) noexcept
	//	{
	//		return _mm256_cmp_pd(lhs.r, rhs.r, _CMP_NEQ_OQ);
	//	}

	//	furud_inline friend Vec4d furud_vectorapi GreaterThan(const Vec4d& lhs, const Vec4d& rhs) noexcept
	//	{
	//		return _mm256_cmp_pd(lhs.r, rhs.r, _CMP_GT_OQ);
	//	}

	//	furud_inline friend Vec4d furud_vectorapi LessThan(const Vec4d& lhs, const Vec4d& rhs) noexcept
	//	{
	//		return _mm256_cmp_pd(lhs.r, rhs.r, _CMP_LT_OQ);
	//	}

	//	furud_inline friend Vec4d furud_vectorapi GreaterThanOrEqual(const Vec4d& lhs, const Vec4d& rhs) noexcept
	//	{
	//		return _mm256_cmp_pd(lhs.r, rhs.r, _CMP_GE_OQ);
	//	}

	//	furud_inline friend Vec4d furud_vectorapi LessThanOrEqual(const Vec4d& lhs, const Vec4d& rhs) noexcept
	//	{
	//		return _mm256_cmp_pd(lhs.r, rhs.r, _CMP_LE_OQ);
	//	}


	//public:
	//	//****************************************************************
	//	// @brief    Basic numerical operations.
	//	// @return   Vec4d( x opr rhs.x, same for yzw )
	//	// @details  加减乘除。
	//	//****************************************************************

	//	furud_inline Vec4d furud_vectorapi operator - () const noexcept { return _mm256_sub_pd(_mm256_set1_pd(0), r); }

	//	furud_inline Vec4d furud_vectorapi operator + (const Vec4d& rhs) const noexcept { return _mm256_add_pd(r, rhs.r); }
	//	furud_inline Vec4d furud_vectorapi operator - (const Vec4d& rhs) const noexcept { return _mm256_sub_pd(r, rhs.r); }
	//	furud_inline Vec4d furud_vectorapi operator * (const Vec4d& rhs) const noexcept { return _mm256_mul_pd(r, rhs.r); }
	//	furud_inline Vec4d furud_vectorapi operator / (const Vec4d& rhs) const noexcept { return _mm256_div_pd(r, rhs.r); }

	//	furud_inline Vec4d furud_vectorapi operator + (const __m256d& rhs) const noexcept { return _mm256_add_pd(r, rhs); }
	//	furud_inline Vec4d furud_vectorapi operator - (const __m256d& rhs) const noexcept { return _mm256_sub_pd(r, rhs); }
	//	furud_inline Vec4d furud_vectorapi operator * (const __m256d& rhs) const noexcept { return _mm256_mul_pd(r, rhs); }
	//	furud_inline Vec4d furud_vectorapi operator / (const __m256d& rhs) const noexcept { return _mm256_div_pd(r, rhs); }

	//	furud_inline friend Vec4d furud_vectorapi operator + (const __m256d& lhs, const Vec4d& rhs) noexcept { return _mm256_add_pd(lhs, rhs.r); }
	//	furud_inline friend Vec4d furud_vectorapi operator - (const __m256d& lhs, const Vec4d& rhs) noexcept { return _mm256_sub_pd(lhs, rhs.r); }
	//	furud_inline friend Vec4d furud_vectorapi operator * (const __m256d& lhs, const Vec4d& rhs) noexcept { return _mm256_mul_pd(lhs, rhs.r); }
	//	furud_inline friend Vec4d furud_vectorapi operator / (const __m256d& lhs, const Vec4d& rhs) noexcept { return _mm256_div_pd(lhs, rhs.r); }

	//	furud_inline const Vec4d& operator += (const Vec4d& rhs) noexcept { r = _mm256_add_pd(r, rhs.r); return *this; }
	//	furud_inline const Vec4d& operator -= (const Vec4d& rhs) noexcept { r = _mm256_sub_pd(r, rhs.r); return *this; }
	//	furud_inline const Vec4d& operator *= (const Vec4d& rhs) noexcept { r = _mm256_mul_pd(r, rhs.r); return *this; }
	//	furud_inline const Vec4d& operator /= (const Vec4d& rhs) noexcept { r = _mm256_div_pd(r, rhs.r); return *this; }

	//	furud_inline const Vec4d& operator += (const __m256d& rhs) noexcept { r = _mm256_add_pd(r, rhs); return *this; }
	//	furud_inline const Vec4d& operator -= (const __m256d& rhs) noexcept { r = _mm256_sub_pd(r, rhs); return *this; }
	//	furud_inline const Vec4d& operator *= (const __m256d& rhs) noexcept { r = _mm256_mul_pd(r, rhs); return *this; }
	//	furud_inline const Vec4d& operator /= (const __m256d& rhs) noexcept { r = _mm256_div_pd(r, rhs); return *this; }

	//public:
	//	//****************************************************************
	//	// @brief    Basic numerical operations.
	//	// @return   Vec4d( x opr rhs.x, same for yzw )
	//	// @details  加减乘除。
	//	//****************************************************************

	//	furud_inline Vec4d furud_vectorapi operator + (const double& rhs) const noexcept { return _mm256_add_pd(r, _mm256_set1_pd(rhs)); }
	//	furud_inline Vec4d furud_vectorapi operator - (const double& rhs) const noexcept { return _mm256_sub_pd(r, _mm256_set1_pd(rhs)); }
	//	furud_inline Vec4d furud_vectorapi operator * (const double& rhs) const noexcept { return _mm256_mul_pd(r, _mm256_set1_pd(rhs)); }
	//	furud_inline Vec4d furud_vectorapi operator / (const double& rhs) const noexcept { return _mm256_div_pd(r, _mm256_set1_pd(rhs)); }

	//	furud_inline friend Vec4d furud_vectorapi operator + (const double& lhs, const Vec4d& rhs) noexcept { return _mm256_add_pd(_mm256_set1_pd(lhs), rhs.r); }
	//	furud_inline friend Vec4d furud_vectorapi operator - (const double& lhs, const Vec4d& rhs) noexcept { return _mm256_sub_pd(_mm256_set1_pd(lhs), rhs.r); }
	//	furud_inline friend Vec4d furud_vectorapi operator * (const double& lhs, const Vec4d& rhs) noexcept { return _mm256_mul_pd(_mm256_set1_pd(lhs), rhs.r); }
	//	furud_inline friend Vec4d furud_vectorapi operator / (const double& lhs, const Vec4d& rhs) noexcept { return _mm256_div_pd(_mm256_set1_pd(lhs), rhs.r); }

	//	furud_inline const Vec4d& operator += (const double& rhs) noexcept { r = _mm256_add_pd(r, _mm256_set1_pd(rhs)); return *this; }
	//	furud_inline const Vec4d& operator -= (const double& rhs) noexcept { r = _mm256_sub_pd(r, _mm256_set1_pd(rhs)); return *this; }
	//	furud_inline const Vec4d& operator *= (const double& rhs) noexcept { r = _mm256_mul_pd(r, _mm256_set1_pd(rhs)); return *this; }
	//	furud_inline const Vec4d& operator /= (const double& rhs) noexcept { r = _mm256_div_pd(r, _mm256_set1_pd(rhs)); return *this; }


	//public:
	//	//****************************************************************
	//	// @brief   Returns the minimum/maximum values of two Vec4d.
	//	// @return  Vec4d( min( lhs.x, rhs.x )/max( lhs.x, rhs.x ), same for yzw )
	//	// @details  最大/最小值。
	//	//****************************************************************

	//	furud_inline friend Vec4d furud_vectorapi Min(const Vec4d& lhs, const Vec4d& rhs) noexcept { return _mm256_min_pd(lhs.r, rhs.r); }
	//	furud_inline friend Vec4d furud_vectorapi Max(const Vec4d& lhs, const Vec4d& rhs) noexcept { return _mm256_max_pd(lhs.r, rhs.r); }
	//	furud_inline friend Vec4d furud_vectorapi Min(const Vec4d& lhs, const Vec4d& mhs, const Vec4d& rhs) noexcept { return _mm256_min_pd(lhs.r, _mm256_min_pd(mhs.r, rhs.r)); }
	//	furud_inline friend Vec4d furud_vectorapi Max(const Vec4d& lhs, const Vec4d& mhs, const Vec4d& rhs) noexcept { return _mm256_max_pd(lhs.r, _mm256_max_pd(mhs.r, rhs.r)); }


	//public:
	//	/**
	//	 * @brief    Computes the square roots.
	//	 * @return   Vec4d( sqrt(x), same for yzw )
	//	 * @details  平方根。
	//	 */
	//	furud_inline Vec4d furud_vectorapi Sqrt() const noexcept { return _mm256_sqrt_pd(r); }

	//	/**
	//	 * @brief    Computes the squares.
	//	 * @return   Vec4d( reg1.x * reg1.x, same for yzw )
	//	 * @details  平方根倒数。
	//	 */
	//	furud_inline Vec4d furud_vectorapi Squares() const noexcept { return _mm256_mul_pd(r, r); }

	//	/*
	//	 * @brief    Computes the approximations of reciprocals.
	//	 * @return   Vec4d( 1 / reg1.x, same for yzw )
	//	 * @details  倒数。
	//	 */
	//	furud_inline Vec4d furud_vectorapi Reciprocal() const noexcept { return _mm256_rcp14_pd(r); }

	//	/**
	//	 * @brief    Returns the absolute value.
	//	 * @return   Vec4d( abs(r.x), same for yzw )
	//	 * @details  绝对值。
	//	 */
	//	furud_inline Vec4d furud_vectorapi Abs() const noexcept
	//	{
	//		__m256i mask = _mm256_set1_epi64x(int64_t(~(1ull << 63)));
	//		return _mm256_and_pd(r, _mm256_loadu_pd((double*)(&mask)));
	//	}

	//	/**
	//	 * @brief    Returns a sum of four elements.
	//	 * @return   ( r.x + r.y + r.z + r.w )
	//	 * @details  向量元素求和。
	//	 */
	//	furud_inline double Sum() const noexcept
	//	{
	//		__m256d xy_zw = _mm256_hadd_pd(r, r);
	//		return xy_zw.m256d_f64[0] + xy_zw.m256d_f64[2];
	//		//__m256d xy_zw = _mm256_hadd_pd(r, _mm256_permute2f128_pd(r, r, 1));
	//		//__m256d xyzw = _mm256_hadd_pd(xy_zw, xy_zw);
	//		//return _mm256_cvtsd_f64(xyzw);
	//	}

	//	/**
	//	 * @brief    Normalizes this vec4d.
	//	 * @return   *this / Length
	//	 * @details  单位化。
	//	 */
	//	furud_inline Vec4d furud_vectorapi Normalize(const double& tolerance = IDouble::SMALLER) const noexcept
	//	{
	//		const double squareSum = Squares().Sum();
	//		if (squareSum > tolerance)
	//		{
	//			Vec4d v = _mm256_set1_pd(squareSum);
	//			return (*this) * v.Sqrt().Reciprocal();
	//		}
	//		return *this;
	//	}

	//	/**
	//	 * @brief    Dot product two Vec4d.
	//	 * @return   ( lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w + rhs.w )
	//	 * @details  点乘。
	//	 */
	//	furud_inline friend double furud_vectorapi DotProduct(const Vec4d& lhs, const Vec4d& rhs) noexcept
	//	{
	//		return (lhs * rhs).Sum();
	//	}


	//public:
	//	//****************************************************************
	//	// @brief    Combines two Vec4d using bitwise logic operation.
	//	// @return   Vec4d( this.x opr reg2.x, same for yzw )
	//	// @details  逻辑运算。
	//	//****************************************************************

	//	furud_inline friend Vec4d furud_vectorapi And(const Vec4d& lhs, const Vec4d& rhs) noexcept { return _mm256_and_pd(lhs.r, rhs.r); }
	//	furud_inline friend Vec4d furud_vectorapi AndNot(const Vec4d& lhs, const Vec4d& rhs) noexcept { return _mm256_andnot_pd(lhs.r, rhs.r); }
	//	furud_inline friend Vec4d furud_vectorapi Or(const Vec4d& lhs, const Vec4d& rhs) noexcept { return _mm256_or_pd(lhs.r, rhs.r); }
	//	furud_inline friend Vec4d furud_vectorapi Xor(const Vec4d& lhs, const Vec4d& rhs) noexcept { return _mm256_xor_pd(lhs.r, rhs.r); }


	//public:
	//	/**
	//	 * @brief    Returns an integer bit-mask (0x00 - 0x0f) based on the sign-bit for each elements in a R128f.
	//	 * @return   Bit 0 = sign(mask.x), Bit 1 = sign(mask.y), Bit 2 = sign(mask.z), Bit 3 = sign(mask.w)
	//	 * @details  掩码。
	//	 */
	//	furud_inline int32_t MaskBits() const noexcept { return _mm256_movemask_pd(r); }

	//	/**
	//	 * @brief    Copy sign.
	//	 * @return   Vec4d( sign.x > 0 ? abs(value.x) : -abs(value.x), same for yzw )
	//	 * @details  复制符号位。
	//	 */
	//	furud_inline friend Vec4d furud_vectorapi Copysign(const Vec4d& value, const Vec4d& sign) noexcept
	//	{
	//		__m256i mask = _mm256_set1_epi64x(int64_t(~(1ull << 63)));
	//		__m256d signBit = _mm256_loadu_pd((double*)(&mask));
	//		return _mm256_or_pd(_mm256_and_pd(signBit, sign.r), _mm256_andnot_pd(signBit, value.r));
	//	}
	//};
}