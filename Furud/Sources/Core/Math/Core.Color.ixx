//
// Core.Color.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Definitions of color.
//
module;

#include <Furud.hpp>
#include <stdint.h>



export module Furud.Core.Color;

import Furud.Core.Matrix;
import Furud.Core.Math;



/** 32-bit RGBA color. */
export namespace Furud
{
	/**
	 * @brief    The 32-bit RGBA color.
	 * @details  32位颜色值。
	 */
	struct Color
	{
		union
		{
			struct { uint8_t r, g, b, a; };
			uint32_t value;
		};


	public:
		static const Color Transparent;
		static const Color Black;
		static const Color White;
		static const Color Red;
		static const Color Green;
		static const Color Blue;


	public:
		Color() noexcept 
			: b(0), g(0), r(0), a(255)
		{}
		
		Color(uint8_t const& sr, uint8_t const& sg, uint8_t const& sb, uint8_t const& sa = 255) noexcept
			: r(sr), g(sg), b(sb), a(sa)
		{}
		
		
	public:
		bool operator == (const Color& rhs) { return value == rhs.value; }
		bool operator != (const Color& rhs) { return value != rhs.value; }


	public:
		furud_nodiscard furud_inline static Color FromLinearVector(Vector4f const& color)
		{
			Vector4f v = Clamp(color * 255.f, 0.f, 255.f);
			return { uint8_t(v.x), uint8_t(v.y), uint8_t(v.z), uint8_t(v.w) };
		}
		
		furud_nodiscard furud_inline static Color FromLinearVector(Vector3f const& color)
		{
			Vector4f v = Clamp(Vector4f(color) * 255.f, 0.f, 255.f);
			return { uint8_t(v.x), uint8_t(v.y), uint8_t(v.z) };
		}

		furud_nodiscard furud_inline static Color FromVector(Vector4f const& color)
		{
			Vector4f v = Clamp(color, 0.f, 255.f);
			return { uint8_t(v.x), uint8_t(v.y), uint8_t(v.z), uint8_t(v.w) };
		}

		furud_nodiscard furud_inline static Color FromVector(Vector3f const& color)
		{
			Vector4f v = Clamp(Vector4f(color), 0.f, 255.f);
			return { uint8_t(v.x), uint8_t(v.y), uint8_t(v.z) };
		}
		
		furud_nodiscard furud_inline static Color FromClampedVector(Vector4f const& color)
		{
			return { uint8_t(color.x), uint8_t(color.y), uint8_t(color.z), uint8_t(color.w) };
		}
		
		furud_nodiscard furud_inline static Color FromClampedVector(Vector3f const& color)
		{
			return { uint8_t(color.x), uint8_t(color.y), uint8_t(color.z) };
		}
	};
}