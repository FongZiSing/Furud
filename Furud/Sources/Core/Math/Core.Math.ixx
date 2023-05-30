//
// Core.Math.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Common Math Library.
//
module;

#include <Furud.hpp>



export module Furud.Core.Math;

export namespace Furud::IMath
{
	/**
		* @brief    Clamps `inVal` to be between `inMin` and `inMax`, inclusive.
		* @details  钳制。
		*/
	template <typename T>
	furud_nodiscard constexpr decltype(auto) Clamp
		( const T& inVal
		, const T& inMin
		, const T& inMax
	) noexcept
	{
		return (inVal < inMin) ? inMin : (inVal < inMax ? inVal : inMax);
	}

	/**
		* @brief    Barycentric Interpolation.
		* @details  重心坐标插值。
		*/
	template <typename T>
	furud_nodiscard constexpr decltype(auto) Interpolate
		( const float& alpha
		, const float& beta
		, const float& gamma
		, const T& attribute1
		, const T& attribute2
		, const T& attribute3
	) noexcept
	{
		return attribute1 * alpha + attribute2 * beta + attribute3 * gamma;
	}

	/**
		* @brief    Interpolation.
		* @details  插值。
		*/
	template <typename T>
	furud_nodiscard constexpr decltype(auto) Lerp
		( const float& alpha
		, const T& attribute1
		, const T& attribute2
	) noexcept
	{
		return attribute1 + (attribute2 - attribute1) * alpha;
	}
}