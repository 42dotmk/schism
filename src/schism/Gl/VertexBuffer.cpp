#include "VertexBuffer.h"
#include "schism/System/Log.h"

#include <glad/glad.h>

namespace Schism::Gl
{
	void VertexBuffer::CreateBuffer()
	{
		glCreateBuffers(1, &m_BufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	}

	VertexBuffer::VertexBuffer(float* vertices, size_t size, const BufferLayout& layout, bool dynamic)
		:
			m_Layout(layout),
			m_Dynamic(dynamic)
	{
		CreateBuffer();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(std::vector<float>& vertices, const BufferLayout& layout, bool dynamic)
		:
			m_Layout(layout),
			m_Dynamic(dynamic)
	{
		CreateBuffer();
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(const BufferLayout& layout)
		:
			m_Layout(layout),
			m_Dynamic(true)
	{
		CreateBuffer();
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	Ref<VertexBuffer> VertexBuffer::Create(const BufferLayout& layout)
	{
		return MakeRef<VertexBuffer>(layout);
	}

	Ref<VertexBuffer> VertexBuffer::Create(void* vertices, size_t size, const BufferLayout& layout)
	{
		auto ref = Create(layout);
		ref->SetData(vertices, size);
		return ref;
	}

	
	void VertexBuffer::SetData(void* vertices, size_t size)
	{
		if (!m_Dynamic) return;
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
	}

	void VertexBuffer::UpdateSubData(void* vertices, size_t size)
	{
		// Todo: Finish this
	}
	
	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::Clear()
	{
		glDeleteBuffers(1, &m_BufferID);
	}
}
