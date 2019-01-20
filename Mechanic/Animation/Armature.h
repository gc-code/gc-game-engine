#ifndef __ARMATURE_H__
#define __ARMATURE_H__

#include "AnimBone.h"

namespace Mechanic
{
	class Armature
	{
	public:

		Armature();

		virtual ~Armature();

		std::vector<AnimBone*>& GetAnimationFrames() { return mAnimationFrames; }

	private:

		std::vector<AnimBone*> mAnimationFrames;
	};
}

#endif