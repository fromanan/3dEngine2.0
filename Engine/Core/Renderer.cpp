#pragma once
#include "Renderer.h"


Texture::Texture(const char* name, const char* path) {
	name = name;
	std::cout << "Loading Texture " << path << std::endl;
	texture = loader::loadDDS(path);
	int texturenumTemp = CurrentTextureNumber++;
	textureNumber = texturenumTemp - GL_TEXTURE0;
	std::cout << "TextureNumbers loaded at " << textureNumber << std::endl;

}


const char* Texture::GetName() {
	return name;
}
int Texture::GetTextureNumber() {
	return textureNumber;
}
GLuint Texture::GetTexture() {
	return texture;
}

int Texture::CurrentTextureNumber = GL_TEXTURE1;

namespace Renderer {
	GLuint programID;
	int Renderer::init(const char* vertex, const char* fragment) {
		programID = LoadShaders::LoadShaders(vertex, fragment);
		return 0;
	}
	int Renderer::LoadShader(const char* vertex, const char* fragment) {
		programID = LoadShaders::LoadShaders(vertex, fragment);
		return 0;
	}
	void Renderer::SwapBuffers(GLFWwindow* window) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	GLuint Renderer::GetProgramID() {
		return programID;
	}
}

