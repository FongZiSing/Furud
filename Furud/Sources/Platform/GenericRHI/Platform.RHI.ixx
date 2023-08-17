//
// RHI.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Render Hardware Interface.
//
module;

#include "RHICommon.hpp"

// C++ Standard Library.
#include <stdint.h>
#include <span>


export module Furud.Platform.RHI;
export import Furud.Core.Matrix;
export import Furud.Platform.RHI.Resource;


namespace Furud
{
	struct TrimeshSectionBuffer
	{
		struct Desc
		{
			uint32_t numIndexCount = 0;
			uint32_t startIndexLocation = 0;
			uint32_t baseVertexLocation = 0;
		};

		std::string id { "native" };
		std::vector<Desc> buffer;
	};

	struct TrimeshVertexBuffer
	{
		class Trimesh* parent;
		std::span<Vector3f> view;
		RHIBuffer buffer;
	};

	enum class TrimeshIndexFormat : unsigned char
	{
		R16_UINT,
		R32_UINT,
	};


	struct TrimeshIndexBuffer
	{
		class Trimesh* parent;

		TrimeshIndexFormat indexFormat;

		union
		{
			std::span<uint16_t> view16;
			std::span<uint32_t> view32;
		};
		
		RHIBuffer buffer;
	};

	struct TrimeshLODs
	{
		TrimeshSectionBuffer sections;
		TrimeshVertexBuffer vertices;
		TrimeshIndexBuffer indices;
	};

	struct TrimeshMaterials
	{
		// TODO
	};

	class Trimesh
	{
		TrimeshLODs lods[6];
		TrimeshMaterials materials;
	};
}

namespace Furud::RHI
{
	export void Init();

	export void CreateViewport(void* windowHandle, uint32_t clientWidth, uint32_t clientHeight);

	export void ResizeViewport(uint32_t clientWidth, uint32_t clientHeight);

	export void DrawViewport();

	export void FlushCommandQueue();

	export RHIBufferRef CreateBuffer(const RHIBufferCreateInfo& info);
}