#pragma once
#include "Renderer.h"


Texture::Texture(const char* name, const char* path) {
	this->name = name;
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
