#ifndef __OCTREE_H__
#define __OCTREE_H__

/*
	An Octree is a means of sub-dividing a scene into manageable chunks
	to be submitted to the graphics processor. This is just a container
	class that leaves the actual job of subdivision to the map class.
*/

namespace Utils
{
	struct Octet
	{
		float CentralPoint[3];
		Octet* Children;
		bool IsFinalNode;
	};

	class Octree
	{
	public:
		Octree();

		virtual ~Octree();

	private:


	};
}

#endif