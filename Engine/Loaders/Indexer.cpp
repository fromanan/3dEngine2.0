#include "pch.h"

#include "Indexer.h"

namespace Indexer
{
	// Returns true iif v1 can be considered equal to v2
	bool Indexer::is_near(const float v1, const float v2) {
		return fabs(v1 - v2) < 0.01f;
	}

	// Searches through all already-exported vertices
	// for a similar one.
	// Similar = same position + same UVs + same normal
	bool Indexer::getSimilarVertexIndex(
		const glm::vec3& in_vertex,
		const glm::vec2& in_uv,
		const glm::vec3& in_normal,
		const std::vector<glm::vec3>& out_vertices,
		const std::vector<glm::vec2>& out_uvs,
		const std::vector<glm::vec3>& out_normals,
		unsigned short& result
	) {
		// Lame linear search
		for (unsigned int i = 0; i < out_vertices.size(); i++) {
			if (
				is_near(in_vertex.x, out_vertices[i].x) &&
				is_near(in_vertex.y, out_vertices[i].y) &&
				is_near(in_vertex.z, out_vertices[i].z) &&
				is_near(in_uv.x, out_uvs[i].x) &&
				is_near(in_uv.y, out_uvs[i].y) &&
				is_near(in_normal.x, out_normals[i].x) &&
				is_near(in_normal.y, out_normals[i].y) &&
				is_near(in_normal.z, out_normals[i].z)
				) {
				result = i;
				return true;
			}
		}
		
		// No other vertex could be used instead.
		// Looks like we'll have to add it to the VBO.
		return false;
	}

	void Indexer::indexVBO(
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
	) {
		// For each input vertex
		for (unsigned int i = 0; i < in_vertices.size(); i++) {

			// Try to find a similar vertex in out_XXXX
			unsigned short index;
			if (const bool found = getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i], out_vertices, out_uvs, out_normals, index)) { // A similar vertex is already in the VBO, use it instead !
				out_indices.push_back(index);

				// Average the tangents and the bitangents
				out_tangents[index] += in_tangents[i];
				out_bitangents[index] += in_bitangents[i];
			}
			else { // If not, it needs to be added in the output data.
				out_vertices.push_back(in_vertices[i]);
				out_uvs.push_back(in_uvs[i]);
				out_normals.push_back(in_normals[i]);
				out_tangents.push_back(in_tangents[i]);
				out_bitangents.push_back(in_bitangents[i]);
				out_indices.push_back(static_cast<unsigned short>(out_vertices.size() - 1));
			}
		}
	}

	bool Indexer::getSimilarVertexIndex_fast(
		const PackedVertex& packed,
		std::map<PackedVertex, unsigned short>& VertexToOutIndex,
		unsigned short& result
	) {
		if (const std::map<PackedVertex, unsigned short>::iterator it = VertexToOutIndex.find(packed); it == VertexToOutIndex.end()) {
			return false;
		}
		else {
			result = it->second;
			return true;
		}
	}
}
