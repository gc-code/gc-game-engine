#include "Renderer.h"

namespace Renderer
{
	RenderSystem* RenderManager::mRenderSystem = NULL;
	ShaderManager* RenderManager::mShaderManager = NULL;

	RenderManager::RenderManager()
	{
		mRenderSystem = new RenderSystem();
		mShaderManager = new ShaderManager();
	}

	RenderManager::~RenderManager()
	{
		delete mShaderManager;
		delete mRenderSystem;
	}
}