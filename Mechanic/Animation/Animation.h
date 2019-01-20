#ifndef __ANIMATION_H__
#define __ANIMATION_H__

namespace Mechanic
{
	class Animation
	{
	public:

		Animation();

		virtual ~Animation();

		enum AnimFlags
		{
			UPDATE_WAITING,
			UPDATE_READY
		};

		void UpdateAnimation();

	private:

		bool CheckUpdate();

		unsigned int mFPS;

		int isUpdateReady;
	};
}

#endif