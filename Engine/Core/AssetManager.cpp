#pragma once
#include "AssetManager.h"


namespace AssetManager
{
	std::vector<GameObject> GameObjects;
	std::vector<Texture> Textures;
	std::vector<Decal> Decals;

	void AssetManager::Init() {
		Textures.clear();
		GameObjects.clear();
	}

	void AssetManager::LoadAssets(const char* loadJson) {
		std::ifstream f(loadJson);
		json data = json::parse(f);
		std::cout << "loaded object count " << data["GameObjects"].size() << std::endl;

		for (int gameobject = 0; gameobject < data["GameObjects"].size(); gameobject++)
		{
			std::string name = data["GameObjects"][gameobject][0];
			std::string Parentname = data["GameObjects"][gameobject][1];

			glm::vec3 position = glm::vec3(data["GameObjects"][gameobject][2], data["GameObjects"][gameobject][3], data["GameObjects"][gameobject][4]);
			glm::vec3 rotaion = glm::vec3(data["GameObjects"][gameobject][5], data["GameObjects"][gameobject][6], data["GameObjects"][gameobject][7]);
			glm::vec3 scale = glm::vec3(data["GameObjects"][gameobject][8], data["GameObjects"][gameobject][9], data["GameObjects"][gameobject][10]);

			std::vector<unsigned short> indices = data["GameObjects"][gameobject][11];
			std::vector<glm::vec3> indexed_vertices;
			std::vector<glm::vec2> indexed_uvs;
			std::vector<glm::vec3> indexed_normals;
			std::vector<float> verticies = data["GameObjects"][gameobject][12];
			std::vector<float> Uvs = data["GameObjects"][gameobject][13];
			std::vector<float> normals = data["GameObjects"][gameobject][13];


			std::string textureName = data["GameObjects"][gameobject][15];
			//Texture* texture = GetTexture("container");
			Texture* texture = GetTexture(textureName.c_str());

			for (int vert = 0; vert < verticies.size(); vert++) {
				indexed_vertices.push_back(glm::vec3(data["GameObjects"][gameobject][12][vert], data["GameObjects"][gameobject][12][vert + 1], data["GameObjects"][gameobject][12][vert + 2]));
				vert = vert + 2;
			}
			for (int uvs = 0; uvs < Uvs.size(); uvs++) {
				indexed_uvs.push_back(glm::vec2(data["GameObjects"][gameobject][13][uvs], data["GameObjects"][gameobject][13][uvs + 1]));
				uvs = uvs + 1;
			}
			for (int normal = 0; normal < normals.size(); normal++) {
				indexed_normals.push_back(glm::vec3(data["GameObjects"][gameobject][14][normal], data["GameObjects"][gameobject][14][normal + 1], data["GameObjects"][gameobject][14][normal + 2]));
				normal = normal + 2;
			}
			bool save = data["GameObjects"][gameobject][16];
			GameObjects.push_back(GameObject(name.data(), Parentname.data(), texture, position, rotaion, scale, indices, indexed_vertices, indexed_uvs, indexed_normals, save,0,Box));
		}
	}

