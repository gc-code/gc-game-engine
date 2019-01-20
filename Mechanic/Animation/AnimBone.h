#ifndef __ANIMFRAME_H__
#define __ANIMFRAME_H__

#include <Windows.h>
#include <xnamath.h>
#include <string>
#include <vector>

namespace Mechanic
{
	class AnimBone
	{
	public:

		AnimBone();

		virtual ~AnimBone();

		void SetFrameRotation(XMFLOAT4 frameRot) { mBoneRotation = frameRot; }
		void SetFramePosition(XMFLOAT3 framePos) { mBonePosition = framePos; }
		void SetParentIndex(unsigned int parent) { mParentIndex = parent; }
		void SetName(std::string name) { mBoneName = name; }

		XMFLOAT4 GetFrameRotation() { return mBoneRotation; }
		XMFLOAT3 GetFramePosition() { return mBonePosition; }

	private:

		std::string mBoneName;
		XMFLOAT4 mBoneRotation;
		XMFLOAT3 mBonePosition;

		unsigned int mParentIndex;
	};
}

#endif