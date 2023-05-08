#include "mesh.h"

namespace Engine
{
	void VertexAttribute::EnableAndConfigure() const
	{
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, components, type, GL_FALSE, stride, (const void*)offset);
	}

	void VertexAttribute::Disable() const
	{
		glDisableVertexAttribArray(location);
	}

	Mesh::Mesh() :
		vertexArrayObject(0),
		indexBufferObject(0)
	{}

	Mesh::~Mesh(){}

	void Mesh::Init(const DataBuffer& indexBuffer,
		const std::vector<IndexAttribute>& indexAttributes,
		const std::vector<DataBuffer>& vertexBuffers,
		const std::vector<VertexAttribute>& vertexAttributes)
	{
		primitiveGroups = indexAttributes;

		// start setup of vertex array object
		glGenVertexArrays(1, &vertexArrayObject);
		glBindVertexArray(vertexArrayObject);

		// store the index data in the index buffer object
		glGenBuffers(1, &indexBufferObject);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.byteSize, indexBuffer.bufferStart, GL_STATIC_DRAW);

		for (size_t i = 0; i < vertexBuffers.size(); i++) {
			GLuint attributeBuffer = 0;
			glGenBuffers(1, &attributeBuffer);
			attributeBufferObjects.push_back(attributeBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, attributeBuffer);
			glBufferData(GL_ARRAY_BUFFER, vertexBuffers[i].byteSize, vertexBuffers[i].bufferStart, GL_STATIC_DRAW);

			// the vertex data is either packed into one buffer or separated into individual buffers
			if (vertexBuffers.size() == 1) {
				for (size_t j = 0; j < vertexAttributes.size(); j++) {
					vertexAttributes[j].EnableAndConfigure();
				}
			}
			else {
				vertexAttributes[i].EnableAndConfigure();
			}
		}

		// end setup of vertex array object
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		for (size_t i = 0; i < vertexAttributes.size(); i++) {
			vertexAttributes[i].Disable();
		}
	}

	void Mesh::Deinit()
	{
		glDeleteVertexArrays(1, &vertexArrayObject);
		glDeleteBuffers(1, &indexBufferObject);

		if (attributeBufferObjects.size() > 0)
			glDeleteBuffers(attributeBufferObjects.size(), &attributeBufferObjects[0]);
	}

	void Mesh::Bind() const
	{
		glBindVertexArray(vertexArrayObject);
	}

	void Mesh::Draw(GLuint primitiveGroupIndex) const
	{
		IndexAttribute group = this->primitiveGroups[primitiveGroupIndex];
		glDrawElements(GL_TRIANGLES, group.count, group.type, (void*)group.offset);
	}

	void Mesh::Unbind() const
	{
		glBindVertexArray(0);
	}

	void Mesh::ScreenQuad()
	{
		GLubyte indices[]
		{
			0, 1, 2,
			0, 2, 3
		};

		GLfloat positions[]
		{
			-1.f, -1.f,
			-1.f, 1.f,
			1.f, 1.f,
			1.f, -1.f
		};

		DataBuffer indexBuffer;
		indexBuffer.bufferStart = indices;
		indexBuffer.byteSize = sizeof(GLubyte) * 6;

		IndexAttribute indexAttrib;
		indexAttrib.offset = 0;
		indexAttrib.count = 6;
		indexAttrib.type = GL_UNSIGNED_BYTE;

		DataBuffer posBuffer;
		posBuffer.bufferStart = (GLubyte*)&positions;
		posBuffer.byteSize = sizeof(GLfloat) * 8;

		VertexAttribute posAttrib;
		posAttrib.location = 0;
		posAttrib.components = 2;
		posAttrib.stride = sizeof(GLfloat) * 2;
		posAttrib.offset = 0;
		posAttrib.type = GL_FLOAT;

		Init(indexBuffer, {indexAttrib}, { posBuffer }, { posAttrib });
	}

	bool Mesh::LoadObj(const std::string& meshPath)
	{
		return true;
	}
}