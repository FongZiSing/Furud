//
// Core.Rotator.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
//  Euler angles definition.
//
module;

#include <Furud.hpp>



export module Furud.Core.Rotator;

import Furud.Platform.SIMD;
import Furud.Core.Matrix;
import Furud.Platform.Math;



namespace Furud
{
	/**
	 * @brief  Implements a container for rotation information.
	 *         all rotation values are stored in degrees, range in [-180, 180].
	 *         the rotation order is Y-X-Z
	 * @note
	 *    Y (yaw operation)
	 *             |  / -Z (roll operation)
	 *             | /
	 *             |/______ X (pitch operation)
	 */
	struct Rotator
	{
		// Rotation around the up axis (around Y-Axis).
		float yaw;

		// Rotation around the right axis (around X-Axis).
		float pitch;

		// Rotation around the forward axis (around negative Z-Axis).
		float roll;


	public:
		static const Rotator Zero;


	public:
		Rotator() noexcept : yaw(0.f), pitch(0.f), roll(0.f) {}
		Rotator(const float& sYaw, const float& sPitch, const float& sRoll) noexcept : yaw(sYaw), pitch(sPitch), roll(sRoll) {}
		Rotator(const Vector3f& euler) : yaw(euler.z), pitch(euler.y), roll(euler.x) {}
		
	public:
		Rotator(const Rotator&) = default;
		Rotator& operator = (const Rotator&) = default;
		Rotator(Rotator&&) = default;
		Rotator& operator = (Rotator&&) = default;
		~Rotator() noexcept {}


	public:
		FURUD_INLINE Vector3f Euler() { return { roll, pitch, yaw }; }


	public:
		FURUD_INLINE bool operator == (const Rotator& rhs) { return rhs.yaw == yaw && rhs.pitch == pitch && rhs.roll == roll; }
		FURUD_INLINE bool operator != (const Rotator& rhs) { return rhs.yaw != yaw || rhs.pitch != pitch || rhs.roll != roll; }


	public:
		Rotator operator - () const noexcept { return { -yaw, -pitch, -roll }; }
		
		FURUD_INLINE Rotator operator + (const Rotator& rhs) { return { yaw + rhs.yaw, pitch + rhs.pitch, roll + rhs.roll }; }
		FURUD_INLINE Rotator operator - (const Rotator& rhs) { return { yaw - rhs.yaw, pitch - rhs.pitch, roll - rhs.roll }; }
		FURUD_INLINE Rotator operator * (const Rotator& rhs) { return { yaw * rhs.yaw, pitch * rhs.pitch, roll * rhs.roll }; }
		FURUD_INLINE Rotator operator / (const Rotator& rhs) { return { yaw / rhs.yaw, pitch / rhs.pitch, roll / rhs.roll }; }
		
		FURUD_INLINE Rotator operator + (const Rotator& rhs) const { return { yaw + rhs.yaw, pitch + rhs.pitch, roll + rhs.roll }; }
		FURUD_INLINE Rotator operator - (const Rotator& rhs) const { return { yaw - rhs.yaw, pitch - rhs.pitch, roll - rhs.roll }; }
		FURUD_INLINE Rotator operator * (const Rotator& rhs) const { return { yaw * rhs.yaw, pitch * rhs.pitch, roll * rhs.roll }; }
		FURUD_INLINE Rotator operator / (const Rotator& rhs) const { return { yaw / rhs.yaw, pitch / rhs.pitch, roll / rhs.roll }; }
		
		FURUD_INLINE const Rotator& operator += (const Rotator& rhs) { yaw += rhs.yaw; pitch += rhs.pitch; roll += rhs.roll; return *this; }
		FURUD_INLINE const Rotator& operator -= (const Rotator& rhs) { yaw -= rhs.yaw; pitch -= rhs.pitch; roll -= rhs.roll; return *this; }
		FURUD_INLINE const Rotator& operator *= (const Rotator& rhs) { yaw *= rhs.yaw; pitch *= rhs.pitch; roll *= rhs.roll; return *this; }
		FURUD_INLINE const Rotator& operator /= (const Rotator& rhs) { yaw /= rhs.yaw; pitch /= rhs.pitch; roll /= rhs.roll; return *this; }

		FURUD_INLINE Rotator operator + (const float& rhs) { return { yaw + rhs, pitch + rhs, roll + rhs }; }
		FURUD_INLINE Rotator operator - (const float& rhs) { return { yaw - rhs, pitch - rhs, roll - rhs }; }
		FURUD_INLINE Rotator operator * (const float& rhs) { return { yaw * rhs, pitch * rhs, roll * rhs }; }
		FURUD_INLINE Rotator operator / (const float& rhs) { return { yaw / rhs, pitch / rhs, roll / rhs }; }
		
		FURUD_INLINE Rotator operator + (const float& rhs) const { return { yaw + rhs, pitch + rhs, roll + rhs }; }
		FURUD_INLINE Rotator operator - (const float& rhs) const { return { yaw - rhs, pitch - rhs, roll - rhs }; }
		FURUD_INLINE Rotator operator * (const float& rhs) const { return { yaw * rhs, pitch * rhs, roll * rhs }; }
		FURUD_INLINE Rotator operator / (const float& rhs) const { return { yaw / rhs, pitch / rhs, roll / rhs }; }
		
		FURUD_INLINE const Rotator& operator += (const float& rhs) { yaw += rhs; pitch += rhs; roll += rhs; return *this; }
		FURUD_INLINE const Rotator& operator -= (const float& rhs) { yaw -= rhs; pitch -= rhs; roll -= rhs; return *this; }
		FURUD_INLINE const Rotator& operator *= (const float& rhs) { yaw *= rhs; pitch *= rhs; roll *= rhs; return *this; }
		FURUD_INLINE const Rotator& operator /= (const float& rhs) { yaw /= rhs; pitch /= rhs; roll /= rhs; return *this; }