	// TODO: this doesn't work yet, still needs to be updated
	void AssetManager::SaveAssets(const char* path) {
		json save;
		std::vector<json> SerializedGameObjects;
		// name,parentname,pos,rotation,scale,indices,indexvert,indexuv,indexnormal,texturename
		for (int i = 0; i < GameObjects.size(); i++) {
			if (!GameObjects[i].CanSave())
				continue;
			std::vector<float> verticies;
			std::vector<float> Uvs;
			std::vector<float> normals;
			std::vector<glm::vec3> indexed_vertices = GameObjects[i].getIndexedVerticies();
			std::vector<glm::vec2> indexed_uvs = GameObjects[i].getIndexedUvs();
			std::vector<glm::vec3> indexed_normals = GameObjects[i].getIndexedNormals();

			for (int vert = 0; vert < indexed_vertices.size(); vert++) {
				verticies.push_back(indexed_vertices[vert].x);
				verticies.push_back(indexed_vertices[vert].y);
				verticies.push_back(indexed_vertices[vert].z);
			}
			for (int uvs = 0; uvs < indexed_uvs.size(); uvs++) {
				Uvs.push_back(indexed_uvs[uvs].x);
				Uvs.push_back(indexed_uvs[uvs].y);
			}
			for (int normal = 0; normal < indexed_normals.size(); normal++) {
				normals.push_back(indexed_normals[normal].x);
				normals.push_back(indexed_normals[normal].y);
				normals.push_back(indexed_normals[normal].z);
			}
			json gameobject = { GameObjects[i].GetName(),
				GameObjects[i].GetParentName(),
				GameObjects[i].getPosition().x,
				GameObjects[i].getPosition().y,
				GameObjects[i].getPosition().z,
				GameObjects[i].getRotation().x,
				GameObjects[i].getRotation().y,
				GameObjects[i].getRotation().z,
				GameObjects[i].getScale().x,
				GameObjects[i].getScale().y,
				GameObjects[i].getScale().z,
				GameObjects[i].getIndices(),
				verticies, Uvs, normals,
				GameObjects[i].GetTextureName(),
				true
			};
			SerializedGameObjects.push_back(gameobject);
		}
		save["GameObjects"] = SerializedGameObjects;

		// Write JSON object to file
		std::ofstream file(path);
		if (file.is_open()) {
			file << std::setw(4) << save << std::endl;
			file.close();
			std::cout << "JSON data has been written to " << path << std::endl;
		}
		else {
			std::cerr << "Failed to open " << path << std::endl;
		}

	}

	// returns index of object
	int AssetManager::AddGameObject(GameObject gameobject) {
		GameObjects.push_back(gameobject);
		GameObjects[GameObjects.size() - 1].GetRigidBody()->setUserPointer((void*)(GameObjects.size() - 1));
		return GameObjects.size() - 1;
	}
	int AssetManager::AddGameObject(std::string name, const char* path, Texture* texture, glm::vec3 position, bool save, float mass, ColliderShape shape) {
		GameObjects.push_back(GameObject(name, path, texture, position,save, mass, shape));
		GameObjects[GameObjects.size() - 1].GetRigidBody()->setUserIndex(GameObjects.size() - 1);
		return GameObjects.size() - 1;
	}
	
	int AddDecal(glm::vec3 position, glm::vec3 normal, glm::vec3 scale, Texture* texture, GameObject* parent) {
		Decals.push_back(Decal(position, normal, scale, texture, parent));
		return Decals.size() - 1;
	}
	Decal* GetDecal(int index) {
		return &Decals[index];
	}

	std::vector<Decal>* GetAllDecals() {
		return &Decals;
	}

	int AssetManager::AddTexture(Texture texture) {
		Textures.push_back(texture);
		return Textures.size() - 1;
	}
	int AssetManager::AddTexture(const char* name, const char* path) {
		Textures.push_back(Texture(name, path));
		return Textures.size() - 1;
	}
	int AddTexture(const char* name, const char* path, const char* normalPath) {
		Textures.push_back(Texture(name, path,normalPath));
		return Textures.size() - 1;
	}


	void AssetManager::RemoveGameObject(std::string name) {
		for (int i = 0; i < GameObjects.size(); i++) {
			if (GameObjects[i].GetName() == name)
				GameObjects.erase(GameObjects.begin() + i);
		}
	}
	void AssetManager::RemoveGameObject(int index) {
		GameObjects.erase(GameObjects.begin() + index);
	}
	void AssetManager::CleanUp() {
		for (int i = 0; i < GameObjects.size(); i++) {
			if (GameObjects[i].ShouldDlete())
				GameObjects.erase(GameObjects.begin() + i);
		}
	}
	GameObject* AssetManager::GetGameObject(std::string name) {
		for (int i = 0; i < GameObjects.size(); i++) {
			if (GameObjects[i].GetName() == name)
				return &GameObjects[i];
		}
		return nullptr;
	}
	GameObject* AssetManager::GetGameObject(int index) {
		if (index >= GameObjects.size() || index < 0)
			return nullptr;
		return &GameObjects[index];
	}
	std::vector<GameObject> AssetManager::GetAllGameObjects() {
		return GameObjects;
	}
	int AssetManager::GetGameObjectsSize() {
		return GameObjects.size();
	}
	int AssetManager::GetDecalsSize() {
		return Decals.size();
	}
	Texture* AssetManager::GetTexture(std::string name) {
		for (int i = 0; i < Textures.size(); i++) {
			if (Textures[i].GetName() == name)
				return &Textures[i];
		}
		return nullptr;
	}
}