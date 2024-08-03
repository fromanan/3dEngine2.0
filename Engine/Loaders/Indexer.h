#pragma once

namespace Indexer
{
	// Returns true iif v1 can be considered equal to v2
	bool is_near(float v1, float v2);

	// Searches through all already-exported vertices
	// for a similar one.
	// Similar = same position + same UVs + same normal
	bool getSimilarVertexIndex(
		const glm::vec3& in_vertex,
		const glm::vec2& in_uv,
		const glm::vec3& in_normal,
		const std::vector<glm::vec3>& out_vertices,
		const std::vector<glm::vec2>& out_uvs,
		const std::vector<glm::vec3>& out_normals,
		unsigned short& result
	);

	void indexVBO_slow(
		std::vector<glm::vec3>& in_vertices,
		std::vector<glm::vec2>& in_uvs,
		std::vector<glm::vec3>& in_normals,

		std::vector<unsigned short>& out_indices,
		std::vector<glm::vec3>& out_vertices,
		std::vector<glm::vec2>& out_uvs,
		std::vector<glm::vec3>& out_normals
	);
	
	struct PackedVertex {
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normal;
		bool operator<(const PackedVertex that) const {
			return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
		}
	};

	bool getSimilarVertexIndex_fast(
		const PackedVertex& packed,
		std::map<PackedVertex, unsigned short>& VertexToOutIndex,
		unsigned short& result
	);

	void indexVBO(
		const std::vector<glm::vec3>& in_vertices,
		const std::vector<glm::vec2>& in_uvs,
		const std::vector<glm::vec3>& in_normals,
		const std::vector<glm::vec3>& in_tangents,
		const std::vector<glm::vec3>& in_bitangents,

		std::vector<unsigned short>& out_indices,
		std::vector<glm::vec3>& out_vertices,
		std::vector<glm::vec2>& out_uvs,
		std::vector<glm::vec3>& out_normals,
		std::vector<glm::vec3>& out_tangents,
		std::vector<glm::vec3>& out_bitangents
	);

	void indexVBO_TBN(
		std::vector<glm::vec3>& in_vertices,
		std::vector<glm::vec2>& in_uvs,
		std::vector<glm::vec3>& in_normals,
		std::vector<glm::vec3>& in_tangents,
		std::vector<glm::vec3>& in_bitangents,

		std::vector<unsigned short>& out_indices,
		std::vector<glm::vec3>& out_vertices,
		std::vector<glm::vec2>& out_uvs,
		std::vector<glm::vec3>& out_normals,
		std::vector<glm::vec3>& out_tangents,
		std::vector<glm::vec3>& out_bitangents
	);
}