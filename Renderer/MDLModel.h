#ifndef __MDLMODEL_H__
#define __MDLMODEL_H__

#define MDL_LOADER_VERSION 1

namespace Renderer
{
	class MDLModel : public Model
	{
	public:

		MDLModel();

		virtual ~MDLModel();

		enum MaterialCodes
		{
			MAT_AMBIENT = 0x0A1,
			MAT_DIFFUSE = 0x0A2,
			MAT_SPECULAR = 0x0A3,

			MAT_DIFFUSE_MAP = 0x0A4,
			MAT_SPECULAR_MAP = 0x0A5,
			MAT_BUMP_MAP = 0x0A6,

			MAT_END = 0x0C1,
		};

		bool Load(const char* filePath);

		void Free();

	private:

		/* Header data */
		unsigned char mFileVersion;
		unsigned char mNumMaterials;
		unsigned char mNumMeshes;
	};
}

#endif