#pragma once

class Door
{
public:
	Door(const std::string& Name, const char* doorPath, const char* framePath, Texture* door_texture, Texture* frameTexture, glm::vec3 position);
	void Interact();
	void Update(float deltaTime);

private:
	std::string name;
	bool opened;
	bool opening;
	glm::vec3 door_position;
	float rotation = 0.0f;
	float openingSpeed = 0.02f;
};
