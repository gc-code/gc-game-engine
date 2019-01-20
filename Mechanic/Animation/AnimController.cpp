#include "AnimController.h"

namespace Mechanic
{
	AnimationController::AnimationController()
	{
		mAnimTimer = new Utils::Timer();
		mAnimationList.clear();
		totalanimations = 0;
		numofanimations = 0;
	}

	AnimationController::~AnimationController()
	{
		delete mAnimTimer;
		mAnimationList.clear();
		totalanimations = 0;
		numofanimations = 0;
	}

	void AnimationController::AddAnimation(Animation* animation)
	{
		mAnimationList.push_back(animation);
		totalanimations = mAnimationList.size();
	}

	void AnimationController::UpdateAnimations()
	{
		for (int i = 0; i < totalanimations; i++)
		{
			Update(mAnimationList[i]);
		}
	}

	void AnimationController::ClearAnimations()
	{
		mAnimationList.clear();
	}

	int AnimationController::NumofAnimations()
	{
		numofanimations = mAnimationList.size();
		return numofanimations;
	}

	void AnimationController::Update(Animation* anim)
	{
		anim->UpdateAnimation();
	}
}