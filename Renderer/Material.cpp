#include "Renderer.h"

namespace Renderer
{
	Material::Material()
		: mDiffuseTexture(0)
		, mSpecularTexture(0)
		, mBumpTexture(0)
	{}

	Material::~Material()
	{
		Free();
	}

	bool Material::CreateTextures()
	{
		RenderSystem* renderSys = RenderManager::GetRenderSystem();

		if (!mDiffuseMapPath.empty())
		{
			mDiffuseTexture = renderSys->CreateTexture2D(mDiffuseMapPath.c_str());
			if (!mDiffuseTexture)
				return false;
		}
		if (!mSpecularMapPath.empty())
		{
			mSpecularTexture = renderSys->CreateTexture2D(mSpecularMapPath.c_str());
			if (!mSpecularTexture)
				return false;
		}
		if (!mBumpMapPath.empty())
		{
			mBumpTexture = renderSys->CreateTexture2D(mBumpMapPath.c_str());
			if (!mBumpTexture)
				return false;
		}

		return true;
	}

	void Material::BindTextures()
	{
		RenderSystem* renderSys = RenderManager::GetRenderSystem();
		if (mDiffuseTexture) renderSys->BindTexture2D(mDiffuseTexture, 0);
	}

	void Material::Free()
	{
		if (mDiffuseTexture) delete mDiffuseTexture;
		if (mSpecularTexture) delete mSpecularTexture;
		if (mBumpTexture) delete mBumpTexture;
	}
}