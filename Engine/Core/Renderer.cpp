#include "Renderer.h"

#include "AssetPaths.h"
#include "Scene/SceneManager.h"

namespace Renderer
{
	std::map<const char*, GLuint> shaderProgramIds;

	GLuint LightID;
	GLuint MatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;
	GLuint ModelView3x3MatrixID;
	GLuint gPosition;
	
	GLuint ubo;

	unsigned int quadVAO;
	unsigned int VBO;

	unsigned int depthMapFBO;
	unsigned int depthMap;

	GLuint Renderer::GetProgramID(const char* name) {
		return shaderProgramIds[name];
	}

	void Renderer::SetTextureShader(glm::mat4 mvp, glm::mat4 model, glm::mat4 view, glm::mat3 ModelView3x3Matrix) {
		setMat4(MatrixID, mvp);
		setMat4(ModelMatrixID, model);
		setMat4(ViewMatrixID, view);
		glUniformMatrix3fv(ModelView3x3MatrixID, 1, GL_FALSE, &ModelView3x3Matrix[0][0]);
	}
	
	void Renderer::SetLights(const std::vector<Light>& lights) {
		// Upload lights data to the GPU
		std::vector<glm::vec3> lightPositions;
		std::vector<glm::vec3> lightColors;
		std::vector<float> LightConstants;
		std::vector<float> LightLinears;
		std::vector<float> LightQuadratics;

		for (const auto& light : lights) {
			lightPositions.push_back(light.position);
			lightColors.push_back(light.colour);
			LightConstants.push_back(light.constant);
			LightLinears.push_back(light.linear);
			LightQuadratics.push_back(light.quadratic);
		}

		// Set up uniform arrays in the shader
		GLuint lightPositionsLoc = glGetUniformLocation(GetProgramID("Texture"), "LightPositions_worldspace");
		GLuint lightColorsLoc = glGetUniformLocation(GetProgramID("Texture"), "LightColors");
		GLuint LightConstantsLoc = glGetUniformLocation(GetProgramID("Texture"), "LightConstants");
		GLuint LightLinearsLoc = glGetUniformLocation(GetProgramID("Texture"), "LightLinears");
		GLuint LightQuadraticsLoc = glGetUniformLocation(GetProgramID("Texture"), "LightQuadratics");


		glUniform3fv(lightPositionsLoc, static_cast<GLsizei>(lights.size()), glm::value_ptr(lightPositions[0]));
		glUniform3fv(lightColorsLoc, static_cast<GLsizei>(lights.size()), glm::value_ptr(lightColors[0]));
		glUniform1fv(LightConstantsLoc, static_cast<GLsizei>(lights.size()), LightConstants.data());
		glUniform1fv(LightLinearsLoc, static_cast<GLsizei>(lights.size()), LightLinears.data());
		glUniform1fv(LightQuadraticsLoc, static_cast<GLsizei>(lights.size()), LightQuadratics.data());

		//setVec3(LightID, lights[0].position);
	}

	int Renderer::init(const char* vertex, const char* fragment, const char* name) {
		UseProgram(LoadShader(vertex, fragment, name));
		std::cout << "Loaded texture shader at: " << GetProgramID("Texture") << std::endl;

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		MatrixID = glGetUniformLocation(Renderer::GetCurrentProgramID(), "MVP");
		ViewMatrixID = glGetUniformLocation(Renderer::GetCurrentProgramID(), "V");
		ModelMatrixID = glGetUniformLocation(Renderer::GetCurrentProgramID(), "M");
		LightID = glGetUniformLocation(Renderer::GetCurrentProgramID(), "LightPosition_worldspace");
		ModelView3x3MatrixID = glGetUniformLocation(Renderer::GetCurrentProgramID(), "MV3x3");

		// Skybox
		LoadShader(AssetPaths::Shader_Skybox_Vert, AssetPaths::Shader_Skybox_Frag, "skybox");
		std::cout << "Loaded skybox shader at: " << GetProgramID("skybox") << std::endl;
		LoadShader(AssetPaths::Shader_Sprite_Vert, AssetPaths::Shader_Sprite_Frag, "sprite");
		std::cout << "Loaded sprite shader at: " << GetProgramID("sprite") << std::endl;

		UseProgram(GetProgramID("sprite"));
		
		// Configure VAO/VBO
		const float vertices[] = {
			// pos      // tex
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};

		glGenVertexArrays(1, &Renderer::quadVAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindVertexArray(Renderer::quadVAO);

		UseProgram(GetProgramID(name));

		return 0;
	}

	void Renderer::setMat4(const GLuint id, glm::mat4& mat4) {
		glUniformMatrix4fv(id, 1, GL_FALSE, &mat4[0][0]);
	}
	
	void Renderer::setVec3(const GLuint id, const glm::vec3& vec3) {
		glUniform3f(id, vec3.x, vec3.y, vec3.z);
	}
	
	void Renderer::ClearScreen() {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::UseProgram(const GLuint ProgramID) {
		glUseProgram(ProgramID);
	}
	
	int Renderer::LoadShader(const char* vertex, const char* fragment, const char* name) {
		shaderProgramIds[name] = LoadShaders::LoadShaders(vertex, fragment); 
		return shaderProgramIds[name];
	} 

	void Renderer::RenderText(const char* text, const int x, const int y, const int size) {
		Renderer::UseProgram(Text2D::GetProgramID());
		Text2D::printText2D(text, x, y, size);
	}

	// TODO: Doesn't work right now still in progress
	void Renderer::DrawSprite(Texture* texture, const glm::vec2 position, const glm::vec2 size, const float rotate, const glm::vec3 color) {
		UseProgram(GetProgramID("sprite"));

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribPointer(VBO, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position, 0.0f));

		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
		model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

		model = glm::scale(model, glm::vec3(size, 1.0f));

		setVec3(glGetUniformLocation(GetProgramID("sprite"), "spriteColor"),color);
		setMat4(glGetUniformLocation(GetProgramID("sprite"), "model"),model);
		
		glActiveTexture(texture->GetTextureNumber() + GL_TEXTURE0);

		const GLuint TextureID = glGetUniformLocation(GetProgramID("sprite"), "image");

		glBindTexture(GL_TEXTURE_2D, texture->GetTexture());
		glUniform1i(TextureID, texture->GetTextureNumber());

		glBindVertexArray(Renderer::quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
	
	void Renderer::RendererSkybox(const glm::mat4& view, glm::mat4 projection, Skybox skybox) {
		glDepthMask(GL_FALSE);
		UseProgram(GetProgramID("skybox"));
		const GLuint projectionid = glGetUniformLocation(GetProgramID("skybox"), "projection");
		const GLuint viewid = glGetUniformLocation(GetProgramID("skybox"), "view");
		glm::mat4 viewWithoutTranslation = glm::mat4(glm::mat3(view));

		setMat4(viewid, viewWithoutTranslation);
		setMat4(projectionid, projection);
		glBindVertexArray(skybox.GetSkyboxVAO());
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.GetTextureID());
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);
	}

	void Renderer::SwapBuffers(GLFWwindow* window) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	GLuint Renderer::GetCurrentProgramID() {
		GLint currentProgramID;
		glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgramID);
		return currentProgramID;
	}
}
