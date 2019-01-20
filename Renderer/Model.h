#ifndef __MODEL_H__
#define __MODEL_H__

namespace Mechanic
{
	class Armature;
}

namespace Renderer
{
	class MeshGeometry;
	class Material;
	class GPUBuffer;

	class Model
	{
	public:

		Model();

		virtual ~Model();

		bool CreateResources(RenderScene* renderScene);

		void RotateWorldX(float angle);
		void RotateWorldY(float angle);

		XMFLOAT4X4 GetWorldMatrix() { return mPerModelAttributes.World; }
		XMFLOAT4X4* GetWorldMatrixPtr() { return &mPerModelAttributes.World; }

		void RenderModel();

		void Free();

	protected:

		std::vector<MeshGeometry*> mMeshGeometryList;
		std::vector<Material*> mMaterialList;
		Mechanic::Armature* mArmature;

		cbPerModel mPerModelAttributes;
		GPUBuffer* mPerModelBuffer;
	};
}

#endif