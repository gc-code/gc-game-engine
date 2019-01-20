#ifndef _ANIMCONTROLLER_H
#define _ANIMCONTROLLER_H

#include "Timer.h"
#include "Animation.h"
#include <vector>

namespace Mechanic
{

	class AnimationController
	{
	public:
		AnimationController();

		~AnimationController();

		void AddAnimation(Animation* animation);

		void UpdateAnimations();

		void ClearAnimations();

		int NumofAnimations();

		int numofanimations;

	private:
		void Update(Animation* anim);

		Utils::Timer* mAnimTimer;

		std::vector<Animation*>mAnimationList;

		int totalanimations;
	};

}

#endif