	public:
		FURUD_NODISCARD FURUD_INLINE Rotator Normalize() const
		{
			Rotator result;
			Vec4f temp { yaw, pitch, roll, 0 };

			// shift in the range [-360,360]
			Vec4f v0 = temp % Vec4f::D360;
			Vec4f v1 = v0 + Vec4f::D360;
			Vec4f v2 = Select(GreaterThan(v0, Vec4f::ZERO), v0, v1);

			// shift to [-180,180]
			Vec4f v3 = v2 - Vec4f::D360;
			Vec4f v4 = Select(GreaterThan(v2, Vec4f::ZERO), v3, v2);
			v4.Store3(&result);

			return  result;
		}

		FURUD_NODISCARD FURUD_INLINE Matrix44f ToMatrix() const
		{
			Vector4f degrees { 0, yaw, pitch, roll };
			Vector4f radians { degrees * IFloat::DEG_TO_RAD };

			Vector4f s; // ( Unused, yaw, pitch, roll )
			Vector4f c; // ( Unused, yaw, pitch, roll )
			SinCos(radians, s, c);

			Matrix44f result;
			result.m[0][0] = c[1] * c[3] + s[1] * s[2] * s[3];
			result.m[1][0] = s[1] * s[2] * c[3] - c[1] * s[3];
			result.m[2][0] = s[1] * c[2];
			result.m[3][0] = 0;

			result.m[0][1] = c[2] * s[3];
			result.m[1][1] = c[2] * c[3];
			result.m[2][1] = -s[2];
			result.m[3][1] = 0;

			result.m[0][2] = c[1] * s[2] * s[3] - s[1] * c[3];
			result.m[1][2] = c[1] * s[2] * c[3] + s[1] * s[3];
			result.m[2][2] = c[1] * c[2];
			result.m[3][2] = 0;

			result.m[0][3] = 0;
			result.m[1][3] = 0;
			result.m[2][3] = 0;
			result.m[3][3] = 1;

			return result;
		}

		FURUD_NODISCARD FURUD_INLINE Matrix44f ToInvMatrix() const
		{
			Vector4f degrees { 0, yaw, pitch, roll };
			Vector4f radians { degrees * IFloat::DEG_TO_RAD };

			Vector4f s; // ( Unused, yaw, pitch, roll )
			Vector4f c; // ( Unused, yaw, pitch, roll )
			SinCos(radians, s, c);

			Matrix44f result;
			result.m[0][0] = c[1] * c[3] + s[1] * s[2] * s[3];
			result.m[1][0] = c[2] * s[3];
			result.m[2][0] = c[1] * s[2] * s[3] - s[1] * c[3];
			result.m[3][0] = 0;

			result.m[0][1] = s[1] * s[2] * c[3] - c[1] * s[3];
			result.m[1][1] = c[2] * c[3];
			result.m[2][1] = c[1] * s[2] * c[3] + s[1] * s[3];
			result.m[3][1] = 0;

			result.m[0][2] = s[1] * c[2];
			result.m[1][2] = -s[2];
			result.m[2][2] = c[1] * c[2];
			result.m[3][2] = 0;

			result.m[0][3] = 0;
			result.m[1][3] = 0;
			result.m[2][3] = 0;
			result.m[3][3] = 1;

			return result;
		}

		FURUD_NODISCARD FURUD_INLINE Vector3f GetForwardVector() const
		{
			// The same as `ToMatrix() * Vector3f( 0, 0, -1 )`.
			Vector4f degrees { 0, yaw, pitch, roll };
			Vector4f radians { degrees * IFloat::DEG_TO_RAD };

			Vector4f s; // ( Unused, yaw, pitch, roll )
			Vector4f c; // ( Unused, yaw, pitch, roll )
			SinCos(radians, s, c);

			Vector3f result;
			result.x = -s[1] * c[2];
			result.y = s[2];
			result.z = -c[1] * c[2];

			return result;
		}

		FURUD_NODISCARD FURUD_INLINE Vector3f GetRightVector() const
		{
			// The same as `ToMatrix() * Vector3f( 1, 0, 0 )`.
			Vector4f degrees { 0, yaw, pitch, roll };
			Vector4f radians { degrees * IFloat::DEG_TO_RAD };

			Vector4f s; // ( Unused, yaw, pitch, roll )
			Vector4f c; // ( Unused, yaw, pitch, roll )
			SinCos(radians, s, c);

			Vector3f result;
			result.x = c[1] * c[3] + s[1] * s[2] * s[3];
			result.y = c[2] * s[3];
			result.z = c[1] * s[2] * s[3] - s[1] * c[3];

			return result;
		}

		FURUD_NODISCARD FURUD_INLINE Vector3f GetUpVector() const
		{
			// The same as `ToMatrix() * Vector3f( 0, 0, -1 )`.
			Vector4f degrees { 0, yaw, pitch, roll };
			Vector4f radians { degrees * IFloat::DEG_TO_RAD };

			Vector4f s; // ( Unused, yaw, pitch, roll )
			Vector4f c; // ( Unused, yaw, pitch, roll )
			SinCos(radians, s, c);

			Vector3f result;
			result.x = s[1] * s[2] * c[3] - c[1] * s[3];
			result.y = c[2] * c[3];
			result.z = c[1] * s[2] * c[3] + s[1] * s[3];

			return result;
		}
	};


	const Rotator Rotator::Zero = { 0.f };
}