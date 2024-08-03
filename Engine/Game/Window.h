#pragma once

class Texture;

class Window
{
public:
	Window(const std::string& name, const char* frame_path, Texture* frame_texture, const char* window_path, Texture* window_texture, glm::vec3 position, glm::vec3 rotation);
	void Render(GLuint programID, const glm::mat4& View, const glm::mat4& projection) const;
private:
	GameObject window;
	std::string name;
};
