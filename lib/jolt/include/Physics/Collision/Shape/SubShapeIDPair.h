// SPDX-FileCopyrightText: 2021 Jorrit Rouwe
// SPDX-License-Identifier: MIT

#pragma once

#include <Physics/Body/BodyID.h>
#include <Physics/Collision/Shape/SubShapeID.h>
#include <Core/HashCombine.h>

namespace JPH {

/// A pair of bodies and their sub shape ID's. Can be used as a key in a map to find a contact point.
class SubShapeIDPair
{
public:
	/// Constructor
							SubShapeIDPair() = default;
							SubShapeIDPair(const BodyID &inBody1ID, const SubShapeID &inSubShapeID1, const BodyID &inBody2ID, const SubShapeID &inSubShapeID2) : mBody1ID(inBody1ID), mSubShapeID1(inSubShapeID1), mBody2ID(inBody2ID), mSubShapeID2(inSubShapeID2) { }
							SubShapeIDPair(const SubShapeIDPair &) = default;

	/// Equality operator
	inline bool				operator == (const SubShapeIDPair &inRHS) const		
	{ 
		return UVec4::sLoadInt4(reinterpret_cast<const uint32 *>(this)) == UVec4::sLoadInt4(reinterpret_cast<const uint32 *>(&inRHS));
	}

	/// Less than operator, used to consistently order contact points for a deterministic simulation
	inline bool				operator < (const SubShapeIDPair &inRHS) const
	{
		if (mBody1ID != inRHS.mBody1ID)
			return mBody1ID < inRHS.mBody1ID;

		if (mSubShapeID1.GetValue() != inRHS.mSubShapeID1.GetValue())
			return mSubShapeID1.GetValue() < inRHS.mSubShapeID1.GetValue();

		if (mBody2ID != inRHS.mBody2ID)
			return mBody2ID < inRHS.mBody2ID;

		return mSubShapeID2.GetValue() < inRHS.mSubShapeID2.GetValue();
	}

	const BodyID &			GetBody1ID() const				{ return mBody1ID; }
	const SubShapeID &		GetSubShapeID1() const			{ return mSubShapeID1; }
	const BodyID &			GetBody2ID() const				{ return mBody2ID; }
	const SubShapeID &		GetSubShapeID2() const			{ return mSubShapeID2; }

private:
	BodyID					mBody1ID;
	SubShapeID				mSubShapeID1;
	BodyID					mBody2ID;
	SubShapeID				mSubShapeID2;
};

static_assert(sizeof(SubShapeIDPair) == 16, "Unexpected size"); 
static_assert(alignof(SubShapeIDPair) == 4, "Assuming 4 byte aligned");

} // JPH

JPH_MAKE_HASHABLE(JPH::SubShapeIDPair, t.GetBody1ID().GetIndexAndSequenceNumber(), t.GetSubShapeID1().GetValue(), t.GetBody2ID().GetIndexAndSequenceNumber(), t.GetSubShapeID2().GetValue())
