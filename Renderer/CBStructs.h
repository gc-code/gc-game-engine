#ifndef __CBSTRUCTS_H__
#define __CBSTRUCTS_H__

/*
	Data structures for the different constant data
	scenarios
*/

namespace Renderer
{
	struct cbPerLevel
	{
		XMFLOAT4X4 View;
		XMFLOAT4X4 Projection;
	};

	struct cbPerModel
	{
		XMFLOAT4X4 World;
	};

	struct cbMaterialAttributes
	{
		XMFLOAT3 Ambient;
		XMFLOAT3 Diffuse;
		XMFLOAT3 Specular;
	};
}

#endif