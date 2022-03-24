// SPDX-FileCopyrightText: 2021 Jorrit Rouwe
// SPDX-License-Identifier: MIT

#pragma once

namespace JPH {

/// How collision detection functions will treat back facing triangles
enum class EBackFaceMode : uint8
{
	IgnoreBackFaces,									///< Ignore collision with back facing surfaces/triangles
	CollideWithBackFaces,								///< Collide with back facing surfaces/triangles
};

} // JPH