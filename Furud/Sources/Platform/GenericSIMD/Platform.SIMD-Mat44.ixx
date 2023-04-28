//
// Platform.SIMD-Mat44.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Vectorized matrix structure.
//
module;

#include <Furud.hpp>
#include <immintrin.h>



export module Furud.Platform.SIMD:Mat44;

import :Vec4;



/** Forward declaration. */
export namespace Furud
{
	class Mat44f;
	FURUD_INLINE extern Vec4f FURUD_VECTORAPI operator * (const Vec4f& lhs, const Mat44f& rhs) noexcept;


	//****************************************************************
	// @brief    Replicates one element into all four elements and returns a new Vec4f.
	// @return   Vec4i( r[index], same for yzw )
	// @detials  复制向量元素。
	//****************************************************************

	FURUD_INLINE Vec4f FURUD_VECTORAPI ReplicateX(const __m128& r) noexcept { return _mm_shuffle_ps(r, r, 0);    }
	FURUD_INLINE Vec4f FURUD_VECTORAPI ReplicateY(const __m128& r) noexcept { return _mm_shuffle_ps(r, r, 85u);  }
	FURUD_INLINE Vec4f FURUD_VECTORAPI ReplicateZ(const __m128& r) noexcept { return _mm_shuffle_ps(r, r, 170u); }
	FURUD_INLINE Vec4f FURUD_VECTORAPI ReplicateW(const __m128& r) noexcept { return _mm_shuffle_ps(r, r, 255u); }


	/**
	 * @brief    Creates a Vec4f through selecting two elements from each Vec4f via a shuffle mask.
	 * @return   Vec4f( lhs.r[x], lhs.r[y], rhs.r[z], rhs.r[w] )
	 * @details  交换向量元素位置。
	 */
	template<uint32_t i, uint32_t j, uint32_t k, uint32_t l>
	FURUD_INLINE Vec4f FURUD_VECTORAPI Shuffle(const __m128& lhs, const __m128& rhs) noexcept
	{
		constexpr uint32_t v = i | (j << 2) | (k << 4) | (l << 6);
		return _mm_shuffle_ps(lhs, rhs, v);
	}


	/**
	 * @brief   2x2 row major Matrix multiply.
	 * @return  ( a * b )
	 */
	FURUD_INLINE Vec4f FURUD_VECTORAPI Matrix2x2Multiply(const Vec4f& vec1, const Vec4f& vec2)
	{
		return vec1 * vec2.Swizzle<0, 3, 0, 3>() + vec1.Swizzle<1, 0, 3, 2>() * vec2.Swizzle<2, 1, 2, 1>();
	}


	/**
	 * @brief   2x2 row major Matrix adjugate multiply.
	 * @return  ( (a#) * b )
	 */
	FURUD_INLINE Vec4f FURUD_VECTORAPI Matrix2x2AdjMultiply(const Vec4f& vec1, const Vec4f& vec2)
	{
		return vec2 * vec1.Swizzle<3, 3, 0, 0>() - vec1.Swizzle<1, 1, 2, 2>() * vec2.Swizzle<2, 3, 0, 1>();
	}


	/**
	 * @brief   2x2 row major Matrix multiply adjugate.
	 * @return  ( a * (b#) )
	 */
	FURUD_INLINE Vec4f FURUD_VECTORAPI Matrix2x2MultiplyAdj(const Vec4f& vec1, const Vec4f& vec2)
	{
		return vec1 * vec2.Swizzle<3, 0, 3, 0>() - vec1.Swizzle<1, 0, 3, 2>() * vec2.Swizzle<2, 1, 2, 1>();
	}
}



/** Column-major matrix (single floating-point). */
export namespace Furud
{
	/**
	 * @brief    Column-major matrix (single floating-point).
	 * @details  列主序矩阵。
	 *           因为是列主序，所以矩阵运算规则颠倒。
	 *           - 数学上的右乘列向量，变为左乘行向量。	( Mat1 * Vcol = Vcol => Vrow * Mat1 = Vrow )
	 *           - 数学上的左乘矩阵，变为右乘矩阵。		( Mat1 * Mat2 = Mat3 => Mat2 * Mat1 = Mat3 )
	 */ 
	class alignas(32) Mat44f final
	{
	private:
		__m128 r[4];


