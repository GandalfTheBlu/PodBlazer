#include "map_generator.h"
#include "file_manager.h"
#include <regex>
#include <utility>

namespace Game
{
	bool LoadMapFile(const std::string& path, std::vector<glm::vec2>& outPoints)
	{
		std::string text;
		if (!Engine::ReadTextFile(path, text))
			return false;

		std::regex coordsRgx("\\(([\\-0-9.]+)\\,\\s([\\-0-9.]+)\\)");
		std::smatch coordsMatch;
		std::string::const_iterator coordsStart(text.cbegin());
		while (std::regex_search(coordsStart, text.cend(), coordsMatch, coordsRgx))
		{
			float x = std::stof(coordsMatch[1]);
			float y = std::stof(coordsMatch[2]);
			outPoints.push_back(glm::vec2(x, y));
			coordsStart = coordsMatch.suffix().first;
		}

		return true;
	}

#define PUSH3(v, x, y, z) v.push_back(x);v.push_back(y);v.push_back(z)
#define PUSH2(v, x, y) v.push_back(x);v.push_back(y)

	std::shared_ptr<Engine::Mesh> GenerateRoadMesh(const std::vector<glm::vec2>& mapData)
	{
		std::shared_ptr<Engine::Mesh> mesh = std::make_shared<Engine::Mesh>();

		std::vector<GLfloat> positions;
		std::vector<GLfloat> uvs;
		std::vector<GLfloat> normals;
		std::vector<GLuint> indices;

		float halfRoadWidth = 4.f;

		for (int i = 0; i < (int)mapData.size(); i++)
		{
			glm::vec2 p0 = mapData[i];
			glm::vec2 p1 = mapData[(i + 1) % mapData.size()];
			glm::vec2 p2 = mapData[(i + 2) % mapData.size()];

			glm::vec2 dir0 = glm::normalize(p1 - p0);
			glm::vec2 dir1 = glm::normalize(p2 - p1);
			glm::vec2 leftDir0 = glm::vec2(-dir0.y, dir0.x);
			glm::vec2 leftDir1 = glm::vec2(-dir1.y, dir1.x);
			
			glm::vec2 c0 = p0 + leftDir0 * halfRoadWidth;
			glm::vec2 c1 = p0 - leftDir0 * halfRoadWidth;
			glm::vec2 c2 = p1 + leftDir1 * halfRoadWidth;
			glm::vec2 c3 = p1 - leftDir1 * halfRoadWidth;

			GLuint index = positions.size() / 3;

			PUSH3(positions, c0.x, 0.f, c0.y);
			PUSH3(positions, c1.x, 0.f, c1.y);
			PUSH3(positions, c2.x, 0.f, c2.y);
			PUSH3(positions, c3.x, 0.f, c3.y);

			PUSH2(uvs, 0.f, 0.f);
			PUSH2(uvs, 1.f, 0.f);
			PUSH2(uvs, 0.f, 1.f);
			PUSH2(uvs, 1.f, 1.f);

			PUSH3(normals, 0.f, 1.f, 0.f);
			PUSH3(normals, 0.f, 1.f, 0.f);
			PUSH3(normals, 0.f, 1.f, 0.f);
			PUSH3(normals, 0.f, 1.f, 0.f);

			indices.push_back(index);
			indices.push_back(index+3);
			indices.push_back(index+2);
			indices.push_back(index);
			indices.push_back(index+1);
			indices.push_back(index+3);
		}

		Engine::DataBuffer indexBuffer;
		indexBuffer.bufferStart = (GLubyte*)&indices[0];
		indexBuffer.byteSize = sizeof(GLuint) * indices.size();

		Engine::IndexAttribute indexAttrib;
		indexAttrib.offset = 0;
		indexAttrib.count = indices.size();
		indexAttrib.type = GL_UNSIGNED_INT;

		Engine::DataBuffer posBuffer;
		posBuffer.bufferStart = (GLubyte*)&positions[0];
		posBuffer.byteSize = sizeof(GLfloat) * positions.size();

		Engine::VertexAttribute posAttrib;
		posAttrib.location = 0;
		posAttrib.components = 3;
		posAttrib.stride = 0;
		posAttrib.offset = 0;
		posAttrib.type = GL_FLOAT;

		Engine::DataBuffer uvBuffer;
		uvBuffer.bufferStart = (GLubyte*)&uvs[0];
		uvBuffer.byteSize = sizeof(GLfloat) * uvs.size();

		Engine::VertexAttribute uvAttrib;
		uvAttrib.location = 1;
		uvAttrib.components = 2;
		uvAttrib.stride = 0;
		uvAttrib.offset = 0;
		uvAttrib.type = GL_FLOAT;

		Engine::DataBuffer normBuffer;
		normBuffer.bufferStart = (GLubyte*)&normals[0];
		normBuffer.byteSize = sizeof(GLfloat) * normals.size();

		Engine::VertexAttribute normAttrib;
		normAttrib.location = 2;
		normAttrib.components = 3;
		normAttrib.stride = 0;
		normAttrib.offset = 0;
		normAttrib.type = GL_FLOAT;

		mesh->Init(indexBuffer, { indexAttrib }, { posBuffer, uvBuffer, normBuffer}, { posAttrib, uvAttrib, normAttrib});
		return mesh;
	}

	void SpawnSideObjects(const std::vector<glm::vec2>& mapData, const std::vector<Prefab*>& prefabs, std::vector<GameObject*>& outGameObjects)
	{
		int objectsPerSegment = 3;
		size_t prefabIndex = 0;

		float roadOffset = 6.f;

		for (int i = 0; i < (int)mapData.size(); i++)
		{
			glm::vec2 p0 = mapData[i];
			glm::vec2 p1 = mapData[(i + 1) % mapData.size()];

			glm::vec3 q0 = glm::vec3(p0.x, 0.f, p0.y);
			glm::vec3 q1 = glm::vec3(p1.x, 0.f, p1.y);

			glm::vec3 dir = glm::normalize(q1 - q0);
			glm::vec3 leftDir = glm::vec3(-dir.z, 0.f, dir.x);

			for (int j = 0; j < objectsPerSegment; j++)
			{
				GameObject* obj = new GameObject(prefabs[prefabIndex]);
				float side = j % 2 == 0 ? -1.f : 1.f;
				obj->transform.position = q0 + (q1 - q0) * ((float)j / objectsPerSegment) + leftDir * (side * roadOffset);
				obj->transform.scale *= 3.5f;
				outGameObjects.push_back(obj);

				prefabIndex = (prefabIndex + 1) % prefabs.size();
			}
		}
	}

	void GenerateObstaclePoints(const std::vector<glm::vec2>& mapData, std::vector<glm::vec2>& outObstacles)
	{
		size_t pointsBetweenObstacles = 3;
		for (size_t i = 0; i < mapData.size(); i += pointsBetweenObstacles)
		{
			float angle = (float)i * 0.2f;
			glm::vec2 offset = glm::vec2(glm::cos(angle), glm::sin(angle)) * 3.f;
			glm::vec2 obstacle = mapData[i] + offset;
			outObstacles.push_back(obstacle);
		}
	}
}
