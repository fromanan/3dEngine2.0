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

		for (auto& gameObject : data["GameObjects"]) {
			std::string name = gameObject[0];
			std::string Parentname = gameObject[1];

			glm::vec3 position = glm::vec3(gameObject[2], gameObject[3], gameObject[4]);
			glm::vec3 rotation = glm::vec3(gameObject[5], gameObject[6], gameObject[7]);
			glm::vec3 scale = glm::vec3(gameObject[8], gameObject[9], gameObject[10]);

			std::vector<unsigned short> indices = gameObject[11];
			std::vector<glm::vec3> indexed_vertices;
			std::vector<glm::vec2> indexed_uvs;
			std::vector<glm::vec3> indexed_normals;
			std::vector<float> vertices = gameObject[12];
			std::vector<float> Uvs = gameObject[13];
			std::vector<float> normals = gameObject[13];

			std::string textureName = gameObject[15];
			//Texture* texture = GetTexture("container");
			Texture* texture = GetTexture(textureName);

			for (int vert = 0; vert < vertices.size(); vert++) {
				indexed_vertices.emplace_back(gameObject[12][vert], gameObject[12][vert + 1], gameObject[12][vert + 2]);
				vert = vert + 2;
			}
			for (int uvs = 0; uvs < Uvs.size(); uvs++) {
				indexed_uvs.emplace_back(gameObject[13][uvs], gameObject[13][uvs + 1]);
				uvs = uvs + 1;
			}
			for (int normal = 0; normal < normals.size(); normal++) {
				indexed_normals.emplace_back(gameObject[14][normal], gameObject[14][normal + 1], gameObject[14][normal + 2]);
				normal = normal + 2;
			}
			bool save = gameObject[16];
			GameObjects.emplace_back(name.data(), Parentname.data(), texture, position, rotation, scale, indices, indexed_vertices, indexed_uvs, indexed_normals, save,0,Box);
		}
	}

	// TODO: this doesn't work yet, still needs to be updated
	void AssetManager::SaveAssets(const char* path) {
		json save;
		std::vector<json> SerializedGameObjects;
		
		// name,parentname,pos,rotation,scale,indices,indexvert,indexuv,indexnormal,texturename
		for (auto& GameObject : GameObjects) {
			if (!GameObject.CanSave())
				continue;
			std::vector<float> vertices;
			std::vector<float> Uvs;
			std::vector<float> normals;
			std::vector<glm::vec3> indexed_vertices = GameObject.getIndexedVertices();
			std::vector<glm::vec2> indexed_uvs = GameObject.getIndexedUvs();
			std::vector<glm::vec3> indexed_normals = GameObject.getIndexedNormals();

			for (auto& indexed_vertex : indexed_vertices) {
				vertices.push_back(indexed_vertex.x);
				vertices.push_back(indexed_vertex.y);
				vertices.push_back(indexed_vertex.z);
			}
			
			for (auto& indexed_uv : indexed_uvs) {
				Uvs.push_back(indexed_uv.x);
				Uvs.push_back(indexed_uv.y);
			}
			
			for (auto& indexed_normal : indexed_normals) {
				normals.push_back(indexed_normal.x);
				normals.push_back(indexed_normal.y);
				normals.push_back(indexed_normal.z);
			}
			
			json gameObject = {
				GameObject.GetName(),
				GameObject.GetParentName(),
				GameObject.getPosition().x,
				GameObject.getPosition().y,
				GameObject.getPosition().z,
				GameObject.getRotation().x,
				GameObject.getRotation().y,
				GameObject.getRotation().z,
				GameObject.getScale().x,
				GameObject.getScale().y,
				GameObject.getScale().z,
				GameObject.getIndices(),
				vertices, Uvs, normals,
				GameObject.GetTextureName(),
				true
			};
			
			SerializedGameObjects.emplace_back(gameObject);
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

	// Returns index of object
	size_t AssetManager::AddGameObject(const GameObject& gameObject) {
		GameObjects.emplace_back(gameObject);
		GameObjects[GameObjects.size() - 1].GetRigidBody()->setUserPointer(reinterpret_cast<void*>(GameObjects.size() - 1));
		return GameObjects.size() - 1;
	}

	size_t AssetManager::AddGameObject(const std::string& name, const char* path, Texture* texture, glm::vec3 position,
	                                   bool save, float mass, ColliderShape shape) {
		GameObjects.emplace_back(name, path, texture, position, save, mass, shape);
		GameObjects[GameObjects.size() - 1].GetRigidBody()->setUserIndex(static_cast<int>(GameObjects.size()) - 1);
		return GameObjects.size() - 1;
	}

	size_t AddDecal(glm::vec3 position, glm::vec3 normal, glm::vec3 scale, Texture* texture, GameObject* parent) {
		Decals.emplace_back(position, normal, scale, texture, parent);
		return Decals.size() - 1;
	}
	
	Decal* GetDecal(const int index) {
		return &Decals[index];
	}

	std::vector<Decal>* GetAllDecals() {
		return &Decals;
	}

	size_t AssetManager::AddTexture(Texture texture) {
		Textures.emplace_back(texture);
		return Textures.size() - 1;
	}

	size_t AssetManager::AddTexture(const char* name, const char* path) {
		Textures.emplace_back(name, path);
		return Textures.size() - 1;
	}

	size_t AddTexture(const char* name, const char* path, const char* normalPath) {
		Textures.emplace_back(name, path, normalPath);
		return Textures.size() - 1;
	}

	void AssetManager::RemoveGameObject(const std::string& name) {
		for (int i = 0; i < GameObjects.size(); i++) {
			if (GameObjects[i].GetName() == name)
				GameObjects.erase(GameObjects.begin() + i);
		}
	}
	
	void AssetManager::RemoveGameObject(const int index) {
		GameObjects.erase(GameObjects.begin() + index);
	}
	
	void AssetManager::CleanUp() {
		for (int i = 0; i < GameObjects.size(); i++) {
			if (GameObjects[i].ShouldDelete())
				GameObjects.erase(GameObjects.begin() + i);
		}
	}
	
	GameObject* AssetManager::GetGameObject(const std::string& name) {
		for (auto& gameObject : GameObjects) {
			if (gameObject.GetName() == name)
				return &gameObject;
		}
		return nullptr;
	}
	
	GameObject* AssetManager::GetGameObject(const int index) {
		if (index >= GameObjects.size() || index < 0)
			return nullptr;
		return &GameObjects[index];
	}
	
	std::vector<GameObject> AssetManager::GetAllGameObjects() {
		return GameObjects;
	}

	size_t AssetManager::GetGameObjectsSize() {
		return GameObjects.size();
	}

	size_t AssetManager::GetDecalsSize() {
		return Decals.size();
	}
	
	Texture* AssetManager::GetTexture(const std::string& name) {
		for (auto& texture : Textures) {
			if (texture.GetName() == name)
				return &texture;
		}
		return nullptr;
	}
}