	public:
		constexpr Mat44f() noexcept {}


	public:
		Mat44f* operator & () = delete;
		const Mat44f* operator & () const = delete;


	public:
		//****************************************************************
		// @brief    Loads/Stores a Vec4i from/to aligned memory.
		// @details  加载/存储向量。
		//****************************************************************

		FURUD_INLINE Mat44f& Load(const void* data) noexcept
		{
			const float* FURUD_RESTRICT m = static_cast<const float*>(data);
			r[0] = _mm_load_ps(m);
			r[1] = _mm_load_ps(m + 4);
			r[2] = _mm_load_ps(m + 8);
			r[3] = _mm_load_ps(m + 12);
			return *this;
		}

		FURUD_INLINE void Store(void* data) const noexcept
		{
			float* FURUD_RESTRICT m = static_cast<float*>(data);
			_mm_store_ps(m, r[0]);
			_mm_store_ps(m + 4, r[1]);
			_mm_store_ps(m + 8, r[2]);
			_mm_store_ps(m + 12, r[3]);
		}

		FURUD_INLINE static void Copy(const void* src, void* dst) noexcept
		{
			const float* FURUD_RESTRICT s = static_cast<const float*>(src);
			float* FURUD_RESTRICT d = static_cast<float*>(dst);
			_mm256_store_ps(d, _mm256_load_ps(s));
			_mm256_store_ps(d + 8, _mm256_load_ps(s + 8));
		}


	public:
		//****************************************************************
		// @brief    Visits element.
		// @details  访问向量元素。
		//****************************************************************

		constexpr __m128& operator [] (const int32_t& index) noexcept { return r[index]; }
		constexpr const __m128& operator [] (const int32_t& index) const noexcept { return r[index]; }

		constexpr void SetRow0(const Vec4f& value) noexcept { r[0] = value.r; }
		constexpr void SetRow1(const Vec4f& value) noexcept { r[1] = value.r; }
		constexpr void SetRow2(const Vec4f& value) noexcept { r[2] = value.r; }
		constexpr void SetRow3(const Vec4f& value) noexcept { r[3] = value.r; }


	public:
		//****************************************************************
		// @brief    Basic numerical operations.
		// @details  加减乘除。
		//****************************************************************

		FURUD_INLINE Mat44f FURUD_VECTORAPI operator - () const noexcept
		{
			Mat44f m;
			const Vec4f* m1 = reinterpret_cast<const Vec4f*>(&r);

			m.r[0] = -m1[0];
			m.r[1] = -m1[1];
			m.r[2] = -m1[2];
			m.r[3] = -m1[3];
			return m;
		}

		FURUD_INLINE Mat44f FURUD_VECTORAPI operator + (const Mat44f& rhs) const noexcept
		{
			Mat44f m;
			const Vec4f* m1 = reinterpret_cast<const Vec4f*>(&r);
			const Vec4f* m2 = reinterpret_cast<const Vec4f*>(&rhs.r);
			
			m.r[0] = m1[0] + m2[0];
			m.r[1] = m1[1] + m2[1];
			m.r[2] = m1[2] + m2[2];
			m.r[3] = m1[3] + m2[3];
			return m;
		}

		FURUD_INLINE Mat44f FURUD_VECTORAPI operator - (const Mat44f& rhs) const noexcept
		{
			Mat44f m;
			const Vec4f* m1 = reinterpret_cast<const Vec4f*>(&r);
			const Vec4f* m2 = reinterpret_cast<const Vec4f*>(&rhs.r);

			m.r[0] = m1[0] - m2[0];
			m.r[1] = m1[1] - m2[1];
			m.r[2] = m1[2] - m2[2];
			m.r[3] = m1[3] - m2[3];
			return m;
		}

