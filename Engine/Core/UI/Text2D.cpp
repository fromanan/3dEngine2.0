#include "pch.h"

#include "Text2D.h"

#include "AssetPaths.h"
#include "Loaders/Loader.hpp"
#include "Loaders/ShaderLoader.hpp"
#include "Engine/Core/Renderer.h"

namespace Text2D
{
	unsigned int Text2DTextureID;
	unsigned int Text2DVertexBufferID;
	unsigned int Text2DUVBufferID;
	unsigned int Text2DShaderID;
	unsigned int Text2DUniformID;

	GLuint GetProgramID() {
		return Text2DShaderID;
	}

	void Text2D::initText2D(const char* texturePath) {
		// Initialize texture
		Text2DTextureID = Loader::loadDDS(texturePath);
		
		// Initialize VBO
		glGenBuffers(1, &Text2DVertexBufferID);
		glGenBuffers(1, &Text2DUVBufferID);

		// Initialize Shader
		Text2DShaderID = ShaderLoader::LoadShaders(AssetPaths::Shader_Text_Vert, AssetPaths::Shader_Text_Frag);
		Renderer::UseProgram(Text2DShaderID);
		
		// Initialize uniforms' IDs
		Text2DUniformID = glGetUniformLocation(Text2DShaderID, "textShader");
	}

	void Text2D::printText2D(const char* text, const int x, const int y, const int size) {
		const size_t length = strlen(text);
		glEnable(GL_BLEND);
		
		// Fill buffers
		std::vector<glm::vec2> vertices;
		std::vector<glm::vec2> UVs;
		
		for (size_t i = 0; i < length; i++) {
			glm::vec2 vertex_up_left = glm::vec2(x + i * size, y + size);
			glm::vec2 vertex_up_right = glm::vec2(x + i * size + size, y + size);
			glm::vec2 vertex_down_right = glm::vec2(x + i * size + size, y);
			glm::vec2 vertex_down_left = glm::vec2(x + i * size, y);

			vertices.push_back(vertex_up_left);
			vertices.push_back(vertex_down_left);
			vertices.push_back(vertex_up_right);

			vertices.push_back(vertex_down_right);
			vertices.push_back(vertex_up_right);
			vertices.push_back(vertex_down_left);

			const char character = text[i];
			const float uv_x = static_cast<float>(character % 16) / 16.0f;
			const float uv_y = static_cast<float>(character / 16) / 16.0f;

			glm::vec2 uv_up_left = glm::vec2(uv_x, uv_y);
			glm::vec2 uv_up_right = glm::vec2(uv_x + 1.0f / 16.0f, uv_y);
			glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f / 16.0f, (uv_y + 1.0f / 16.0f));
			glm::vec2 uv_down_left = glm::vec2(uv_x, (uv_y + 1.0f / 16.0f));
			UVs.push_back(uv_up_left);
			UVs.push_back(uv_down_left);
			UVs.push_back(uv_up_right);

			UVs.push_back(uv_down_right);
			UVs.push_back(uv_up_right);
			UVs.push_back(uv_down_left);
		}
		
		glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size()) * sizeof(glm::vec2), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(UVs.size()) * sizeof(glm::vec2), UVs.data(), GL_STATIC_DRAW);

		// Bind shader
		glUseProgram(Text2DShaderID);

		// Bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
		
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(Text2DUniformID, 0);

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		//glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));

		glDisable(GL_BLEND);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}

	void Text2D::cleanupText2D() {
		// Delete buffers
		glDeleteBuffers(1, &Text2DVertexBufferID);
		glDeleteBuffers(1, &Text2DUVBufferID);

		// Delete texture
		glDeleteTextures(1, &Text2DTextureID);

		// Delete shader
		glDeleteProgram(Text2DShaderID);
	}
}
