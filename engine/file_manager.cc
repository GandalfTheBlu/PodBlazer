#include "file_manager.h"
#include <fstream>
#include <sstream>
#include "../exts/glm/vec3.hpp"
#include "../exts/glm/vec2.hpp"
#include <regex>
#include <utility>
#include <unordered_map>

namespace Engine
{
	bool ReadTextFile(const std::string& path, std::string& text)
	{
		std::ifstream file;
		file.open(path);

		if (!file.is_open()) {
			printf("[ERROR] failed to open file '%s'\n", path.c_str());
			return false;
		}

		std::stringstream stringStream;
		stringStream << file.rdbuf();
		text = stringStream.str();
		file.close();

		return true;
	}

	void WriteTextFile(const std::string& path, const std::string& text, bool append)
	{
		std::ofstream file;
		if (append)
			file.open(path, std::ios::app);
		else
			file.open(path);

		file << text;

		file.close();
	}

	bool LoadOBJFile(Mesh& outMesh, std::vector<ObjMaterialInfo>& outMatInfo, const std::string& path)
	{
		std::regex pathRgx("([\\S\\s]*)\\/.*$");
		std::smatch pathMatch;
		if (!std::regex_search(path, pathMatch, pathRgx))
		{
			printf("[ERROR] invalid path to obj-file '%s'\n", path.c_str());
			return false;
		}
		std::string folderPath = pathMatch[1];


		// get text from obj-file
		std::string objText;
		if (!ReadTextFile(path, objText))
			return false;

		// try get name of mtl-file
		std::regex mtlRgx("mtllib\\s(.*)");
		std::smatch mtlMatch;
		if (!std::regex_search(objText, mtlMatch, mtlRgx))
		{
			printf("[ERROR] no material-file is defined in obj-file '%s'\n", path.c_str());
			return false;
		}

		// get text from mtl-file
		std::string mtlFilename = mtlMatch[1];
		std::string mtlText;
		if (!ReadTextFile(folderPath + "/" + mtlFilename, mtlText))
			return false;

		// get all material colors
		std::unordered_map<std::string, glm::vec3> materialColors;
		std::regex matColRgx("newmtl\\s(.*)[\\S\\s]*?Kd\\s([0-9.]+)\\s([0-9.]+)\\s([0-9.]+)");
		std::smatch matColMatch;
		std::string::const_iterator matColStart(mtlText.cbegin());
		while (std::regex_search(matColStart, mtlText.cend(), matColMatch, matColRgx))
		{
			std::string matName = matColMatch[1];
			float x = std::stof(matColMatch[2]);
			float y = std::stof(matColMatch[3]);
			float z = std::stof(matColMatch[4]);
			materialColors[matName] = glm::vec3(x, y, z);
			matColStart = matColMatch.suffix().first;
		}

		// get all positions
		std::vector<glm::vec3> objPositions;
		std::regex posRgx("v\\s*([0-9.\\-E]+)\\s([0-9.\\-E]+)\\s([0-9.\\-E]+)");
		std::smatch posMatch;
		std::string::const_iterator posStart(objText.cbegin());
		while (std::regex_search(posStart, objText.cend(), posMatch, posRgx))
		{
			float x = std::stof(posMatch[1]);
			float y = std::stof(posMatch[2]);
			float z = std::stof(posMatch[3]);
			objPositions.push_back(glm::vec3(x, y, z));
			posStart = posMatch.suffix().first;
		}

		// get all uvs
		std::vector<glm::vec2> objUvs;
		std::regex uvRgx("vt\\s*([0-9.\\-E]+)\\s([0-9.\\-E]+)");
		std::smatch uvMatch;
		std::string::const_iterator uvStart(objText.cbegin());
		while (std::regex_search(uvStart, objText.cend(), uvMatch, uvRgx))
		{
			float x = std::stof(uvMatch[1]);
			float y = std::stof(uvMatch[2]);
			objUvs.push_back(glm::vec2(x, y));
			uvStart = uvMatch.suffix().first;
		}
		bool hasUvs = objUvs.size() > 0;

		// get all normals
		std::vector<glm::vec3> objNormals;
		std::regex normRgx("vn\\s*([0-9.\\-E]+)\\s([0-9.\\-E]+)\\s([0-9.\\-E]+)");
		std::smatch normMatch;
		std::string::const_iterator normStart(objText.cbegin());
		while (std::regex_search(normStart, objText.cend(), normMatch, normRgx))
		{
			float x = std::stof(normMatch[1]);
			float y = std::stof(normMatch[2]);
			float z = std::stof(normMatch[3]);
			objNormals.push_back(glm::vec3(x, y, z));
			normStart = normMatch.suffix().first;
		}
		bool hasNormals = objNormals.size() > 0;

		// get string of primitive groups and their material names
		struct ObjPGroupStr
		{
			std::string mtlName;
			std::string indices;
		};
		std::vector<ObjPGroupStr> objPGroupStrings;
		std::regex pGroupRgx("usemtl\\s(.*)\\s*([f\\s0-9\\/]+)");
		std::smatch pGroupMatch;
		std::string::const_iterator pGroupStart(objText.cbegin());
		while (std::regex_search(pGroupStart, objText.cend(), pGroupMatch, pGroupRgx))
		{
			std::string mtlName = pGroupMatch[1];
			std::string indices = pGroupMatch[2];
			objPGroupStrings.push_back({ mtlName, indices });
			pGroupStart = pGroupMatch.suffix().first;
		}

		// get primitive groups
		struct IndexGroup
		{
			GLuint posIndex;
			GLuint uvIndex;
			GLuint normIndex;
		};
		struct Face
		{
			std::vector<IndexGroup> indices;
		};
		struct ObjPGroup
		{
			std::string mtlName;
			std::vector<Face> faces;
		};

		std::vector<ObjPGroup> objPGroups;
		std::regex faceRgx("f\\s*(.*)");
		std::regex iGroupRgx("([0-9]+)\\/([0-9]+)?\\/([0-9]+)?");
		for (auto& pGroupStr : objPGroupStrings)
		{
			// parse faces
			ObjPGroup pGroup;
			pGroup.mtlName = pGroupStr.mtlName;
			std::smatch faceMatch;
			std::string::const_iterator faceStart = pGroupStr.indices.cbegin();
			while (std::regex_search(faceStart, pGroupStr.indices.cend(), faceMatch, faceRgx))
			{
				// parse index groups
				Face face;
				std::string faceStr = faceMatch[1];
				std::smatch iGroupMatch;
				std::string::const_iterator iGroupStart = faceStr.cbegin();
				std::vector<IndexGroup> initialIndices;
				while (std::regex_search(iGroupStart, faceStr.cend(), iGroupMatch, iGroupRgx))
				{
					GLuint vertIndex = std::stoi(iGroupMatch[1]);
					GLuint uvIndex = (hasUvs ? std::stoi(iGroupMatch[2]) : 1);
					GLuint normIndex = (hasNormals ? std::stoi(iGroupMatch[(hasUvs ? 3 : 2)]) : 1);
					initialIndices.push_back({ vertIndex - 1, uvIndex - 1, normIndex - 1 });
					iGroupStart = iGroupMatch.suffix().first;
				}

				// triangulate
				for (int i = 1; i < (int)initialIndices.size() - 1; i++)
				{
					GLuint triangulation[]{ 0, i + 1, i };
					for (int j = 0; j < 3; j++)
					{
						face.indices.push_back(initialIndices[triangulation[j]]);
					}
				}

				pGroup.faces.push_back(face);
				faceStart = faceMatch.suffix().first;
			}
			objPGroups.push_back(pGroup);
		}

		// translate to opengl-friendly buffers
		std::vector<IndexAttribute> glPGroups;
		std::vector<GLuint> glIndices;
		std::vector<GLfloat> glPositions;
		std::vector<GLfloat> glUvs;
		std::vector<GLfloat> glNormals;

		std::unordered_map<GLuint, GLuint> iGroupToIndex;
		for (auto& pGroup : objPGroups)
		{
			GLuint pGroupOffset = sizeof(GLuint) * glIndices.size();
			GLuint pGroupCount = 0;

			for (auto& face : pGroup.faces)
			{
				for (auto& iGroup : face.indices)
				{
					pGroupCount++;
					GLuint iGroupKey = iGroup.posIndex +
						objPositions.size() * iGroup.uvIndex +
						objPositions.size() * (hasUvs ? objUvs.size() : 1) * iGroup.normIndex;

					// if vertex already exists, just add the index
					if (iGroupToIndex.count(iGroupKey) != 0)
					{
						glIndices.push_back(iGroupToIndex[iGroupKey]);
					}
					// if vertex is new, then add new index and vertex data
					else
					{
						GLuint indexBufferIndex = glPositions.size() / 3;
						glIndices.push_back(indexBufferIndex);
						iGroupToIndex[iGroupKey] = indexBufferIndex;

						glm::vec3& pos = objPositions[iGroup.posIndex];
						glPositions.push_back(pos.x);
						glPositions.push_back(pos.y);
						glPositions.push_back(pos.z);

						if (hasUvs)
						{
							glm::vec2& uv = objUvs[iGroup.uvIndex];
							glUvs.push_back(uv.x);
							glUvs.push_back(uv.y);
						}
						if (hasNormals)
						{
							glm::vec3& norm = objNormals[iGroup.normIndex];
							glNormals.push_back(norm.x);
							glNormals.push_back(norm.y);
							glNormals.push_back(norm.z);
						}
					}
				}
			}

			glPGroups.push_back({ pGroupOffset, pGroupCount, GL_UNSIGNED_INT });
			outMatInfo.push_back({
				materialColors[pGroup.mtlName]
			});
		}

		DataBuffer indexBuffer;
		indexBuffer.bufferStart = (GLubyte*)&glIndices[0];
		indexBuffer.byteSize = sizeof(GLuint) * glIndices.size();

		std::vector<DataBuffer> vertexBuffers;
		std::vector<VertexAttribute> vertexAttributes;

		DataBuffer posBuffer;
		posBuffer.bufferStart = (GLubyte*)&glPositions[0];
		posBuffer.byteSize = sizeof(GLfloat) * glPositions.size();
		vertexBuffers.push_back(posBuffer);

		VertexAttribute posAttrib;
		posAttrib.location = 0;
		posAttrib.components = 3;
		posAttrib.stride = 0;
		posAttrib.offset = 0;
		posAttrib.type = GL_FLOAT;
		vertexAttributes.push_back(posAttrib);

		if (hasUvs)
		{
			DataBuffer uvBuffer;
			uvBuffer.bufferStart = (GLubyte*)&glUvs[0];
			uvBuffer.byteSize = sizeof(GLfloat) * glUvs.size();
			vertexBuffers.push_back(uvBuffer);

			VertexAttribute uvAttrib;
			uvAttrib.location = 1;
			uvAttrib.components = 2;
			uvAttrib.stride = 0;
			uvAttrib.offset = 0;
			uvAttrib.type = GL_FLOAT;
			vertexAttributes.push_back(uvAttrib);
		}
		if (hasNormals)
		{
			DataBuffer normBuffer;
			normBuffer.bufferStart = (GLubyte*)&glNormals[0];
			normBuffer.byteSize = sizeof(GLfloat) * glNormals.size();
			vertexBuffers.push_back(normBuffer);

			VertexAttribute normAttrib;
			normAttrib.location = 2;
			normAttrib.components = 3;
			normAttrib.stride = 0;
			normAttrib.offset = 0;
			normAttrib.type = GL_FLOAT;
			vertexAttributes.push_back(normAttrib);
		}

		outMesh.Init(indexBuffer, glPGroups, vertexBuffers, vertexAttributes);
		return true;
	}
}