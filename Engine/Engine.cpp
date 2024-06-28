#pragma once
#include "Engine.h"



namespace Engine {
	
	Camera camera;

	int Engine::Run() {
		//init Engine comps
		Input::Init();


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
			camera.Update(dt);
			
			std::cout << "X:" << camera.position.x << " Y:" << camera.position.y << std::endl;
			sceneManager.Update(dt);

			//Rendering
			Renderer::ClearScreen();
			sceneManager.Render(Renderer::GetProgramID());
			Renderer::SwapBuffers(Backend::GetWindowPointer());
		}

		return 0;
	}
}