#pragma once
#include "Engine.h"



namespace Engine {
	
	int Engine::Run() {
		//init Engine comps
		Input::Init();
		Input::HideCursor();
		Text2D::initText2D("Assets/Fonts/Holstein.DDS");


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
			//std::cout << "FPS: " << (1 / dt) << "/" << dt << std::endl;

			
			//Update Managers
			Input::Update();
			Input::CenterMouse();
			PhysicsManager::Update(dt);
			sceneManager.Update(dt);
			Camera::Update(dt);
			//Rendering
			Renderer::ClearScreen();
			sceneManager.Render();
			Renderer::SwapBuffers(Backend::GetWindowPointer());
		}

		return 0;
	}
}


