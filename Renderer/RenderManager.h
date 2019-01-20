#ifndef __RENDERMANAGER_H__
#define __RENDERMANAGER_H__

namespace Renderer
{
	class RenderManager
	{
	public:

		RenderManager();

		virtual ~RenderManager();

		static RenderSystem* GetRenderSystem() { return mRenderSystem; }
		static ShaderManager* GetShaderManager() { return mShaderManager; }

	private:

		static RenderSystem* mRenderSystem;
		static ShaderManager* mShaderManager;
	};
}

#endif