//
// RHICommon.cpp
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @author FongZiSing
//
// Common headers.
//
#pragma once

// Windows Header.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

// DirectX 12 header.
#include <d3d12.h>
#include <dxgi1_4.h>
#include "../../Editor/MainWindow/DirectXHeaders/d3dx12.h"

// DirectX math headers.
#include <DirectXColors.h>

// Smart Pointer Header.
#include <wrl/client.h>

// C++ Standard Library.
#include <stdint.h>
#include <format>
#include <cassert>
#include <vector>

// Intrin
#include <intrin.h>

// Furud headers.
#include <Furud.hpp>