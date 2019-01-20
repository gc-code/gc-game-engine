#ifndef __HEIGHTMAP_H__
#define __HEIGHTMAP_H__

#define HMP_VERSION	1
#define HMP_BASE_DATA_SIZE 2

namespace Renderer
{
	class HeightMap
	{
	public:
		HeightMap();

		virtual ~HeightMap();

		bool LoadMapFromImage(const char* dataFile);

		void SetDefaultMatrial(Material* material) { mDefaultMaterial = material; }

		bool SaveMapAsHMP(const char* fileName);

		bool CacheData();

		void Render();

		void FreeRawImage();

	private:

		Image* mRawImageData;
		Material* mDefaultMaterial;
		MeshGeometry* mMeshData;

		cbPerModel mPerModelAttributes;
	};
}

#endif