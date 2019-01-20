#ifndef __MATERIAL_H__
#define __MATERIAL_H__

namespace Renderer
{
	class Texture2D;

	class Material
	{
	public:

		Material();

		virtual ~Material();

		bool CreateTextures();

		void BindTextures();

		void SetMaterialName(std::string matName) { mMaterialName = matName; }

		std::string GetMaterialName() { return mMaterialName; }

		void SetDiffuseMapPath(std::string diffuseMap) { mDiffuseMapPath = diffuseMap; }
		void SetSpecularMapPath(std::string specMap) { mSpecularMapPath = specMap; }
		void SetBumpMapPath(std::string bumpMap) { mBumpMapPath = bumpMap; }

		void SetAmbientColour(XMFLOAT3 ambient) { mAmbient = ambient; }
		void SetSpecularColour(XMFLOAT3 specular) { mSpecular = specular; }
		void SetDiffuseColour(XMFLOAT3 diffuse) { mDiffuse = diffuse; }

		XMFLOAT3 GetAmbientColour() { return mAmbient; }
		XMFLOAT3 GetSpecularColour() { return mSpecular; }
		XMFLOAT3 GetDiffuseColour() { return mDiffuse; }

		void Free();

	private:

		std::string mMaterialName;

		std::string mDiffuseMapPath;
		std::string mSpecularMapPath;
		std::string mBumpMapPath;

		XMFLOAT3 mAmbient;
		XMFLOAT3 mSpecular;
		XMFLOAT3 mDiffuse;

		Texture2D* mDiffuseTexture;
		Texture2D* mSpecularTexture;
		Texture2D* mBumpTexture;
	};
}

#endif