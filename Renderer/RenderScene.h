#ifndef __RENDERSCENE_H__
#define __RENDERSCENE_H__

namespace Renderer
{
	class Model;
	class GPUBuffer;

	class RenderScene
	{
	public:

		RenderScene();

		virtual ~RenderScene();

		void AddModel(Model* model) { mModelsList.push_back(model); }

		void SetWorldMap(HeightMap* heightMap) { mWorldMap = heightMap; }

		void SetViewMatrix(XMFLOAT4X4 *viewMat) { memcpy(&mPerLevelAttributes.View, viewMat, sizeof(XMFLOAT4X4)); }
		void SetProjectionMatrix(XMFLOAT4X4 projMat)  { mPerLevelAttributes.Projection = projMat; }

		void SetProjectionMatrixPerspective();
		void SetViewMatrixLookAt();

		XMFLOAT4X4 GetViewMatrix() { return mPerLevelAttributes.View; }
		XMFLOAT4X4 GetProjectionMatrix() { return mPerLevelAttributes.Projection; }

		bool CacheData();

		void Render();

	private:

		std::vector<Model*> mModelsList;
		HeightMap* mWorldMap;

		cbPerLevel mPerLevelAttributes;
		GPUBuffer* mPerLevelBuffer;
	};
}

#endif