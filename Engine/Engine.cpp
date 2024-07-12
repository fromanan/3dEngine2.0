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
		double lastTimeDT = glfwGetTime();
		double previousTime = glfwGetTime();
		int frameCount = 0;
		int FPS = 0;

		while (Backend::IsWindowOpen()) {
			
			//Delta time stuff
			double currentTime = glfwGetTime();
			float dt = currentTime - lastTimeDT;
			lastTimeDT = currentTime;
			frameCount++;
			if (currentTime - previousTime >= 1.0)
			{
				FPS = frameCount;
				frameCount = 0;
				previousTime = currentTime;
			}


			
			//Update Managers
			float test = glfwGetTime();
			Input::Update();
			Input::CenterMouse();
			PhysicsManager::Update(dt);
			sceneManager.Update(dt);
			//AnimationManager::Update(dt);
			Camera::Update(dt);
			//Rendering
			Renderer::ClearScreen();
			sceneManager.Render();
			std::ostringstream oss;
			oss << "FPS: " << FPS;
			Renderer::RenderText(oss.str().c_str(), 660, 585, 15);
			Renderer::SwapBuffers(Backend::GetWindowPointer());

			//AssetManager::CleanUp();
		}

		return 0;
	}
}


