#include "Texture.h"

#include <iostream>

#include "AssetPaths.h"
#include "Loaders/stb_image.h"

Texture::Texture(const char* name, const char* path) {
	this->name = name;
	int texturenumTemp = CurrentTextureNumber++;
	textureNumber = texturenumTemp - GL_TEXTURE0;

	std::cout << "TextureNumbers loaded at " << textureNumber << std::endl;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	// Set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// Load and generate the texture
	int width, height;
	unsigned char* data = stbi_load(path, &width, &height, 0, STBI_rgb_alpha);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	
	stbi_image_free(data);

	textureNormalNumber = CurrentTextureNumber++ - GL_TEXTURE0;

	std::cout << "Texture Numbers Normal loaded at " << textureNormalNumber << std::endl;

	glGenTextures(1, &textureNormal);
	glBindTexture(GL_TEXTURE_2D, textureNormal);
	
	// Set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// Load and generate the texture
	int width1, height1;
	unsigned char* data1 = stbi_load(AssetPaths::Normal_None, &width1, &height1, 0, STBI_rgb_alpha);
	if (data1) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	
	stbi_image_free(data1);
}

Texture::Texture(const char* name, const char* path, const char* normalPath) {
	this->name = name;
	int texturenumTemp = CurrentTextureNumber++;
	textureNumber = texturenumTemp - GL_TEXTURE0;

	std::cout << "TextureNumbers loaded at " << textureNumber << std::endl;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	// Set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// Load and generate the texture
	int width, height;
	unsigned char* data = stbi_load(path, &width, &height, 0, STBI_rgb_alpha);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	textureNormalNumber = CurrentTextureNumber++ - GL_TEXTURE0;

	std::cout << "Texture Numbers Normal loaded at " << textureNormalNumber << std::endl;

	glGenTextures(1, &textureNormal);
	glBindTexture(GL_TEXTURE_2D, textureNormal);
	
	// Set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// Load and generate the texture
	int width1, height1;

	unsigned char* data1 = stbi_load(normalPath, &width1, &height1, 0, STBI_rgb_alpha);
	if (data1) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	
	stbi_image_free(data1);
}

int Texture::GetTextureNormalNumber() {
	return textureNormalNumber;
}

GLuint Texture::GetTextureNormal() {
	return textureNormal;
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