		FURUD_INLINE Mat44f FURUD_VECTORAPI operator * (const float& rhs) const noexcept
		{
			Mat44f m;
			const Vec4f* m1 = reinterpret_cast<const Vec4f*>(&r);
			const Vec4f v2 = rhs;

			m.r[0] = m1[0] * v2;
			m.r[1] = m1[1] * v2;
			m.r[2] = m1[2] * v2;
			m.r[3] = m1[3] * v2;
			return m;
		}

		FURUD_INLINE Mat44f FURUD_VECTORAPI operator / (const float& rhs) const noexcept
		{
			Mat44f m;
			const Vec4f* m1 = reinterpret_cast<const Vec4f*>(&r);
			const Vec4f v2 = rhs;

			m.r[0] = m1[0] / v2;
			m.r[1] = m1[1] / v2;
			m.r[2] = m1[2] / v2;
			m.r[3] = m1[3] / v2;
			return m;
		}


	public:
		//****************************************************************
		// @brief    Basic numerical operations.
		// @details  矩阵乘法。
		//****************************************************************

		FURUD_INLINE Mat44f FURUD_VECTORAPI operator * (const Mat44f& rhs) const noexcept
		{
			const Mat44f& m1 = *this;
			const Mat44f& m2 = rhs;

			Mat44f m;
			Vec4f temp;

			// First row of result (inMatrix1[0] * inMatrix2).
			temp = ReplicateX(m1[0]) * m2[0];
			temp = MultiplyAdd(ReplicateY(m1[0]), m2[1], temp);
			temp = MultiplyAdd(ReplicateZ(m1[0]), m2[2], temp);
			temp = MultiplyAdd(ReplicateW(m1[0]), m2[3], temp);
			m.SetRow0(temp);

			// Second row of result (inMatrix1[1] * inMatrix2).
			temp = ReplicateX(m1[1]) * m2[0];
			temp = MultiplyAdd(ReplicateY(m1[1]), m2[1], temp);
			temp = MultiplyAdd(ReplicateZ(m1[1]), m2[2], temp);
			temp = MultiplyAdd(ReplicateW(m1[1]), m2[3], temp);
			m.SetRow1(temp);

			// Third row of result (inMatrix1[2] * inMatrix2).
			temp = ReplicateX(m1[2]) * m2[0];
			temp = MultiplyAdd(ReplicateY(m1[2]), m2[1], temp);
			temp = MultiplyAdd(ReplicateZ(m1[2]), m2[2], temp);
			temp = MultiplyAdd(ReplicateW(m1[2]), m2[3], temp);
			m.SetRow2(temp);

			// Fourth row of result (inMatrix1[3] * inMatrix2).
			temp = ReplicateX(m1[3]) * m2[0];
			temp = MultiplyAdd(ReplicateY(m1[3]), m2[1], temp);
			temp = MultiplyAdd(ReplicateZ(m1[3]), m2[2], temp);
			temp = MultiplyAdd(ReplicateW(m1[3]), m2[3], temp);
			m.SetRow3(temp);

			// Store result.
			return m;
		}

		FURUD_INLINE const Mat44f& FURUD_VECTORAPI operator *= (const Mat44f& rhs) noexcept
		{
			*this = this->operator*(rhs);
			return *this;
		}

		FURUD_INLINE friend Vec4f FURUD_VECTORAPI operator * (const Vec4f& lhs, const Mat44f& rhs) noexcept
		{
			const Vec4f& v = lhs;
			const Mat44f& m = rhs;

			// Mul by the matrix.
			Vec4f temp = v.ReplicateX() * m[0];
			temp = MultiplyAdd(v.ReplicateY(), m[1], temp);
			temp = MultiplyAdd(v.ReplicateZ(), m[2], temp);
			temp = MultiplyAdd(v.ReplicateW(), m[3], temp);
			return temp;
		}


	public:
		//****************************************************************
		// @brief    Basic matrix operations.
		// @details  矩阵运算。
		//****************************************************************

