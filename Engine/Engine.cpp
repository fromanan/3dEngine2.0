#pragma once
#include "Engine.h"



namespace Engine {
	
	int Engine::Run() {
		Scene basicScene = Scene();
		SceneManager sceneManager = SceneManager();

		sceneManager.CreateScene(basicScene);
		sceneManager.LoadScene(1);

		// For speed computation
		double lastTime = glfwGetTime();
		double lastTimeDT = glfwGetTime();

		while (Backend::IsWindowOpen()) {
			double currentTime = glfwGetTime();

			float dt = currentTime - lastTimeDT;
			lastTimeDT = currentTime;

			sceneManager.Update(dt);
			sceneManager.Render(Renderer::GetProgramID());

			//draw to screen
			Renderer::SwapBuffers(Backend::GetWindowPointer());
		}

		return 0;
	}
}