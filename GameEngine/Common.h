#ifndef __COMMON_H__
#define __COMMON_H__

class EngineCommon
{
public:

	EngineCommon();

	virtual ~EngineCommon();

	bool Init(int nCmdShow);	// Initialize all system resources

	void RenderFrame();	// Run the per-frame operations

	void ProcessEvents();	// Process key input events

	void Free();	// Free all the game resources

private:

	Renderer::RenderManager* mRenderManager;
	Renderer::RenderSystem* mRenderSystem;
	Renderer::MDLModel* mTestModel;
	Renderer::RenderScene* mScene;
	Renderer::HeightMap* mWorldEditorMap;
	Renderer::GUIHandler* mGUIHandler;
	
	System::Keyboard* mKeyboard;
};

#endif