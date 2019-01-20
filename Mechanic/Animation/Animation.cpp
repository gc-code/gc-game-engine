#include "Animation.h"

namespace Mechanic
{
	Animation::Animation()
	{
		isUpdateReady = UPDATE_WAITING;
	}

	Animation::~Animation()
	{}

	bool Animation::CheckUpdate()
	{
		if (isUpdateReady == UPDATE_READY)
		{
			return true;
		}

		else if (isUpdateReady == UPDATE_WAITING)
		{
			return false;
		}

		else
		{
			return false;
		}
	}

	void Animation::UpdateAnimation()
	{
		if (CheckUpdate() == true)
		{
			//Code to update animation
		}
		else
		{
			return;
		}
	}
}