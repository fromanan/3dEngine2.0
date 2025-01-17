#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine/Core/GameObject.h"
#include "Engine/Core/AssetManager.h"


#include <vector>
#include <string>

//This is still in working progress and hasnt been implemented into the engine

struct KeyFrame {
	float duration;
	glm::vec3 position;
	glm::vec3 rotation;

	void Create(glm::vec3 Position, glm::vec3 Rotation, float Duration);
};

class Animation {
public:
	Animation(std::string Name);
	Animation(std::vector<KeyFrame> Keyframes, std::string Name);

	void AddKeyFrame(KeyFrame Keyframe);
	int GetKeyFrameSize();
	bool Playing();
	void Stop();
	void Start();
	void Pause();
	void SetKeyFrame(int keyframeIndex);
	void Transform();
	std::string GetName();
	void SetGameObject(GameObject* gameobject);

private:
	void NextKeyFrame();
	std::vector<KeyFrame> keyframes;

	float timeStart = 0;
	int currentKeyFrame = -1;
	bool playing = false;

	glm::vec3 startingPosition;
	glm::vec3 startingRotation;

	std::string name;

	GameObject* gameObject;

};

namespace AnimationManager {
	void AddAnimation(Animation animation);
	void Play(std::string Name, std::string ObjectName);
	void Stop(std::string Name);
	void Pause(std::string Name);
	void Update(float deltaTime);

	Animation* GetAnimation(std::string Name);



};

