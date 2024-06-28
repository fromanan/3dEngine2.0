#pragma once
#include "Engine.h"



namespace Engine {
	
	int Engine::Run() {
		//init Engine comps
		Input::Init();
		Input::HideCursor();


		Scene basicScene = Scene();
		SceneManager sceneManager = SceneManager();

		sceneManager.CreateScene(basicScene);
		sceneManager.LoadScene(1);

		// For speed computation
		double lastTime = glfwGetTime();
		double lastTimeDT = glfwGetTime();

		while (Backend::IsWindowOpen()) {
			//Delta time stuff
			double currentTime = glfwGetTime();
			float dt = currentTime - lastTimeDT;
			lastTimeDT = currentTime;
			
			//Update Managers
			Input::Update();
			Input::CenterMouse();
			Camera::Update(dt);
			
			sceneManager.Update(dt);

			//Rendering
			Renderer::ClearScreen();
			sceneManager.Render(Renderer::GetProgramID());
			Renderer::SwapBuffers(Backend::GetWindowPointer());
		}

		return 0;
	}
}