		FURUD_INLINE Mat44f FURUD_VECTORAPI Transpose() const noexcept
		{
			Mat44f result = *this;
			return result.Transposed();
		}

		FURUD_INLINE Mat44f& Transposed() noexcept
		{
			_MM_TRANSPOSE4_PS(r[0], r[1], r[2], r[3]);
			return *this;
		}

		FURUD_INLINE Mat44f FURUD_VECTORAPI Inverse() const noexcept
		{
			// use block matrix method
			// A is a matrix, then
			//     i(A)    (or iA means)         inverse of A,
			//     a#      (or A_ in code)       means adjugate of A,
			//     |A|     ( or DetA in code)    is determinant,
			//     TR(A)                         is trace
			const Mat44f& m = *this;

			// sub matrices
			const Vec4f a = Shuffle<0, 1, 0, 1>(m[0], m[1]);
			const Vec4f b = Shuffle<2, 3, 2, 3>(m[0], m[1]);
			const Vec4f c = Shuffle<0, 1, 0, 1>(m[2], m[3]);
			const Vec4f d = Shuffle<2, 3, 2, 3>(m[2], m[3]);

			// determinant as (|A| |B| |C| |D|)
			const Vec4f detSub =
				Shuffle<0, 2, 0, 2>(m[0], m[2]) * Shuffle<1, 3, 1, 3>(m[1], m[3]) -
				Shuffle<1, 3, 1, 3>(m[0], m[2]) * Shuffle<0, 2, 0, 2>(m[1], m[3]);


			const Vec4f detA = detSub.ReplicateX();
			const Vec4f detB = detSub.ReplicateY();
			const Vec4f detC = detSub.ReplicateZ();
			const Vec4f detD = detSub.ReplicateW();

			// let iM = 1/|M| * | X  Y |
			//                  | Z  W |

			// D#C
			const Vec4f d_c = Matrix2x2AdjMultiply(d, c);
			// A#B
			const Vec4f a_b = Matrix2x2AdjMultiply(a, b);
			// X# = |D|A - b(D#C)
			Vec4f x_ = detD * a - Matrix2x2Multiply(b, d_c);
			// W# = |A|D - C(A#B)
			Vec4f w_ = detA * d - Matrix2x2Multiply(c, a_b);

			// |M| = |A|*|D| + ... (continue later)
			Vec4f detM = detA * detD;

			// Y# = |B|C - D(A#B)#
			Vec4f y_ = detB * c - Matrix2x2MultiplyAdj(d, a_b);
			// Z# = |C|B - A(D#C)#
			Vec4f z_ = detC * b - Matrix2x2MultiplyAdj(a, d_c);

			// |M| = |A|*|D| + |B|*|C| ... (continue later)
			detM = detM + detB * detC;

			// tr((A#B)(D#C))
			Vec4f tr = a_b * d_c.Swizzle<0, 2, 1, 3>();
			tr = tr + tr.Swizzle<2, 3, 0, 1>();
			tr = tr + tr.Swizzle<1, 2, 3, 0>();
			// |M| = |A|*|D| + |B|*|C| - TR((A#B)(D#C)
			detM = detM - tr;


			if (detM.X() != 0.0f)
			{
				const Vec4f adjSignMask{ 1.f, -1.f, -1.f, 1.f };
				// (1/|M|, -1/|M|, -1/|M|, 1/|M|)
				const Vec4f invDetM = adjSignMask / detM;

				x_ = x_ * invDetM;
				y_ = y_ * invDetM;
				z_ = z_ * invDetM;
				w_ = w_ * invDetM;

				Mat44f result;

				// apply adjugate and store, here we combine adjugate shuffle and store shuffle
				result.SetRow0(Shuffle<3, 1, 3, 1>(x_, y_));
				result.SetRow1(Shuffle<2, 0, 2, 0>(x_, y_));
				result.SetRow2(Shuffle<3, 1, 3, 1>(z_, w_));
				result.SetRow3(Shuffle<2, 0, 2, 0>(z_, w_));
				return result;
			}

			return m;
		}
	};
}