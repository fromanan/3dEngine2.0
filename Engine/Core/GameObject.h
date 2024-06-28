#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "Common.h"
#include "Renderer.h"

#include "../Loaders/vboindexer.h"
#include "../Loaders/loader.hpp"



class GameObject
{
public:
	GameObject(const char* name);
	GameObject(const char* name, glm::vec3 position);
	GameObject(const char* name, const char* path);
	GameObject(const char* name, const char* path, glm::vec3 position);
	GameObject(const char* name, const char* path,Texture* texture, glm::vec3 position);
	void LoadModel(const char* path);
	glm::mat4 GetModelMatrix();

	void RenderObject(GLuint& programID);

	void setPosition(glm::vec3 position);
	void setRotation(glm::vec3 rotation);
	void setScale(glm::vec3 scale);

	glm::vec3 getPosition();
	glm::vec3 getRotation();
	glm::vec3 getScale();

	void setPositionX(float x);
	void setPositionY(float y);
	void setPositionZ(float z);

	void SetRotationX(float rotation);
	void SetRotationY(float rotation);
	void SetRotationZ(float rotation);

	void SetScale(float scale);

	const char* GetName();

	const char* GetParentName();
	void SetParentName(const char* name);

private:
	Transform transform;
	Texture* texture = NULL;

	const char* parentName;
	const char* name;
	

	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	std::vector<unsigned short> indices;

	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;
	GLuint VertexArrayID;

};
