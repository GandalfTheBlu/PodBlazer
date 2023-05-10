#pragma once
#include "GL/glew.h"
#include <vector>
#include <string>

namespace Engine
{
	struct VertexAttribute
	{
		GLuint location;
		GLuint components;
		GLint stride;
		GLint offset;
		GLenum type;

		void EnableAndConfigure() const;
		void Disable() const;
	};

	struct IndexAttribute
	{
		GLuint offset;
		GLuint count;
		GLenum type;
	};

	struct DataBuffer
	{
		GLubyte* bufferStart;
		GLuint byteSize;
	};

	class Mesh
	{
	private:
		GLuint vertexArrayObject;
		GLuint indexBufferObject;
		std::vector<GLuint> attributeBufferObjects;

	public:
		std::vector<IndexAttribute> primitiveGroups;

		Mesh();
		~Mesh();

		void Init(
			const DataBuffer& indexBuffer, 
			const std::vector<IndexAttribute>& indexAttributes,
			const std::vector<DataBuffer>& vertexBuffers, 
			const std::vector<VertexAttribute>& vertexAttributes);

		void Deinit();

		void Bind() const;
		void Draw(GLuint primitiveGroupIndex) const;
		void Unbind() const;

		void ScreenQuad();
	};
}